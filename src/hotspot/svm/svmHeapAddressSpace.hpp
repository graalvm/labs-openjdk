/*
 * Copyright (c) 2026, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Oracle designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Oracle in the LICENSE file that accompanied this code.
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
 */

#ifndef SVM_HEAP_ADDRESS_SPACE_HPP
#define SVM_HEAP_ADDRESS_SPACE_HPP

#include "oops/oop.hpp"
#include "svmGlobalData.hpp"
#include "svmImageHeap.hpp"
#include "svmMetaspace.hpp"


namespace svm_gc {

// Classifies addresses in the adjacent metaspace and image-heap reservations.
class SVMHeapAddressSpace : public AllStatic {
public:
  static inline bool is_image_heap_or_metaspace_object(oop object) {
    return is_in_image_heap_or_metaspace(cast_from_oop<HeapWord*>(object));
  }

  // Checks reserved address ranges, not whether memory at the address was allocated.
  static inline bool is_open_image_heap_or_metaspace_object(oop object) {
    return SVMImageHeap::is_open_image_heap_object(object) || SVMMetaspace::is_in_address_space(object);
  }

  // Checks the reserved address range, not whether memory at the address was allocated.
  static inline bool is_in_image_heap_or_metaspace(const void* address) {
    assert(SVMIsolateData::_metaspace_start_addr < SVMIsolateData::_open_image_heap_end_addr, "must be");
    const char* addr = (const char*)address;
    return addr >= SVMIsolateData::_metaspace_start_addr && addr < SVMIsolateData::_open_image_heap_end_addr;
  }

};


} // namespace svm_gc

#endif // SVM_HEAP_ADDRESS_SPACE_HPP
