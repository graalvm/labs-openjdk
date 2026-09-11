/*
 * Copyright (c) 2017, 2023, Oracle and/or its affiliates. All rights reserved.
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

#ifndef SHARE_RUNTIME_THREADSMR_HPP
#define SHARE_RUNTIME_THREADSMR_HPP

#include "oops/oop.hpp"
#include "runtime/javaThread.hpp"
#include "runtime/vmOperation.hpp"
#include "runtime/threadSMR.hpp"
#ifdef SVM
#include "runtime/safepoint.hpp"
#include "svmIsolateThread.hpp"
#endif // SVM

// NOTE (chaeubl): it is crucial that native-image and the GC have a common view of the Java threads that are alive.

// This stack allocated ThreadsListHandle and JavaThreadIterator combo
// is used to walk the ThreadsList in the included ThreadsListHandle
// using the following style:
//
//   for (JavaThreadIteratorWithHandle jtiwh; JavaThread *jt = jtiwh.next(); ) {
//     ...
//   }
//

namespace svm_gc {

class JavaThreadIteratorWithHandle : public StackObj {
  uint _index;
  IsolateThread *_next;

  public:
    JavaThreadIteratorWithHandle() : _index(-1), _next(IsolateThread::get_first_thread()) {
      assert(SafepointSynchronize::is_at_safepoint() || SVMIsolateData::_during_teardown, "not safe to iterate the thread list");
    }

    JavaThread *next() {
      if (_next != nullptr) {
        IsolateThread *current = _next;
        _next = _next->next_thread();
        return current->java_thread();
      }
      return nullptr;
    }

    void rewind() {
      _next = IsolateThread::get_first_thread();
    }
};


} // namespace svm_gc

#endif // SHARE_RUNTIME_THREADSMR_HPP
