/*
 * Copyright (c) 1997, 2024, Oracle and/or its affiliates. All rights reserved.
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

#ifndef SHARE_RUNTIME_SAFEPOINT_HPP
#define SHARE_RUNTIME_SAFEPOINT_HPP

#include "memory/allStatic.hpp"
#include "runtime/javaThread.hpp"
#include "runtime/os.hpp"
#include "runtime/vmOperation.hpp"
#include "utilities/ostream.hpp"
#include "utilities/waitBarrier.hpp"

//
// Safepoint synchronization
////
// The VMThread or CMS_thread uses the SafepointSynchronize::begin/end
// methods to enter/exit a safepoint region. The begin method will roll
// all JavaThreads forward to a safepoint.
//
// JavaThreads must use the ThreadSafepointState abstraction (defined in
// thread.hpp) to indicate that that they are at a safepoint.
//
// The Mutex/Condition variable and ObjectLocker classes calls the enter/
// exit safepoint methods, when a thread is blocked/restarted. Hence, all mutex exter/
// exit points *must* be at a safepoint.


class JavaThread;

//
// Implements roll-forward to safepoint (safepoint synchronization)
//
class SafepointSynchronize : AllStatic {
 public:
  // the values are linked to the ones defined on the SVM-side
  enum SafepointState {
    not_at_safepoint,
    synchronizing,
    at_safepoint
  };

  // Query
  inline static bool is_at_safepoint()       { return get_safepoint_state() == at_safepoint; }
  inline static bool is_synchronizing()      { return get_safepoint_state() == synchronizing; }

  inline static uint64_t safepoint_id()      {
    return RawAccess<>::load_at(SVMIsolateData::_safepoint, SVMGlobalData::_offsets._safepoint._safepoint_id);
  }

  inline static int get_safepoint_state() {
    return RawAccess<>::load_at(SVMIsolateData::_safepoint, SVMGlobalData::_offsets._safepoint._safepoint_state);
  }
};

// Some helper assert macros for safepoint checks.

#define assert_at_safepoint()                                           \
  assert(SafepointSynchronize::is_at_safepoint(), "should be at a safepoint")

#define assert_at_safepoint_msg(...)                                    \
  assert(SafepointSynchronize::is_at_safepoint(), __VA_ARGS__)

#define assert_not_at_safepoint()                                       \
  assert(!SafepointSynchronize::is_at_safepoint(), "should not be at a safepoint")

#define assert_not_at_safepoint_msg(...)                                \
  assert(!SafepointSynchronize::is_at_safepoint(), __VA_ARGS__)

#endif // SHARE_RUNTIME_SAFEPOINT_HPP
