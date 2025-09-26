/*
 * Copyright (c) 2019, 2025, Oracle and/or its affiliates. All rights reserved.
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

#ifndef SVM_TO_GC_HPP
#define SVM_TO_GC_HPP

#include "exports/sharedGCStructs.hpp"
#include "svmGlobalData.hpp"
#include "svmIsolateThread.hpp"
#include "svmTypes.hpp"
#include "runtime/thread.hpp"

#if ((defined(__GNUC__) && ((__GNUC__ > 4) || (__GNUC__ == 4) && (__GNUC_MINOR__ > 2))) || __has_attribute(visibility))
  #ifdef ARM
    #define EXPORT_FOR_SVM     __attribute__((externally_visible,visibility("default")))
  #else
    #define EXPORT_FOR_SVM     __attribute__((visibility("default")))
  #endif
#else
  #define EXPORT_FOR_SVM
#endif


namespace svm_gc {

class InstanceStackChunkKlass;
class InstancePodKlass;
class nmethod;

static struct GCConstants gc_constants;

extern "C" {
  EXPORT_FOR_SVM void svm_gc_parse_options(int actual_native_image_version, int argc, char *argv[], char *image_build_hosted_args, char *image_build_runtime_args,
      size_t max_heap_address_space_size, size_t heap_base_alignment, size_t null_regions_size, size_t image_heap_size,
      bool use_compressed_references, int compressed_reference_shift, bool is_containerized, jlong container_memory_limit_in_bytes, int container_active_processor_count, HeapOptions *result);

  EXPORT_FOR_SVM GCConstants* svm_gc_create(IsolateThread *isolate_thread, char *heap_base,
      Klass *dynamic_hub_klass, InstanceKlass *filler_object_klass, TypeArrayKlass *filler_array_klass, Klass *string_klass, Klass *system_klass,
      objArrayOop static_object_fields, typeArrayOop static_primitive_fields, oop vm_operation_thread, oop safepoint, oop runtime_code_info_memory,
      int reference_map_compressed_offset_shift, SVMOopMap *thread_locals_reference_map,
      objArrayOop klasses_assumed_reachable_for_code_unloading, bool use_perf_data, bool use_string_inlining, bool closed_type_world,
      bool use_interface_hashing, int interface_hashing_max_id, int dynamic_hub_hashing_interface_mask, int dynamic_hub_hashing_shift_offset,
      char *offsets, int offsets_length,
      queueVmOperationFunc collect_for_allocation_op, queueVmOperationFunc execute_pause_remark_op, queueVmOperationFunc execute_pause_cleanup_op,
      queueVmOperationFunc collect_full_op, queueVmOperationFunc verify_heap_op, queueVmOperationFunc try_initiate_conc_mark_op,
      vmOperationStatusFunc wait_for_vm_operation_execution_status, vmOperationStatusFunc update_vm_operation_execution_status, vmOperationDataFunc is_vm_operation_finished,
      fetchThreadStackFramesFunc fetch_thread_stack_frames, freeThreadStackFramesFunc free_thread_stack_frames,
      fetchContinuationStackFramesFunc fetch_continuation_stack_frames, freeContinuationStackFramesFunc free_continuation_stack_frames,
      fetchCodeInfosFunc fetch_code_infos, freeCodeInfosFunc free_code_infos, cleanRuntimeCodeCacheFunc clean_runtime_code_cache,
      threadStateTransitionFunc transition_vm_to_native, fastThreadStateTransitionFunc fast_transition_native_to_vm, threadStateTransitionFunc slow_transition_native_to_vm);

  EXPORT_FOR_SVM void svm_gc_update_option_value(oop optionName, jlong value);

  EXPORT_FOR_SVM bool svm_gc_teardown();

  EXPORT_FOR_SVM void svm_gc_attach_thread(IsolateThread *thread);

  EXPORT_FOR_SVM void svm_gc_detach_thread(IsolateThread *thread);

  EXPORT_FOR_SVM void svm_gc_retire_tlab();

  EXPORT_FOR_SVM void svm_gc_prepare_for_safepoint();

  EXPORT_FOR_SVM void svm_gc_end_safepoint();

  EXPORT_FOR_SVM void svm_gc_collect(int cause);

  EXPORT_FOR_SVM bool svm_gc_execute_vm_operation_prologue(VM_OperationData *data);

  EXPORT_FOR_SVM void svm_gc_execute_vm_operation_main(VM_OperationData *data);

  EXPORT_FOR_SVM void svm_gc_execute_vm_operation_epilogue(VM_OperationData *data);

  EXPORT_FOR_SVM oop svm_gc_allocate_instance(InstanceKlass *k);

  EXPORT_FOR_SVM oop svm_gc_allocate_array(ArrayKlass *k, int length);

  EXPORT_FOR_SVM oop svm_gc_allocate_stack_chunk(InstanceStackChunkKlass *k, int length);

  EXPORT_FOR_SVM oop svm_gc_allocate_pod(InstancePodKlass *k, int length);

  EXPORT_FOR_SVM void svm_gc_pin_object(oop o);

  EXPORT_FOR_SVM void svm_gc_unpin_object(oop o);

  EXPORT_FOR_SVM void svm_gc_pre_write_barrier(oop obj);

  EXPORT_FOR_SVM void svm_gc_post_write_barrier(void *card_addr);

  EXPORT_FOR_SVM void svm_gc_dirty_all_references_of(stackChunkOop stackChunk);

  EXPORT_FOR_SVM void svm_gc_verify_oop(oop obj);

  EXPORT_FOR_SVM bool svm_gc_validate_object(oop parent, oop child);

  EXPORT_FOR_SVM void svm_gc_log_printf(char* format, jlong v1, jlong v2, jlong v3);

  EXPORT_FOR_SVM jlong svm_gc_millis_since_last_whole_heap_examined();

  EXPORT_FOR_SVM bool svm_gc_has_reference_pending_list();

  EXPORT_FOR_SVM oop svm_gc_get_and_clear_reference_pending_list();

  EXPORT_FOR_SVM uint64_t svm_gc_get_reference_pending_list_wakeup_count();

  EXPORT_FOR_SVM bool svm_gc_wait_for_reference_pending_list(uint64_t initial_wakeup_count);

  EXPORT_FOR_SVM void svm_gc_wake_up_reference_pending_list_waiters();

  EXPORT_FOR_SVM void svm_gc_register_object_fields(nmethod* nm);

  EXPORT_FOR_SVM void svm_gc_register_code_constants(nmethod* nm);

  EXPORT_FOR_SVM void svm_gc_register_frame_metadata(nmethod* nm);

  EXPORT_FOR_SVM void svm_gc_register_deopt_metadata(nmethod* nm);

  EXPORT_FOR_SVM void svm_gc_get_internal_state(GCInternalState *gc_internal_data);

  EXPORT_FOR_SVM const char* svm_gc_get_current_thread_name();

  EXPORT_FOR_SVM jlong svm_gc_get_thread_allocated_memory(IsolateThread *thread);

  EXPORT_FOR_SVM jlong svm_gc_get_used_memory();

  EXPORT_FOR_SVM jlong svm_gc_get_free_memory();

  EXPORT_FOR_SVM jlong svm_gc_get_total_memory();

  EXPORT_FOR_SVM jlong svm_gc_get_max_memory();

  EXPORT_FOR_SVM size_t svm_gc_get_used_memory_after_last_gc();
}


} // namespace svm_gc

#endif // SVM_TO_GC_HPP
