/*
 * Copyright (c) 2019, 2026, Oracle and/or its affiliates. All rights reserved.
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

#ifndef SVM_G1_GC_STRUCTS_H
#define SVM_G1_GC_STRUCTS_H

#include <sys/types.h>

struct G1HeapOptions {
  size_t max_heap_size;
  size_t heap_address_space_size;
  size_t physical_memory_size;
};

struct G1InitState {
  void* card_table_address;
  void* gc_total_collections_address;
  int tlab_top_offset;
  int tlab_end_offset;
  int satb_queue_marking_offset;
  int satb_queue_buffer_offset;
  int satb_queue_index_offset;
  int card_queue_buffer_offset;
  int card_queue_index_offset;
  int card_table_shift;
  int log_of_heap_region_grain_bytes;
  int java_thread_size;
  int vm_operation_data_size;
  int vm_operation_wrapper_data_size;
  char dirty_card_value;
  char young_card_value;
};

struct G1RegionBoundaries {
  unsigned char *bottom;
  unsigned char *top;
};

struct G1RegionInfo {
  unsigned char *bottom;
  unsigned char *top;
  unsigned char *end;
  unsigned char *top_at_mark_start;
  unsigned char *parsable_bottom;
  size_t pinned_object_count;
  char in_collection_set;
  char remembered_set_state;
  char region_type;
};

struct G1InternalState {
  unsigned int total_collections;
  unsigned int full_collections;

  void* card_table_start;
  size_t card_table_size;

  void* block_offset_table_start;
  size_t block_offset_table_size;
};

#endif // SVM_G1_GC_STRUCTS_H
