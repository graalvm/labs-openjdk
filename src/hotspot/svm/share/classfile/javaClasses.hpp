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

#ifndef SHARE_CLASSFILE_JAVACLASSES_HPP
#define SHARE_CLASSFILE_JAVACLASSES_HPP

#include "classfile/vmClasses.hpp"
#include "oops/instanceKlass.hpp"
#include "oops/oopsHierarchy.hpp"
#include "oops/symbol.hpp"
#include "runtime/handles.hpp"
#include "runtime/os.hpp"
#include "utilities/macros.hpp"
#include "utilities/utf8.hpp"
#include "utilities/vmEnums.hpp"

namespace svm_gc {

class java_lang_String: AllStatic {
 public:

  // Coders
  enum Coder {
    CODER_LATIN1 =  0,
    CODER_UTF16  =  1
  };

  static inline jbyte* base(oop string);

  // Instance creation
  static inline void set_value(oop string, typeArrayOop buffer);

  // Set the deduplication_forbidden flag true.  This flag is sticky; once
  // set it never gets cleared.  This is set when a String is interned in
  // the StringTable, to prevent string deduplication from changing the
  // String's value array.
  static inline void set_deduplication_forbidden(oop java_string);

  // Test and set the deduplication_requested flag.  Returns the old value
  // of the flag.  This flag is sticky; once set it never gets cleared.
  // Some GCs may use this flag when deciding whether to request
  // deduplication of a String, to avoid multiple requests for the same
  // object.
  static inline bool test_and_set_deduplication_requested(oop java_string);

  // Accessors
  static inline typeArrayOop value(oop java_string);
  static inline typeArrayOop value_no_keepalive(oop java_string);
  static inline bool is_latin1(oop java_string);
  static inline bool deduplication_forbidden(oop java_string);
  static inline int length(oop java_string);
  static inline int length(oop java_string, int arr_length);

  // Compare strings (of different types/encodings), length is the string (array) length
  static bool equals(oop java_string, const char* chars);
  static inline bool value_equals(typeArrayOop str_value1, typeArrayOop str_value2);

  // Testers
  static inline bool is_instance(oop obj);

  static char* as_latin1(oop string);
};

// Interface to java.lang.ref.Reference objects

class java_lang_ref_Reference: AllStatic {
 public:
  // Accessors
  static inline oop weak_referent_no_keepalive(oop ref);
  static inline oop phantom_referent_no_keepalive(oop ref);
  static inline oop unknown_referent_no_keepalive(oop ref);
  static inline void clear_referent_raw(oop ref);
  static inline HeapWord* referent_addr_raw(oop ref);
  static inline oop next(oop ref);
  static inline void set_next(oop ref, oop value);
  static inline void set_next_raw(oop ref, oop value);
  static inline oop discovered(oop ref);
  static inline void set_discovered_raw(oop ref, oop value);
  static inline HeapWord* discovered_addr_raw(oop ref);
  static bool is_referent_field(oop obj, ptrdiff_t offset);
  static inline bool is_final(oop ref);
  static inline bool is_phantom(oop ref);
  static inline bool is_weak(oop ref);
  static inline bool is_soft(oop ref);

  static int referent_offset()    { return SVMGlobalData::_offsets._reference._referent; }
  static int next_offset()        { return SVMGlobalData::_offsets._reference._next; }
  static int discovered_offset()  { return SVMGlobalData::_offsets._reference._discovered; }
};

// Interface to java.lang.ref.SoftReference objects

class java_lang_ref_SoftReference: public java_lang_ref_Reference {
 public:
  // Accessors
  static jlong timestamp(oop ref);

  // Accessors for statics
  static jlong clock();
  static void set_clock(jlong value);
};


} // namespace svm_gc

#endif // SHARE_CLASSFILE_JAVACLASSES_HPP
