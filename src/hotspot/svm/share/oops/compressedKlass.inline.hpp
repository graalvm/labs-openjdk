/*
 * Copyright (c) 2017, 2025, Oracle and/or its affiliates. All rights reserved.
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

#ifndef SHARE_OOPS_COMPRESSEDKLASS_INLINE_HPP
#define SHARE_OOPS_COMPRESSEDKLASS_INLINE_HPP

#include "oops/compressedKlass.hpp"

#include "memory/universe.hpp"
#include "oops/oop.hpp"
#include "utilities/align.hpp"
#include "utilities/globalDefinitions.hpp"


namespace svm_gc {

inline bool CompressedKlassPointers::is_null(Klass* v) {
  return v == nullptr;
}

inline bool CompressedKlassPointers::is_null(narrowKlass v) {
  return extract_compressed_klass(v) == 0;
}

inline compressedKlass CompressedKlassPointers::extract_compressed_klass(narrowKlass v) {
#ifdef SVM_COMPRESSED_REFERENCES
  return (compressedKlass)(v >> markWord::num_reserved_klass_ptr_bits);
#else
  return v;
#endif // SVM_COMPRESSED_REFERENCES
}

inline Klass* CompressedKlassPointers::decode_not_null_without_asserts(narrowKlass v) {
  return (Klass*)((uintptr_t)CompressedOops::base() + ((uintptr_t)extract_compressed_klass(v) << CompressedOops::shift()));
}

inline Klass* CompressedKlassPointers::decode_without_asserts(narrowKlass v) {
  return is_null(v) ? nullptr : decode_not_null_without_asserts(v);
}

inline Klass* CompressedKlassPointers::decode_not_null(narrowKlass v) {
  assert(!is_null(v), "narrow klass value can never be zero");
  return decode_not_null_without_asserts(v);
}

inline Klass* CompressedKlassPointers::decode(narrowKlass v) {
  return is_null(v) ? nullptr : decode_not_null(v);
}

inline narrowKlass CompressedKlassPointers::encode_not_null_without_asserts(Klass* v) {
  uint64_t result = (uint64_t)(pointer_delta(v, CompressedOops::base(), 1));
#ifdef SVM_COMPRESSED_REFERENCES
  result = result << (markWord::num_reserved_klass_ptr_bits - CompressedOops::shift());
  assert((result & markWord::reserved_klass_ptr_bits_mask) == 0, "must be");
#endif // SVM_COMPRESSED_REFERENCES
  assert(decode_not_null_without_asserts(result) == v, "reversibility");
  return (narrowKlass)result;
}

inline narrowKlass CompressedKlassPointers::encode_not_null(Klass* v) {
  assert(!is_null(v), "klass value can never be zero");
  return encode_not_null_without_asserts(v);
}

inline narrowKlass CompressedKlassPointers::encode(Klass* v) {
  return is_null(v) ? (narrowKlass)0 : encode_not_null(v);
}


} // namespace svm_gc

#endif // SHARE_OOPS_COMPRESSEDKLASS_INLINE_HPP
