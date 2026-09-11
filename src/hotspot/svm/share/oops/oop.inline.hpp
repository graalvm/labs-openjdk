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

#ifndef SHARE_OOPS_OOP_INLINE_HPP
#define SHARE_OOPS_OOP_INLINE_HPP

#include "oops/oop.hpp"

#include "memory/universe.hpp"
#include "memory/iterator.inline.hpp"
#include "oops/access.inline.hpp"
#include "oops/arrayKlass.hpp"
#include "oops/arrayOop.hpp"
#include "oops/compressedKlass.inline.hpp"
#include "oops/instanceKlass.hpp"
#include "oops/objLayout.inline.hpp"
#include "oops/markWord.inline.hpp"
#include "oops/oopsHierarchy.hpp"
#include "runtime/atomic.hpp"
#include "runtime/globals.hpp"
#include "utilities/align.hpp"
#include "utilities/debug.hpp"
#include "utilities/macros.hpp"
#include "utilities/globalDefinitions.hpp"
#ifdef SVM
#include "svmImageHeap.hpp"
#endif // SVM

// Implementation of all inlined member functions defined in oop.hpp
// We need a separate file to avoid circular references


namespace svm_gc {

markWord* oopDesc::raw_mark_word_ptr() const {
  ByteSize offset = byte_offset_of(oopDesc, _raw_mark_word);
  return (markWord*) (((char*)this) + in_bytes(offset));
}

markWord oopDesc::mark() const {
  return Atomic::load(raw_mark_word_ptr());
}

void oopDesc::set_mark(markWord m) {
  Atomic::store(raw_mark_word_ptr(), m);
}

markWord oopDesc::cas_set_mark(markWord new_mark, markWord old_mark) {
  return Atomic::cmpxchg(raw_mark_word_ptr(), old_mark, new_mark);
}

markWord oopDesc::cas_set_mark(markWord new_mark, markWord old_mark, atomic_memory_order order) {
  return Atomic::cmpxchg(raw_mark_word_ptr(), old_mark, new_mark, order);
}

void oopDesc::init_mark() {
  set_mark(markWord::prototype_for_klass(klass()));
}

Klass* oopDesc::klass() const {
  return CompressedKlassPointers::decode_not_null(_klass);
}

Klass* oopDesc::klass_or_null() const {
  return CompressedKlassPointers::decode(_klass);
}

Klass* oopDesc::klass_or_null_acquire() const {
  narrowKlass nklass = Atomic::load_acquire(&_klass);
  return CompressedKlassPointers::decode(nklass);
}

Klass* oopDesc::klass_without_asserts() const {
  return CompressedKlassPointers::decode_without_asserts(_klass);
}

markWord* oopDesc::mark_word_addr(HeapWord* mem) {
  ByteSize offset = byte_offset_of(oopDesc, _raw_mark_word);
  return (markWord*) (((char*)mem) + in_bytes(offset));
}

narrowKlass* oopDesc::compressed_klass_addr(HeapWord* mem) {
  return (narrowKlass*) (((char*)mem) + klass_offset_in_bytes());
}

void oopDesc::initialize_obj_header(HeapWord* mem, Klass* klass) {
  assert(klass != nullptr && klass->is_klass(), "incorrect Klass");
#ifdef SVM_COMPRESSED_REFERENCES
  // write the klass ptr and the mark word as a single 64-bit value
  Atomic::release_store(mark_word_addr(mem), markWord::prototype_for_klass(klass));
#else
  Atomic::store(mark_word_addr(mem), markWord::prototype_for_klass(klass));
  Atomic::release_store(compressed_klass_addr(mem), CompressedKlassPointers::encode_not_null(klass));
#endif // SVM_COMPRESSED_REFERENCES
}

void oopDesc::set_klass_gap(HeapWord* mem, int v) {
  assert(has_klass_gap(), "precondition");
  *(int*)(((char*)mem) + klass_gap_offset_in_bytes()) = v;
}

bool oopDesc::is_a(Klass* k) const {
  return klass()->is_subtype_of(k);
}

size_t oopDesc::size()  {
  return size_given_klass(klass());
}

size_t oopDesc::size_given_klass(Klass* klass)  {
  int lh = klass->layout_helper();
  size_t s;

  // lh is now a value computed at class initialization that may hint
  // at the size.  For instances, this is positive and equal to the
  // size.  For arrays, this is negative and provides log2 of the
  // array element size.  For other oops, it is zero and thus requires
  // a virtual call.
  //
  // We go to all this trouble because the size computation is at the
  // heart of phase 2 of mark-compaction, and called for every object,
  // alive or dead.  So the speed here is equal in importance to the
  // speed of allocation.

  if (lh > Klass::_lh_neutral_value) {
    assert(Klass::layout_helper_is_instance(lh), "must be");
    s = lh >> LogHeapWordSize;  // deliver size scaled by wordSize
  } else {
    assert(lh < Klass::_lh_neutral_value, "must be");
    // Second most common case is arrays.  We have to fetch the
    // length of the array, shift (multiply) it appropriately,
    // up to wordSize, add the header, and align to object size.
    size_t size_in_bytes;
    size_t array_length = (size_t) ((arrayOop)this)->length();
    size_in_bytes = array_length << Klass::layout_helper_log2_element_size(lh);
    size_in_bytes += Klass::layout_helper_header_size(lh);

    // This code could be simplified, but by keeping array_header_in_bytes
    // in units of bytes and doing it this way we can round up just once,
    // skipping the intermediate round to HeapWordSize.
    s = align_up(size_in_bytes, MinObjAlignmentInBytes) / HeapWordSize;
  }

  assert(s > 0, "Oop size must be greater than zero, not %zu", s);
  assert(is_object_aligned(s), "Oop size is not properly aligned: %zu", s);
  return s;
}

bool oopDesc::is_instance()    const { return klass()->is_instance_klass();             }
bool oopDesc::is_instanceRef() const { return klass()->is_reference_instance_klass();   }
bool oopDesc::is_pod()         const { return klass()->is_pod_instance_klass();         }
bool oopDesc::is_stackChunk()  const { return klass()->is_stack_chunk_instance_klass(); }
// NOTE (chaeubl): is_array(), is_objArray(), and is_typeArray() are potentially dangerous (see oop.hpp)
bool oopDesc::is_array()       const { return klass()->is_array_klass();                }
bool oopDesc::is_objArray()    const { return klass()->is_objArray_klass();             }
bool oopDesc::is_typeArray()   const { return klass()->is_typeArray_klass();            }


template<typename T>
T*       oopDesc::field_addr(int offset)     const { return reinterpret_cast<T*>(cast_from_oop<intptr_t>(as_oop()) + offset); }

template <DecoratorSet decorators>
inline oop  oopDesc::obj_field_access(int offset) const             { return HeapAccess<decorators>::oop_load_at(as_oop(), offset); }
inline oop  oopDesc::obj_field(int offset) const                    { return HeapAccess<>::oop_load_at(as_oop(), offset);  }


inline void oopDesc::obj_field_put(int offset, oop value)           { HeapAccess<>::oop_store_at(as_oop(), offset, value); }

inline jlong oopDesc::long_field(int offset) const                  { return *field_addr<jlong>(offset);    }
inline void  oopDesc::long_field_put(int offset, jlong value)       { *field_addr<jlong>(offset) = value;   }

bool oopDesc::is_gc_marked() const {
  return mark().is_marked();
}

// Used by scavengers
bool oopDesc::is_forwarded() const {
  return mark().is_forwarded();
}

bool oopDesc::is_self_forwarded() const {
  return mark().is_self_forwarded();
}

// Used by scavengers
void oopDesc::forward_to(oop p) {
  assert(cast_from_oop<oopDesc*>(p) != this,
         "must not be used for self-forwarding, use forward_to_self() instead");
  markWord m = markWord::encode_pointer_as_mark(p);
  assert(m.decode_pointer() == p, "encoding must be reversible");
  set_mark(m);
}

void oopDesc::forward_to_self() {
  set_mark(mark().set_self_forwarded());
}

oop oopDesc::cas_set_forwardee(markWord new_mark, markWord compare, atomic_memory_order order) {
  assert_svm_only(!SVMImageHeap::is_image_heap_object(this), "image heap object must not be forwarded");
  markWord old_mark = cas_set_mark(new_mark, compare, order);
  if (old_mark == compare) {
    return nullptr;
  } else {
    assert(old_mark.is_forwarded(), "must be forwarded here");
    return forwardee(old_mark);
  }
}

oop oopDesc::forward_to_atomic(oop p, markWord compare, atomic_memory_order order) {
  assert_svm_only(!SVMImageHeap::is_image_heap_object(this), "image heap object must not be forwarded");
  assert(cast_from_oop<oopDesc*>(p) != this,
         "must not be used for self-forwarding, use forward_to_self_atomic() instead");
  markWord m = markWord::encode_pointer_as_mark(p);
  assert(forwardee(m) == p, "encoding must be reversible");
  return cas_set_forwardee(m, compare, order);
}

oop oopDesc::forward_to_self_atomic(markWord old_mark, atomic_memory_order order) {
  assert_svm_only(!SVMImageHeap::is_image_heap_object(this), "image heap object must not be forwarded");
  markWord new_mark = old_mark.set_self_forwarded();
  assert(forwardee(new_mark) == cast_to_oop(this), "encoding must be reversible");
  return cas_set_forwardee(new_mark, old_mark, order);
}

oop oopDesc::forwardee(markWord mark) const {
  assert(mark.is_forwarded(), "only decode when actually forwarded");
  if (mark.is_self_forwarded()) {
    return cast_to_oop(this);
  } else {
    return mark.forwardee();
  }
}

// Note that the forwardee is not the same thing as the displaced_mark.
// The forwardee is used when copying during scavenge and mark-sweep.
// It does need to clear the low two locking- and GC-related bits.
oop oopDesc::forwardee() const {
  return forwardee(mark());
}

void oopDesc::unset_self_forwarded() {
  set_mark(mark().unset_self_forwarded());
}

// The following method needs to be MT safe.
uint oopDesc::age() const {
  assert(!mark().is_marked(), "Attempt to read age from forwarded mark");
  return mark().age();
}

void oopDesc::incr_age() {
  assert(!mark().is_marked(), "Attempt to increment age of forwarded mark");
  set_mark(mark().incr_age());
}

template <typename OopClosureType>
void oopDesc::oop_iterate(OopClosureType* cl) {
  OopIteratorClosureDispatch::oop_oop_iterate(cl, this, klass());
}

template <typename OopClosureType>
void oopDesc::oop_iterate(OopClosureType* cl, MemRegion mr) {
  OopIteratorClosureDispatch::oop_oop_iterate(cl, this, klass(), mr);
}

template <typename OopClosureType>
size_t oopDesc::oop_iterate_size(OopClosureType* cl) {
  Klass* k = klass();
  size_t size = size_given_klass(k);
  OopIteratorClosureDispatch::oop_oop_iterate(cl, this, k);
  return size;
}

template <typename OopClosureType>
size_t oopDesc::oop_iterate_size(OopClosureType* cl, MemRegion mr) {
  Klass* k = klass();
  size_t size = size_given_klass(k);
  OopIteratorClosureDispatch::oop_oop_iterate(cl, this, k, mr);
  return size;
}

template <typename OopClosureType>
void oopDesc::oop_iterate_backwards(OopClosureType* cl) {
  oop_iterate_backwards(cl, klass());
}

template <typename OopClosureType>
void oopDesc::oop_iterate_backwards(OopClosureType* cl, Klass* k) {
  // In this assert, we cannot safely access the Klass* with compact headers.
  assert(k == klass(), "wrong klass");
  OopIteratorClosureDispatch::oop_oop_iterate_backwards(cl, this, k);
}

bool oopDesc::mark_must_be_preserved() const {
  return mark_must_be_preserved(mark());
}

bool oopDesc::mark_must_be_preserved(markWord m) const {
  return m.must_be_preserved();
}


} // namespace svm_gc

#endif // SHARE_OOPS_OOP_INLINE_HPP
