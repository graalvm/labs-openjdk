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

#include "svmGlobalData.hpp"
#include "svmOopMap.hpp"
#include "oops/oop.hpp"


namespace svm_gc {

int SVMIsolateData::_argc = 0;
char** SVMIsolateData::_argv = 0;
size_t SVMIsolateData::_max_heap_address_space_size = 0;

char* SVMIsolateData::_heap_base = nullptr;
typeArrayOop SVMIsolateData::_image_heap_region_types = nullptr;
typeArrayOop SVMIsolateData::_image_heap_region_free_spaces = nullptr;
char* SVMIsolateData::_closed_image_heap_start_addr = nullptr;
char* SVMIsolateData::_closed_image_heap_end_addr = nullptr;
char* SVMIsolateData::_open_image_heap_start_addr = nullptr;
char* SVMIsolateData::_open_image_heap_end_addr = nullptr;
objArrayOop SVMIsolateData::_static_object_fields = nullptr;
typeArrayOop SVMIsolateData::_static_primitive_fields = nullptr;
oop SVMIsolateData::_vm_operation_thread = nullptr;
oop SVMIsolateData::_safepoint = nullptr;
oop SVMIsolateData::_runtime_code_info_memory = nullptr;
objArrayOop SVMIsolateData::_klasses_assumed_reachable_for_code_unloading = nullptr;
oop SVMIsolateData::_performance_data = nullptr;
bool SVMIsolateData::_during_teardown = false;

size_t SVMGlobalData::_heap_base_alignment = 0;
size_t SVMGlobalData::_null_regions_size = 0;
size_t SVMGlobalData::_image_heap_size = 0;
size_t SVMGlobalData::_image_heap_used = 0;
size_t SVMGlobalData::_image_heap_waste = 0;
char* SVMGlobalData::_image_build_hosted_args = 0;
char* SVMGlobalData::_image_build_runtime_args = 0;

bool SVMGlobalData::_is_containerized = false;
jlong SVMGlobalData::_container_memory_limit_in_bytes = 0;
int SVMGlobalData::_container_active_processor_count = 0;

SVMOffsets SVMGlobalData::_offsets;
int SVMGlobalData::_closed_image_heap_regions = -1;
int SVMGlobalData::_open_image_heap_regions = -1;
const uint8_t* SVMGlobalData::_image_heap_block_offset_table = nullptr;
size_t SVMGlobalData::_image_heap_block_offset_table_size = 0;
SVMOopMap* SVMGlobalData::_thread_locals_reference_map = nullptr;
bool SVMGlobalData::_closed_type_world = false;
bool SVMGlobalData::_use_interface_hashing = false;
int SVMGlobalData::_interface_hashing_max_id = -1;
queueVmOperationFunc SVMGlobalData::_collect_for_allocation_op = nullptr;
queueVmOperationFunc SVMGlobalData::_execute_pause_remark_op = nullptr;
queueVmOperationFunc SVMGlobalData::_execute_pause_cleanup_op = nullptr;
queueVmOperationFunc SVMGlobalData::_collect_full_op = nullptr;
queueVmOperationFunc SVMGlobalData::_verify_heap_op = nullptr;
queueVmOperationFunc SVMGlobalData::_try_initiate_conc_mark_op = nullptr;
vmOperationStatusFunc SVMGlobalData::_wait_for_vm_operation_execution_status = nullptr;
vmOperationStatusFunc SVMGlobalData::_update_vm_operation_execution_status = nullptr;
vmOperationDataFunc SVMGlobalData::_is_vm_operation_finished = nullptr;
fetchThreadStackFramesFunc SVMGlobalData::_fetch_thread_stack_frames = nullptr;
freeThreadStackFramesFunc SVMGlobalData::_free_thread_stack_frames = nullptr;
fetchContinuationStackFramesFunc SVMGlobalData::_fetch_continuation_stack_frames = nullptr;
freeContinuationStackFramesFunc SVMGlobalData::_free_continuation_stack_frames = nullptr;
fetchCodeInfosFunc SVMGlobalData::_fetch_code_infos = nullptr;
freeCodeInfosFunc SVMGlobalData::_free_code_infos = nullptr;
cleanRuntimeCodeCacheFunc SVMGlobalData::_clean_runtime_code_cache = nullptr;
threadStateTransitionFunc SVMGlobalData::_transition_vm_to_native = nullptr;
fastThreadStateTransitionFunc SVMGlobalData::_try_fast_transition_native_to_vm = nullptr;
threadStateTransitionFunc SVMGlobalData::_slow_transition_native_to_vm = nullptr;

void SVMGlobalData::initialize_offsets(char *offsets, int offsets_length) {
  guarantee(offsets_length == sizeof(SVMOffsets), "must match");
  memcpy(&SVMGlobalData::_offsets, offsets, offsets_length);
}

void SVMGlobalData::verify_offsets(bool use_perf_data) {
  assert(SVMGlobalData::_offsets._object_layout._hub == oopDesc::klass_offset_in_bytes(), "must match");
  assert(SVMGlobalData::_offsets._object_layout._object_alignment == ObjectAlignmentInBytes, "must be");
  assert(SVMGlobalData::_offsets._object_layout._obj_base > oopDesc::klass_offset_in_bytes(), "must be");
  assert(SVMGlobalData::_offsets._object_layout._min_obj_size > oopDesc::klass_offset_in_bytes(), "must be");
  assert(SVMGlobalData::_offsets._object_layout._array_length > oopDesc::klass_offset_in_bytes(), "must be");

  assert(!SVMGlobalData::_closed_type_world || SVMGlobalData::_offsets._hybrid_layout._closed_type_world_type_check_slots > oopDesc::klass_offset_in_bytes(), "must be");

  assert(SVMGlobalData::_offsets._thread_locals._next_thread >= 0, "must be");
  assert(SVMGlobalData::_offsets._thread_locals._java_thread >= 0, "must be");
  assert(SVMGlobalData::_offsets._thread_locals._thread_status >= 0, "must be");
  assert(SVMGlobalData::_offsets._thread_locals._pod_reference_map >= 0, "must be");

  assert(SVMGlobalData::_offsets._code_info._object_fields >= 0, "must be");
  assert(SVMGlobalData::_offsets._code_info._state >= 0, "must be");
  assert(SVMGlobalData::_offsets._code_info._gc_data >= 0, "must be");
  assert(SVMGlobalData::_offsets._code_info._frame_info_object_constants >= 0, "must be");
  assert(SVMGlobalData::_offsets._code_info._deoptimization_object_constants >= 0, "must be");
  assert(SVMGlobalData::_offsets._code_info._stack_reference_map_encoding >= 0, "must be");
  assert(SVMGlobalData::_offsets._code_info._code_start >= 0, "must be");
  assert(SVMGlobalData::_offsets._code_info._code_constants_reference_map_encoding >= 0, "must be");
  assert(SVMGlobalData::_offsets._code_info._code_constants_reference_map_index >= 0, "must be");
  assert(SVMGlobalData::_offsets._code_info._are_all_objects_in_image_heap >= 0, "must be");

  assert(SVMGlobalData::_offsets._vm_threads._static_vm_threads_head >= 0, "must be");

  assert(SVMGlobalData::_offsets._java_threads._static_non_daemon_threads >= 0, "must be");

  assert(SVMGlobalData::_offsets._vm_operation_thread._isolate_thread > oopDesc::klass_offset_in_bytes(), "must be");

  assert(SVMGlobalData::_offsets._safepoint._safepoint_state > oopDesc::klass_offset_in_bytes(), "must be");
  assert(SVMGlobalData::_offsets._safepoint._safepoint_id > oopDesc::klass_offset_in_bytes(), "must be");

  assert(SVMGlobalData::_offsets._dynamicHub._name > oopDesc::klass_offset_in_bytes(), "must be");
  assert(SVMGlobalData::_offsets._dynamicHub._class_type > oopDesc::klass_offset_in_bytes(), "must be");
  assert(SVMGlobalData::_offsets._dynamicHub._reference_type > oopDesc::klass_offset_in_bytes(), "must be");
  assert(SVMGlobalData::_offsets._dynamicHub._layout_encoding > oopDesc::klass_offset_in_bytes(), "must be");
  assert(SVMGlobalData::_offsets._dynamicHub._component_hub > oopDesc::klass_offset_in_bytes(), "must be");
  assert(SVMGlobalData::_offsets._dynamicHub._reference_map_compressed_offset > oopDesc::klass_offset_in_bytes(), "must be");
  assert(!SVMGlobalData::_closed_type_world || SVMGlobalData::_offsets._dynamicHub._closed_type_world_type_check_start > oopDesc::klass_offset_in_bytes(), "must be");
  assert(!SVMGlobalData::_closed_type_world || SVMGlobalData::_offsets._dynamicHub._closed_type_world_type_check_range > oopDesc::klass_offset_in_bytes(), "must be");
  assert(!SVMGlobalData::_closed_type_world || SVMGlobalData::_offsets._dynamicHub._closed_type_world_type_check_slot > oopDesc::klass_offset_in_bytes(), "must be");
  assert(SVMGlobalData::_closed_type_world || SVMGlobalData::_offsets._dynamicHub._open_type_world_type_id > oopDesc::klass_offset_in_bytes(), "must be");
  assert(SVMGlobalData::_closed_type_world || SVMGlobalData::_offsets._dynamicHub._open_type_world_type_id_depth > oopDesc::klass_offset_in_bytes(), "must be");
  assert(SVMGlobalData::_closed_type_world || SVMGlobalData::_offsets._dynamicHub._open_type_world_num_class_types > oopDesc::klass_offset_in_bytes(), "must be");
  assert(SVMGlobalData::_closed_type_world || SVMGlobalData::_offsets._dynamicHub._open_type_world_num_iterable_interface_types > oopDesc::klass_offset_in_bytes(), "must be");
  assert(SVMGlobalData::_closed_type_world || SVMGlobalData::_offsets._dynamicHub._open_type_world_type_check_slots > oopDesc::klass_offset_in_bytes(), "must be");
  assert(SVMGlobalData::_closed_type_world || SVMGlobalData::_offsets._dynamicHub._open_type_world_interface_id > oopDesc::klass_offset_in_bytes(), "must be");
  assert(SVMGlobalData::_closed_type_world || SVMGlobalData::_offsets._dynamicHub._open_type_world_interface_hash_table > oopDesc::klass_offset_in_bytes(), "must be");
  assert(SVMGlobalData::_closed_type_world || SVMGlobalData::_offsets._dynamicHub._open_type_world_interface_hash_param > oopDesc::klass_offset_in_bytes(), "must be");

  assert(SVMGlobalData::_offsets._string._value > oopDesc::klass_offset_in_bytes(), "must be");
  assert(SVMGlobalData::_offsets._string._coder == -1 || SVMGlobalData::_offsets._string._coder > oopDesc::klass_offset_in_bytes(), "must be");

  assert(SVMGlobalData::_offsets._atomic_integer._value > oopDesc::klass_offset_in_bytes(), "must be");

  assert(SVMGlobalData::_offsets._reference._referent > oopDesc::klass_offset_in_bytes(), "must be");
  assert(SVMGlobalData::_offsets._reference._next > oopDesc::klass_offset_in_bytes(), "must be");
  assert(SVMGlobalData::_offsets._reference._discovered > oopDesc::klass_offset_in_bytes(), "must be");

  assert(SVMGlobalData::_offsets._soft_reference._timestamp > oopDesc::klass_offset_in_bytes(), "must be");
  assert(SVMGlobalData::_offsets._soft_reference._static_clock >= 0, "must be");

  assert(SVMGlobalData::_offsets._runtime_code_info_memory._table > oopDesc::klass_offset_in_bytes(), "must be");

  if (use_perf_data) {
    assert(SVMGlobalData::_offsets._tlab_perf_data._alloc_threads > oopDesc::klass_offset_in_bytes(), "must be");
    assert(SVMGlobalData::_offsets._tlab_perf_data._fills > oopDesc::klass_offset_in_bytes(), "must be");
    assert(SVMGlobalData::_offsets._tlab_perf_data._max_fills > oopDesc::klass_offset_in_bytes(), "must be");
    assert(SVMGlobalData::_offsets._tlab_perf_data._alloc > oopDesc::klass_offset_in_bytes(), "must be");
    assert(SVMGlobalData::_offsets._tlab_perf_data._gc_waste > oopDesc::klass_offset_in_bytes(), "must be");
    assert(SVMGlobalData::_offsets._tlab_perf_data._max_gc_waste > oopDesc::klass_offset_in_bytes(), "must be");
    assert(SVMGlobalData::_offsets._tlab_perf_data._refill_waste > oopDesc::klass_offset_in_bytes(), "must be");
    assert(SVMGlobalData::_offsets._tlab_perf_data._max_refill_waste > oopDesc::klass_offset_in_bytes(), "must be");
    assert(SVMGlobalData::_offsets._tlab_perf_data._slow_alloc > oopDesc::klass_offset_in_bytes(), "must be");
    assert(SVMGlobalData::_offsets._tlab_perf_data._max_slow_alloc > oopDesc::klass_offset_in_bytes(), "must be");

    assert(SVMGlobalData::_offsets._gc_policy_perf_data._desired_survivor_size > oopDesc::klass_offset_in_bytes(), "must be");
    assert(SVMGlobalData::_offsets._gc_policy_perf_data._gc_time_limit_exceeded > oopDesc::klass_offset_in_bytes(), "must be");
    assert(SVMGlobalData::_offsets._gc_policy_perf_data._max_tenuring_threshold > oopDesc::klass_offset_in_bytes(), "must be");
    assert(SVMGlobalData::_offsets._gc_policy_perf_data._tenuring_threshold > oopDesc::klass_offset_in_bytes(), "must be");

    assert(SVMGlobalData::_offsets._gc_causes_perf_data._cause > oopDesc::klass_offset_in_bytes(), "must be");
    assert(SVMGlobalData::_offsets._gc_causes_perf_data._last_cause > oopDesc::klass_offset_in_bytes(), "must be");

    assert(SVMGlobalData::_offsets._collector_perf_data._invocations > oopDesc::klass_offset_in_bytes(), "must be");
    assert(SVMGlobalData::_offsets._collector_perf_data._last_entry_time > oopDesc::klass_offset_in_bytes(), "must be");
    assert(SVMGlobalData::_offsets._collector_perf_data._last_exit_time > oopDesc::klass_offset_in_bytes(), "must be");
    assert(SVMGlobalData::_offsets._collector_perf_data._time > oopDesc::klass_offset_in_bytes(), "must be");

    assert(SVMGlobalData::_offsets._space_perf_data._capacity > oopDesc::klass_offset_in_bytes(), "must be");
    assert(SVMGlobalData::_offsets._space_perf_data._init_capacity > oopDesc::klass_offset_in_bytes(), "must be");
    assert(SVMGlobalData::_offsets._space_perf_data._max_capacity > oopDesc::klass_offset_in_bytes(), "must be");
    assert(SVMGlobalData::_offsets._space_perf_data._used > oopDesc::klass_offset_in_bytes(), "must be");

    assert(SVMGlobalData::_offsets._generation_perf_data._capacity > oopDesc::klass_offset_in_bytes(), "must be");
    assert(SVMGlobalData::_offsets._generation_perf_data._max_capacity > oopDesc::klass_offset_in_bytes(), "must be");
    assert(SVMGlobalData::_offsets._generation_perf_data._min_capacity > oopDesc::klass_offset_in_bytes(), "must be");
    assert(SVMGlobalData::_offsets._generation_perf_data._spaces > oopDesc::klass_offset_in_bytes(), "must be");

    assert(SVMGlobalData::_offsets._age_table_perf_data._table > oopDesc::klass_offset_in_bytes(), "must be");

    assert(SVMGlobalData::_offsets._perf_data._tlab > oopDesc::klass_offset_in_bytes(), "must be");
    assert(SVMGlobalData::_offsets._perf_data._gc_policy > oopDesc::klass_offset_in_bytes(), "must be");
    assert(SVMGlobalData::_offsets._perf_data._gc_causes > oopDesc::klass_offset_in_bytes(), "must be");
    assert(SVMGlobalData::_offsets._perf_data._collectors > oopDesc::klass_offset_in_bytes(), "must be");
    assert(SVMGlobalData::_offsets._perf_data._generations > oopDesc::klass_offset_in_bytes(), "must be");
    assert(SVMGlobalData::_offsets._perf_data._age_table > oopDesc::klass_offset_in_bytes(), "must be");
    assert(SVMGlobalData::_offsets._perf_data._cpu_time > oopDesc::klass_offset_in_bytes(), "must be");

    assert(SVMGlobalData::_offsets._perf_data._age_table > oopDesc::klass_offset_in_bytes(), "must be");

    assert(SVMGlobalData::_offsets._perf_long._value > oopDesc::klass_offset_in_bytes(), "must be");

    assert(SVMGlobalData::_offsets._perf_string_variable._null_terminated_value > oopDesc::klass_offset_in_bytes(), "must be");
  }
}

} // namespace svm_gc

