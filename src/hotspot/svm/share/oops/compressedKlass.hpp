/*
 * Copyright (c) 2019, 2025, Oracle and/or its affiliates. All rights reserved.
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

#ifndef SHARE_OOPS_COMPRESSEDKLASS_HPP
#define SHARE_OOPS_COMPRESSEDKLASS_HPP

#include "memory/allStatic.hpp"
#include "utilities/align.hpp"
#include "utilities/globalDefinitions.hpp"
#ifdef SVM
#include "oops/oopsHierarchy.hpp"
#endif // SVM

// NOTE (chaeubl): For SVM, everything around narrowKlass is a bit more complicated.
// - narrowKlass (32 bit):
//     This format is used for the hub reference in the object header. If compressed oops are enabled, a few bits
//     are reserved and used for mark word data. Because of those mark word bits, we can hardly do anything with a
//     narrowKlass and always need to convert it to a compressedKlass first. Unlike a narrowOop, a narrowKlass
//     always has 32 bit.
//
// - compressedKlass (32 bits):
//     Never contains any mark word data and is therefore similar to a narrowOop (i.e., a heap-relative reference).
//     However, unlike a narrowOop, a compressedKlass always has 32 bit. A compressedKlass must be uncompressed to
//     a Klass* before the data can be accessed.
//
// - Klass* (64 bits):
//     The uncompressed Klass pointer that can be accessed directly.
typedef juint narrowKlass;

#ifdef SVM_COMPRESSED_REFERENCES
typedef juint compressedKlass;
#else
typedef julong compressedKlass;
#endif // SVM_COMPRESSED_REFERENCES

// For UseCompressedClassPointers.

class CompressedKlassPointers : public AllStatic {
private:
  static inline Klass* decode_not_null_without_asserts(narrowKlass v);

public:
  static inline bool is_null(Klass* v);
  static inline bool is_null(narrowKlass v);

  static inline Klass* decode_without_asserts(narrowKlass v);
  static inline Klass* decode_not_null(narrowKlass v);
  static inline Klass* decode(narrowKlass v);

  static inline narrowKlass encode_not_null_without_asserts(Klass* v);
  static inline narrowKlass encode_not_null(Klass* v);
  static inline narrowKlass encode(Klass* v);

private:
  static inline compressedKlass extract_compressed_klass(narrowKlass v);
};

#endif // SHARE_OOPS_COMPRESSEDKLASS_HPP
