/*
 * Copyright (c) 2026, Oracle and/or its affiliates. All rights reserved.
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

#ifndef SVM_METASPACE_HPP
#define SVM_METASPACE_HPP

#include "svmGlobalData.hpp"
#include "utilities/debug.hpp"


namespace svm_gc {

// Shared interface for collector-specific metaspace implementations.
class SVMMetaspace {
 private:
  static SVMMetaspace* _metaspace;

 protected:
  ~SVMMetaspace() = default;

 public:
  // The collector must register its metaspace before this accessor is used.
  static SVMMetaspace* metaspace() {
    assert(_metaspace != nullptr, "not initialized");
    return _metaspace;
  }
  static void set_metaspace(SVMMetaspace* metaspace);

  static inline bool is_in_address_space(const void* address) {
    const char* addr = (const char*)address;
    return SVMIsolateData::_metaspace_start_addr != nullptr &&
           addr >= SVMIsolateData::_metaspace_start_addr && addr < SVMIsolateData::_metaspace_end_addr;
  }

  virtual bool is_in_allocated_memory(const void* address) const = 0;
  virtual void update_used_at_last_gc() = 0;
};


} // namespace svm_gc

#endif // SVM_METASPACE_HPP
