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

#ifndef SHARE_OOPS_OOP_HPP
#define SHARE_OOPS_OOP_HPP

#include "memory/iterator.hpp"
#include "memory/memRegion.hpp"
#include "oops/compressedKlass.hpp"
#include "oops/accessDecorators.hpp"
#include "oops/markWord.hpp"
#include "oops/metadata.hpp"
#include "oops/objLayout.hpp"
#include "runtime/atomic.hpp"
#include "utilities/globalDefinitions.hpp"
#include "utilities/macros.hpp"
#include <type_traits>
#ifdef SVM
#include "svmGlobalData.hpp"
#endif // SVM

// oopDesc is the top baseclass for objects classes. The {name}Desc classes describe
// the format of Java objects so the fields can be accessed from C++.
// oopDesc is abstract.
// (see oopHierarchy for complete oop class hierarchy)
//
// no virtual functions allowed

class oopDesc {
  friend class VMStructs;
  friend class JVMCIVMStructs;
 private:
  volatile rawMarkWord _raw_mark_word;
  narrowKlass _klass;

  static inline markWord* mark_word_addr(HeapWord* mem);
  static inline narrowKlass* compressed_klass_addr(HeapWord* mem);

  // There may be ordering constraints on the initialization of fields that
  // make use of the C++ copy/assign incorrect.
  NONCOPYABLE(oopDesc);

  inline oop cas_set_forwardee(markWord new_mark, markWord old_mark, atomic_memory_order order);

 public:
  // Must be trivial; see verifying static assert after the class.
  oopDesc() = default;

  inline markWord  mark()      const;
  inline markWord* raw_mark_word_ptr() const;

  inline void set_mark(markWord m);

  inline markWord cas_set_mark(markWord new_mark, markWord old_mark);
  inline markWord cas_set_mark(markWord new_mark, markWord old_mark, atomic_memory_order order);

  // Used only to re-initialize the mark word (e.g., of promoted
  // objects during a GC) -- requires a valid klass pointer
  inline void init_mark();

  inline Klass* klass() const;
  inline Klass* klass_or_null() const;
  inline Klass* klass_or_null_acquire() const;
  // Get the klass without running any asserts.
  inline Klass* klass_without_asserts() const;

  static inline void initialize_obj_header(HeapWord* mem, Klass* klass);

  // For klass field compression
  static inline void set_klass_gap(HeapWord* mem, int z);

  // Size of object header in words
  static int header_size() {
    // NOTE (chaeubl): the returned value can be too large. However, the same applies to the original HotSpot code as well.
    return SVMGlobalData::_offsets._object_layout._min_obj_size >> LogHeapWordSize;
  }

  inline bool is_a(Klass* k) const;

  // Returns the actual oop size of the object in machine words
  inline size_t size();

  // Sometimes (for complicated concurrency-related reasons), it is useful
  // to be able to figure out the size of an object knowing its klass.
  inline size_t size_given_klass(Klass* klass);

  // type test operations (inlined in oop.inline.hpp)
  inline bool is_instance()    const;
  inline bool is_instanceRef() const;
  inline bool is_pod()         const;
  inline bool is_stackChunk()  const;
  // NOTE (chaeubl): is_array(), is_objArray(), and is_typeArray() are potentially dangerous because they
  // return false for hybrid objects. This can be misleading but it usually does not cause any problems
  // because the array part of a hybrid object can only contain primitive data at the moment.
  inline bool is_array()       const;
  inline bool is_objArray()    const;
  inline bool is_typeArray()   const;

 protected:
  inline oop        as_oop() const { return const_cast<oopDesc*>(this); }

 public:
  template<typename T>
  inline T* field_addr(int offset) const;

  // Access to fields in a instanceOop through these methods.
  template<DecoratorSet decorator>
  oop obj_field_access(int offset) const;
  oop obj_field(int offset) const;
  void obj_field_put(int offset, oop value);
  void obj_field_put_raw(int offset, oop value);

  jlong long_field(int offset) const;
  void long_field_put(int offset, jlong contents);

  // printing functions for VM debugging
  void print_on(outputStream* st) const;        // First level print
  void print_value_on(outputStream* st) const;  // Second level print.
  void print_address_on(outputStream* st) const; // Address printing

  // printing on default output stream
  void print();

  // asserts and guarantees
  static bool is_oop(oop obj, bool ignore_mark_word = false);
  static bool is_oop_or_null(oop obj, bool ignore_mark_word = false);

  // garbage collection
  inline bool is_gc_marked() const;

  // Forward pointer operations for scavenge
  inline bool is_forwarded() const;
  inline bool is_self_forwarded() const;

  inline void forward_to(oop p);
  inline void forward_to_self();

  // Like "forward_to", but inserts the forwarding pointer atomically.
  // Exactly one thread succeeds in inserting the forwarding pointer, and
  // this call returns null for that thread; any other thread has the
  // value of the forwarding pointer returned and does not modify "this".
  inline oop forward_to_atomic(oop p, markWord compare, atomic_memory_order order = memory_order_conservative);
  inline oop forward_to_self_atomic(markWord compare, atomic_memory_order order = memory_order_conservative);

  inline oop forwardee() const;
  inline oop forwardee(markWord header) const;

  inline void unset_self_forwarded();

  // Age of object during scavenge
  inline uint age() const;
  inline void incr_age();

  template <typename OopClosureType>
  inline void oop_iterate(OopClosureType* cl);

  template <typename OopClosureType>
  inline void oop_iterate(OopClosureType* cl, MemRegion mr);

  template <typename OopClosureType>
  inline size_t oop_iterate_size(OopClosureType* cl);

  template <typename OopClosureType>
  inline size_t oop_iterate_size(OopClosureType* cl, MemRegion mr);

  template <typename OopClosureType>
  inline void oop_iterate_backwards(OopClosureType* cl);

  template <typename OopClosureType>
  inline void oop_iterate_backwards(OopClosureType* cl, Klass* klass);

  // Checks if the mark word needs to be preserved
  inline bool mark_must_be_preserved() const;
  inline bool mark_must_be_preserved(markWord m) const;

  inline static bool has_klass_gap() {
    int gap = SVMGlobalData::_offsets._object_layout._min_obj_size - SVMGlobalData::_offsets._object_layout._obj_base;
    assert(gap == 0 || gap == 4, "must be");
    return gap > 0;
  }

  // for code generation
  static int klass_offset_in_bytes()     {
    return offset_of(oopDesc, _klass);
  }
  static int klass_gap_offset_in_bytes() {
    assert(has_klass_gap(), "only applicable to compressed klass pointers");
    return klass_offset_in_bytes() + sizeof(narrowKlass);
  }
};

// An oopDesc is not initialized via a constructor.  Space is allocated in
// the Java heap, and static functions provided here on HeapWord* are used
// to fill in certain parts of that memory.  The allocated memory is then
// treated as referring to an oopDesc.  For that to be valid, the oopDesc
// class must have a trivial default constructor (C++14 3.8/1).
static_assert(std::is_trivially_default_constructible<oopDesc>::value, "required");

#endif // SHARE_OOPS_OOP_HPP
