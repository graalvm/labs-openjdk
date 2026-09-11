/*
 * Copyright (c) 2021, 2024, Oracle and/or its affiliates. All rights reserved.
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

#ifndef SHARE_OOPS_STACKCHUNKOOP_INLINE_HPP
#define SHARE_OOPS_STACKCHUNKOOP_INLINE_HPP

#include "oops/stackChunkOop.hpp"

#include "gc/shared/collectedHeap.hpp"
#include "gc/shared/barrierSet.hpp"
#include "gc/shared/barrierSetStackChunk.hpp"
#include "gc/shared/gc_globals.hpp"
#include "memory/memRegion.hpp"
#include "memory/universe.hpp"
#include "oops/access.inline.hpp"
#include "oops/instanceStackChunkKlass.inline.hpp"
#include "runtime/continuationJavaClasses.inline.hpp"
#include "runtime/frame.hpp"
#include "runtime/globals.hpp"
#include "runtime/handles.inline.hpp"
#include "runtime/objectMonitor.hpp"
#include "runtime/registerMap.hpp"
#include "runtime/smallRegisterMap.inline.hpp"
#include "utilities/macros.hpp"
#include CPU_HEADER_INLINE(stackChunkOop)


namespace svm_gc {

inline stackChunkOop stackChunkOopDesc::cast(oop obj) {
  assert(obj == nullptr || obj->is_stackChunk(), "Wrong type");
  return stackChunkOop(obj);
}


} // namespace svm_gc

#endif // SHARE_OOPS_STACKCHUNKOOP_INLINE_HPP
