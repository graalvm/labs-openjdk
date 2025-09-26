/*
 * Copyright (c) 2021, 2025, Oracle and/or its affiliates. All rights reserved.
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

#include "code/nmethod.hpp"
#include "code/scopeDesc.hpp"
#include "gc/shared/barrierSet.hpp"
#include "gc/shared/barrierSetStackChunk.hpp"
#include "logging/log.hpp"
#include "logging/logStream.hpp"
#include "memory/memRegion.hpp"
#include "oops/instanceStackChunkKlass.inline.hpp"
#include "oops/oop.inline.hpp"
#include "oops/stackChunkOop.inline.hpp"
#include "runtime/frame.hpp"
#include "runtime/registerMap.hpp"
#include "runtime/smallRegisterMap.inline.hpp"
#include "runtime/stackChunkFrameStream.inline.hpp"
#ifdef SVM
#include "exports/sharedGCStructs.hpp"
#include "svmCodeReferenceMapDecoder.hpp"
#endif // SVM


namespace svm_gc {

class BarrierClosure: public OopClosure {
public:
  BarrierClosure() {}

  virtual void do_oop(oop* p)       override { do_oop_work(p); }
  virtual void do_oop(narrowOop* p) override { do_oop_work(p); }

  template <class T> inline void do_oop_work(T* p) {
    oop value = (oop)HeapAccess<>::oop_load(p);
    HeapAccess<>::oop_store(p, value);
  }
};

void stackChunkOopDesc::do_barriers() {
  BarrierClosure oops_closure;
  iterate_stack(&oops_closure);
}

void stackChunkOopDesc::iterate_stack(OopClosure* closure) {
  if (jdk_internal_vm_StackChunk::ip(this) == nullptr) {
    // The object is still being initialized and does not contain valid stack data yet.
    return;
  }

  StackFrames *stack = SVMGlobalData::_fetch_continuation_stack_frames(CompressedOops::base(), this);
  for (int i = 0; i < stack->count; i++) {
    StackFrame& frame = stack->frames[i];
    SVMCodeReferenceMapDecoder::walk_offsets_from_pointer(frame.stack_pointer, frame.encoded_reference_map, frame.reference_map_index, closure);
  }
  SVMGlobalData::_free_continuation_stack_frames(CompressedOops::base(), stack);
}

} // namespace svm_gc

