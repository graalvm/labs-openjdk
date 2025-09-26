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

#ifndef SHARE_RUNTIME_MUTEXLOCKER_HPP
#define SHARE_RUNTIME_MUTEXLOCKER_HPP

#include "memory/allocation.hpp"
#include "runtime/flags/flagSetting.hpp"
#include "runtime/mutex.hpp"
#ifdef SVM
#include "svm/svmIsolateThread.hpp"
#endif // SVM

// Mutexes used in the VM.

extern Monitor* Heap_lock;                       // a lock on the heap
extern Monitor* G1StrongCodeRoots_lock;          // NOTE (chaeubl): was named CodeCache_lock previously - as this lock is reachable from uninterruptible SVM code, we may only hold it in VERY few places.
extern Mutex*   NonJavaThreadsList_lock;         // a lock on the NonJavaThreads list
extern Mutex*   NonJavaThreadsListSync_lock;     // a lock for NonJavaThreads list synchronization
extern Monitor* CGC_lock;                        // used for coordination between
                                                 // fore- & background GC threads.
extern Monitor* STS_lock;                        // used for joining/leaving SuspendibleThreadSet.
extern Monitor* G1OldGCCount_lock;               // in support of "concurrent" full gc
extern Mutex*   G1RareEvent_lock;                // Synchronizes (rare) parallel GC operations.
extern Mutex*   G1DetachedRefinementStats_lock;  // Lock protecting detached refinement stats
extern Mutex*   MarkStackFreeList_lock;          // Protects access to the global mark stack free list.
extern Mutex*   MarkStackChunkList_lock;         // Protects access to the global mark stack chunk list.
extern Mutex*   MonitoringSupport_lock;          // Protects updates to the serviceability memory pools and allocated memory high water mark.
extern Monitor* ConcurrentGCBreakpoints_lock;    // Protects concurrent GC breakpoint management
extern Monitor* Terminator_lock;                 // a lock used to guard termination of the vm
extern Monitor* InitCompleted_lock;              // a lock used to signal threads waiting on init completed

extern Mutex*   FreeList_lock;                   // protects the free region list during safepoints
extern Mutex*   OldSets_lock;                    // protects the old region sets
extern Mutex*   Uncommit_lock;                   // protects the uncommit list when not at safepoints
extern Monitor* RootRegionScan_lock;             // used to notify that the CM threads have finished scanning the IM snapshot regions

extern Monitor* PeriodicTask_lock;               // protects the periodic task structure

extern Mutex*   tty_lock;                        // lock to synchronize output.

// A MutexLocker provides mutual exclusion with respect to a given mutex
// for the scope which contains the locker.  The lock is an OS lock, not
// an object lock, and the two do not interoperate.  Do not use Mutex-based
// locks to lock on Java objects, because they will not be respected if a
// that object is locked using the Java locking mechanism.
//
//                NOTE WELL!!
//
// See orderAccess.hpp.  We assume throughout the VM that MutexLocker's
// and friends constructors do a fence, a lock and an acquire *in that
// order*.  And that their destructors do a release and unlock, in *that*
// order.  If their implementations change such that these assumptions
// are violated, a whole lot of code will break.

char *lock_name(Mutex *mutex);

// for debugging: check that we're already owning this lock (or are at a safepoint / handshake)
#ifdef ASSERT
void assert_locked_or_safepoint(const Mutex* lock);
void assert_lock_strong(const Mutex* lock);
#else
#define assert_locked_or_safepoint(lock)
#define assert_lock_strong(lock)
#endif

// Internal implementation. Skips on null Mutex.
// Subclasses enforce stronger invariants.
class MutexLockerImpl: public StackObj {
 protected:
  Mutex* _mutex;

  MutexLockerImpl(Mutex* mutex, Mutex::SafepointCheckFlag flag = Mutex::_safepoint_check_flag) :
    _mutex(mutex) {
    bool no_safepoint_check = flag == Mutex::_no_safepoint_check_flag;
    if (_mutex != nullptr) {
      if (no_safepoint_check) {
        _mutex->lock_without_safepoint_check();
      } else {
        _mutex->lock();
      }
    }
  }

  MutexLockerImpl(Thread* thread, Mutex* mutex, Mutex::SafepointCheckFlag flag = Mutex::_safepoint_check_flag) :
    _mutex(mutex) {
    bool no_safepoint_check = flag == Mutex::_no_safepoint_check_flag;
    if (_mutex != nullptr) {
      if (no_safepoint_check) {
        _mutex->lock_without_safepoint_check(thread);
      } else {
        _mutex->lock(thread);
      }
    }
  }

  ~MutexLockerImpl() {
    if (_mutex != nullptr) {
      assert_lock_strong(_mutex);
      _mutex->unlock();
    }
  }

 public:
  static void post_initialize();
};

// Simplest mutex locker.
// Does not allow null mutexes.
class MutexLocker: public MutexLockerImpl {
 public:
   MutexLocker(Mutex* mutex, Mutex::SafepointCheckFlag flag = Mutex::_safepoint_check_flag) :
     MutexLockerImpl(mutex, flag) {
     assert(mutex != nullptr, "null mutex not allowed");
   }

   MutexLocker(Thread* thread, Mutex* mutex, Mutex::SafepointCheckFlag flag = Mutex::_safepoint_check_flag) :
     MutexLockerImpl(thread, mutex, flag) {
     assert(mutex != nullptr, "null mutex not allowed");
   }
};

// Conditional mutex locker.
// Like MutexLocker above, but only locks when condition is true.
class ConditionalMutexLocker: public MutexLockerImpl {
 public:
   ConditionalMutexLocker(Mutex* mutex, bool condition, Mutex::SafepointCheckFlag flag = Mutex::_safepoint_check_flag) :
     MutexLockerImpl(condition ? mutex : nullptr, flag) {
     assert(!condition || mutex != nullptr, "null mutex not allowed when locking");
   }

   ConditionalMutexLocker(Thread* thread, Mutex* mutex, bool condition, Mutex::SafepointCheckFlag flag = Mutex::_safepoint_check_flag) :
     MutexLockerImpl(thread, condition ? mutex : nullptr, flag) {
     assert(!condition || mutex != nullptr, "null mutex not allowed when locking");
   }
};

// A MonitorLocker is like a MutexLocker above, except it allows
// wait/notify as well which are delegated to the underlying Monitor.
// It also disallows null.

class MonitorLocker: public MutexLockerImpl {
  Mutex::SafepointCheckFlag _flag;

 protected:
  Monitor* as_monitor() const {
    return static_cast<Monitor*>(_mutex);
  }

 public:
  MonitorLocker(Monitor* monitor, Mutex::SafepointCheckFlag flag = Mutex::_safepoint_check_flag) :
    MutexLockerImpl(monitor, flag), _flag(flag) {
    // Superclass constructor did locking
    assert(monitor != nullptr, "NULL monitor not allowed");
  }

  MonitorLocker(Thread* thread, Monitor* monitor, Mutex::SafepointCheckFlag flag = Mutex::_safepoint_check_flag) :
    MutexLockerImpl(thread, monitor, flag), _flag(flag) {
    // Superclass constructor did locking
    assert(monitor != nullptr, "NULL monitor not allowed");
  }

  bool wait(int64_t timeout = 0) {
    return _flag == Mutex::_safepoint_check_flag ?
          as_monitor()->wait(timeout) : as_monitor()->wait_without_safepoint_check(timeout);
  }

  void notify_all() {
    as_monitor()->notify_all();
  }

  void notify() {
    as_monitor()->notify();
  }
};

// A MutexUnlocker temporarily exits a previously
// entered mutex for the scope which contains the unlocker.

class MutexUnlocker: StackObj {
 private:
  Mutex* _mutex;
  bool _no_safepoint_check;

 public:
  MutexUnlocker(Mutex* mutex, Mutex::SafepointCheckFlag flag = Mutex::_safepoint_check_flag) :
    _mutex(mutex),
    _no_safepoint_check(flag == Mutex::_no_safepoint_check_flag) {
    _mutex->unlock();
  }

  ~MutexUnlocker() {
    if (_no_safepoint_check) {
      _mutex->lock_without_safepoint_check();
    } else {
      _mutex->lock();
    }
  }
};

#endif // SHARE_RUNTIME_MUTEXLOCKER_HPP
