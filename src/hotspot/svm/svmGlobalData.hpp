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

#ifndef SVM_GLOBAL_DATA_HPP
#define SVM_GLOBAL_DATA_HPP

#include "oops/oopsHierarchy.hpp"
#include "svmOopMap.hpp"
#include "svmTypes.hpp"

// NOTE (chaeubl): when reading data from the image heap, we don't need to worry about write barriers.
// For this, we use methods such as RawAccess<>::load_oop_at(...) and RawAccess<>::load(...) instead of
// a pointer-based memory access as those methods know about compressed oops.
//
// Writing oops to the image heap may require write barriers, so depending on the situation we either use
// RawAccess (no barriers) or HeapAccess (barriers).


namespace svm_gc {

struct SVMObjectLayoutOffsets {
  int _mark_word;
  int _hub;
  int _object_alignment;
  int _min_obj_size;
  int _obj_base;
  int _array_length;
};

struct SVMHybridLayoutOffsets {
  int _closed_type_world_type_check_slots;
};

struct SVMThreadLocalOffsets {
  int _next_thread;
  int _java_thread;
  int _thread_status;
  int _pod_reference_map;
};

struct SVMCodeInfoOffsets {
  int _object_fields;
  int _state;
  int _gc_data;
  int _frame_info_object_constants;
  int _deoptimization_object_constants;
  int _stack_reference_map_encoding;
  int _code_start;
  int _code_constants_reference_map_encoding;
  int _code_constants_reference_map_index;
  int _are_all_objects_in_image_heap;
};

struct SVMVmThreadsOffsets {
  int _static_vm_threads_head;
  int _static_num_attached_threads;
};

struct SVMJavaThreadsOffsets {
  int _static_non_daemon_threads;
};

struct SVMVMOperationThreadOffsets {
  int _isolate_thread;
};

struct SVMSafepointOffsets {
  int _safepoint_state;
  int _safepoint_id;
};

struct SVMDynamicHubOffsets {
  int _name;
  int _class_type;
  int _reference_type;
  int _layout_encoding;
  int _component_hub;
  int _reference_map_compressed_offset;
  int _closed_type_world_type_check_start;
  int _closed_type_world_type_check_range;
  int _closed_type_world_type_check_slot;
  int _open_type_world_type_id;
  int _open_type_world_type_id_depth;
  int _open_type_world_num_class_types;
  int _open_type_world_num_iterable_interface_types;
  int _open_type_world_type_check_slots;
  int _open_type_world_interface_id;
  int _open_type_world_interface_hash_table;
  int _open_type_world_interface_hash_param;
};

struct SVMStringOffsets {
  int _value;
  int _coder;
};

struct SVMAtomicIntegerOffsets {
  int _value;
};

struct SVMReferenceOffsets {
  int _referent;
  int _next;
  int _discovered;
};

struct SVMSoftReferenceOffsets {
  int _timestamp;
  int _static_clock;
};

struct SVMStackChunkOffsets {
  int _ip;
};

struct SVMRuntimeCodeInfoMemoryOffsets {
  int _table;
};

struct SVMG1TLABPerfDataOffsets {
  int _alloc_threads;
  int _fills;
  int _max_fills;
  int _alloc;
  int _gc_waste;
  int _max_gc_waste;
  int _refill_waste;
  int _max_refill_waste;
  int _slow_alloc;
  int _max_slow_alloc;
};

struct SVMG1GCPolicyPerfDataOffsets {
  int _desired_survivor_size;
  int _gc_time_limit_exceeded;
  int _max_tenuring_threshold;
  int _tenuring_threshold;
};

struct SVMG1GCCausesPerfDataOffsets {
  int _cause;
  int _last_cause;
};

struct SVMG1CollectorPerfDataOffsets {
  int _invocations;
  int _last_entry_time;
  int _last_exit_time;
  int _time;
};

struct SVMG1SpacePerfDataOffsets {
  int _capacity;
  int _init_capacity;
  int _max_capacity;
  int _used;
};

struct SVMG1GenerationPerfDataOffsets {
  int _capacity;
  int _max_capacity;
  int _min_capacity;
  int _spaces;
};

struct SVMG1AgeTablePerfDataOffsets {
  int _table;
};

struct SVMG1CpuTimePerfDataOffsets {
  int _gc_total;
  int _gc_parallel_workers;
  int _gc_conc_mark;
  int _gc_conc_refine;
  int _gc_service;
};

struct SVMG1PerfDataOffsets {
  int _tlab;
  int _gc_policy;
  int _gc_causes;
  int _collectors;
  int _generations;
  int _age_table;
  int _cpu_time;
};

struct SVMPerfLongOffsets {
  int _value;
};

struct SVMPerfStringVariableOffsets {
  int _null_terminated_value;
};

struct SVMOffsets {
  struct SVMObjectLayoutOffsets _object_layout;
  struct SVMHybridLayoutOffsets _hybrid_layout;
  struct SVMThreadLocalOffsets _thread_locals;
  struct SVMCodeInfoOffsets _code_info;
  struct SVMVmThreadsOffsets _vm_threads;
  struct SVMJavaThreadsOffsets _java_threads;
  struct SVMVMOperationThreadOffsets _vm_operation_thread;
  struct SVMSafepointOffsets _safepoint;
  struct SVMDynamicHubOffsets _dynamicHub;
  struct SVMStringOffsets _string;
  struct SVMAtomicIntegerOffsets _atomic_integer;
  struct SVMReferenceOffsets _reference;
  struct SVMSoftReferenceOffsets _soft_reference;
  struct SVMStackChunkOffsets _stack_chunk;
  struct SVMRuntimeCodeInfoMemoryOffsets _runtime_code_info_memory;
  struct SVMG1TLABPerfDataOffsets _tlab_perf_data;
  struct SVMG1GCPolicyPerfDataOffsets _gc_policy_perf_data;
  struct SVMG1GCCausesPerfDataOffsets _gc_causes_perf_data;
  struct SVMG1CollectorPerfDataOffsets _collector_perf_data;
  struct SVMG1SpacePerfDataOffsets _space_perf_data;
  struct SVMG1GenerationPerfDataOffsets _generation_perf_data;
  struct SVMG1AgeTablePerfDataOffsets _age_table_perf_data;
  struct SVMG1CpuTimePerfDataOffsets _cpu_time_perf_data;
  struct SVMG1PerfDataOffsets _perf_data;
  struct SVMPerfLongOffsets _perf_long;
  struct SVMPerfStringVariableOffsets _perf_string_variable;
};

// Holds data that is valid across all isolates.
struct SVMGlobalData {
  // values that are set before we parse any arguments and before SVM maps the image heap
  static size_t _heap_base_alignment;
  static size_t _null_regions_size;
  // image heap size is a multiple of the region size
  static size_t _image_heap_size;
  static size_t _image_heap_used;
  static size_t _image_heap_waste;
  static char* _image_build_hosted_args;
  static char* _image_build_runtime_args;

  // container information
  static bool _is_containerized;
  static jlong _container_memory_limit_in_bytes;
  static int _container_active_processor_count;

  // values that are set after SVM mapped the image heap
  static SVMOffsets _offsets;
  static int _closed_image_heap_regions;
  static int _open_image_heap_regions;
  static const uint8_t* _image_heap_block_offset_table;
  static size_t _image_heap_block_offset_table_size;
  static SVMOopMap* _thread_locals_reference_map;
  static bool _closed_type_world;
  static bool _use_interface_hashing;
  static int _interface_hashing_max_id;
  static queueVmOperationFunc _collect_for_allocation_op;
  static queueVmOperationFunc _execute_pause_remark_op;
  static queueVmOperationFunc _execute_pause_cleanup_op;
  static queueVmOperationFunc _collect_full_op;
  static queueVmOperationFunc _verify_heap_op;
  static queueVmOperationFunc _try_initiate_conc_mark_op;
  static vmOperationStatusFunc _wait_for_vm_operation_execution_status;
  static vmOperationStatusFunc _update_vm_operation_execution_status;
  static vmOperationDataFunc _is_vm_operation_finished;
  static fetchThreadStackFramesFunc _fetch_thread_stack_frames;
  static freeThreadStackFramesFunc _free_thread_stack_frames;
  static fetchContinuationStackFramesFunc _fetch_continuation_stack_frames;
  static freeContinuationStackFramesFunc _free_continuation_stack_frames;
  static fetchCodeInfosFunc _fetch_code_infos;
  static freeCodeInfosFunc _free_code_infos;
  static cleanRuntimeCodeCacheFunc _clean_runtime_code_cache;
  static threadStateTransitionFunc _transition_vm_to_native;
  static fastThreadStateTransitionFunc _try_fast_transition_native_to_vm;
  static threadStateTransitionFunc _slow_transition_native_to_vm;

  static void initialize_offsets(char *offsets, int offsets_length);
  static void verify_offsets(bool use_perf_data);
};

// Holds isolate-specific data.
struct SVMIsolateData {
  // values that are set before we parse any arguments and before SVM maps the image heap
  static int _argc;
  static char** _argv;
  static size_t _max_heap_address_space_size;

  // values that are set after SVM mapped the image heap
  static char* _heap_base;
  static typeArrayOop _image_heap_region_types;
  static typeArrayOop _image_heap_region_free_spaces;
  static char* _closed_image_heap_start_addr;
  static char* _closed_image_heap_end_addr;
  static char* _open_image_heap_start_addr;
  static char* _open_image_heap_end_addr;
  static objArrayOop _static_object_fields;
  static typeArrayOop _static_primitive_fields;
  static oop _vm_operation_thread;
  static oop _safepoint;
  static oop _runtime_code_info_memory;
  static objArrayOop _klasses_assumed_reachable_for_code_unloading;
  static oop _performance_data;
  static bool _during_teardown;
};


} // namespace svm_gc

#endif // SVM_GLOBAL_DATA_HPP
