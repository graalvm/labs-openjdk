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

#ifndef SHARE_OOPS_INSTANCEKLASS_HPP
#define SHARE_OOPS_INSTANCEKLASS_HPP

#include "memory/allocation.hpp"
#include "memory/referenceType.hpp"
#include "oops/annotations.hpp"
#include "oops/constMethod.hpp"
#include "oops/fieldInfo.hpp"
#include "oops/instanceKlassFlags.hpp"
#include "oops/instanceOop.hpp"
#include "runtime/handles.hpp"
#include "runtime/javaThread.hpp"
#include "utilities/accessFlags.hpp"
#include "utilities/align.hpp"
#include "utilities/growableArray.hpp"
#include "utilities/macros.hpp"
#if INCLUDE_JFR
#include "jfr/support/jfrKlassExtension.hpp"
#endif
#ifdef SVM
#include "oops/compressedOops.hpp"
#include "utilities/devirtualizer.hpp"
#include "svmOopMap.hpp"
#endif // SVM

// Describes where oops are located in instances of this klass.
class OopMapBlock {
 public:
  // Byte offset of the first oop mapped by this block.
  int offset() const          { return _offset; }
  void set_offset(int offset) { _offset = offset; }

  // Number of oops in this block.
  uint count() const         { return _count; }
  void set_count(uint count) { _count = count; }

  // sizeof(OopMapBlock) in words.
  static const int size_in_words() {
    return align_up((int)sizeof(OopMapBlock), wordSize) >>
      LogBytesPerWord;
  }

 private:
  int  _offset;
  uint _count;
};

class InstanceKlass: public Klass {
 public:
  static const KlassKind Kind = InstanceKlassKind;

 public:
  unsigned int nonstatic_oop_map_count() const {
    return reference_map()->length();
  }

 public:
  static InstanceKlass* cast(Klass* k) {
    return const_cast<InstanceKlass*>(cast(const_cast<const Klass*>(k)));
  }

  static const InstanceKlass* cast(const Klass* k) {
    assert(k != nullptr, "k should not be null");
    assert(k->is_instance_klass(), "cast to InstanceKlass");
    return static_cast<const InstanceKlass*>(k);
  }

  OopMapBlock* start_of_nonstatic_oop_maps() const {
    return reference_map()->first_oop_map_block();
  }

  // Use this to return the size of an instance in heap words:
  int size_helper() const {
    return layout_helper_to_size_helper(layout_helper());
  }

  // Oop fields iterators
  //
  // The InstanceKlass iterators also visits the Object's klass.

  // Forward iteration
 public:
  // Iterate over all oop fields in the oop maps.
  template <typename T, class OopClosureType>
  inline void oop_oop_iterate_oop_maps(oop obj, OopClosureType* closure);

  // Iterate over all oop fields and metadata.
  template <typename T, class OopClosureType>
  inline void oop_oop_iterate(oop obj, OopClosureType* closure);

  // Iterate over all oop fields in one oop map.
  template <typename T, class OopClosureType>
  inline void oop_oop_iterate_oop_map(OopMapBlock* map, oop obj, OopClosureType* closure);


  // Reverse iteration
  // Iterate over all oop fields and metadata.
  template <typename T, class OopClosureType>
  inline void oop_oop_iterate_reverse(oop obj, OopClosureType* closure);

 private:
  // Iterate over all oop fields in the oop maps.
  template <typename T, class OopClosureType>
  inline void oop_oop_iterate_oop_maps_reverse(oop obj, OopClosureType* closure);

  // Iterate over all oop fields in one oop map.
  template <typename T, class OopClosureType>
  inline void oop_oop_iterate_oop_map_reverse(OopMapBlock* map, oop obj, OopClosureType* closure);


  // Bounded range iteration
 public:
  // Iterate over all oop fields in the oop maps.
  template <typename T, class OopClosureType>
  inline void oop_oop_iterate_oop_maps_bounded(oop obj, OopClosureType* closure, MemRegion mr);

  // Iterate over all oop fields and metadata.
  template <typename T, class OopClosureType>
  inline void oop_oop_iterate_bounded(oop obj, OopClosureType* closure, MemRegion mr);

 private:
  // Iterate over all oop fields in one oop map.
  template <typename T, class OopClosureType>
  inline void oop_oop_iterate_oop_map_bounded(OopMapBlock* map, oop obj, OopClosureType* closure, MemRegion mr);

  SVMOopMap* reference_map() const {
    int compressedOffset = RawAccess<>::load_at((oop)this, SVMGlobalData::_offsets._dynamicHub._reference_map_compressed_offset);
    address map = CompressedOops::base() + (compressedOffset << ReferenceMapCompressedOffsetShift);
    return (SVMOopMap*)map;
  }
};

class OopIterateFilterClosure: public OopClosure {
private:
  OopIterateClosure* const _closure;
  MemRegion _bound;

public:

  OopIterateFilterClosure(OopIterateClosure* closure, MemRegion bound)
    : _closure(closure),
      _bound(bound) {}

  virtual void do_oop(oop* p)       override { do_oop_work(p); }
  virtual void do_oop(narrowOop* p) override { do_oop_work(p); }

  template <typename T>
  void do_oop_work(T* p) {
    if (_bound.contains(p)) {
      Devirtualizer::do_oop(_closure, p);
    }
  }
};

#endif // SHARE_OOPS_INSTANCEKLASS_HPP
