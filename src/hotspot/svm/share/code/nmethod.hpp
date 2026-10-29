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

#ifndef SHARE_CODE_NMETHOD_HPP
#define SHARE_CODE_NMETHOD_HPP

#include "code/codeBlob.hpp"
#include "code/pcDesc.hpp"
#include "oops/metadata.hpp"
#include "oops/method.hpp"
#ifdef SVM
#include "runtime/safepoint.hpp"
#include "svmGlobalData.hpp"
#endif // SVM

// NOTE (chaeubl): there must not be any virtual methods in that class. Otherwise, we could not simply cast the SVM class
// CodeInfo* to nmethod*.
//
// nmethods (native methods) are JIT compiled code. This class duplicates parts of the code that exists on the
// SVM-side in the classes CodeInfo, CodeInfoImpl, CodeInfoAccess, and RuntimeCodeInfoAccess. Similar to HotSpot, we
// also support nmethod unloading if the nmethod has a weak reference to otherwise unreachable objects. However, our
// nmethod lifecycle is completely different from HotSpot.
//
// The GC is notified whenever Native Image adds nmethod oops (see for example gc_register_object_fields). Based on those
// notifications, the GC keeps track of code roots for each heap region (i.e., each heap region has a list of nmethods that
// contain references into that region). This is essentially an optimization to avoid that the GC needs to walk all nmethods.
// The GC rebuilds those code roots when necessary (see for example RebuildCodeRootClosure).
//
// Code unloading may only happen:
// a. during a full GC: this is fairly straight forward, as it is based on the mark bitmap of the full GC.
//
// b. during the remark phase of concurrent marking:
// - During a "Concurrent Start" GC, we must not mark any nmethod oops as this could impeded code unloading (note that
//   this is a major difference to HotSpot).
// - During remark, we do the following:
//   - finish marking: the mark bitmap now contains the oop liveness according to snapshot-at-the-beginning (SATB)
//   - mark the tether of all the nmethods that are currently on the stack. This is necessary to:
//     - prevent that we unload non-entrant code that is currently on the stack (SATB mark bitmap only shows us the past,
//       so it is not accurate enough).
//     - prevent that we invalidate code that is currently on the stack
//   - walk all nmethods in the code cache and do code unloading based on nmethod state and oop liveness (see
//     G1ConditionalMarkCodeCacheClosure).
//
// Note that Native Image must not overwrite or destroy any oops that are stored in nmethods (only the GC is allowed to do
// that during code unloading at a safepoint). Otherwise, we may end up with an inconsistent SATB mark bitmap (because we
// don't have pre-write barriers for oops stored in nmethods), which may result in crashes where the Java heap contains
// references to objects that are dead according to the SATB mark bitmap. This can lead to a variety of failures (type
// check errors, wrong vtable calls, ...) because dead objects may get overwritten with filler objects or filler arrays.
//
// Other differences to HotSpot:
// - Code unloading is independent from the ClassUnloading/ClassUnloadingWithConcurrentMark setting.
// - Unlike HotSpot, we don't start a GC if the code cache exceeds a threshold (see GCCause::_codecache_GC_aggressive).
class nmethod {
#ifdef SVM
 public:
  enum NMethodState {
    state_created,
    state_code_constants_live,
    state_non_entrant,
    state_pending_removal_from_code_cache, // GC internal state
    state_removed_from_code_cache,
    state_pending_free                     // GC internal state
  };

  enum NMethodObjectFields {
    tether_index                                = 0,
    name_index                                  = 1,

    first_weakly_referenced_object_field        = name_index + 1
  };

#endif // SVM

 private:
#ifdef SVM
  address address_of(int offset) const { return ((address)this) + offset; };

  template <typename T>
  T read_value(int offset) const { return *((T*)address_of(offset)); }

  template <typename T>
  void write_value(int offset, T value) { *((T*)address_of(offset)) = value; }
#endif // SVM

  struct oops_do_mark_link; // Opaque data type.
  static nmethod* volatile _oops_do_mark_nmethods;

#ifdef SVM
  oops_do_mark_link** _oops_do_mark_link_addr() {
    assert_at_safepoint_msg("must only be accessed at a safepoint - might need volatile semantics otherwise");
    return (oops_do_mark_link**)address_of(SVMGlobalData::_offsets._code_info._gc_data);
  }
  oops_do_mark_link*  _oops_do_mark_link() { return *_oops_do_mark_link_addr(); }
#endif // SVM

  // STW two-phase nmethod root processing helpers.
  //
  // When determining liveness of a given nmethod to do code cache unloading,
  // some collectors need to to different things depending on whether the nmethods
  // need to absolutely be kept alive during root processing; "strong"ly reachable
  // nmethods are known to be kept alive at root processing, but the liveness of
  // "weak"ly reachable ones is to be determined later.
  //
  // We want to allow strong and weak processing of nmethods by different threads
  // at the same time without heavy synchronization. Additional constraints are
  // to make sure that every nmethod is processed a minimal amount of time, and
  // nmethods themselves are always iterated at most once at a particular time.
  //
  // Note that strong processing work must be a superset of weak processing work
  // for this code to work.
  //
  // We store state and claim information in the _oops_do_mark_link member, using
  // the two LSBs for the state and the remaining upper bits for linking together
  // nmethods that were already visited.
  // The last element is self-looped, i.e. points to itself to avoid some special
  // "end-of-list" sentinel value.
  //
  // _oops_do_mark_link special values:
  //
  //   _oops_do_mark_link == nullptr: the nmethod has not been visited at all yet, i.e.
  //      is Unclaimed.
  //
  // For other values, its lowest two bits indicate the following states of the nmethod:
  //
  //   weak_request (WR): the nmethod has been claimed by a thread for weak processing
  //   weak_done (WD): weak processing has been completed for this nmethod.
  //   strong_request (SR): the nmethod has been found to need strong processing while
  //       being weak processed.
  //   strong_done (SD): strong processing has been completed for this nmethod .
  //
  // The following shows the _only_ possible progressions of the _oops_do_mark_link
  // pointer.
  //
  // Given
  //   N as the nmethod
  //   X the current next value of _oops_do_mark_link
  //
  // Unclaimed (C)-> N|WR (C)-> X|WD: the nmethod has been processed weakly by
  //   a single thread.
  // Unclaimed (C)-> N|WR (C)-> X|WD (O)-> X|SD: after weak processing has been
  //   completed (as above) another thread found that the nmethod needs strong
  //   processing after all.
  // Unclaimed (C)-> N|WR (O)-> N|SR (C)-> X|SD: during weak processing another
  //   thread finds that the nmethod needs strong processing, marks it as such and
  //   terminates. The original thread completes strong processing.
  // Unclaimed (C)-> N|SD (C)-> X|SD: the nmethod has been processed strongly from
  //   the beginning by a single thread.
  //
  // "|" describes the concatentation of bits in _oops_do_mark_link.
  //
  // The diagram also describes the threads responsible for changing the nmethod to
  // the next state by marking the _transition_ with (C) and (O), which mean "current"
  // and "other" thread respectively.
  //

  // States used for claiming nmethods during root processing.
  static const uint claim_weak_request_tag = 0;
  static const uint claim_weak_done_tag = 1;
  static const uint claim_strong_request_tag = 2;
  static const uint claim_strong_done_tag = 3;

  static oops_do_mark_link* mark_link(nmethod* nm, uint tag) {
    assert(tag <= claim_strong_done_tag, "invalid tag %u", tag);
    assert(is_aligned(nm, 4), "nmethod pointer must have zero lower two LSB");
    return (oops_do_mark_link*)(((uintptr_t)nm & ~0x3) | tag);
  }

  static uint extract_state(oops_do_mark_link* link) {
    return (uint)((uintptr_t)link & 0x3);
  }

  static nmethod* extract_nmethod(oops_do_mark_link* link) {
    return (nmethod*)((uintptr_t)link & ~0x3);
  }

  void oops_do_log_change(const char* state);


  // Attempt Unclaimed -> N|WR transition. Returns true if successful.
  bool oops_do_try_claim_weak_request();

  // Attempt Unclaimed -> N|SD transition. Returns the current link.
  oops_do_mark_link* oops_do_try_claim_strong_done();
  // Attempt N|WR -> X|WD transition. Returns nullptr if successful, X otherwise.
  nmethod* oops_do_try_add_to_list_as_weak_done();

  // Attempt X|WD -> N|SR transition. Returns the current link.
  oops_do_mark_link* oops_do_try_add_strong_request(oops_do_mark_link* next);
  // Attempt X|WD -> X|SD transition. Returns true if successful.
  bool oops_do_try_claim_weak_done_as_strong_done(oops_do_mark_link* next);

  // Do the N|SD -> X|SD transition.
  void oops_do_add_to_list_as_strong_done();

  // Sets this nmethod as strongly claimed (as part of N|SD -> X|SD and N|SR -> X|SD
  // transitions).
  void oops_do_set_strong_done(nmethod* old_head);

#ifdef SVM
  static void array_oops_do(objArrayOop array, OopClosure* f);

 public:
  // Whether the code represented by this object exists and could have live invocations.
  bool has_code_constants() const { NMethodState s = state(); return s == state_code_constants_live || s == state_non_entrant || s == state_removed_from_code_cache; }
  bool will_be_freed()      const { NMethodState s = state(); return s == state_pending_removal_from_code_cache || s == state_pending_free; }
#endif // SVM

 public:
  // visit all oops
  void oops_do(OopClosure* f);

#ifdef SVM
  // methods to visit a specific part of the oops
  void oops_do_all(OopClosure* f);
  void oops_do_weak(OopClosure* f);
  void oops_do_object_fields(OopClosure* f);
  void oops_do_code_constants(OopClosure* f);
  void oops_do_frame_metadata(OopClosure* f);
  void oops_do_deopt_metadata(OopClosure* f);

  bool has_weak_reference_to_unreachable_object(BoolObjectClosure* is_alive);

  objArrayOop object_fields()                          const { return read_value<objArrayOop>(SVMGlobalData::_offsets._code_info._object_fields); }
  objArrayOop deoptimization_object_constants()        const { return read_value<objArrayOop>(SVMGlobalData::_offsets._code_info._deoptimization_object_constants); }
  objArrayOop frame_info_object_constants()            const { return read_value<objArrayOop>(SVMGlobalData::_offsets._code_info._frame_info_object_constants); }
  NMethodState state()                                 const { return read_value<NMethodState>(SVMGlobalData::_offsets._code_info._state); }
  address code_begin()                                 const { return read_value<address>(SVMGlobalData::_offsets._code_info._code_start); }
  typeArrayOop code_constants_reference_map_encoding() const { return read_value<typeArrayOop>(SVMGlobalData::_offsets._code_info._code_constants_reference_map_encoding); }
  jlong objects_reference_map_index()                  const { return read_value<jlong>(SVMGlobalData::_offsets._code_info._code_constants_reference_map_index); }
  jboolean are_all_objects_in_image_heap()             const { return read_value<jboolean>(SVMGlobalData::_offsets._code_info._are_all_objects_in_image_heap); }

  oop tether()             const { return object_fields()->obj_at(nmethod::tether_index); }
  narrowOop* tether_addr() const { return object_fields()->obj_at_addr<narrowOop>(nmethod::tether_index); }
  oop name()               const { return object_fields()->obj_at(nmethod::name_index); }

  void set_state(NMethodState value) { assert(state() < value, "nmethod state may only increase"); write_value(SVMGlobalData::_offsets._code_info._state, value); }
#endif // SVM

  // All-in-one claiming of nmethods: returns true if the caller successfully claimed that
  // nmethod.
  bool oops_do_try_claim();

  // Class containing callbacks for the oops_do_process_weak/strong() methods
  // below.
  class OopsDoProcessor {
  public:
    // Process the oops of the given nmethod based on whether it has been called
    // in a weak or strong processing context, i.e. apply either weak or strong
    // work on it.
    virtual void do_regular_processing(nmethod* nm) = 0;
    // Assuming that the oops of the given nmethod has already been its weak
    // processing applied, apply the remaining strong processing part.
    virtual void do_remaining_strong_processing(nmethod* nm) = 0;
  };

  // The following two methods do the work corresponding to weak/strong nmethod
  // processing.
  void oops_do_process_weak(OopsDoProcessor* p);
  void oops_do_process_strong(OopsDoProcessor* p);

  static void oops_do_marking_prologue();
  static void oops_do_marking_epilogue();

  void print() const;

 private:
  static void print(outputStream* st, const nmethod* nm, const char *msg);
};

#endif // SHARE_CODE_NMETHOD_HPP
