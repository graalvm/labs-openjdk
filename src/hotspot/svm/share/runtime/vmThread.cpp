/*
 * Copyright (c) 1998, 2025, Oracle and/or its affiliates. All rights reserved.
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

#include "compiler/compileBroker.hpp"
#include "gc/shared/collectedHeap.hpp"
#include "jfr/jfrEvents.hpp"
#include "jfr/support/jfrThreadId.hpp"
#include "logging/log.hpp"
#include "logging/logStream.hpp"
#include "logging/logConfiguration.hpp"
#include "memory/resourceArea.hpp"
#include "memory/universe.hpp"
#include "oops/oop.inline.hpp"
#include "oops/verifyOopClosure.hpp"
#include "runtime/atomic.hpp"
#include "runtime/cpuTimeCounters.hpp"
#include "runtime/handles.inline.hpp"
#include "runtime/interfaceSupport.inline.hpp"
#include "runtime/javaThread.inline.hpp"
#include "runtime/jniHandles.hpp"
#include "runtime/mutexLocker.hpp"
#include "runtime/os.hpp"
#include "runtime/perfData.hpp"
#include "runtime/safepoint.hpp"
#include "runtime/synchronizer.hpp"
#include "runtime/timerTrace.hpp"
#include "runtime/vmThread.hpp"
#include "runtime/vmOperations.hpp"
#include "utilities/dtrace.hpp"
#include "utilities/events.hpp"
#include "utilities/vmError.hpp"


void VMThread::execute(VM_Operation* op) {
  Thread* current_thread = Thread::current();

  // NOTE (chaeubl): The VM operation thread in native-image is a normal Java thread and can allocate Java objects.
  // Therefore, the VM thread can also end up in code paths that lock the Heap_lock (e.g., via the allocation
  // slow-path or a GC). This is a major difference to HotSpot and therefore requires extra care to avoid deadlocks.
  //
  // Even if the current thread is the VM thread, we still call into native-image for executing the VM operation.
  // This is necessary as the new VM operation may have other safepoint requirements than the VM operation that
  // is currently being executed.
  // As the native-image VM thread is a normal Java thread, we have to execute VM_Operation::doit_prologue() and
  // VM_Operation::doit_epilogue() even if the current thread is the VM thread.
  //
  // If thread A queues a VM operation, then it is crucial that no other thread can queue another VM operation
  // until the VM operation of thread A was executed and thread A is back in VM mode. Otherwise, the race condition
  // mentioned below can occur. In other words: each executed VM operation must be able to transport allocated
  // memory back to the thread that requested the allocation.
  //
  // Most VM operations lock the Heap_lock in the prologue. This is something that we can't do without risking a
  // deadlock with the VM operation thread (see the example below). Therefore, we need a rather complex mechanism
  // that at first blocks the VM operation thread before executing the prologue.
  //
  // Race condition example:
  // - thread A queues VM operation X
  // - thread B queues VM operation Y
  // - the VM operation thread executes X and allocates Java heap memory (no klass ptr is written yet)
  // - the VM operation thread executes Y, which triggers a GC
  // - the GC crashes as it encounters uninitialized memory
  //
  // Deadlock example:
  // - thread A queues VM operation X
  // - the VM thread starts executing VM operation X
  // - thread B wants to queue the VM operation Y. It acquires the Heap_lock and calls into SVM for queuing the
  //   VM operation Y. There it is blocked when trying to lock the VM operation mutex.
  // - the VM thread allocates a Java object and needs a slow-path allocation. For that, it tries to lock the
  //   Heap_lock and gets blocked as thread B holds the Heap_lock.

  address heap_base = CompressedOops::base();
  IsolateThread *isolate_thread = nullptr;
  if (current_thread->is_Java_thread()) {
    isolate_thread = ((JavaThread*)current_thread)->isolate_thread();
  }

  assert(isolate_thread == nullptr || isolate_thread->has_status_vm(), "isolate thread must be in VM state");
  if (isolate_thread != nullptr && !current_thread->is_VM_thread()) {
    SVMGlobalData::_transition_vm_to_native(isolate_thread);
  }

  VM_OperationData *op_data = op->data();
  VM_OperationWrapperData wrapper_data;
  memset(&wrapper_data, 0, sizeof(VM_OperationWrapperData));
  switch (op->type()) {
    case VM_Operation::VMOp_G1CollectForAllocation:
      SVMGlobalData::_collect_for_allocation_op(heap_base, isolate_thread, op_data, &wrapper_data);
      break;
    case VM_Operation::VMOp_G1PauseRemark:
      SVMGlobalData::_execute_pause_remark_op(heap_base, isolate_thread, op_data, &wrapper_data);
      break;
    case VM_Operation::VMOp_G1PauseCleanup:
      SVMGlobalData::_execute_pause_cleanup_op(heap_base, isolate_thread, op_data, &wrapper_data);
      break;
    case VM_Operation::VMOp_G1CollectFull:
      SVMGlobalData::_collect_full_op(heap_base, isolate_thread, op_data, &wrapper_data);
      break;
    case VM_Operation::VMOp_Verify:
      SVMGlobalData::_verify_heap_op(heap_base, isolate_thread, op_data, &wrapper_data);
      break;
    case VM_Operation::VMOp_G1TryInitiateConcMark:
      SVMGlobalData::_try_initiate_conc_mark_op(heap_base, isolate_thread, op_data, &wrapper_data);
      break;
    default:
      ShouldNotReachHere();
  }

  // The VM thread executes the VM operation directly. All other threads need to do some extra synchronization work.
  if (!current_thread->is_VM_thread()) {
    // Wait until the VM thread is in place and allows us to execute the VM operation prologue. Otherwise, deadlocks between
    // the prologue and the VM operation thread could occur (e.g., the prologue locks the Heap_lock but the VM operation thread needs a slowpath allocation).
    SVMGlobalData::_wait_for_vm_operation_execution_status(heap_base, isolate_thread, &wrapper_data, VM_Operation::EXECUTE_PROLOGUE);

    bool execute = op->doit_prologue();
    if (execute) {
      SVMGlobalData::_update_vm_operation_execution_status(heap_base, isolate_thread, &wrapper_data, VM_Operation::EXECUTE_VM_OPERATION);
      SVMGlobalData::_wait_for_vm_operation_execution_status(heap_base, isolate_thread, &wrapper_data, VM_Operation::ADJUST_THREAD_STATUS);

      if (isolate_thread != nullptr) {
        // The fast transition is always possible because no one can request a safepoint while the VM operation thread is blocked.
        bool in_vm = SVMGlobalData::_try_fast_transition_native_to_vm(isolate_thread);
        assert(in_vm && isolate_thread->has_status_vm(), "must be back in VM state");
      }

      SVMGlobalData::_update_vm_operation_execution_status(heap_base, isolate_thread, &wrapper_data, VM_Operation::FINISHED);

      op->doit_epilogue();
    } else {
      if (isolate_thread != nullptr) {
        // The fast transition is always possible because no one can request a safepoint while the VM operation thread is blocked.
        bool in_vm = SVMGlobalData::_try_fast_transition_native_to_vm(isolate_thread);
        assert(in_vm && isolate_thread->has_status_vm(), "must be back in VM state");
      }

      SVMGlobalData::_update_vm_operation_execution_status(heap_base, isolate_thread, &wrapper_data, VM_Operation::CANCELLED);
    }

    // Wait until the VM operation finished. It would be nicer to just wait until the VM thread broadcasts that the VM operation finished.
    // However, this is not necessarily possible as we would need to acquire the VM operation mutex for that to work reliably. This could
    // result in deadlocks:
    // - The VM operation thread broadcasts that the VM operation finished
    // - The VM operation thread immediately continues with the next VM operation in the queue. This VM operation needs a safepoint.
    // - The queuing thread gets blocked when trying to acquire the VM operation mutex.
    // - The VM operation can't initiate the safepoint because the queuing thread is in VM state.
    while (!SVMGlobalData::_is_vm_operation_finished(heap_base, isolate_thread, &wrapper_data)) {
      os::naked_yield();
    }
  }

  guarantee(isolate_thread == nullptr || isolate_thread->has_status_vm(), "isolate thread must be back in VM state");
}
