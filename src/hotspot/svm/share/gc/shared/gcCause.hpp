/*
 * Copyright (c) 2002, 2024, Oracle and/or its affiliates. All rights reserved.
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

#ifndef SHARE_GC_SHARED_GCCAUSE_HPP
#define SHARE_GC_SHARED_GCCAUSE_HPP

#include "memory/allStatic.hpp"
#include "utilities/debug.hpp"

class GCCause : public AllStatic {
 public:
  enum Cause {
    // GC causes that must be kept in sync with native-image
    _java_lang_system_gc = 0,
    _unit_test = 1,
    _test_gc_in_deoptimizer = 2,
    _hinted_gc = 3,
    _jvmti_force_gc = 4,
    _heap_dump = 5,
    _dcmd_gc_run = 6,

    // GC causes that are used but that native-image does not know about
    _scavenge_alot = 10,
    _wb_breakpoint,

    _no_gc,

    _g1_inc_collection_pause,
    _g1_compaction_pause,
    _g1_humongous_allocation,
    _g1_periodic_collection,

    // GC causes that are unused and only kept to reduce the number of necessary code changes
    _gc_locker,
    _heap_inspection,

    _wb_young_gc,
    _wb_full_gc,

    _no_cause_specified,

    _codecache_GC_threshold,
    _codecache_GC_aggressive,

    _last_gc_cause
  };

  inline static bool is_user_requested_gc(GCCause::Cause cause) {
    return (cause == GCCause::_java_lang_system_gc ||
            cause == GCCause::_dcmd_gc_run);
  }

  inline static bool is_explicit_full_gc(GCCause::Cause cause) {
    return (is_user_requested_gc(cause) ||
            is_serviceability_requested_gc(cause) ||
            cause == GCCause::_wb_full_gc);
  }

  inline static bool is_serviceability_requested_gc(GCCause::Cause
                                                             cause) {
    return (cause == GCCause::_jvmti_force_gc ||
            cause == GCCause::_heap_inspection ||
            cause == GCCause::_heap_dump);
  }

  // Return a string describing the GCCause.
  static const char* to_string(GCCause::Cause cause);
};

#endif // SHARE_GC_SHARED_GCCAUSE_HPP
