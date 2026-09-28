/*
 * Copyright (c) 2001, 2025, Oracle and/or its affiliates. All rights reserved.
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

#include "classfile/vmSymbols.hpp"
#include "jvm.h"
#include "logging/log.hpp"
#include "memory/allocation.inline.hpp"
#include "oops/oop.inline.hpp"
#include "runtime/arguments.hpp"
#include "runtime/handles.inline.hpp"
#include "runtime/java.hpp"
#include "runtime/javaCalls.hpp"
#include "runtime/mutex.hpp"
#include "runtime/mutexLocker.hpp"
#include "runtime/os.hpp"
#include "runtime/perfData.inline.hpp"
#include "utilities/exceptions.hpp"
#include "utilities/globalCounter.inline.hpp"
#include "utilities/globalDefinitions.hpp"


namespace svm_gc {

oop load_oop(void* base, ptrdiff_t offset) {
  return RawAccess<>::oop_load_at((oop)base, offset);
}

void PerfStringVariable::set_value(const char* s2) {
  typeArrayOop byte_arr = (typeArrayOop)load_oop(this, SVMGlobalData::_offsets._perf_string_variable._null_terminated_value);
  char *byte_arr_base = (char*)byte_arr->base();
  int length = byte_arr->length();

  // copy n bytes of the string, assuring the null string is copied if s2 == NULL.
  strncpy(byte_arr_base, s2 == nullptr ? "" : s2, length);

  // assure the string is null terminated when strlen(s2) >= _length
  byte_arr_base[length] = '\0';
}

//
// SVM-specific wrapper classes
//

// G1TLABPerfData
PerfLongVariable* G1TLABPerfData::alloc_threads() {
  return (PerfLongVariable*) load_oop(this, SVMGlobalData::_offsets._tlab_perf_data._alloc_threads);
}

PerfLongVariable* G1TLABPerfData::fills() {
  return (PerfLongVariable*) load_oop(this, SVMGlobalData::_offsets._tlab_perf_data._fills);
}

PerfLongVariable* G1TLABPerfData::max_fills() {
  return (PerfLongVariable*) load_oop(this, SVMGlobalData::_offsets._tlab_perf_data._max_fills);
}

PerfLongVariable* G1TLABPerfData::alloc() {
  return (PerfLongVariable*) load_oop(this, SVMGlobalData::_offsets._tlab_perf_data._alloc);
}

PerfLongVariable* G1TLABPerfData::gc_waste() {
  return (PerfLongVariable*) load_oop(this, SVMGlobalData::_offsets._tlab_perf_data._gc_waste);
}

PerfLongVariable* G1TLABPerfData::max_gc_waste() {
  return (PerfLongVariable*) load_oop(this, SVMGlobalData::_offsets._tlab_perf_data._max_gc_waste);
}

PerfLongVariable* G1TLABPerfData::refill_waste() {
  return (PerfLongVariable*) load_oop(this, SVMGlobalData::_offsets._tlab_perf_data._refill_waste);
}

PerfLongVariable* G1TLABPerfData::max_refill_waste() {
  return (PerfLongVariable*) load_oop(this, SVMGlobalData::_offsets._tlab_perf_data._max_refill_waste);
}

PerfLongVariable* G1TLABPerfData::slow_alloc() {
  return (PerfLongVariable*) load_oop(this, SVMGlobalData::_offsets._tlab_perf_data._slow_alloc);
}

PerfLongVariable* G1TLABPerfData::max_slow_alloc() {
  return (PerfLongVariable*) load_oop(this, SVMGlobalData::_offsets._tlab_perf_data._max_slow_alloc);
}

// G1GCPolicyPerfData
PerfLongVariable* G1GCPolicyPerfData::desired_survivor_size() {
  return (PerfLongVariable*) load_oop(this, SVMGlobalData::_offsets._gc_policy_perf_data._desired_survivor_size);
}

PerfLongVariable* G1GCPolicyPerfData::gc_time_limit_exceeded() {
  return (PerfLongVariable*) load_oop(this, SVMGlobalData::_offsets._gc_policy_perf_data._gc_time_limit_exceeded);
}

PerfLongVariable* G1GCPolicyPerfData::max_tenuring_threshold() {
  return (PerfLongVariable*) load_oop(this, SVMGlobalData::_offsets._gc_policy_perf_data._max_tenuring_threshold);
}

PerfLongVariable* G1GCPolicyPerfData::tenuring_threshold() {
  return (PerfLongVariable*) load_oop(this, SVMGlobalData::_offsets._gc_policy_perf_data._tenuring_threshold);
}

// G1GCCausesPerfData
PerfStringVariable* G1GCCausesPerfData::cause() {
  return (PerfStringVariable*) load_oop(this, SVMGlobalData::_offsets._gc_causes_perf_data._cause);
}

PerfStringVariable* G1GCCausesPerfData::last_cause() {
  return (PerfStringVariable*) load_oop(this, SVMGlobalData::_offsets._gc_causes_perf_data._last_cause);
}

// G1CollectorPerfData
PerfLongCounter* G1CollectorPerfData::invocations() {
  return (PerfLongCounter*) load_oop(this, SVMGlobalData::_offsets._collector_perf_data._invocations);
}

PerfLongVariable* G1CollectorPerfData::last_entry_time() {
  return (PerfLongVariable*) load_oop(this, SVMGlobalData::_offsets._collector_perf_data._last_entry_time);
}

PerfLongVariable* G1CollectorPerfData::last_exit_time() {
  return (PerfLongVariable*) load_oop(this, SVMGlobalData::_offsets._collector_perf_data._last_exit_time);
}

PerfLongCounter* G1CollectorPerfData::time() {
  return (PerfLongCounter*) load_oop(this, SVMGlobalData::_offsets._collector_perf_data._time);
}

// G1SpacePerfData
PerfLongVariable* G1SpacePerfData::capacity() {
  return (PerfLongVariable*) load_oop(this, SVMGlobalData::_offsets._space_perf_data._capacity);
}

PerfLongVariable* G1SpacePerfData::init_capacity() {
  return (PerfLongVariable*) load_oop(this, SVMGlobalData::_offsets._space_perf_data._init_capacity);
}

PerfLongVariable* G1SpacePerfData::max_capacity() {
  return (PerfLongVariable*) load_oop(this, SVMGlobalData::_offsets._space_perf_data._max_capacity);
}

PerfLongVariable* G1SpacePerfData::used() {
  return (PerfLongVariable*) load_oop(this, SVMGlobalData::_offsets._space_perf_data._used);
}

// G1GenerationPerfData
PerfLongVariable* G1GenerationPerfData::capacity() {
  return (PerfLongVariable*) load_oop(this, SVMGlobalData::_offsets._generation_perf_data._capacity);
}

PerfLongVariable* G1GenerationPerfData::max_capacity() {
  return (PerfLongVariable*) load_oop(this, SVMGlobalData::_offsets._generation_perf_data._max_capacity);
}

PerfLongVariable* G1GenerationPerfData::min_capacity() {
  return (PerfLongVariable*) load_oop(this, SVMGlobalData::_offsets._generation_perf_data._min_capacity);
}

G1SpacePerfData* G1GenerationPerfData::space(int ordinal) {
  objArrayOop spaces = (objArrayOop)load_oop(this, SVMGlobalData::_offsets._generation_perf_data._spaces);
  return (G1SpacePerfData*) spaces->obj_at_raw(ordinal);
}

// G1AgeTablePerfData
PerfLongVariable* G1AgeTablePerfData::entry(int age) {
  objArrayOop table = (objArrayOop)load_oop(this, SVMGlobalData::_offsets._age_table_perf_data._table);
  return (PerfLongVariable*)table->obj_at_raw(age);
}

// G1CpuTimePerfData
PerfLongCounter* G1CpuTimePerfData::gc_total() {
  return (PerfLongCounter*) load_oop(this, SVMGlobalData::_offsets._cpu_time_perf_data._gc_total);
}

PerfLongCounter* G1CpuTimePerfData::gc_parallel_workers() {
  return (PerfLongCounter*) load_oop(this, SVMGlobalData::_offsets._cpu_time_perf_data._gc_parallel_workers);
}

PerfLongCounter* G1CpuTimePerfData::gc_conc_mark() {
  return (PerfLongCounter*) load_oop(this, SVMGlobalData::_offsets._cpu_time_perf_data._gc_conc_mark);
}

PerfLongCounter* G1CpuTimePerfData::gc_conc_refine() {
  return (PerfLongCounter*) load_oop(this, SVMGlobalData::_offsets._cpu_time_perf_data._gc_conc_refine);
}

PerfLongCounter* G1CpuTimePerfData::gc_service() {
  return (PerfLongCounter*) load_oop(this, SVMGlobalData::_offsets._cpu_time_perf_data._gc_service);
}

// G1PerfData
G1TLABPerfData* G1PerfData::tlab() {
  return (G1TLABPerfData*) load_oop(this, SVMGlobalData::_offsets._perf_data._tlab);
}

G1GCPolicyPerfData* G1PerfData::gc_policy() {
  return (G1GCPolicyPerfData*) load_oop(this, SVMGlobalData::_offsets._perf_data._gc_policy);
}

G1GCCausesPerfData* G1PerfData::causes() {
  return (G1GCCausesPerfData*) load_oop(this, SVMGlobalData::_offsets._perf_data._gc_causes);
}

G1CollectorPerfData* G1PerfData::collector(int ordinal) {
  objArrayOop collectors = (objArrayOop)load_oop(this, SVMGlobalData::_offsets._perf_data._collectors);
  return (G1CollectorPerfData*) collectors->obj_at_raw(ordinal);
}

G1GenerationPerfData* G1PerfData::generation(int ordinal) {
  objArrayOop generations = (objArrayOop)load_oop(this, SVMGlobalData::_offsets._perf_data._generations);
  return (G1GenerationPerfData*) generations->obj_at_raw(ordinal);
}

G1AgeTablePerfData* G1PerfData::age_table() {
  return (G1AgeTablePerfData*) load_oop(this, SVMGlobalData::_offsets._perf_data._age_table);
}

G1CpuTimePerfData* G1PerfData::cpu_time() {
  return (G1CpuTimePerfData*) load_oop(this, SVMGlobalData::_offsets._perf_data._cpu_time);
}

} // namespace svm_gc

