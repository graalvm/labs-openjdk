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

#ifndef SVM_ISOLATE_THREAD_HPP
#define SVM_ISOLATE_THREAD_HPP

#include "oops/access.hpp"
#include "svmGlobalData.hpp"


namespace svm_gc {

class IsolateThread {
 public:
  // must be in sync with SVM
  enum IsolateThreadStatus {
    STATUS_CREATED = 0,
    STATUS_IN_JAVA = STATUS_CREATED + 1,
    STATUS_IN_SAFEPOINT = STATUS_IN_JAVA + 1,
    STATUS_IN_NATIVE = STATUS_IN_SAFEPOINT + 1,
    STATUS_IN_VM = STATUS_IN_NATIVE + 1
  };

 public:
  IsolateThread* next_thread() {
    return *(IsolateThread**)thread_local_at(SVMGlobalData::_offsets._thread_locals._next_thread);
  }

  JavaThread* java_thread() {
    return (JavaThread*)thread_local_at(SVMGlobalData::_offsets._thread_locals._java_thread);
  }

  address thread_local_at(int offset) {
    return (address)this + offset;
  }

  IsolateThreadStatus thread_status() {
    return *(IsolateThreadStatus*)(thread_local_at(SVMGlobalData::_offsets._thread_locals._thread_status));
  }

  bool has_status_created() {
    return thread_status() == STATUS_CREATED;
  }

  bool has_status_java() {
    return thread_status() == STATUS_IN_JAVA;
  }

  bool has_status_native() {
    return thread_status() == STATUS_IN_NATIVE;
  }

  bool has_status_safepoint() {
    return thread_status() == STATUS_IN_SAFEPOINT;
  }

  bool has_status_native_or_safepoint() {
    int s = thread_status();
    return s == STATUS_IN_NATIVE || s == STATUS_IN_SAFEPOINT;
  }

  bool has_status_vm() {
    return thread_status() == STATUS_IN_VM;
  }

  typeArrayOop get_pod_reference_map();

  static IsolateThread* get_first_thread() {
    return RawAccess<>::load_at((oop)SVMIsolateData::_static_primitive_fields, (ptrdiff_t)SVMGlobalData::_offsets._vm_threads._static_vm_threads_head);
  }

  static uint get_num_attached_threads() {
    return RawAccess<>::load_at((oop)SVMIsolateData::_static_primitive_fields, (ptrdiff_t)SVMGlobalData::_offsets._vm_threads._static_num_attached_threads);
  }

  static IsolateThread* from_java_thread(const JavaThread *java_thread) {
    return (IsolateThread*)((address)java_thread - SVMGlobalData::_offsets._thread_locals._java_thread);
  }

  static IsolateThread* current();
};


} // namespace svm_gc

#endif // SVM_ISOLATE_THREAD_HPP
