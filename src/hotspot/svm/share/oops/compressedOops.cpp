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

#include "logging/log.hpp"
#include "logging/logStream.hpp"
#include "memory/memRegion.hpp"
#include "memory/reservedSpace.hpp"
#include "memory/resourceArea.hpp"
#include "memory/universe.hpp"
#include "oops/compressedOops.hpp"
#include "gc/shared/collectedHeap.hpp"
#include "runtime/arguments.hpp"
#include "runtime/globals.hpp"
#ifdef SVM
#include "gc/shared/gc_globals.hpp"
#endif // SVM

// For UseCompressedOops.

namespace svm_gc {

MemRegion CompressedOops::_heap_address_range;

void CompressedOops::initialize() {
  address heap_start = ((address)SVMIsolateData::_heap_base) + SVMGlobalData::_null_regions_size;
  address heap_end = heap_start + MaxHeapSize;

  _heap_address_range = MemRegion((HeapWord*)heap_start, (HeapWord*)heap_end);

  // base() is one page below the heap.
  assert((intptr_t)base() <= ((intptr_t)_heap_address_range.start() - os::vm_page_size()) ||
         base() == nullptr, "invalid value");
  assert(CompressedOopShift == LogMinObjAlignmentInBytes ||
         CompressedOopShift == 0, "invalid value");
}

bool CompressedOops::is_in(void* addr) {
  return _heap_address_range.contains(addr);
}


} // namespace svm_gc

