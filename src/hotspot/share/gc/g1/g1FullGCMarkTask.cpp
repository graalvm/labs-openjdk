/*
 * Copyright (c) 2017, 2025, Oracle and/or its affiliates. All rights reserved.
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

#include "gc/g1/g1CollectedHeap.hpp"
#include "gc/g1/g1FullCollector.hpp"
#include "gc/g1/g1FullGCMarker.hpp"
#include "gc/g1/g1FullGCMarkTask.hpp"
#include "gc/g1/g1FullGCOopClosures.inline.hpp"
#include "gc/shared/gcTraceTime.inline.hpp"
#include "gc/shared/referenceProcessor.hpp"
#include "memory/iterator.inline.hpp"


namespace svm_gc {

G1FullGCMarkTask::G1FullGCMarkTask(G1FullCollector* collector) :
    G1FullGCTask("G1 Parallel Marking Task", collector),
    _root_processor(G1CollectedHeap::heap(), collector->workers()),
    _terminator(collector->workers(), collector->array_queue_set()) {
}

void G1FullGCMarkTask::work(uint worker_id) {
  Ticks start = Ticks::now();
  ResourceMark rm;
  G1FullGCMarker* marker = collector()->marker(worker_id);
  MarkingNMethodClosure code_closure(marker->mark_closure(), !NMethodToOopClosure::FixRelocations, true /* keepalive nmethods */);

#ifdef SVM
  // NOTE (chaeubl): for code unloading, it is important to use the same logic as the one that HotSpot
  // uses with ClassUnloading enabled (process_strong_roots instead of process_all_roots).
  _root_processor.process_strong_roots(marker->mark_closure(), &code_closure);
#else
  if (ClassUnloading) {
    _root_processor.process_strong_roots(marker->mark_closure(),
                                         marker->cld_closure(),
                                         &code_closure);
  } else {
    _root_processor.process_all_roots(marker->mark_closure(),
                                      marker->cld_closure(),
                                      &code_closure);
  }
#endif // SVM

  // Mark stack is populated, now process and drain it.
  marker->complete_marking(collector()->oop_queue_set(), collector()->array_queue_set(), &_terminator);

  // This is the point where the entire marking should have completed.
  assert(marker->oop_stack()->is_empty(), "Marking should have completed");
  assert(marker->objarray_stack()->is_empty(), "Array marking should have completed");
  log_task("Marking task", worker_id, start);
}

#ifdef SVM
// NOTE (chaeubl): similar to G1FullGCMarkTask
G1FullGCMarkCodeCacheTask::G1FullGCMarkCodeCacheTask(G1FullCollector* collector) :
    G1FullGCTask("G1 Parallel CodeCache Marking Task", collector),
    _terminator(collector->workers(), collector->array_queue_set()) {
}

void G1FullGCMarkCodeCacheTask::work(uint worker_id) {
  Ticks start = Ticks::now();
  ResourceMark rm;
  G1FullGCMarker* marker = collector()->marker(worker_id);

  {
    // Now that we have marked all strongly reachable objects, we can do the conditional marking of JIT compiled code.
    G1IsAliveClosure is_alive(collector());
    G1ConditionalMarkCodeCacheClosure mark_code_cache_closure(marker->mark_closure(), &is_alive, false);
    CodeCache::nmethods_do(&mark_code_cache_closure);
  }

  // Mark stack is populated, now process and drain it.
  marker->complete_marking(collector()->oop_queue_set(), collector()->array_queue_set(), &_terminator);

  // This is the point where the entire marking should have completed.
  assert(marker->oop_stack()->is_empty(), "Marking should have completed");
  assert(marker->objarray_stack()->is_empty(), "Array marking should have completed");
  log_task("Marking CodeCache task", worker_id, start);
}
#endif // SVM

} // namespace svm_gc

