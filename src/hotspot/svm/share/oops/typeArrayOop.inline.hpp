/*
 * Copyright (c) 1997, 2020, Oracle and/or its affiliates. All rights reserved.
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

#ifndef SHARE_OOPS_TYPEARRAYOOP_INLINE_HPP
#define SHARE_OOPS_TYPEARRAYOOP_INLINE_HPP

#include "oops/typeArrayOop.hpp"

#include "oops/access.inline.hpp"
#include "oops/oop.inline.hpp"
#include "oops/arrayOop.hpp"


namespace svm_gc {

inline jchar*    typeArrayOopDesc::char_base()   const { return (jchar*)   base(); }
inline jbyte*    typeArrayOopDesc::byte_base()   const { return (jbyte*)   base(); }
inline jint*     typeArrayOopDesc::int_base()    const { return (jint*)    base(); }
inline jlong*    typeArrayOopDesc::long_base()   const { return (jlong*)   base(); }

inline jbyte* typeArrayOopDesc::byte_at_addr(int which) const {
  assert(is_within_bounds(which), "index %d out of bounds %d", which, length());
  return &byte_base()[which];
}

inline jchar* typeArrayOopDesc::char_at_addr(int which) const {
  assert(is_within_bounds(which), "index %d out of bounds %d", which, length());
  return &char_base()[which];
}

inline jint* typeArrayOopDesc::int_at_addr(int which) const {
  assert(is_within_bounds(which), "index %d out of bounds %d", which, length());
  return &int_base()[which];
}

inline jlong* typeArrayOopDesc::long_at_addr(int which) const {
  assert(is_within_bounds(which), "index %d out of bounds %d", which, length());
  return &long_base()[which];
}

inline jbyte typeArrayOopDesc::byte_at(int which) const {
  return *byte_at_addr(which);
}
inline void typeArrayOopDesc::byte_at_put(int which, jbyte contents) {
  *byte_at_addr(which) = contents;
}

inline jchar typeArrayOopDesc::char_at(int which) const {
  return *char_at_addr(which);
}
inline void typeArrayOopDesc::char_at_put(int which, jchar contents) {
  *char_at_addr(which) = contents;
}

inline jint typeArrayOopDesc::int_at(int which) const {
  return *int_at_addr(which);
}
inline void typeArrayOopDesc::int_at_put(int which, jint contents) {
  *int_at_addr(which) = contents;
}

inline jlong typeArrayOopDesc::long_at(int which) const {
  return *long_at_addr(which);
}
inline void typeArrayOopDesc::long_at_put(int which, jlong contents) {
  *long_at_addr(which) = contents;
}

inline void* typeArrayOopDesc::word_at(int which) const {
  return (void*)long_at(which);
}


} // namespace svm_gc

#endif // SHARE_OOPS_TYPEARRAYOOP_INLINE_HPP
