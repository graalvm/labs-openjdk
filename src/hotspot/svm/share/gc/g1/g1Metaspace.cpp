/*
 * Copyright (c) 2026, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Oracle designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Oracle in the LICENSE file that accompanied this code.
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
 */

#include "gc/g1/g1CollectedHeap.inline.hpp"
#include "gc/g1/g1HeapRegion.inline.hpp"
#include "gc/g1/g1HeapRegionManager.hpp"
#include "oops/arrayOop.hpp"
#include "oops/instanceKlass.hpp"
#include "oops/klass.inline.hpp"
#include "oops/oop.inline.hpp"
#include "runtime/javaThread.hpp"
#include "runtime/mutexLocker.hpp"
#include "utilities/align.hpp"
#include "utilities/copy.hpp"
#include "svmGlobalData.hpp"
#include "gc/g1/g1Metaspace.hpp"


namespace svm_gc {

uint G1Metaspace::_current_region = 0;
uint G1Metaspace::_committed_regions = 0;
size_t G1Metaspace::_used = 0;
size_t G1Metaspace::_used_at_last_gc = 0;
size_t G1Metaspace::_allocation_counts[AllocationKindCount];
size_t G1Metaspace::_allocation_sizes[AllocationKindCount];

void G1Metaspace::initialize() {
  _current_region = 0;
  _committed_regions = 0;
  for (uint i = 0; i < AllocationKindCount; i++) {
    _allocation_counts[i] = 0;
    _allocation_sizes[i] = 0;
  }
  _used = 0;
  _used_at_last_gc = 0;
  SVMMetaspace::set_metaspace(this);
}

void G1Metaspace::update_used_at_last_gc() {
  _used_at_last_gc = _used;
}

size_t G1Metaspace::waste() {
  return SVMGlobalData::_metaspace_size - _used;
}

oop G1Metaspace::allocate_instance(InstanceKlass* klass) {
  assert(klass != nullptr && klass->is_instance_klass(), "must be");
  return allocate(klass, klass->size_helper(), -1, ObjectAllocation);
}

oop G1Metaspace::allocate_array(Klass* klass, int length, AllocationKind allocation_kind) {
  assert(klass != nullptr, "must be");
  assert(length >= 0, "must be");

  int layout_helper = klass->layout_helper();
  int element_shift = Klass::layout_helper_log2_element_size(layout_helper);
  size_t header_size = Klass::layout_helper_header_size(layout_helper);
  if ((size_t)length > ((SIZE_MAX - header_size) >> element_shift)) {
    return nullptr;
  }

  size_t size_in_bytes = header_size + ((size_t)length << element_shift);
  size_t word_size = align_object_size(heap_word_size(size_in_bytes));
  return allocate(klass, word_size, length, allocation_kind);
}

oop G1Metaspace::allocate(Klass* klass, size_t word_size, int array_length, AllocationKind allocation_kind) {
  DEBUG_ONLY(JavaThread::current()->check_for_valid_safepoint_state());
  assert(!G1CollectedHeap::heap()->is_stw_gc_active(), "Allocation during GC pause not allowed");
  assert_heap_not_locked();
  assert(word_size > 0, "must be");

  if (word_size > (size_t)SVMGlobalData::_metaspace_regions * G1HeapRegion::GrainWords) {
    return nullptr;
  }

  MutexLocker ml(Heap_lock);
  // Metaspace objects are humongous only above one full region, unlike ordinary G1's half-region threshold.
  if (word_size > G1HeapRegion::GrainWords) {
    return allocate_humongous(klass, word_size, array_length, allocation_kind);
  }

  G1CollectedHeap* g1h = G1CollectedHeap::heap();

  // A failed allocation must leave the current region available for smaller objects.
  for (uint region_index = _current_region; region_index < (uint)SVMGlobalData::_metaspace_regions; region_index++) {
    G1HeapRegion* region = g1h->_hrm.at(region_index);
    assert(region->is_metaspace(), "must be");

    HeapWord* mem = region->top();
    if (pointer_delta(region->end(), mem) < word_size) {
      continue;
    }
    commit_regions(region_index + 1, true);

    HeapWord* new_top = mem + word_size;
    region->update_bot_for_block(mem, new_top);
    oop result = initialize_object(mem, klass, word_size, array_length);
    region->set_top_release(new_top);
    _current_region = region_index;
    record_allocation(word_size, allocation_kind);
    return result;
  }

  return nullptr;
}

oop G1Metaspace::allocate_humongous(Klass* klass, size_t word_size, int array_length, AllocationKind allocation_kind) {
  G1CollectedHeap* g1h = G1CollectedHeap::heap();
  uint first_region = _current_region;
  while (first_region < (uint)SVMGlobalData::_metaspace_regions && !g1h->_hrm.at(first_region)->is_empty()) {
    first_region++;
  }

  uint region_count = (uint)G1CollectedHeap::humongous_obj_size_in_regions(word_size);
  uint end_region = first_region + region_count;
  if (end_region > (uint)SVMGlobalData::_metaspace_regions) {
    return nullptr;
  }
  commit_regions(end_region, false);

  G1HeapRegion* first = g1h->_hrm.at(first_region);
  HeapWord* memory = first->bottom();
  HeapWord* object_end = memory + word_size;
  first->set_starts_humongous_in_metaspace();

  for (uint i = first_region + 1; i < end_region; i++) {
    G1HeapRegion* region = g1h->_hrm.at(i);
    region->set_continues_humongous_in_metaspace(first);
  }

  oop result = initialize_object(memory, klass, word_size, array_length);
  for (uint i = first_region; i + 1 < end_region; i++) {
    G1HeapRegion* region = g1h->_hrm.at(i);
    region->set_top_release(region->end());
  }
  g1h->_hrm.at(end_region - 1)->set_top_release(object_end);

  _current_region = end_region;
  record_allocation(word_size, allocation_kind);
  return result;
}

oop G1Metaspace::initialize_object(HeapWord* memory, Klass* klass, size_t word_size, int array_length) {
  Copy::fill_to_aligned_words(memory, word_size);
  if (array_length >= 0) {
    arrayOopDesc::set_length(memory, array_length);
  }
  oopDesc::initialize_obj_header(memory, klass);
  return cast_to_oop(memory);
}

void G1Metaspace::record_allocation(size_t word_size, AllocationKind allocation_kind) {
  size_t size_in_bytes = word_size * HeapWordSize;
  G1CollectedHeap::heap()->increase_used(size_in_bytes);
  _used += size_in_bytes;
  _allocation_counts[allocation_kind]++;
  _allocation_sizes[allocation_kind] += size_in_bytes;
}

void G1Metaspace::commit_regions(uint end_region, bool commit_bot) {
  G1CollectedHeap* g1h = G1CollectedHeap::heap();
  while (_committed_regions < end_region) {
    G1HeapRegion* region = g1h->_hrm.at(_committed_regions);
    assert(region->is_metaspace(), "must be");
    g1h->_hrm.commit_metaspace_region(region, commit_bot, g1h->workers());
    _committed_regions++;
  }
}

bool G1Metaspace::is_in_allocated_memory(const void* address) const {
  if (!is_in_address_space(address)) {
    return false;
  }

  G1CollectedHeap* g1h = G1CollectedHeap::heap();
  G1HeapRegion* region = g1h->heap_region_containing_or_null(address);
  const HeapWord* value = static_cast<const HeapWord*>(address);
  return region != nullptr && region->is_metaspace() && value >= region->bottom() && value < region->top_acquire();
}


} // namespace svm_gc
