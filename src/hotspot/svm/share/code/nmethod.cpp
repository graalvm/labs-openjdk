/*
 * Copyright (c) 1997, 2025, Oracle and/or its affiliates. All rights reserved.
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

#include "asm/assembler.inline.hpp"
#include "code/codeCache.hpp"
#include "code/compiledIC.hpp"
#include "code/dependencies.hpp"
#include "code/nativeInst.hpp"
#include "code/nmethod.inline.hpp"
#include "code/relocInfo.hpp"
#include "code/scopeDesc.hpp"
#include "compiler/abstractCompiler.hpp"
#include "compiler/compilationLog.hpp"
#include "compiler/compileBroker.hpp"
#include "compiler/compileLog.hpp"
#include "compiler/compileTask.hpp"
#include "compiler/compilerDirectives.hpp"
#include "compiler/compilerOracle.hpp"
#include "compiler/directivesParser.hpp"
#include "compiler/disassembler.hpp"
#include "compiler/oopMap.inline.hpp"
#include "gc/shared/barrierSet.hpp"
#include "gc/shared/barrierSetNMethod.hpp"
#include "gc/shared/classUnloadingContext.hpp"
#include "gc/shared/collectedHeap.hpp"
#include "interpreter/bytecode.inline.hpp"
#include "jvm.h"
#include "logging/log.hpp"
#include "logging/logStream.hpp"
#include "memory/allocation.inline.hpp"
#include "memory/resourceArea.hpp"
#include "memory/universe.hpp"
#include "oops/access.inline.hpp"
#include "oops/klass.inline.hpp"
#include "oops/method.inline.hpp"
#include "oops/methodData.hpp"
#include "oops/oop.inline.hpp"
#include "oops/weakHandle.inline.hpp"
#include "prims/jvmtiImpl.hpp"
#include "prims/jvmtiThreadState.hpp"
#include "prims/methodHandles.hpp"
#include "runtime/continuation.hpp"
#include "runtime/atomic.hpp"
#include "runtime/deoptimization.hpp"
#include "runtime/flags/flagSetting.hpp"
#include "runtime/frame.inline.hpp"
#include "runtime/handles.inline.hpp"
#include "runtime/jniHandles.inline.hpp"
#include "runtime/orderAccess.hpp"
#include "runtime/os.hpp"
#include "runtime/safepointVerifiers.hpp"
#include "runtime/serviceThread.hpp"
#include "runtime/sharedRuntime.hpp"
#include "runtime/signature.hpp"
#include "runtime/threadWXSetters.inline.hpp"
#include "runtime/vmThread.hpp"
#include "utilities/align.hpp"
#include "utilities/copy.hpp"
#include "utilities/dtrace.hpp"
#include "utilities/events.hpp"
#include "utilities/globalDefinitions.hpp"
#include "utilities/resourceHash.hpp"
#include "utilities/xmlstream.hpp"
#if INCLUDE_JVMCI
#include "jvmci/jvmciRuntime.hpp"
#endif
#ifdef SVM
#include "svmCodeReferenceMapDecoder.hpp"
#endif // SVM


namespace svm_gc {

void nmethod::oops_do(OopClosure* f) {
  if (will_be_freed()) {
    // Only the object fields are alive as those are needed for the invalidation/freeing. The GC will free
    // the nmethod during the current safepoint.
    oops_do_object_fields(f);
  } else {
    oops_do_all(f);
  }
}

#ifdef SVM
void nmethod::oops_do_all(OopClosure* f) {
  oops_do_object_fields(f);
  if (has_code_constants()) {
    oops_do_code_constants(f);
  }
  oops_do_frame_metadata(f);
  oops_do_deopt_metadata(f);
}

void nmethod::oops_do_weak(OopClosure* f) {
  objArrayOop obj_fields = this->object_fields();
  obj_fields->oop_iterate_range(f, first_weakly_referenced_object_field, obj_fields->length());
  if (has_code_constants()) {
    oops_do_code_constants(f);
  }
  oops_do_frame_metadata(f);
  oops_do_deopt_metadata(f);
}

void nmethod::oops_do_object_fields(OopClosure* f) {
  this->object_fields()->oop_iterate(f);
}

void nmethod::oops_do_code_constants(OopClosure* f) {
  assert(has_code_constants(), "wrong state");
  u_char *encoded_reference_map = (u_char*)(this->code_constants_reference_map_encoding()->base());
  SVMCodeReferenceMapDecoder::walk_offsets_from_pointer(this->code_begin(), encoded_reference_map, this->objects_reference_map_index(), f);
}

void nmethod::oops_do_frame_metadata(OopClosure* f) {
  array_oops_do(this->frame_info_object_constants(), f);
}

void nmethod::oops_do_deopt_metadata(OopClosure* f) {
  array_oops_do(this->deoptimization_object_constants(), f);
}

void nmethod::array_oops_do(objArrayOop array, OopClosure* f) {
  if (array != nullptr) {
    array->oop_iterate(f);
  }
}

class ReferencesUnreachableObjectClosure : public OopClosure {
 private:
  BoolObjectClosure* _is_alive;
  bool _result;

 public:
  ReferencesUnreachableObjectClosure(BoolObjectClosure* is_alive) : OopClosure(), _is_alive(is_alive), _result(false) {
  }

  bool result() { return _result; }

  bool is_assumed_reachable(oop obj) {
    Klass *klass = obj->klass();
    objArrayOop klasses_assumed_reachable = SVMIsolateData::_klasses_assumed_reachable_for_code_unloading;
    for (int i = 0; i < klasses_assumed_reachable->length(); i++) {
      Klass* klass_assumed_reachable = (Klass*)klasses_assumed_reachable->obj_at(i);
      if (klass_assumed_reachable->is_subtype_of(klass)) {
          return true;
      }
    }
    return false;
  }

  template <class T> void do_oop_work(T* p) {
    if (_result) {
      return;
    }

    T heap_oop = RawAccess<>::oop_load(p);
    if (CompressedOops::is_null(heap_oop)) {
      return;
    }

    oop obj = CompressedOops::decode_not_null(heap_oop);
    if (!_is_alive->do_object_b(obj) && !is_assumed_reachable(obj)) {
      _result = true;
    }
  }
  void do_oop(      oop* p) { do_oop_work(p); }
  void do_oop(narrowOop* p) { do_oop_work(p); }
};

bool nmethod::has_weak_reference_to_unreachable_object(BoolObjectClosure* is_alive) {
  ReferencesUnreachableObjectClosure cl(is_alive);
  oops_do_weak(&cl);
  return cl.result();
}
#endif // SVM

nmethod* volatile nmethod::_oops_do_mark_nmethods;

void nmethod::oops_do_log_change(const char* state) {
  LogTarget(Trace, gc, nmethod) lt;
  if (lt.is_enabled()) {
    LogStream ls(lt);
    print(&ls, this, "oop_do_log_change");
  }
}

bool nmethod::oops_do_try_claim() {
  if (oops_do_try_claim_weak_request()) {
    nmethod* result = oops_do_try_add_to_list_as_weak_done();
    assert(result == nullptr, "adding to global list as weak done must always succeed.");
    return true;
  }
  return false;
}

bool nmethod::oops_do_try_claim_weak_request() {
  assert(SafepointSynchronize::is_at_safepoint(), "only at safepoint");

  if ((_oops_do_mark_link() == nullptr) &&
      (Atomic::replace_if_null(_oops_do_mark_link_addr(), mark_link(this, claim_weak_request_tag)))) {
    oops_do_log_change("oops_do, mark weak request");
    return true;
  }
  return false;
}

void nmethod::oops_do_set_strong_done(nmethod* old_head) {
  *(_oops_do_mark_link_addr()) = mark_link(old_head, claim_strong_done_tag);
}


nmethod* nmethod::oops_do_try_add_to_list_as_weak_done() {
  assert(SafepointSynchronize::is_at_safepoint(), "only at safepoint");

  assert(extract_state(_oops_do_mark_link()) == claim_weak_request_tag ||
         extract_state(_oops_do_mark_link()) == claim_strong_request_tag,
         "must be but is nmethod " PTR_FORMAT " %u", p2i(extract_nmethod(_oops_do_mark_link())), extract_state(_oops_do_mark_link()));

  nmethod* old_head = Atomic::xchg(&_oops_do_mark_nmethods, this);
  // Self-loop if needed.
  if (old_head == nullptr) {
    old_head = this;
  }
  // Try to install end of list and weak done tag.
  if (Atomic::cmpxchg(_oops_do_mark_link_addr(), mark_link(this, claim_weak_request_tag), mark_link(old_head, claim_weak_done_tag)) == mark_link(this, claim_weak_request_tag)) {
    oops_do_log_change("oops_do, mark weak done");
    return nullptr;
  } else {
    return old_head;
  }
}


void nmethod::oops_do_process_weak(OopsDoProcessor* p) {
  if (!oops_do_try_claim_weak_request()) {
    // Failed to claim for weak processing.
    oops_do_log_change("oops_do, mark weak request fail");
    return;
  }

  p->do_regular_processing(this);

  nmethod* old_head = oops_do_try_add_to_list_as_weak_done();
  if (old_head == nullptr) {
    return;
  }
  oops_do_log_change("oops_do, mark weak done fail");
  // Adding to global list failed, another thread added a strong request.
  assert(extract_state(_oops_do_mark_link()) == claim_strong_request_tag,
         "must be but is %u", extract_state(_oops_do_mark_link()));

  oops_do_log_change("oops_do, mark weak request -> mark strong done");

  oops_do_set_strong_done(old_head);
  // Do missing strong processing.
  p->do_remaining_strong_processing(this);
}


void nmethod::oops_do_marking_prologue() {
  assert_at_safepoint();

  log_trace(gc, nmethod)("oops_do_marking_prologue");
  assert(_oops_do_mark_nmethods == nullptr, "must be empty");
}

void nmethod::oops_do_marking_epilogue() {
  assert_at_safepoint();

  nmethod* next = _oops_do_mark_nmethods;
  _oops_do_mark_nmethods = nullptr;
  if (next != nullptr) {
    nmethod* cur;
    do {
      cur = next;
      next = extract_nmethod(cur->_oops_do_mark_link());
      *(cur->_oops_do_mark_link_addr()) = nullptr;

      LogTarget(Trace, gc, nmethod) lt;
      if (lt.is_enabled()) {
        LogStream ls(lt);
        print(&ls, cur, "oops_do, unmark");
      }
      // End if self-loop has been detected.
    } while (cur != next);
  }
  log_trace(gc, nmethod)("oops_do_marking_epilogue");
}

// -----------------------------------------------------------------------------
// Printing operations


void nmethod::print(outputStream* st, const nmethod* nm, const char *msg) {
  st->print("  nmethod %s (state: %d)", java_lang_String::as_latin1(nm->name()), nm->state());
  if (msg != nullptr) {
    st->print("   %s", msg);
  }
}

} // namespace svm_gc

