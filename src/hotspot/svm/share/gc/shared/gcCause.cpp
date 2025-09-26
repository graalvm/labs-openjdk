/*
 * Copyright (c) 2002, 2025, Oracle and/or its affiliates. All rights reserved.
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

#include "gc/shared/gcCause.hpp"

const char* GCCause::to_string(GCCause::Cause cause) {
  switch (cause) {
    case _java_lang_system_gc:
      return "java.lang.System.gc()";

    case _unit_test:
      return "Forced GC in unit test";

    case _test_gc_in_deoptimizer:
      return "Test GC in deoptimizer";

    case _hinted_gc:
      return "Hinted GC";

    case _jvmti_force_gc:
      return "JvmtiEnv ForceGarbageCollection";

    case _heap_dump:
      return "Heap Dump Initiated GC";

    case _wb_breakpoint:
      return "WhiteBox Initiated Run to Breakpoint";

    case _no_gc:
      return "No GC";

    case _g1_inc_collection_pause:
      return "G1 Evacuation Pause";

    case _g1_compaction_pause:
      return "G1 Compaction Pause";

    case _g1_humongous_allocation:
      return "G1 Humongous Allocation";

    case _g1_periodic_collection:
      return "G1 Periodic Collection";

    case _last_gc_cause:
          return "ILLEGAL VALUE - last gc cause - ILLEGAL VALUE";

    case _no_cause_specified:
    default:
      return "unknown GCCause";
  }
  ShouldNotReachHere();
}
