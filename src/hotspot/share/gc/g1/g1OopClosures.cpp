/*
 * Copyright (c) 2014, 2025, Oracle and/or its affiliates. All rights reserved.
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

#include "gc/g1/g1CollectedHeap.inline.hpp"
#include "gc/g1/g1OopClosures.inline.hpp"
#include "gc/g1/g1ParScanThreadState.hpp"
#include "memory/iterator.inline.hpp"
#include "utilities/stack.inline.hpp"

G1ParCopyHelper::G1ParCopyHelper(G1CollectedHeap* g1h,  G1ParScanThreadState* par_scan_state) :
  _g1h(g1h),
  _par_scan_state(par_scan_state),
  _worker_id(par_scan_state->worker_id()),
#ifndef SVM
  _scanned_cld(nullptr),
#endif // !SVM
  _cm(_g1h->concurrent_mark())
{ }

G1ScanClosureBase::G1ScanClosureBase(G1CollectedHeap* g1h, G1ParScanThreadState* par_scan_state) :
  _g1h(g1h), _par_scan_state(par_scan_state)
{ }

#ifndef SVM
void G1CLDScanClosure::do_cld(ClassLoaderData* cld) {
  // If the class loader data has not been dirtied we know that there's
  // no references into the young gen and we can skip it.
  if (!_process_only_dirty || cld->has_modified_oops()) {

    // Tell the closure that this class loader data is the CLD to scavenge
    // and is the one to dirty if oops are left pointing into the young gen.
    _closure->set_scanned_cld(cld);
    // Clean modified oops since we're going to scavenge all the metadata.
    cld->oops_do(_closure, ClassLoaderData::_claim_none, true /*clear_modified_oops*/);

    _closure->set_scanned_cld(nullptr);

    _closure->trim_queue_partially();
  }
  _count++;
}
#endif // !SVM

#ifdef SVM
// On the SVM-side, similar logic is located in the class RuntimeCodeCacheWalker. This code is called when:
// - a full GC is done with marking
// - concurrent marking ends (remark phase)
//
// When this code is called during the remark phase of concurrent marking, then we need to visit *all* nmethod
// oops, even if the nmethod is going to be freed during the current safepoint. This is necessary to ensure
// that we have a consistent SATB mark bitmap (we need to assume that all nmethod oops were alive when the
// marking started).
// Otherwise, it could happen that the Java heap contains references to objects that are dead from SATB
// perspective (e.g., deoptimization may store nmethod oops into the Java heap).
void G1ConditionalMarkCodeCacheClosure::do_nmethod(nmethod* nm) {
  if (nm->oops_do_try_claim()) {
    if (nm->are_all_objects_in_image_heap()) {
      return;
    }

    oop tether = nm->tether();
    if (tether != nullptr && !_is_alive->do_object_b(tether)) {
      nmethod::NMethodState state = nm->state();
      if (state == nmethod::state_removed_from_code_cache) {
        if (_remark) {
          nm->oops_do(_mark_cl);
        } else {
          nm->oops_do_object_fields(_mark_cl);
        }
        unregister_nmethod(nm);
        nm->set_state(nmethod::state_pending_free);
        return;
      }

      if (state == nmethod::state_non_entrant || (TreatRuntimeCodeInfoReferencesAsWeak && state == nmethod::state_code_constants_live && nm->has_weak_reference_to_unreachable_object(_is_alive))) {
        if (_remark) {
          nm->oops_do(_mark_cl);
        } else {
          nm->oops_do_object_fields(_mark_cl);
        }
        unregister_nmethod(nm);
        nm->set_state(nmethod::state_pending_removal_from_code_cache);
        return;
      }
    }

    nm->oops_do(_mark_cl);
  }
}

// It is necessary to unregister all nmethods that will be freed during the current safepoint right away.
// Later on, either the nmethod state would be incorrect or the oops would no longer be valid.
void G1ConditionalMarkCodeCacheClosure::unregister_nmethod(nmethod* nm) {
  assert(nm->has_code_constants(), "must be");
  MutexLocker ml(G1StrongCodeRoots_lock, Mutex::_no_safepoint_check_flag);
  G1CollectedHeap::heap()->unregister_nmethod(nm);
}
#endif // SVM
