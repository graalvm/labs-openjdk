/*
 * Copyright (c) 2019, 2025, Oracle and/or its affiliates. All rights reserved.
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

#include "cds/cdsConfig.hpp"
#include "cds/serializeClosure.hpp"
#include "classfile/vmClasses.hpp"
#include "compiler/oopMap.inline.hpp"
#include "gc/shared/gc_globals.hpp"
#include "memory/oopFactory.hpp"
#include "memory/resourceArea.hpp"
#include "oops/instanceStackChunkKlass.inline.hpp"
#include "oops/stackChunkOop.inline.hpp"
#include "runtime/continuation.hpp"
#include "runtime/continuationJavaClasses.inline.hpp"
#include "runtime/frame.hpp"
#include "runtime/handles.hpp"
#include "runtime/registerMap.hpp"
#include "runtime/smallRegisterMap.inline.hpp"
#include "runtime/stackChunkFrameStream.inline.hpp"
#include "utilities/devirtualizer.inline.hpp"
#include "utilities/globalDefinitions.hpp"
#include "utilities/macros.hpp"
#include "utilities/ostream.hpp"
#ifdef SVM
#include "gc/shared/memAllocator.hpp"
#include "oops/arrayKlass.inline.hpp"
#endif // SVM

stackChunkOop InstanceStackChunkKlass::allocate(int length) {
  if (length < 0 || length > max_length()) {
    return nullptr;
  }

  int word_size = object_size(length);
  StackChunkAllocator allocator(this, word_size, length);
  return (stackChunkOop)allocator.allocate();
}

void InstanceStackChunkKlass::oop_oop_iterate_stack_slow(stackChunkOop chunk, OopIterateClosure* closure, MemRegion mr) {
  // NOTE (chaeubl): for large continuations, this might be a performance bottleneck because we need to skip parts of the reference map.
  OopIterateFilterClosure filter_closure(closure, mr);
  chunk->iterate_stack(&filter_closure);
}

void InstanceStackChunkKlass::oop_oop_iterate_stack_slow(stackChunkOop chunk, OopClosure* closure) {
  chunk->iterate_stack(closure);
}
