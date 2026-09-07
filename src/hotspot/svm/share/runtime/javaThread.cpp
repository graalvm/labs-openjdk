/*
 * Copyright (c) 1997, 2025, Oracle and/or its affiliates. All rights reserved.
 * Copyright (c) 2021, Azul Systems, Inc. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 *
 */

#include "cds/dynamicArchive.hpp"
#include "ci/ciEnv.hpp"
#include "classfile/javaClasses.inline.hpp"
#include "classfile/javaThreadStatus.hpp"
#include "classfile/systemDictionary.hpp"
#include "classfile/vmClasses.hpp"
#include "classfile/vmSymbols.hpp"
#include "code/codeCache.hpp"
#include "code/scopeDesc.hpp"
#include "compiler/compileTask.hpp"
#include "compiler/compilerThread.hpp"
#include "gc/shared/oopStorage.hpp"
#include "gc/shared/oopStorageSet.hpp"
#include "gc/shared/tlab_globals.hpp"
#include "jfr/jfrEvents.hpp"
#include "jvm.h"
#include "jvmtifiles/jvmtiEnv.hpp"
#include "logging/log.hpp"
#include "logging/logAsyncWriter.hpp"
#include "logging/logStream.hpp"
#include "memory/allocation.inline.hpp"
#include "memory/iterator.hpp"
#include "memory/universe.hpp"
#include "oops/access.inline.hpp"
#include "oops/instanceKlass.hpp"
#include "oops/klass.inline.hpp"
#include "oops/oop.inline.hpp"
#include "oops/oopHandle.inline.hpp"
#include "oops/verifyOopClosure.hpp"
#include "prims/jvm_misc.hpp"
#include "prims/jvmtiDeferredUpdates.hpp"
#include "prims/jvmtiExport.hpp"
#include "prims/jvmtiThreadState.inline.hpp"
#include "runtime/atomic.hpp"
#include "runtime/continuation.hpp"
#include "runtime/continuationEntry.inline.hpp"
#include "runtime/continuationHelper.inline.hpp"
#include "runtime/deoptimization.hpp"
#include "runtime/frame.inline.hpp"
#include "runtime/handles.inline.hpp"
#include "runtime/handshake.hpp"
#include "runtime/interfaceSupport.inline.hpp"
#include "runtime/java.hpp"
#include "runtime/javaCalls.hpp"
#include "runtime/javaThread.inline.hpp"
#include "runtime/jniHandles.inline.hpp"
#include "runtime/lockStack.inline.hpp"
#include "runtime/mutexLocker.hpp"
#include "runtime/orderAccess.hpp"
#include "runtime/os.inline.hpp"
#include "runtime/osThread.hpp"
#include "runtime/safepoint.hpp"
#include "runtime/safepointMechanism.inline.hpp"
#include "runtime/safepointVerifiers.hpp"
#include "runtime/serviceThread.hpp"
#include "runtime/stackFrameStream.inline.hpp"
#include "runtime/stackWatermarkSet.hpp"
#include "runtime/synchronizer.hpp"
#include "runtime/threadIdentifier.hpp"
#include "runtime/threadSMR.inline.hpp"
#include "runtime/threadStatisticalInfo.hpp"
#include "runtime/threadWXSetters.inline.hpp"
#include "runtime/timer.hpp"
#include "runtime/timerTrace.hpp"
#include "runtime/vframe.inline.hpp"
#include "runtime/vframeArray.hpp"
#include "runtime/vframe_hp.hpp"
#include "runtime/vmThread.hpp"
#include "runtime/vmOperations.hpp"
#include "services/threadService.hpp"
#include "utilities/copy.hpp"
#include "utilities/defaultStream.hpp"
#include "utilities/dtrace.hpp"
#include "utilities/events.hpp"
#include "utilities/macros.hpp"
#include "utilities/nativeStackPrinter.hpp"
#include "utilities/preserveException.hpp"
#include "utilities/spinYield.hpp"
#include "utilities/vmError.hpp"
#if INCLUDE_JVMCI
#include "jvmci/jvmci.hpp"
#include "jvmci/jvmciEnv.hpp"
#endif
#if INCLUDE_JFR
#include "jfr/jfr.hpp"
#endif
#ifdef SVM
#include "svmCodeReferenceMapDecoder.hpp"
#endif // SVM

CodeInfos JavaThread::_no_code_info_data;

JavaThread::JavaThread(MemTag mem_tag) :
  Thread(mem_tag),
  // Initialize fields
  _stack_frames(nullptr),
  _code_infos(nullptr),
  _deferred_card_mark(MemRegion()) {
  // NOTE (chaeubl): for the main thread, this constructor is executed before the Java heap is fully initialized.
  // So, we had to move all relevant code parts to JavaThread::initialize().
  assert(_stack_frames == nullptr && _code_infos == nullptr, "memory must have been zeroed out");
}

JavaThread::~JavaThread() {
  // NOTE (chaeubl): see JavaThread::exit(...) and Threads::remove(...)
  if (UseTLAB) {
    retire_tlab();
  }
  BarrierSet::barrier_set()->on_thread_detach(this);
}

void JavaThread::initialize() {
  // NOTE (chaeubl): see attach_current_thread(...) and Threads::add(...)
  os::create_attached_thread(this);
  BarrierSet::barrier_set()->on_thread_attach(this);
  initialize_tlab();
}

void JavaThread::nmethods_do(NMethodClosure* cf) {
  assert(_code_infos != nullptr, "must be");
  for (size_t i = 0; i < _code_infos->count; i++) {
    nmethod *nm = (nmethod*)_code_infos->code_infos[i];
    cf->do_nmethod(nm);
  }
}

void JavaThread::oops_do_no_frames(OopClosure* f, NMethodClosure* cf) {
  // handle the thread locals
  IsolateThread* isolate_thread = this->isolate_thread();
  SVMOopMap* oop_map = SVMGlobalData::_thread_locals_reference_map;
  OopMapBlock* block = oop_map->first_oop_map_block();
  OopMapBlock* const end_block = block + oop_map->length();

  for (; block < end_block; ++block) {
    narrowOop *p = (narrowOop*) (isolate_thread->thread_local_at(block->offset()));
    const narrowOop *end = p + block->count();
    for (; p < end; ++p) {
      f->do_oop(p);
    }
  }
}

void JavaThread::oops_do_frames(OopClosure* f, NMethodClosure* cf) {
  assert(_stack_frames != nullptr, "must be");
  for (size_t i = 0; i < _stack_frames->count; i++) {
    StackFrame& frame = _stack_frames->frames[i];
    SVMCodeReferenceMapDecoder::walk_offsets_from_pointer(frame.stack_pointer, frame.encoded_reference_map, frame.reference_map_index, f);
  }

  if (cf != nullptr) {
    nmethods_do(cf);
  }
}

#ifdef ASSERT
void JavaThread::check_for_valid_safepoint_state() {
  if (is_init_completed()) {
    IsolateThread::IsolateThreadStatus status = this->isolate_thread()->thread_status();
    guarantee(status == IsolateThread::STATUS_IN_NATIVE || status == IsolateThread::STATUS_IN_SAFEPOINT || status == IsolateThread::STATUS_IN_VM, "thread status invalid for safepoint");
  }
}
#endif // ASSERT

void JavaThread::pre_run() {
  // nothing to do
}

// First JavaThread specific code executed by a new Java thread.
void JavaThread::post_run() {
  // nothing to do
}

void JavaThread::verify() {
  // Verify oops in the thread.
  oops_do(&VerifyOopClosure::verify_oop, nullptr);
}
