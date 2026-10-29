/*
 * Copyright (c) 2016, 2023, Oracle and/or its affiliates. All rights reserved.
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

#ifndef SHARE_OOPS_ARRAYKLASS_INLINE_HPP
#define SHARE_OOPS_ARRAYKLASS_INLINE_HPP

#include "oops/arrayKlass.hpp"

#include "runtime/atomic.hpp"
// NOTE (chaeubl): based on objArrayOop::size(int)
static int array_object_size(int base_offset_in_bytes, int bytes_per_elem, int length) {
  // This returns the object size in HeapWords.
  size_t asz = (size_t)length * bytes_per_elem;
  size_t size_words = heap_word_size(base_offset_in_bytes + asz);
  size_t osz = align_object_size(size_words);
  assert(osz < max_jint, "no overflow");
  return osz;
}

// NOTE (chaeubl): heavily simplified version of arrayOop::max_array_length(BasicType)
static int32_t max_array_length(int header_size_in_bytes, int bytes_per_elem) {
  // It should be ok to return max_jint here, but parts of the code
  // (CollectedHeap, Klass::oop_oop_iterate(), and more) uses an int for
  // passing around the size (in words) of an object. So, we need to avoid
  // overflowing an int when we add the header. See CRs 4718400 and 7110613.
  NOT_LP64(guarantee(false, "the code below assumes that we are on a 64-bit platform"));
  int header_size_rounded_to_words = (header_size_in_bytes + HeapWordSize - 1) >> LogHeapWordSize;
  return align_down(max_jint - header_size_rounded_to_words, MinObjAlignment);
}

int ArrayKlass::object_size(int length) const {
  return array_object_size(base_offset_in_bytes(), 1 << log2_element_size(), length);
}

jint ArrayKlass::max_length() const {
  assert(log2_element_size() >= 0, "bad scale");
  return max_array_length(base_offset_in_bytes(), 1 << log2_element_size());
}

#endif // SHARE_OOPS_ARRAYKLASS_INLINE_HPP
