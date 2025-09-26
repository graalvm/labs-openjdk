/*
 * Copyright (c) 2015, 2025, Oracle and/or its affiliates. All rights reserved.
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

#ifndef SHARE_CLASSFILE_JAVACLASSES_INLINE_HPP
#define SHARE_CLASSFILE_JAVACLASSES_INLINE_HPP

#include "classfile/javaClasses.hpp"

#include "memory/referenceType.hpp"
#include "oops/access.inline.hpp"
#include "oops/instanceKlass.inline.hpp"
#include "oops/method.hpp"
#include "oops/oop.inline.hpp"
#include "oops/oopsHierarchy.hpp"
#include "oops/typeArrayOop.inline.hpp"
#ifdef SVM
#include "svmImageHeap.hpp"
#endif // SVM

// java.lang.String
jbyte* java_lang_String::base(oop string) {
  if (SVMGlobalData::_use_string_inlining) {
    int array_base_offset = ((TypeArrayKlass*)string->klass())->base_offset_in_bytes();
    return (jbyte*)(((address) string) + array_base_offset);
  } else {
    typeArrayOop value = java_lang_String::value_no_keepalive(string);
    return (jbyte*)value->base();
  }
}

void java_lang_String::set_value(oop string, typeArrayOop buffer) {
  assert(!SVMGlobalData::_use_string_inlining, "must be disabled");
  string->obj_field_put(SVMGlobalData::_offsets._string._value, cast_to_oop(buffer));
}

void java_lang_String::set_deduplication_forbidden(oop java_string) {
  ShouldNotReachHere();
}

bool java_lang_String::test_and_set_deduplication_requested(oop java_string) {
  ShouldNotReachHere();
  return false;
}

bool java_lang_String::value_equals(typeArrayOop str_value1, typeArrayOop str_value2) {
  assert(!SVMGlobalData::_use_string_inlining, "must be disabled");
  return ((str_value1 == str_value2) ||
          (str_value1->length() == str_value2->length() &&
           (!memcmp(str_value1->base(),
                    str_value2->base(),
                    str_value2->length() * sizeof(jbyte)))));
}

typeArrayOop java_lang_String::value(oop java_string) {
  assert(is_instance(java_string), "must be java_string");
  assert(!SVMGlobalData::_use_string_inlining, "must be disabled");
  return (typeArrayOop) java_string->obj_field(SVMGlobalData::_offsets._string._value);
}

typeArrayOop java_lang_String::value_no_keepalive(oop java_string) {
  assert(is_instance(java_string), "must be java_string");
  assert(!SVMGlobalData::_use_string_inlining, "must be disabled");
  return (typeArrayOop)((oop)RawAccess<>::oop_load_at(java_string, SVMGlobalData::_offsets._string._value));
}

bool java_lang_String::is_latin1(oop java_string) {
  assert(is_instance(java_string), "must be java_string");
  if (SVMGlobalData::_offsets._string._coder > 0) {
    jbyte coder = *((jboolean*)(((address) java_string) + SVMGlobalData::_offsets._string._coder));
    return coder == CODER_LATIN1;
  }
  return true;
}

bool java_lang_String::deduplication_forbidden(oop java_string) {
  ShouldNotReachHere();
  return true;
}

int java_lang_String::length(oop java_string, int arr_length) {
  assert(is_instance(java_string), "must be java_string");
  if (!is_latin1(java_string)) {
    assert((arr_length & 1) == 0, "should be even for UTF16 string");
    arr_length >>= 1; // convert number of bytes to number of elements
  }
  return arr_length;
}

int java_lang_String::length(oop java_string) {
  assert(is_instance(java_string), "must be java_string");
  if (SVMGlobalData::_use_string_inlining) {
    // Strings are not real arrays (so we must not use typeArrayOop when accessing characters) but the length is stored in the same place.
    int arr_length = ((typeArrayOop)java_string)->length();
    return length(java_string, arr_length);
  } else {
    typeArrayOop value = java_lang_String::value_no_keepalive(java_string);
    if (value == nullptr) {
      return 0;
    }
    return length(java_string, value->length());
  }
}

bool java_lang_String::is_instance(oop obj) {
  return obj != nullptr && obj->klass() == vmClasses::String_klass();
}

// java.lang.Reference
oop java_lang_ref_Reference::weak_referent_no_keepalive(oop ref) {
  assert(java_lang_ref_Reference::is_weak(ref) || java_lang_ref_Reference::is_soft(ref), "must be Weak or Soft Reference");
  return ref->obj_field_access<ON_WEAK_OOP_REF | AS_NO_KEEPALIVE>(SVMGlobalData::_offsets._reference._referent);
}

oop java_lang_ref_Reference::phantom_referent_no_keepalive(oop ref) {
  assert(java_lang_ref_Reference::is_phantom(ref), "must be Phantom Reference");
  return ref->obj_field_access<ON_PHANTOM_OOP_REF | AS_NO_KEEPALIVE>(SVMGlobalData::_offsets._reference._referent);
}

oop java_lang_ref_Reference::unknown_referent_no_keepalive(oop ref) {
  return ref->obj_field_access<ON_UNKNOWN_OOP_REF | AS_NO_KEEPALIVE>(SVMGlobalData::_offsets._reference._referent);
}

void java_lang_ref_Reference::clear_referent_raw(oop ref) {
  assert(!SVMImageHeap::is_image_heap_object(ref), "referent field must not be cleared if it points to an object in the image heap");
  ref->obj_field_put_raw(SVMGlobalData::_offsets._reference._referent, nullptr);
}

HeapWord* java_lang_ref_Reference::referent_addr_raw(oop ref) {
  return ref->field_addr<HeapWord>(SVMGlobalData::_offsets._reference._referent);
}

oop java_lang_ref_Reference::next(oop ref) {
  return ref->obj_field(SVMGlobalData::_offsets._reference._next);
}

void java_lang_ref_Reference::set_next(oop ref, oop value) {
  ref->obj_field_put(SVMGlobalData::_offsets._reference._next, value);
}

void java_lang_ref_Reference::set_next_raw(oop ref, oop value) {
  ref->obj_field_put_raw(SVMGlobalData::_offsets._reference._next, value);
}

oop java_lang_ref_Reference::discovered(oop ref) {
  return ref->obj_field(SVMGlobalData::_offsets._reference._discovered);
}

void java_lang_ref_Reference::set_discovered_raw(oop ref, oop value) {
  ref->obj_field_put_raw(SVMGlobalData::_offsets._reference._discovered, value);
}

HeapWord* java_lang_ref_Reference::discovered_addr_raw(oop ref) {
  return ref->field_addr<HeapWord>(SVMGlobalData::_offsets._reference._discovered);
}

bool java_lang_ref_Reference::is_final(oop ref) {
  return InstanceKlass::cast(ref->klass())->reference_type() == REF_FINAL;
}

bool java_lang_ref_Reference::is_phantom(oop ref) {
  return InstanceKlass::cast(ref->klass())->reference_type() == REF_PHANTOM;
}

bool java_lang_ref_Reference::is_weak(oop ref) {
  return InstanceKlass::cast(ref->klass())->reference_type() == REF_WEAK;
}

bool java_lang_ref_Reference::is_soft(oop ref) {
  return InstanceKlass::cast(ref->klass())->reference_type() == REF_SOFT;
}

#endif // SHARE_CLASSFILE_JAVACLASSES_INLINE_HPP
