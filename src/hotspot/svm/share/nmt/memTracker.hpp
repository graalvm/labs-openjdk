/*
 * Copyright (c) 2013, 2025, Oracle and/or its affiliates. All rights reserved.
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

#ifndef SHARE_NMT_MEMTRACKER_HPP
#define SHARE_NMT_MEMTRACKER_HPP

#include "memory/reservedSpace.hpp"
#include "nmt/mallocTracker.hpp"
#include "nmt/memBaseline.hpp"
#include "nmt/nmtCommon.hpp"
#include "nmt/memoryFileTracker.hpp"
#include "nmt/threadStackTracker.hpp"
#include "nmt/virtualMemoryTracker.hpp"
#include "runtime/mutexLocker.hpp"
#include "utilities/debug.hpp"
#include "utilities/nativeCallStack.hpp"
#include "utilities/deferred.hpp"

#define CURRENT_PC NativeCallStack::empty_stack()
#define CALLER_PC  NativeCallStack::empty_stack()

// NOTE (chaeubl): dummy implementation that doesn't do any tracking.

namespace svm_gc {

class MemTracker : AllStatic {
 public:
  static inline bool enabled() {
    return false;
  }

  // Per-malloc overhead incurred by NMT, depending on the current NMT level
  static size_t overhead_per_malloc() {
    return 0;
  }

  static inline void* record_malloc(void* mem_base, size_t size, MemTag mem_tag,
    const NativeCallStack& stack) {
    assert(mem_base != nullptr, "caller should handle null");
    return mem_base;
  }

  // Record malloc free and return malloc base address
  static inline void* record_free(void* memblock) {
    // Never turned on
    assert(memblock != nullptr, "caller should handle null");
    return memblock;
  }

  // Record creation of an arena
  static inline void record_new_arena(MemTag mem_tag) {
  }

  // Record destruction of an arena
  static inline void record_arena_free(MemTag mem_tag) {
  }

  // Record arena size change. Arena size is the size of all arena
  // chunks that are backing up the arena.
  static inline void record_arena_size_change(ssize_t diff, MemTag mem_tag) {
  }

  // Note: virtual memory operations should only ever be called after NMT initialization
  //  (we do not do any reservations before that).

  static inline void record_virtual_memory_reserve(void* addr, size_t size, const NativeCallStack& stack,
      MemTag mem_tag) {
  }

  static inline void record_virtual_memory_release(void* addr, size_t size) {
  }

  static inline void record_virtual_memory_uncommit(void* addr, size_t size) {
  }


  static inline void record_virtual_memory_commit(void* addr, size_t size,
    const NativeCallStack& stack) {
  }

  static inline void record_virtual_memory_tag(const ReservedSpace& rs, MemTag mem_tag) {
  }

  static inline void record_virtual_memory_tag(void* addr, size_t size, MemTag mem_tag) {
  }

  // MallocLimt: Given an allocation size s, check if mallocing this much
  // for MemTag would hit either the global limit or the limit for MemTag.
  static inline bool check_exceeds_limit(size_t s, MemTag mem_tag) {
    return false;
  }

  class NmtVirtualMemoryLocker: StackObj {
  public:
    NmtVirtualMemoryLocker(){}
  };
};


} // namespace svm_gc

#endif // SHARE_NMT_MEMTRACKER_HPP
