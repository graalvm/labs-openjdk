/*
 * Copyright (c) 1997, 2024, Oracle and/or its affiliates. All rights reserved.
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

#ifndef SHARE_RUNTIME_CONTINUATIONJAVACLASSES_INLINE_HPP
#define SHARE_RUNTIME_CONTINUATIONJAVACLASSES_INLINE_HPP

#include "runtime/continuationJavaClasses.hpp"

#include "logging/log.hpp"
#include "oops/access.inline.hpp"
#include "oops/oop.inline.hpp"
#include "oops/stackChunkOop.inline.hpp"
#include "runtime/atomic.hpp"

void jdk_internal_vm_StackChunk::set_ip(HeapWord* chunk, address value) {
  // Used by StackChunkAllocator before the Object has been finished.
  *(address*)(((address)chunk) + SVMGlobalData::_offsets._stack_chunk._ip) = value;
}

address jdk_internal_vm_StackChunk::ip(oop chunk) {
  return *(address*)(((address)chunk) + SVMGlobalData::_offsets._stack_chunk._ip);
}

#endif // SHARE_RUNTIME_CONTINUATIONJAVACLASSES_INLINE_HPP
