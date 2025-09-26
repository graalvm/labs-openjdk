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

#include "gc/shared/gc_globals.hpp"
#include "logging/log.hpp"
#include "logging/logStream.hpp"
#include "memory/resourceArea.hpp"
#include "memory/universe.hpp"
#include "runtime/javaThread.hpp"
#include "runtime/mutexLocker.hpp"
#include "runtime/safepoint.hpp"
#include "runtime/vmThread.hpp"
#include "utilities/vmError.hpp"

// Mutexes used in the VM (see comment in mutexLocker.hpp):

// NOTE (chaeubl): A mutex can either be locked with or without a safepoint check. The safepoint check
// only affects Java threads and implies a thread state transition from STATUS_IN_VM to STATUS_IN_NATIVE.
// If the thread blocks while trying to acquire the mutex, STATUS_IN_NATIVE ensures that the VM can
// reach a ssafepoint. Once the thread successfully acquires the mutex, it does a transition back to
// STATUS_IN_VM.
//
// Most locks are just used GC-internally, so they must not do a safepoint check. The major exception is
// the Heap_lock, for which we must always do a safepoint check as that lock can be locked by both the
// application and the GC (e.g., when doing a slow path allocation).


namespace svm_gc {

Monitor* Heap_lock                    = nullptr;
Monitor* G1StrongCodeRoots_lock       = nullptr;
Mutex*   NonJavaThreadsList_lock      = nullptr;
Mutex*   NonJavaThreadsListSync_lock  = nullptr;
Monitor* CGC_lock                     = nullptr;
Monitor* STS_lock                     = nullptr;
Monitor* G1OldGCCount_lock            = nullptr;
Mutex*   G1RareEvent_lock             = nullptr;
Mutex*   G1DetachedRefinementStats_lock = nullptr;
Mutex*   MarkStackFreeList_lock       = nullptr;
Mutex*   MarkStackChunkList_lock      = nullptr;
Mutex*   MonitoringSupport_lock       = nullptr;
Monitor* Terminator_lock              = nullptr;
Monitor* InitCompleted_lock           = nullptr;

Mutex*   tty_lock                     = nullptr;

Mutex*   FreeList_lock                = nullptr;
Mutex*   OldSets_lock                 = nullptr;
Mutex*   Uncommit_lock                = nullptr;
Monitor* RootRegionScan_lock          = nullptr;
Monitor* ConcurrentGCBreakpoints_lock = nullptr;

Monitor* PeriodicTask_lock            = nullptr;

#ifdef ASSERT
void assert_locked_or_safepoint(const Mutex* lock) {
  if (DebuggingContext::is_enabled() || VMError::is_error_reported()) return;
  // check if this thread owns the lock (common case)
  assert(lock != nullptr, "Need non-null lock");
  if (lock->owned_by_self()) return;
  if (SafepointSynchronize::is_at_safepoint()) return;
  if (!Universe::is_fully_initialized()) return;
  fatal("must own lock %s", lock->name());
}

// a stronger assertion than the above
void assert_lock_strong(const Mutex* lock) {
  if (DebuggingContext::is_enabled() || VMError::is_error_reported()) return;
  assert(lock != nullptr, "Need non-null lock");
  if (lock->owned_by_self()) return;
  fatal("must own lock %s", lock->name());
}
#endif

#define MUTEX_STORAGE_NAME(name) name##_storage
#define MUTEX_STORAGE(name, type) alignas(type) static uint8_t MUTEX_STORAGE_NAME(name)[sizeof(type)]
#define MUTEX_DEF(name, type, pri, ...) {                                                       \
  assert(name == nullptr, "Mutex/Monitor initialized twice");                                   \
  MUTEX_STORAGE(name, type);                                                                    \
  name = ::new(static_cast<void*>(MUTEX_STORAGE_NAME(name))) type((pri), #name, ##__VA_ARGS__); \
  Mutex::add_mutex(name);                                                                       \
}
#define MUTEX_DEFN(name, type, pri, ...) MUTEX_DEF(name, type, Mutex::pri, ##__VA_ARGS__)

// Specify relative ranked lock
#ifdef ASSERT
#define MUTEX_DEFL(name, type, held_lock, ...) MUTEX_DEF(name, type, (held_lock)->rank() - 1, ##__VA_ARGS__)
#else
#define MUTEX_DEFL(name, type, held_lock, ...) MUTEX_DEFN(name, type, safepoint, ##__VA_ARGS__)
#endif

// NOTE (chaeubl): a lot of the lock definitions below are modified. When we update to a new JDK version,
// it is crucial to only apply the delta and to never blindly overwrite those definitions because the
// HotSpot definitions don't make much sense for our special use case.
// Locks with a rank of <= nosafepoint are usually VM-internal locks that are not particularly dangerous
// in terms of deadlocks. Locks with a rank > nosafepoint need to be checked carefully.
//
// There are the following invariants:
// - a normal Java thread may only hold a single lock of rank <= nosafepoint.
// - the checks are less strict for the VM operation thread when it is at a safepoint.

// Using Padded subclasses to prevent false sharing of these global monitors and mutexes.
void mutex_init() {
  MUTEX_DEFN(tty_lock                        , PaddedMutex  , tty);      // allow to lock in VM

  MUTEX_DEFN(STS_lock                        , PaddedMonitor, nosafepoint);

  if (UseG1GC) {
    MUTEX_DEFN(CGC_lock                      , PaddedMonitor, nosafepoint);

    MUTEX_DEFN(G1DetachedRefinementStats_lock, PaddedMutex  , nosafepoint-2);

    MUTEX_DEFN(FreeList_lock                 , PaddedMutex  , service-1);
    MUTEX_DEFN(OldSets_lock                  , PaddedMutex  , nosafepoint);
    MUTEX_DEFN(Uncommit_lock                 , PaddedMutex  , service-2);
    MUTEX_DEFN(RootRegionScan_lock           , PaddedMonitor, nosafepoint-1);

    MUTEX_DEFN(MarkStackFreeList_lock        , PaddedMutex  , nosafepoint);
    MUTEX_DEFN(MarkStackChunkList_lock       , PaddedMutex  , nosafepoint);

    MUTEX_DEFN(MonitoringSupport_lock        , PaddedMutex  , service-1);      // used for serviceability monitoring support
  }

  MUTEX_DEFN(NonJavaThreadsList_lock         , PaddedMutex  , nosafepoint-1);
  MUTEX_DEFN(NonJavaThreadsListSync_lock     , PaddedMutex  , nosafepoint);

  MUTEX_DEFN(Terminator_lock                 , PaddedMonitor, nosafepoint);
  MUTEX_DEFN(InitCompleted_lock              , PaddedMonitor, nosafepoint);

  MUTEX_DEFN(ConcurrentGCBreakpoints_lock    , PaddedMonitor, safepoint, true);

  MUTEX_DEFN(Heap_lock                       , PaddedMonitor, safepoint, true); // Doesn't safepoint check during termination.

  // NOTE (chaeubl): converted to a nosafepoint lock because this lock is used during startup/teardown
  // when uninterruptible code is executed (so, we don't want to risk any safepoints).
  MUTEX_DEFN(PeriodicTask_lock               , PaddedMonitor, nosafepoint);

  MUTEX_DEFN(G1StrongCodeRoots_lock          , PaddedMonitor, nosafepoint);

  if (UseG1GC) {
    MUTEX_DEFN(G1OldGCCount_lock             , PaddedMonitor, safepoint-1, true);
    MUTEX_DEFN(G1RareEvent_lock              , PaddedMutex  , nosafepoint);
  }
}

#undef MUTEX_DEFL
#undef MUTEX_DEFN
#undef MUTEX_DEF
#undef MUTEX_STORAGE
#undef MUTEX_STORAGE_NAME

void MutexLockerImpl::post_initialize() {
}

} // namespace svm_gc

