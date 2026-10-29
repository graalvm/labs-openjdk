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

#ifndef SHARE_OOPS_MARKWORD_HPP
#define SHARE_OOPS_MARKWORD_HPP

#include "metaprogramming/primitiveConversions.hpp"
#include "oops/compressedKlass.hpp"
#include "oops/oopsHierarchy.hpp"
#include "runtime/globals.hpp"

#include <type_traits>

// NOTE (chaeubl): For SVM, the layout of the 64 bit mark word depends on the following:
// - compressed references: the 64 bit mark word is the combination of the 32 bit Klass pointer and the 32 bit mark word, MSB to LSB:
//     - normal:         27 bit Klass ptr, 31 bit hashcode,                  4 bit age,    2 bit unused
//     - self-forwarded: 27 bit Klass ptr, 31 bit hashcode,                  4 bit age,    1 self-forwarding bit, 1 mark bit
//     - forwarded:      27 bit Klass ptr, 3 bit unused, 32 bit compressed forwarding ptr, 1 self-forwarding bit, 1 mark bit
//
// - uncompressed references: there is an explicit 64 bit mark word at offset 0 with the following layout, MSB to LSB:
//     - normal:         27 bit unused, 31 bit hashcode, 4 bit age, 2 unused bits
//     - self-forwarded: 27 bit unused, 31 bit hashcode, 4 bit age, 1 self-forwarding bit, 1 mark bit
//     - forwarded:      62 bit uncompressed forwarding ptr,        1 self-forwarding bit, 1 mark bit
//
// Note that "forwarded" is only used during full GCs. So, the the identity hashcode only needs to be rescued for objects that survive a full GC.

namespace svm_gc {

class markWord {
 private:
  uintptr_t _value;

 public:
  explicit markWord(uintptr_t value) : _value(value) {}
  markWord() = default;         // Doesn't initialize _value.

  // It is critical for performance that this class be trivially
  // destructable, copyable, and assignable.
  ~markWord() = default;
  markWord(const markWord&) = default;
  markWord& operator=(const markWord&) = default;

  static markWord from_pointer(void* ptr) {
    return markWord((uintptr_t)ptr);
  }

  bool operator==(const markWord& other) const {
    return _value == other._value;
  }
  bool operator!=(const markWord& other) const {
    return !operator==(other);
  }

  // Conversion
  uintptr_t value() const { return _value; }


  // Constants
  static const int mark_bits                      = 1;
  static const int self_fwd_bits                  = 1;
  static const int age_bits                       = 4;
  static const int hash_bits                      = 31;
#ifdef SVM_COMPRESSED_REFERENCES
  static const int klass_bits                     = 27;
  static const int fwd_ptr_bits                   = 32;

  static const int num_reserved_klass_ptr_bits    = 5;
  static const int reserved_klass_ptr_bits_mask   = right_n_bits(num_reserved_klass_ptr_bits);
#endif // SVM_COMPRESSED_REFERENCES

  static const int mark_shift                     = 0;
  static const int self_fwd_shift                 = mark_shift + mark_bits;
  static const int age_shift                      = self_fwd_shift + self_fwd_bits;
  static const int hash_shift                     = age_shift + age_bits;
#ifdef SVM_COMPRESSED_REFERENCES
  static const int klass_shift                    = hash_shift + hash_bits;
  static const int fwd_ptr_shift                  = age_shift;
#endif // SVM_COMPRESSED_REFERENCES

  static const uintptr_t mark_mask                = right_n_bits(mark_bits);
  static const uintptr_t mark_mask_in_place       = mark_mask << mark_shift;
  static const uintptr_t self_fwd_mask            = right_n_bits(self_fwd_bits);
  static const uintptr_t self_fwd_mask_in_place   = self_fwd_mask << self_fwd_shift;
  static const uintptr_t age_mask                 = right_n_bits(age_bits);
  static const uintptr_t age_mask_in_place        = age_mask << age_shift;
  static const uintptr_t hash_mask                = right_n_bits(hash_bits);
  static const uintptr_t hash_mask_in_place       = hash_mask << hash_shift;
#ifdef SVM_COMPRESSED_REFERENCES
  static const uintptr_t klass_mask               = right_n_bits(klass_bits);
  static const uintptr_t klass_mask_in_place      = klass_mask << klass_shift;
  static const uintptr_t fwd_ptr_mask             = right_n_bits(fwd_ptr_bits);
  static const uintptr_t fwd_ptr_mask_in_place    = fwd_ptr_mask << fwd_ptr_shift;
#endif // SVM_COMPRESSED_REFERENCES

  static const uintptr_t marked_value             = 1;

  static const uintptr_t no_hash                  = 0;  // no hash value assigned

  static const uint max_age                       = age_mask;

  // Creates a markWord with all bits set to zero.
  static markWord zero() { return markWord(uintptr_t(0)); }

  bool is_marked()   const {
    return (mask_bits(value(), mark_mask_in_place) != 0);
  }
  bool is_forwarded() const {
    // Returns true if normal forwarded (0b01) or self-forwarded (0b1x).
    return mask_bits(value(), mark_mask_in_place | self_fwd_mask_in_place) != 0;
  }

  // Should this header be preserved during GC?
  bool must_be_preserved() const {
    return !has_no_hash();
  }

  // age operations
  markWord set_marked()   { return markWord((value() & ~mark_mask_in_place) | marked_value); }
  markWord set_unmarked() { return markWord(value() & ~mark_mask_in_place); }

  uint age() const {
    assert(!is_marked(), "age may be destroyed");
    return (uint) mask_bits(value() >> age_shift, age_mask);
  }
  markWord set_age(uint v) const {
    assert(!is_marked(), "would destroy the forwarding pointer");
    assert((v & ~age_mask) == 0, "shouldn't overflow age field");
    return markWord((value() & ~age_mask_in_place) | ((v & age_mask) << age_shift));
  }
  markWord incr_age()      const { return age() == max_age ? markWord(_value) : set_age(age() + 1); }

  // hash operations
  intptr_t hash() const {
    return mask_bits(value() >> hash_shift, hash_mask);
  }

  bool has_no_hash() const {
    return hash() == no_hash;
  }

  inline Klass* klass() const;
  inline Klass* klass_or_null() const;
  inline Klass* klass_without_asserts() const;
  inline narrowKlass narrow_klass() const;

  // Helper function for restoration of unmarked mark oops during GC
  inline static markWord prototype_for_klass(Klass* klass);

  // Prepare address of oop for placement into mark
  inline static markWord encode_pointer_as_mark(void* p) { return from_pointer(p).set_marked(); }

  // Recover address of oop from encoded form used in mark
  inline void* decode_pointer() const {
    assert(is_marked(), "must be");
    return (void*)(value() & ~mark_mask_in_place);
  }

  inline bool is_self_forwarded() const {
    return mask_bits(value(), self_fwd_mask_in_place) != 0;
  }

  inline markWord set_self_forwarded() const {
    return markWord(value() | self_fwd_mask_in_place);
  }

  inline markWord unset_self_forwarded() const {
    return markWord(value() & ~self_fwd_mask_in_place);
  }

  inline oop forwardee() const {
    return cast_to_oop(decode_pointer());
  }
};

// Support atomic operations.
template<>
struct PrimitiveConversions::Translate<markWord> : public std::true_type {
  typedef markWord Value;
  typedef uintptr_t Decayed;

  static Decayed decay(const Value& x) { return x.value(); }
  static Value recover(Decayed x) { return Value(x); }
};


} // namespace svm_gc

#endif // SHARE_OOPS_MARKWORD_HPP
