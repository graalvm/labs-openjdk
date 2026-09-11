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

#ifndef SHARE_OOPS_OOPSHIERARCHY_HPP
#define SHARE_OOPS_OOPSHIERARCHY_HPP

#include "metaprogramming/primitiveConversions.hpp"
#include "utilities/globalDefinitions.hpp"

#include <type_traits>

// OBJECT hierarchy
// This hierarchy is a representation hierarchy, i.e. if A is a superclass
// of B, A's representation is a prefix of B's representation.

#ifdef SVM_COMPRESSED_REFERENCES

namespace svm_gc {

enum class narrowOop : uint32_t { null = 0 };
typedef juint rawMarkWord;

} // namespace svm_gc

#else

namespace svm_gc {

enum class narrowOop : uint64_t { null = 0 };
typedef julong rawMarkWord;

} // namespace svm_gc

#endif // SVM_COMPRESSED_REFERENCES


namespace svm_gc {

typedef void* OopOrNarrowOopStar;

typedef class oopDesc*                    oop;
typedef class   instanceOopDesc*            instanceOop;
typedef class     podOopDesc*                 podOop;
typedef class     stackChunkOopDesc*          stackChunkOop;
typedef class   arrayOopDesc*               arrayOop;
typedef class     objArrayOopDesc*            objArrayOop;
typedef class     typeArrayOopDesc*           typeArrayOop;

// Cast functions to convert to and from oops.
template <typename T> inline oop cast_to_oop(T value) {
  return (oopDesc*)value;
}
template <typename T> inline T cast_from_oop(oop o) {
  return (T)(CHECK_UNHANDLED_OOPS_ONLY((oopDesc*))o);
}

// The klass hierarchy is separate from the oop hierarchy.

class Klass;
class   InstanceKlass;
class   ArrayKlass;
class     ObjArrayKlass;
class     TypeArrayKlass;


} // namespace svm_gc

#endif // SHARE_OOPS_OOPSHIERARCHY_HPP
