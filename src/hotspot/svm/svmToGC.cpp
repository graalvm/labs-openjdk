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

#include "svmToGC.hpp"
#include "svmOopMap.hpp"
#include "ci/ciUtilities.hpp"
#include "code/nmethod.hpp"
#include "exports/sharedGCStructs.hpp"
#include "gc/shared/cardTable.hpp"
#include "gc/shared/collectedHeap.inline.hpp"
#include "gc/shared/gcArguments.hpp"
#include "gc/shared/gcCause.hpp"
#include "logging/logConfiguration.hpp"
#include "oops/arrayKlass.inline.hpp"
#include "oops/instanceKlass.hpp"
#include "oops/instanceStackChunkKlass.hpp"
#include "oops/instancePodKlass.hpp"
#include "oops/objArrayKlass.hpp"
#include "oops/objArrayOop.hpp"
#include "oops/oop.hpp"
#include "oops/podOop.hpp"
#include "oops/typeArrayKlass.hpp"
#include "oops/typeArrayOop.hpp"
#include "oops/typeArrayOop.inline.hpp"
#include "runtime/arguments.hpp"
#include "runtime/globals_extension.hpp"
#include "runtime/thread.inline.hpp"
#include "runtime/threads.hpp"
#include "runtime/vmOperations.hpp"
#include "runtime/vmThread.hpp"
#include "utilities/defaultStream.hpp"
#include "utilities/vmError.hpp"

/*
 * NOTE (chaeubl):  When SVM calls any of the exported methods below, it is essential that the correct kind of transition is used for the call. Otherwise,
 * we will end up with either deadlocks or wrong behavior.
 *
 * Possible transitions:
 * - NO_TRANSITION: Similar to @Uninterruptible as the thread will remain in STATUS_JAVA. The C++ code must not block and it must not call back to Java.
 *   It is guaranteed that the C++ code won't be interrupted by a safepoint.
 *
 * - TO_VM: This is the most flexible mode. While the thread is in C++ code, safepoints are prevent. However, it is possible to explicitly do a transition
 *   to native if needed to allow safepoints (e.g., before a blocking call or when doing a call back into uninterruptible Java code). After doing the
 *   transition to native, it is guaranteed that SVM is able to reach a safepoint even if the C++ code blocks. Here is one example where we would end up
 *   in a deadlock if we didn't do a transition to native:
 *   - thread A does a slow-path allocation and acquires the GC-internal mutex
 *   - thread B does a slow-path allocation and blocks when it tries to acquire the GC-internal mutex
 *   - thread A schedules a VM operation that needs a safepoint
 *   - thread B still has STATUS_IN_VM, so SVM can't reach a safepoint and deadlocks
 *
 * - TO_NATIVE: The C++ code may execute blocking calls or may call back into Java code. However, the C++ code must be aware that safepoints can happen at
 *   ANY time. Writing the code in such a way is often not feasible so that this transition is only used for cases where we know that safepoints don't
 *   cause any problems.
 *
 * Some guidance for determining the correct transition:
 * - C++ code uses oops or must not be interrupted by a safepoint -> NO_TRANSITION or TO_VM
 * - C++ code may block or call back to Java -> TO_NATIVE or TO_VM
 */

static inline jlong convert_size_t_to_jlong(size_t val) {
  // In the 64-bit vm, a size_t can overflow a jlong (which is signed).
  NOT_LP64 (return (jlong)val;)
  LP64_ONLY(return (jlong)MIN2(val, (size_t)max_jlong);)
}

extern "C" {
// NO_TRANSITION - This method is called during startup, before anything else is initialized.
EXPORT_FOR_SVM void svm_gc_parse_options(int actual_native_image_version, int argc, char *argv[], char *image_build_hosted_args, char *image_build_runtime_args,
    size_t max_heap_address_space_size, size_t heap_base_alignment, size_t null_regions_size, size_t image_heap_size,
    bool use_compressed_references, int compressed_reference_shift, bool is_containerized, jlong container_memory_limit_in_bytes, int container_active_processor_count, HeapOptions *result) {
  // verify invariants
  int expected_native_image_version = 250100;
  guarantee(actual_native_image_version >= expected_native_image_version, "incompatible GC version: the native-image tries to use a GC that is too new");
  guarantee(actual_native_image_version <= expected_native_image_version, "incompatible GC version: the native-image tries to use a GC that is too old");
#ifdef SVM_COMPRESSED_REFERENCES
  guarantee(use_compressed_references, "incompatible GC version: the native-image was built using -H:+UseCompressedReferences but the GC does not use compressed references");
#else
  guarantee(!use_compressed_references, "incompatible GC version: the native-image was built using -H:-UseCompressedReferences but the GC uses compressed references");
#endif
  guarantee(compressed_reference_shift == CompressedOopShift, "must be");

  // verify arguments
  guarantee(SVMIsolateData::_heap_base == nullptr, "GC doesn't support multiple isolates at the moment.");
  guarantee(argc >= 0, "must be");
  guarantee((argc == 0) == (argv == nullptr), "must be");
  guarantee(image_build_hosted_args != nullptr, "must be");
  guarantee(image_build_runtime_args != nullptr, "must be");
  guarantee(null_regions_size > 0, "must be");
  guarantee(image_heap_size > 0, "must be");

  SVMIsolateData::_argc = argc;
  SVMIsolateData::_argv = argv;
  SVMIsolateData::_max_heap_address_space_size = max_heap_address_space_size;

  SVMGlobalData::_heap_base_alignment = heap_base_alignment;
  SVMGlobalData::_null_regions_size = null_regions_size;
  SVMGlobalData::_image_heap_size = image_heap_size;
  SVMGlobalData::_image_build_hosted_args = image_build_hosted_args;
  SVMGlobalData::_image_build_runtime_args = image_build_runtime_args;

  // Container information needs to be set before the argument parsing
  SVMGlobalData::_is_containerized = is_containerized;
  SVMGlobalData::_container_memory_limit_in_bytes = container_memory_limit_in_bytes;
  SVMGlobalData::_container_active_processor_count = container_active_processor_count;

  Threads::parse_arguments();

  assert(is_aligned(MaxHeapSize, HeapAlignment), "must be");
  result->max_heap_size = MaxHeapSize;
  result->heap_address_space_size = MaxHeapSize + null_regions_size;
  result->physical_memory_size = FLAG_IS_DEFAULT(MaxRAM) ? os::physical_memory() : MaxRAM;

  guarantee(MaxHeapSize <= max_heap_address_space_size, "Java heap must fit into its address space");
  guarantee(ReservedAddressSpaceSize == 0 || MaxHeapSize <= ReservedAddressSpaceSize, "heap address space size is invalid");
}

// NO_TRANSITION - Only called during startup by uninterruptible code before a safepoint can be triggered.
EXPORT_FOR_SVM GCConstants* svm_gc_create(IsolateThread *isolate_thread, char *heap_base,
    Klass *dynamic_hub_klass, InstanceKlass *filler_object_klass, TypeArrayKlass *filler_array_klass, Klass *string_klass, Klass *system_klass,
    objArrayOop static_object_fields, typeArrayOop static_primitive_fields, oop vm_operation_thread, oop safepoint, oop runtime_code_info_memory,
    int reference_map_compressed_offset_shift, SVMOopMap *thread_locals_reference_map,
    objArrayOop klasses_assumed_reachable_for_code_unloading, bool perf_data_support, bool use_string_inlining, bool closed_type_world,
    bool use_interface_hashing, int interface_hashing_max_id, int dynamic_hub_hashing_interface_mask, int dynamic_hub_hashing_shift_offset,
    char *offsets, int offsets_length,
    queueVmOperationFunc collect_for_allocation_op, queueVmOperationFunc execute_pause_remark_op, queueVmOperationFunc execute_pause_cleanup_op,
    queueVmOperationFunc collect_full_op, queueVmOperationFunc verify_heap_op, queueVmOperationFunc try_initiate_conc_mark_op,
    vmOperationStatusFunc wait_for_vm_operation_execution_status, vmOperationStatusFunc update_vm_operation_execution_status,
    vmOperationDataFunc is_vm_operation_finished, fetchThreadStackFramesFunc fetch_thread_stack_frames, freeThreadStackFramesFunc free_thread_stack_frames,
    fetchContinuationStackFramesFunc fetch_continuation_stack_frames, freeContinuationStackFramesFunc free_continuation_stack_frames,
    fetchCodeInfosFunc fetch_code_infos, freeCodeInfosFunc free_code_infos, cleanRuntimeCodeCacheFunc clean_runtime_code_cache,
    threadStateTransitionFunc transition_vm_to_native, fastThreadStateTransitionFunc fast_transition_native_to_vm, threadStateTransitionFunc slow_transition_native_to_vm) {
  assert(isolate_thread->has_status_created(), "unexpected thread state");
  guarantee(SVMIsolateData::_heap_base == nullptr, "GC doesn't support multiple isolates at the moment.");

  // verify that gc_parse_options was executed properly
  guarantee(MaxNewSize >= 0, "must be");
  guarantee(TLABSize >= 0, "must be");

  // verify all arguments
  guarantee(isolate_thread != nullptr, "must be");
  guarantee(heap_base != nullptr, "must be");
  guarantee(dynamic_hub_klass != nullptr, "must be");
  guarantee(filler_object_klass != nullptr, "must be");
  guarantee(filler_array_klass != nullptr, "must be");
  guarantee(string_klass != nullptr, "must be");
  guarantee(system_klass != nullptr, "must be");
  guarantee(static_object_fields != nullptr, "must be");
  guarantee(static_primitive_fields != nullptr, "must be");
  guarantee(vm_operation_thread != nullptr, "must be");
  guarantee(safepoint != nullptr, "must be");
  guarantee(runtime_code_info_memory != nullptr, "must be");
  guarantee(reference_map_compressed_offset_shift == ReferenceMapCompressedOffsetShift, "must be");
  guarantee(thread_locals_reference_map != nullptr, "must be");
  guarantee(offsets != nullptr, "must be");
  guarantee(offsets_length > 0, "must be");
  guarantee(collect_for_allocation_op != nullptr, "must be");
  guarantee(execute_pause_remark_op != nullptr, "must be");
  guarantee(execute_pause_cleanup_op != nullptr, "must be");
  guarantee(collect_full_op != nullptr, "must be");
  guarantee(verify_heap_op != nullptr, "must be");
  guarantee(try_initiate_conc_mark_op != nullptr, "must be");
  guarantee(wait_for_vm_operation_execution_status != nullptr, "must be");
  guarantee(update_vm_operation_execution_status != nullptr, "must be");
  guarantee(is_vm_operation_finished != nullptr, "must be");
  guarantee(fetch_thread_stack_frames != nullptr, "must be");
  guarantee(free_thread_stack_frames != nullptr, "must be");
  guarantee(transition_vm_to_native != nullptr, "must be");
  guarantee(fast_transition_native_to_vm != nullptr, "must be");
  guarantee(slow_transition_native_to_vm != nullptr, "must be");
  guarantee(dynamic_hub_hashing_interface_mask == DynamicHubHashingInterfaceMask, "must be");
  guarantee(dynamic_hub_hashing_shift_offset == DynamicHubHashingShiftOffset, "must be");

  // apply arguments
  SVMIsolateData::_heap_base = heap_base;
  SVMIsolateData::_static_object_fields = static_object_fields;
  SVMIsolateData::_static_primitive_fields = static_primitive_fields;
  SVMIsolateData::_vm_operation_thread = vm_operation_thread;
  SVMIsolateData::_safepoint = safepoint;
  SVMIsolateData::_runtime_code_info_memory = runtime_code_info_memory;
  SVMIsolateData::_klasses_assumed_reachable_for_code_unloading = klasses_assumed_reachable_for_code_unloading;

  SVMGlobalData::_thread_locals_reference_map = thread_locals_reference_map;
  SVMGlobalData::_use_string_inlining = use_string_inlining;
  SVMGlobalData::_closed_type_world = closed_type_world;
  SVMGlobalData::_use_interface_hashing = use_interface_hashing;
  SVMGlobalData::_interface_hashing_max_id = interface_hashing_max_id;
  SVMGlobalData::_collect_for_allocation_op = collect_for_allocation_op;
  SVMGlobalData::_execute_pause_remark_op = execute_pause_remark_op;
  SVMGlobalData::_execute_pause_cleanup_op = execute_pause_cleanup_op;
  SVMGlobalData::_wait_for_vm_operation_execution_status = wait_for_vm_operation_execution_status;
  SVMGlobalData::_update_vm_operation_execution_status = update_vm_operation_execution_status;
  SVMGlobalData::_is_vm_operation_finished = is_vm_operation_finished;
  SVMGlobalData::_collect_full_op = collect_full_op;
  SVMGlobalData::_verify_heap_op = verify_heap_op;
  SVMGlobalData::_try_initiate_conc_mark_op = try_initiate_conc_mark_op;
  SVMGlobalData::_fetch_thread_stack_frames = fetch_thread_stack_frames;
  SVMGlobalData::_free_thread_stack_frames = free_thread_stack_frames;
  SVMGlobalData::_fetch_continuation_stack_frames = fetch_continuation_stack_frames;
  SVMGlobalData::_free_continuation_stack_frames = free_continuation_stack_frames;
  SVMGlobalData::_fetch_code_infos = fetch_code_infos;
  SVMGlobalData::_free_code_infos = free_code_infos;
  SVMGlobalData::_transition_vm_to_native = transition_vm_to_native;
  SVMGlobalData::_try_fast_transition_native_to_vm = fast_transition_native_to_vm;
  SVMGlobalData::_slow_transition_native_to_vm = slow_transition_native_to_vm;
  SVMGlobalData::_clean_runtime_code_cache = clean_runtime_code_cache;
  SVMGlobalData::initialize_offsets(offsets, offsets_length);
  SVMGlobalData::verify_offsets(perf_data_support);

  Universe::_dynamic_hub_klass = dynamic_hub_klass;
  Universe::_fillerArrayKlass = filler_array_klass;
  vmClasses::_string_klass = string_klass;
  vmClasses::_system_klass = system_klass;
  vmClasses::_filler_object_klass = filler_object_klass;
  CollectedHeap::set_filler_object_klass(filler_object_klass);
  guarantee(filler_object_klass->size_helper() == oopDesc::header_size(), "must be");

  // The option UsePerfData is a bit special as it depends on a hosted flag.
  if (!perf_data_support) {
    // AllowVMInspection was disabled when building the native-image. So, no matter which value is passed for UsePerfData
    // (at image build time or at runtime), we always need to disable UsePerfData.
    if (FLAG_SET_CMDLINE(UsePerfData, false) != JVMFlag::SUCCESS) {
      return nullptr;
    }
  }

  CompressedOops::initialize();
  jint result = Threads::create_vm(isolate_thread);

#if defined(PRINT_NI_OPTIONS) && !defined(PRODUCT)
  PRINT_NI_FLAGS;
#endif // PRINT_NI_OPTIONS && !PRODUCT

  if (result == JNI_OK) {
    // verify a couple more data structures (if those guarantees fail, check if there is a mismatch between the gc_constants below and the constants on Native Image side)
    guarantee(IsolateThread::get_first_thread() != nullptr, "main thread must be registered");
    guarantee(IsolateThread::get_first_thread()->next_thread() == nullptr, "at this point in time, only the main thread may exist");
    guarantee(Threads::number_of_non_daemon_threads() == 1, "only one application thread should be active");
    guarantee(SafepointSynchronize::get_safepoint_state() == SafepointSynchronize::not_at_safepoint, "must not be at a safepoint");

    // return a data structure with relevant offsets and constants (some of the values depend on the VM arguments)
    gc_constants.card_table_address = (address)ci_card_table_address();
    gc_constants.gc_total_collections_address = (address)Universe::heap()->total_collections_address();
    gc_constants.tlab_top_offset = in_bytes(Thread::tlab_top_offset());
    gc_constants.tlab_end_offset = in_bytes(Thread::tlab_end_offset());
    gc_constants.card_table_shift = CardTable::card_shift();
    gc_constants.java_thread_size = sizeof(JavaThread);
    gc_constants.vm_operation_data_size = sizeof(VM_OperationData);
    gc_constants.vm_operation_wrapper_data_size = sizeof(VM_OperationWrapperData);
    gc_constants.dirty_card_value = CardTable::dirty_card_val();
    return &gc_constants;
  }
  return nullptr;
}

// NO_TRANSITION - Uninterruptible code that may be called by any Java thread.
EXPORT_FOR_SVM void svm_gc_update_option_value(oop optionName, jlong value) {
  assert(IsolateThread::current()->has_status_created() || IsolateThread::current()->has_status_java(), "unexpected thread state");

  bool update_logging = false;
  if (java_lang_String::equals(optionName, "VerboseGC")) {
    FLAG_SET_MGMT(VerboseGC, value == 1);
    update_logging = true;
  } else if (java_lang_String::equals(optionName, "PrintGC")) {
    FLAG_SET_MGMT(PrintGC, value == 1);
    update_logging = true;
  } else if (java_lang_String::equals(optionName, "DisableExplicitGC")) {
    FLAG_SET_MGMT(DisableExplicitGC, value == 1);
  } else {
    fatal("Only the values of certain GC options can be changed at run-time.");
  }

  if (update_logging) {
    LogConfiguration::disable_logging();
    if (PrintGC || VerboseGC) {
      LogConfiguration::configure_stdout(LogLevel::Info, !VerboseGC, LOG_TAGS(gc));
    }
  }
}

// NO_TRANSITION - Only called during teardown after all other threads were already torn down.
EXPORT_FOR_SVM bool svm_gc_teardown() {
  assert(IsolateThread::current()->has_status_java(), "unexpected thread state");
  SVMIsolateData::_during_teardown = true;
#ifdef ASSERT
  int thread_count = 0;
  for (JavaThreadIteratorWithHandle jtiwh; JavaThread *jt = jtiwh.next(); ) {
    thread_count++;
  }
  assert(thread_count == 1, "all other threads must have been stopped");
#endif // ASSERT
  Threads::destroy_vm();
  return true;
}

// NO_TRANSITION - Uninterruptible code that may be called by any Java thread.
EXPORT_FOR_SVM void svm_gc_attach_thread(IsolateThread *thread) {
  assert(thread->has_status_created(), "unexpected thread state");
  JavaThread *java_thread = new (thread->java_thread()) JavaThread();
  assert(is_aligned(java_thread, wordSize), "must be");
  java_thread->initialize_thread_current();
  java_thread->initialize();
}

// NO_TRANSITION - Uninterruptible code that may be called by any Java thread.
// When this method is called, the thread may already have the state "IGNORE_SAFEPOINT" but by holding the threads lock on the Native Image-side,
// it is guaranteed that no other thread can trigger a safepoint.
EXPORT_FOR_SVM void svm_gc_detach_thread(IsolateThread *thread) {
  assert(thread->has_status_java(), "unexpected thread state");
  JavaThread *java_thread = thread->java_thread();
  java_thread->~JavaThread();
}

// NO_TRANSITION - Uninterruptible code that may be called by any Java thread.
EXPORT_FOR_SVM void svm_gc_retire_tlab() {
  assert(IsolateThread::current()->has_status_java(), "unexpected thread state");
  if (UseTLAB) {
    JavaThread::current()->tlab().retire(nullptr);
  }
}

// NO_TRANSITION - Uninterruptible code that is only called by the VM thread during the safepoint handling. So, no other thread can trigger a safepoint in the meanwhile.
EXPORT_FOR_SVM void svm_gc_prepare_for_safepoint() {
  assert(Thread::current()->is_VM_thread(), "must be the VM thread");
  assert(IsolateThread::current()->has_status_java(), "unexpected thread state");
  Universe::heap()->safepoint_synchronize_begin();
}

// NO_TRANSITION - Uninterruptible code that is only called by the VM thread during the safepoint handling. So, no other thread can trigger a safepoint in the meanwhile.
EXPORT_FOR_SVM void svm_gc_end_safepoint() {
  assert(Thread::current()->is_VM_thread(), "must be the VM thread");
  assert(IsolateThread::current()->has_status_java(), "unexpected thread state");
  Universe::heap()->safepoint_synchronize_end();
}

// TO_VM - Called by any Java thread. Uses oops. May block. May cause a safepoint.
EXPORT_FOR_SVM void svm_gc_collect(int cause) {
  assert(IsolateThread::current()->has_status_vm(), "unexpected thread state");
  if (!DisableExplicitGC) {
    Unimplemented();
  }
}

// TO_NATIVE - Only called from the VM thread.
EXPORT_FOR_SVM bool svm_gc_execute_vm_operation_prologue(VM_OperationData *data) {
  assert(Thread::current()->is_VM_thread(), "must be the VM thread");
  assert(IsolateThread::current()->has_status_native(), "unexpected thread state");
  return data->vm_operation()->doit_prologue();
}

// TO_NATIVE - Only called from the VM thread at a safepoint.
EXPORT_FOR_SVM void svm_gc_execute_vm_operation_main(VM_OperationData *data) {
  assert(Thread::current()->is_VM_thread(), "must be the VM thread");
  assert(SafepointSynchronize::get_safepoint_state() == SafepointSynchronize::at_safepoint, "must be at a safepoint");
  assert(IsolateThread::current()->has_status_native(), "unexpected thread state");
  data->vm_operation()->evaluate();
}

// TO_NATIVE - Only called from the VM thread.
EXPORT_FOR_SVM void svm_gc_execute_vm_operation_epilogue(VM_OperationData *data) {
  assert(Thread::current()->is_VM_thread(), "must be the VM thread");
  assert(IsolateThread::current()->has_status_native(), "unexpected thread state");
  data->vm_operation()->doit_epilogue();
}

// TO_VM - May be called by any Java thread. Uses oops. May block. May cause a safepoint.
EXPORT_FOR_SVM oop svm_gc_allocate_instance(InstanceKlass *k) {
  assert(IsolateThread::current()->has_status_vm(), "unexpected thread state");
  assert(k->is_instance_klass(), "must be");
  oop result = Universe::heap()->obj_allocate(k, k->size_helper());
  if (result != nullptr) {
    BarrierSet::barrier_set()->on_slowpath_allocation_exit(JavaThread::current(), result);
  }
  return result;
}

// TO_VM - May be called by any Java thread. Uses oops. May block. May cause a safepoint.
EXPORT_FOR_SVM oop svm_gc_allocate_array(ArrayKlass *k, int length) {
  assert(IsolateThread::current()->has_status_vm(), "unexpected thread state");
  assert(k->is_array_klass(), "must be");
  assert(length >= 0, "must be");

  oop result = nullptr;
  if (length >= 0 && length <= k->max_length()) {
    int size = k->object_size(length);
    result = Universe::heap()->array_allocate(k, size, length, true);
    if (result != nullptr) {
      BarrierSet::barrier_set()->on_slowpath_allocation_exit(JavaThread::current(), result);
    }
  }
  return result;
}

// TO_VM - May be called by any Java thread. Uses oops. May block. May cause a safepoint.
EXPORT_FOR_SVM oop svm_gc_allocate_stack_chunk(InstanceStackChunkKlass *k, int length) {
  assert(IsolateThread::current()->has_status_vm(), "unexpected thread state");
  assert(k->is_stack_chunk_instance_klass(), "must be");
  assert(length >= 0, "must be");

  oop result = k->allocate(length);
  if (result != nullptr) {
    BarrierSet::barrier_set()->on_slowpath_allocation_exit(JavaThread::current(), result);
  }
  return result;
}

// TO_VM - May be called by any Java thread. Uses oops. May block. May cause a safepoint.
EXPORT_FOR_SVM oop svm_gc_allocate_pod(InstancePodKlass *k, int length) {
  assert(IsolateThread::current()->has_status_vm(), "unexpected thread state");
  assert(k->is_pod_instance_klass(), "must be");
  assert(length >= 0, "must be");

  podOop result = k->allocate(length);
  if (result != nullptr) {
    BarrierSet::barrier_set()->on_slowpath_allocation_exit(JavaThread::current(), result);
  }
  return result;
}

// NO_TRANSITION - Uninterruptible code that may be called by any Java thread.
EXPORT_FOR_SVM void svm_gc_pin_object(oop o) {
  assert(IsolateThread::current()->has_status_java(), "unexpected thread state");
  Unimplemented();
}

// NO_TRANSITION - Uninterruptible code that may be called by any Java thread.
EXPORT_FOR_SVM void svm_gc_unpin_object(oop o) {
  assert(IsolateThread::current()->has_status_java(), "unexpected thread state");
  Unimplemented();
}

// NO_TRANSITION - Uninterruptible code that may be called by any Java thread.
EXPORT_FOR_SVM void svm_gc_pre_write_barrier(oop obj) {
  assert(IsolateThread::current()->has_status_java(), "unexpected thread state");
  assert(Universe::heap()->is_in(obj), "must be in the available part of the heap");

  Unimplemented();
}

// NO_TRANSITION - Uninterruptible code that may be called by any Java thread.
EXPORT_FOR_SVM void svm_gc_post_write_barrier(void *card_addr) {
  assert(IsolateThread::current()->has_status_java(), "unexpected thread state");

  Unimplemented();
}

// NO_TRANSITION - Uninterruptible code that may be called by any Java thread.
EXPORT_FOR_SVM void svm_gc_dirty_all_references_of(stackChunkOop stackChunk) {
  assert(IsolateThread::current()->has_status_java(), "unexpected thread state");

  if (Universe::heap()->requires_barriers(stackChunk)) {
    stackChunk->do_barriers();
  }
}

// NO_TRANSITION - Uninterruptible code that may be called by any Java thread.
EXPORT_FOR_SVM void svm_gc_verify_oop(oop obj) {
  assert(IsolateThread::current()->has_status_java(), "unexpected thread state");
  ShouldNotReachHere();
}

// NO_TRANSITION - Uninterruptible code that may be called by any Java thread.
EXPORT_FOR_SVM bool svm_gc_validate_object(oop parent, oop child) {
  assert(IsolateThread::current()->has_status_java(), "unexpected thread state");
  bool ret = true;
  if (!Universe::heap()->is_in(parent)) {
    tty->print_cr("Parent Object " INTPTR_FORMAT " not in heap", p2i(parent));
    parent->print();
    ret = false;
  }
  if (!Universe::heap()->is_in(child)) {
    tty->print_cr("Child Object " INTPTR_FORMAT " not in heap", p2i(child));
    child->print();
    ret = false;
  }
  return ret;
}

// NO_TRANSITION - Uninterruptible code that may be called by any Java thread.
EXPORT_FOR_SVM void svm_gc_log_printf(char* format, jlong v1, jlong v2, jlong v3) {
  assert(IsolateThread::current()->has_status_java(), "unexpected thread state");
  ResourceMark rm;
  tty->print(format, v1, v2, v3);
}

// NO_TRANSITION - Uninterruptible code that may be called by any Java thread.
EXPORT_FOR_SVM jlong svm_gc_millis_since_last_whole_heap_examined() {
  assert(IsolateThread::current()->has_status_java(), "unexpected thread state");
  size_t n = Universe::heap()->millis_since_last_whole_heap_examined();
  return convert_size_t_to_jlong(n);
}

// TO_NATIVE - Only called by the reference handler thread. May block.
// Checks if an oop reference is non-null but that is fine as we hold the Heap_lock.
EXPORT_FOR_SVM bool svm_gc_has_reference_pending_list() {
  // see JVM_HasReferencePendingList
  assert(IsolateThread::current()->has_status_native_or_safepoint(), "unexpected thread state");
  MonitorLocker ml(Heap_lock);
  return Universe::has_reference_pending_list();
}

// TO_VM - Only called by the reference handler thread. Uses oops. May block.
EXPORT_FOR_SVM oop svm_gc_get_and_clear_reference_pending_list() {
  // see JVM_GetAndClearReferencePendingList
  assert(IsolateThread::current()->has_status_vm(), "unexpected thread state");
  MonitorLocker ml(Heap_lock);
  oop ref = Universe::reference_pending_list();
  if (ref != nullptr) {
    Universe::clear_reference_pending_list();
  }
  return ref;
}

// NO_TRANSITION - Uninterruptible code that is only called by the reference handler thread.
EXPORT_FOR_SVM uint64_t svm_gc_get_reference_pending_list_wakeup_count() {
  assert(IsolateThread::current()->has_status_java(), "unexpected thread state");
  return Universe::reference_pending_list_wakeup_count();
}

// TO_NATIVE - Only called by the reference handler thread. May block.
// Checks if an oop reference is non-null but that is fine as we hold the Heap_lock.
EXPORT_FOR_SVM bool svm_gc_wait_for_reference_pending_list(uint64_t initial_wakeup_count) {
  // see JVM_WaitForReferencePendingList
  assert(IsolateThread::current()->has_status_native_or_safepoint(), "unexpected thread state");
  MonitorLocker ml(Heap_lock);
  while (!Universe::has_reference_pending_list() && Universe::reference_pending_list_wakeup_count() == initial_wakeup_count) {
    ml.wait();
  }
  return Universe::reference_pending_list_wakeup_count() == initial_wakeup_count;
}

// TO_NATIVE - May be called by any thread. May block.
EXPORT_FOR_SVM void svm_gc_wake_up_reference_pending_list_waiters() {
  assert(IsolateThread::current()->has_status_native_or_safepoint(), "unexpected thread state");
  MonitorLocker ml(Heap_lock);
  Universe::request_reference_pending_list_waiters_wakeup();
  ml.notify_all();
}

// NO_TRANSITION - Almost uninterruptible code that may be called from any Java thread.
EXPORT_FOR_SVM void svm_gc_register_object_fields(nmethod* nm) {
  assert(IsolateThread::current()->has_status_java(), "unexpected thread state");
  assert(nm->state() == nmethod::state_code_constants_live, "must be");

  Unimplemented();
}

// NO_TRANSITION - Almost uninterruptible code that may be called from any Java thread.
EXPORT_FOR_SVM void svm_gc_register_code_constants(nmethod* nm) {
  assert(IsolateThread::current()->has_status_java(), "unexpected thread state");
  assert(nm->state() == nmethod::state_code_constants_live, "must be");

  Unimplemented();
}

// NO_TRANSITION - Almost uninterruptible code that may be called from any Java thread.
EXPORT_FOR_SVM void svm_gc_register_frame_metadata(nmethod* nm) {
  assert(IsolateThread::current()->has_status_java(), "unexpected thread state");
  assert(nm->state() == nmethod::state_code_constants_live, "must be");

  Unimplemented();
}

// NO_TRANSITION - Almost uninterruptible code that may be called from any Java thread.
EXPORT_FOR_SVM void svm_gc_register_deopt_metadata(nmethod* nm) {
  assert(IsolateThread::current()->has_status_java(), "unexpected thread state");
  assert(nm->state() == nmethod::state_code_constants_live, "must be");

  Unimplemented();
}

// NO_TRANSITION - Only called when printing diagnostics.
EXPORT_FOR_SVM void svm_gc_get_internal_state(GCInternalState *gc_internal_data) {
  assert(IsolateThread::current()->has_status_java(), "unexpected thread state");

  Unimplemented();
}

// NO_TRANSITION - Only called when printing diagnostics.
EXPORT_FOR_SVM const char* svm_gc_get_current_thread_name() {
  assert(IsolateThread::current()->has_status_java(), "unexpected thread state");
  Thread* cur = Thread::current_or_null();
  if (cur != nullptr) {
    return cur->name();
  }
  return nullptr;
}

// NO_TRANSITION - Can be called by any thread. This is racy by design.
EXPORT_FOR_SVM jlong svm_gc_get_thread_allocated_memory(IsolateThread *thread) {
  assert(IsolateThread::current()->has_status_java(), "unexpected thread state");
  return thread->java_thread()->cooked_allocated_bytes();
}

// TO_VM - May be called by any Java thread. May block.
EXPORT_FOR_SVM jlong svm_gc_get_used_memory() {
  assert(IsolateThread::current()->has_status_vm(), "unexpected thread state");
  MutexLocker x(Heap_lock);
  size_t n = Universe::heap()->used();
  assert(n >= SVMGlobalData::_image_heap_used, "must be");
  return convert_size_t_to_jlong(n - SVMGlobalData::_image_heap_used);
}

// TO_VM - May be called by any Java thread. May block.
EXPORT_FOR_SVM jlong svm_gc_get_free_memory() {
  assert(IsolateThread::current()->has_status_vm(), "unexpected thread state");
  CollectedHeap* ch = Universe::heap();
  size_t n;
  {
     MutexLocker x(Heap_lock);
     n = ch->capacity() - ch->used() - SVMGlobalData::_image_heap_waste;
  }
  return convert_size_t_to_jlong(n);
}

// NO_TRANSITION - Uninterruptible code that may be called by any Java thread.
EXPORT_FOR_SVM jlong svm_gc_get_total_memory() {
  assert(IsolateThread::current()->has_status_java(), "unexpected thread state");
  size_t n = Universe::heap()->capacity();
  assert(n >= SVMGlobalData::_image_heap_size, "must be");
  return convert_size_t_to_jlong(n - SVMGlobalData::_image_heap_size);
}

// NO_TRANSITION - Uninterruptible code that may be called by any Java thread.
EXPORT_FOR_SVM jlong svm_gc_get_max_memory() {
  assert(IsolateThread::current()->has_status_java(), "unexpected thread state");
  size_t n = Universe::heap()->max_capacity();
  assert(n >= SVMGlobalData::_image_heap_size, "must be");
  return convert_size_t_to_jlong(n - SVMGlobalData::_image_heap_size);
}

// NO_TRANSITION - Can be called by any thread.
EXPORT_FOR_SVM size_t svm_gc_get_used_memory_after_last_gc() {
  assert(IsolateThread::current()->has_status_java(), "unexpected thread state");
  size_t n = Universe::heap()->used_at_last_gc();
  assert(n >= SVMGlobalData::_image_heap_used, "must be");
  return convert_size_t_to_jlong(n - SVMGlobalData::_image_heap_used);
}

} // extern C
