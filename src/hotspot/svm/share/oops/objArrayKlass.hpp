/*
 * Copyright (c) 1997, 2025, Oracle and/or its affiliates. All rights reserved.
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

#ifndef SHARE_OOPS_OBJARRAYKLASS_HPP
#define SHARE_OOPS_OBJARRAYKLASS_HPP

#include "oops/arrayKlass.hpp"
#include "utilities/macros.hpp"

// ObjArrayKlass is the klass for objArrays

class ObjArrayKlass : public ArrayKlass {
 public:
  static const KlassKind Kind = ObjArrayKlassKind;

 public:

  // Oop fields (and metadata) iterators
  //
  // The ObjArrayKlass iterators also visits the Object's klass.

  // Iterate over oop elements and metadata.
  template <typename T, typename OopClosureType>
  inline void oop_oop_iterate(oop obj, OopClosureType* closure);

  // Iterate over oop elements and metadata.
  template <typename T, typename OopClosureType>
  inline void oop_oop_iterate_reverse(oop obj, OopClosureType* closure);

  // Iterate over oop elements within mr, and metadata.
  template <typename T, typename OopClosureType>
  inline void oop_oop_iterate_bounded(oop obj, OopClosureType* closure, MemRegion mr);

  // Iterate over oop elements within [start, end), and metadata.
  template <typename T, class OopClosureType>
  inline void oop_oop_iterate_range(objArrayOop a, OopClosureType* closure, int start, int end);

 public:
  // Iterate over all oop elements.
  template <typename T, class OopClosureType>
  inline void oop_oop_iterate_elements(objArrayOop a, OopClosureType* closure);

 private:
  // Iterate over all oop elements with indices within mr.
  template <typename T, class OopClosureType>
  inline void oop_oop_iterate_elements_bounded(objArrayOop a, OopClosureType* closure, void* low, void* high);
};

#endif // SHARE_OOPS_OBJARRAYKLASS_HPP
