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

#ifndef SHARE_OOPS_TYPEARRAYKLASS_HPP
#define SHARE_OOPS_TYPEARRAYKLASS_HPP

#include "oops/arrayKlass.hpp"

// A TypeArrayKlass is the klass of a typeArray
// It contains the type and size of the elements


namespace svm_gc {

class TypeArrayKlass : public ArrayKlass {
 public:
  static const KlassKind Kind = TypeArrayKlassKind;

 private:
  // The implementation used by all oop_oop_iterate functions in TypeArrayKlasses.
  inline void oop_oop_iterate_impl(oop obj, OopIterateClosure* closure);

 public:
  // Wraps oop_oop_iterate_impl to conform to macros.
  template <typename T, typename OopClosureType>
  inline void oop_oop_iterate(oop obj, OopClosureType* closure);

  // Wraps oop_oop_iterate_impl to conform to macros.
  template <typename T, typename OopClosureType>
  inline void oop_oop_iterate_bounded(oop obj, OopClosureType* closure, MemRegion mr);

  // Wraps oop_oop_iterate_impl to conform to macros.
  template <typename T, typename OopClosureType>
  inline void oop_oop_iterate_reverse(oop obj, OopClosureType* closure);

 public:
  static TypeArrayKlass* cast(Klass* k) {
    return const_cast<TypeArrayKlass*>(cast(const_cast<const Klass*>(k)));
  }

  static const TypeArrayKlass* cast(const Klass* k) {
    assert(k->is_typeArray_klass(), "cast to TypeArrayKlass");
    return static_cast<const TypeArrayKlass*>(k);
  }
};


} // namespace svm_gc

#endif // SHARE_OOPS_TYPEARRAYKLASS_HPP
