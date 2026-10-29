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

#ifndef SVM_SHARE_GC_G1_G1METASPACE_HPP
#define SVM_SHARE_GC_G1_G1METASPACE_HPP

#include "oops/oop.hpp"
#include "svmMetaspace.hpp"


namespace svm_gc {

class InstanceKlass;
class Klass;

// Manages Java objects allocated in the G1 metaspace regions. Until class unloading is supported,
// metaspace follows the same basic model as the open image heap. Objects in both spaces are always
// live and are never reclaimed, evacuated, or compacted. Concurrent marking traverses them directly
// without recording them in the mark bitmap. Neither space tracks incoming references in its own
// remembered sets or allocates humongous card-set groups. Outgoing references participate in card
// processing and remembered-set rebuilding for collected regions. Reference stores use the normal
// G1 barriers, and evacuation uses the ordinary post-barrier bookkeeping when updating these fields.
//
// Reference processing:
// Unlike java.lang.ref.Reference objects in the image heap, those in metaspace may have their
// referents cleared by full or concurrent GC. Keeping a Reference object alive in metaspace does not
// keep its referent alive.
//
// Allocation and commitment:
// Metaspace allocates objects at run time within a fixed reservation and can fail when it is
// exhausted. All region descriptors are activated at startup, so empty regions are valid. Heap
// memory and the block offset table (BOT) for non-humongous regions are committed on demand. Like
// the image heap, humongous metaspace regions do not need a BOT. The image heap is already mapped,
// and the card table and bitmap for the combined range are committed before either space's regions
// are activated. GR-79974 tracks committing this auxiliary memory on demand.
//
// Concurrency invariants:
// The image heap's region tops are fixed at startup. Concurrent marking captures the published top
// of each metaspace region and traverses objects up to that limit. Objects allocated afterward are
// implicitly live. SATB preserves the marking snapshot without a metaspace rescan at remark. The
// card barrier covers stores after remembered-set rebuilding captures the region top.
//
// Regions that are empty at remembered-set rebuild start are excluded from rebuilding. Later
// allocations can change them to humongous while initializing an object, so rebuild must not
// inspect their new object or humongous-region metadata.
//
// Unlike ordinary old regions, which may only grow during stop-the-world GC, populated metaspace regions
// can receive new objects while concurrent card refinement is running. Allocation initializes the object
// and its block-offset-table entries before publishing the new top with a release store. Refinement must
// acquire that top before scanning newly included objects.
class G1Metaspace : public SVMMetaspace {
 public:
  enum AllocationKind {
    DynamicHubAllocation = 0,
    ByteArrayAllocation = 1,
    IntArrayAllocation = 2,
    ObjectAllocation = 3,
    AllocationKindCount = 4
  };

 private:
  static uint _current_region;
  static uint _committed_regions;
  static size_t _used;
  static size_t _used_at_last_gc;
  static size_t _allocation_counts[AllocationKindCount];
  static size_t _allocation_sizes[AllocationKindCount];

  static oop allocate(Klass* klass, size_t word_size, int array_length, AllocationKind allocation_kind);
  static oop allocate_humongous(Klass* klass, size_t word_size, int array_length, AllocationKind allocation_kind);
  static oop initialize_object(HeapWord* memory, Klass* klass, size_t word_size, int array_length);
  static void record_allocation(size_t word_size, AllocationKind allocation_kind);
  static void commit_regions(uint end_region, bool commit_bot);

 public:
  void initialize();
  void update_used_at_last_gc() override;

  static oop allocate_instance(InstanceKlass* klass);
  static oop allocate_array(Klass* klass, int length, AllocationKind allocation_kind);

  bool is_in_allocated_memory(const void* address) const override;

  static size_t used() { return _used; }
  static size_t used_at_last_gc() { return _used_at_last_gc; }
  static size_t waste();
  static size_t allocation_count(AllocationKind allocation_kind) { return _allocation_counts[allocation_kind]; }
  static size_t allocation_size(AllocationKind allocation_kind) { return _allocation_sizes[allocation_kind]; }
};


} // namespace svm_gc

#endif // SVM_SHARE_GC_G1_G1METASPACE_HPP
