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

#ifndef GC_SHARED_FULLGCFORWARDING_INLINE_HPP
#define GC_SHARED_FULLGCFORWARDING_INLINE_HPP

#include "gc/shared/fullGCForwarding.hpp"

#include "oops/oop.inline.hpp"
#include "utilities/globalDefinitions.hpp"


namespace svm_gc {

void FullGCForwarding::forward_to(oop from, oop to) {
  assert(Universe::heap()->is_in(to), "must be in the heap");
  assert(from->klass()->is_klass(), "must be");
#ifdef SVM_COMPRESSED_REFERENCES
  // For consistency reasons, we must clear ALL mark word bits except the klass-related bits.
  uintptr_t mark_word_klass = from->mark().value() & markWord::klass_mask_in_place;
  uintptr_t encoded = ((uintptr_t)CompressedOops::encode(to)) << markWord::fwd_ptr_shift;
  from->set_mark(markWord(mark_word_klass | encoded | markWord::marked_value));
#else
  from->forward_to(to);
#endif // SVM_COMPRESSED_REFERENCES
  assert(forwardee(from) == to, "reversibility");
  assert(from->klass_without_asserts()->is_klass(), "klass must not be destroyed");
}

oop FullGCForwarding::forwardee(oop from) {
  assert(is_forwarded(from), "must be");
#ifdef SVM_COMPRESSED_REFERENCES
  uintptr_t mark = from->mark().value();
  uintptr_t encoded_oop = (mark >> markWord::fwd_ptr_shift) & markWord::fwd_ptr_mask;
  return CompressedOops::decode((narrowOop)encoded_oop);
#else
  return from->forwardee();
#endif // SVM_COMPRESSED_REFERENCES
}

bool FullGCForwarding::is_forwarded(oop obj) {
  return obj->mark().is_forwarded();
}


} // namespace svm_gc

#endif // GC_SHARED_FULLGCFORWARDING_INLINE_HPP
