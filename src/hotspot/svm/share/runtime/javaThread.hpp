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

#ifndef SHARE_RUNTIME_JAVATHREAD_HPP
#define SHARE_RUNTIME_JAVATHREAD_HPP

#include "jni.h"
#include "memory/allocation.hpp"
#include "oops/oop.hpp"
#include "oops/oopHandle.hpp"
#include "runtime/continuationEntry.hpp"
#include "runtime/frame.hpp"
#include "runtime/globals.hpp"
#include "runtime/handshake.hpp"
#include "runtime/javaFrameAnchor.hpp"
#include "runtime/lockStack.hpp"
#include "runtime/park.hpp"
#include "runtime/safepointMechanism.hpp"
#include "runtime/stackWatermarkSet.hpp"
#include "runtime/stackOverflow.hpp"
#include "runtime/thread.hpp"
#include "runtime/threadHeapSampler.hpp"
#include "runtime/threadIdentifier.hpp"
#include "runtime/threadStatisticalInfo.hpp"
#include "utilities/exceptions.hpp"
#include "utilities/globalDefinitions.hpp"
#include "utilities/macros.hpp"
#if INCLUDE_JFR
#include "jfr/support/jfrThreadExtension.hpp"
#include "utilities/ticks.hpp"
#endif

class OopClosure;

class JavaThread: public Thread {
public:
  static CodeInfos _no_code_info_data;

private:
  StackFrames* _stack_frames;
  CodeInfos* _code_infos;

  // See ReduceInitialCardMarks: this holds the precise space interval of
  // the most recent slow path allocation for which compiled code has
  // elided card-marks for performance along the fast-path.
  MemRegion     _deferred_card_mark;

public:
  JavaThread(MemTag mem_tag = mtThread);
  ~JavaThread();

  void * operator new (std::size_t, void *ptr)     { assert(is_aligned(ptr, wordSize), "thread must be aligned"); return ptr; }
  void initialize();

  virtual void run() {}
  static JavaThread* current() {
    return JavaThread::cast(Thread::current());
  }
  virtual bool is_Java_thread() const            { return true;  }
  virtual bool is_active_Java_thread() const     { return true; }

  MemRegion deferred_card_mark() const           { return _deferred_card_mark; }
  void set_deferred_card_mark(MemRegion mr)      { _deferred_card_mark = mr;   }

  bool has_stack_frames() const                  { return _stack_frames != nullptr; }
  void set_stack_frames(StackFrames* value)      { assert(value == nullptr ^ _stack_frames == nullptr, "must be"); _stack_frames = value; }
  void set_code_infos(CodeInfos* value)          { assert(value == nullptr ^ _code_infos == nullptr, "must be"); _code_infos = value; }
  IsolateThread* isolate_thread() const          { return IsolateThread::from_java_thread(this); }

  void nmethods_do(NMethodClosure* cf);
  void oops_do_frames(OopClosure* f, NMethodClosure* cf);
  void oops_do_no_frames(OopClosure* f, NMethodClosure* cf);

  bool has_pending_exception() const               { return false; }

  void check_for_valid_safepoint_state() NOT_DEBUG_RETURN;

  void verify();

  // Casts
  static JavaThread* cast(Thread* t) {
    assert(t->is_Java_thread(), "incorrect cast to JavaThread");
    return static_cast<JavaThread*>(t);
  }

  static const JavaThread* cast(const Thread* t) {
    assert(t->is_Java_thread(), "incorrect cast to const JavaThread");
    return static_cast<const JavaThread*>(t);
  }

protected:
 virtual void pre_run();
 virtual void post_run();
};

#endif // SHARE_RUNTIME_JAVATHREAD_HPP
