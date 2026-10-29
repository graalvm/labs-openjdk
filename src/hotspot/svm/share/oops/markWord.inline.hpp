/*
 * Copyright Amazon.com Inc. or its affiliates. All Rights Reserved.
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

#ifndef SHARE_OOPS_MARKWORD_INLINE_HPP
#define SHARE_OOPS_MARKWORD_INLINE_HPP

#include "oops/compressedOops.inline.hpp"
#include "oops/markWord.hpp"

inline markWord markWord::prototype_for_klass(Klass* klass) {
#ifdef SVM_COMPRESSED_REFERENCES
  /* The 64-bit mark word is the combination of the 32-bit narrowKlass and the 32-bit mark word. By default, the 32-bit mark word is 0. */
  narrowKlass encoded_klass = CompressedKlassPointers::encode_not_null(klass);
  uintptr_t value = ((uintptr_t)encoded_klass) << BitsPerInt;
  return markWord(value);
#else
  return markWord::zero();
#endif // SVM_COMPRESSED_REFERENCES
}

narrowKlass markWord::narrow_klass() const {
#ifdef SVM_COMPRESSED_REFERENCES
  assert_svm_only(!is_marked(), "klass is destroyed during forwarding");
  assert(UseCompactObjectHeaders, "only used with compact object headers");
  /* With compressed references enabled, the narrowKlass is stored in the upper 32-bit of the 64-bit mark word. */
  uintptr_t v = value();
  return v >> BitsPerInt;
#else
  ShouldNotReachHere();
  return 0;
#endif
}

Klass* markWord::klass() const {
#ifdef SVM_COMPRESSED_REFERENCES
  return CompressedKlassPointers::decode_not_null(narrow_klass());
#else
  ShouldNotReachHere();
  return 0;
#endif
}

Klass* markWord::klass_or_null() const {
#ifdef SVM_COMPRESSED_REFERENCES
  return CompressedKlassPointers::decode(narrow_klass());
#else
  ShouldNotReachHere();
  return 0;
#endif
}

Klass* markWord::klass_without_asserts() const {
#ifdef SVM_COMPRESSED_REFERENCES
  return CompressedKlassPointers::decode_without_asserts(narrow_klass());
#else
  ShouldNotReachHere();
  return 0;
#endif
}

#endif // SHARE_OOPS_MARKWORD_INLINE_HPP
