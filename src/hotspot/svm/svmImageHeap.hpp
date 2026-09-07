/*
 * Copyright (c) 2019, 2026, Oracle and/or its affiliates. All rights reserved.
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

#ifndef SVM_IMAGE_HEAP_HPP
#define SVM_IMAGE_HEAP_HPP

#include "oops/oop.hpp"

class SVMImageHeap : public AllStatic {
public:
  static inline bool is_image_heap_object(oop object) {
    return is_in_image_heap(cast_from_oop<HeapWord*>(object));
  }

  static inline bool is_closed_image_heap_object(oop object) {
    return is_in_closed_image_heap(cast_from_oop<HeapWord*>(object));
  }

  static inline bool is_open_image_heap_object(oop object) {
    return is_in_open_image_heap(cast_from_oop<HeapWord*>(object));
  }

  static inline bool is_in_image_heap(const HeapWord *heapWord) {
    assert(SVMIsolateData::_closed_image_heap_start_addr < SVMIsolateData::_open_image_heap_end_addr, "must be");
    char *addr = (char*)heapWord;
    return addr >= SVMIsolateData::_closed_image_heap_start_addr && addr < SVMIsolateData::_open_image_heap_end_addr;
  }

  static inline bool is_in_closed_image_heap(const HeapWord *heapWord) {
    char *addr = (char*)heapWord;
    return addr >= SVMIsolateData::_closed_image_heap_start_addr && addr < SVMIsolateData::_closed_image_heap_end_addr;
  }

  static inline bool is_in_open_image_heap(const HeapWord *heapWord) {
    char *addr = (char*)heapWord;
    return addr >= SVMIsolateData::_open_image_heap_start_addr && addr < SVMIsolateData::_open_image_heap_end_addr;
  }
};

#endif // SVM_IMAGE_HEAP_HPP
