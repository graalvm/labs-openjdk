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

#ifndef SHARE_OOPS_ARRAYOOP_HPP
#define SHARE_OOPS_ARRAYOOP_HPP

#include "oops/oop.hpp"
#include "utilities/align.hpp"
#include "utilities/globalDefinitions.hpp"

class arrayOopDesc : public oopDesc {
private:
  // Returns the address of the length "field".  See length_offset_in_bytes().
  static int* length_addr_impl(void* obj_ptr) {
    char* ptr = static_cast<char*>(obj_ptr);
    return reinterpret_cast<int*>(ptr + length_offset_in_bytes());
  }

 public:
  static int header_size_in_bytes() {
    return length_offset_in_bytes() + (int)sizeof(int);
  }

  static int length_offset_in_bytes() {
    return SVMGlobalData::_offsets._object_layout._array_length;
  }

  void* base() const;

  template <typename T>
  static T* obj_offset_to_raw(arrayOop obj, size_t offset_in_bytes, T* raw) {
    if (obj != nullptr) {
      assert(raw == nullptr, "either raw or in-heap");
      char* base = reinterpret_cast<char*>((void*) obj);
      raw = reinterpret_cast<T*>(base + offset_in_bytes);
    } else {
      assert(raw != nullptr, "either raw or in-heap");
    }
    return raw;
  }

  // Tells whether index is within bounds.
  bool is_within_bounds(int index) const        { return 0 <= index && index < length(); }

  // Accessors for array length.  There's not a member variable for
  // it; see length_offset_in_bytes().
  int length() const { return *length_addr_impl(const_cast<arrayOopDesc*>(this)); }
  void set_length(int length) { *length_addr_impl(this) = length; }

  int* length_addr() {
    return length_addr_impl(this);
  }
  static void set_length(HeapWord* mem, int length) {
    *length_addr_impl(mem) = length;
  }
};

#endif // SHARE_OOPS_ARRAYOOP_HPP
