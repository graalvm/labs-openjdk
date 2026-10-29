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

#ifndef SHARE_OOPS_KLASS_HPP
#define SHARE_OOPS_KLASS_HPP

#include "oops/klassFlags.hpp"
#include "memory/iterator.hpp"
#include "memory/memRegion.hpp"
#include "oops/markWord.hpp"
#include "oops/metadata.hpp"
#include "oops/oop.hpp"
#include "oops/oopHandle.hpp"
#include "utilities/accessFlags.hpp"
#include "utilities/macros.hpp"
#if INCLUDE_JFR
#include "jfr/support/jfrTraceIdExtension.hpp"
#endif
#ifdef SVM
#include "memory/referenceType.hpp"
#include "oops/access.hpp"
#include "svmGlobalData.hpp"
#endif // SVM

// Virtual functions are disallowed because SVM DynamicHub objects are modeled as oops and therefore they don't have a vtable.
// Keep in sync with constants in DynamicHubTypeCheckUtil.
constexpr int DynamicHubHashingInterfaceMask = 0xffff;
constexpr int DynamicHubHashingShiftOffset = 24;

class Klass {
 public:
  // Klass Kinds for all subclasses of Klass
  enum KlassKind : u2 {
    InstanceKlassKind = 0,
    InstanceRefKlassKind = 1,
    InstancePodKlassKind = 2,
    InstanceStackChunkKlassKind = 3,
    Other = 4,
    TypeArrayKlassKind = 5,
    ObjArrayKlassKind = 6,
  };
  static const uint KLASS_KIND_COUNT = ObjArrayKlassKind + 1;

  KlassKind kind() const { char value = RawAccess<>::load_at((oop)this, SVMGlobalData::_offsets._dynamicHub._class_type); return (KlassKind)value; }
  ReferenceType reference_type() const { char result = RawAccess<>::load_at((oop)this, SVMGlobalData::_offsets._dynamicHub._reference_type); return (ReferenceType)result; }

  bool is_klass() const;


  // size helper
  int layout_helper() const            { return RawAccess<>::load_at((oop)this, SVMGlobalData::_offsets._dynamicHub._layout_encoding); }

 public:
  // Unpacking layout_helper:
  static const int _lh_neutral_value             = 0;  // neutral non-array non-instance value
  static const int _lh_primitive_value           = _lh_neutral_value + 1;
  static const int _lh_interface_value           = _lh_primitive_value + 1;
  static const int _lh_abstract_value            = _lh_interface_value + 1;
  static const int _lh_stored_continuation_value = _lh_abstract_value + 1;
  static const int _lh_last_special_value        = _lh_stored_continuation_value;

  static const int _lh_array_index_shift_shift   = 0;
  static const int _lh_array_index_shift_mask    = 0xff;
  static const int _lh_array_base_shift          = 8 + _lh_array_index_shift_shift;
  static const int _lh_array_base_mask           = 0xfff;
  static const int _lh_array_tag_bits            = 3;
  static const int _lh_array_tag_shift           = BitsPerInt - _lh_array_tag_bits;
  static const int _lh_array_tag_primitive_value = 0b111;
  static const int _lh_array_tag_object_value    = 0b101;

  static bool layout_helper_is_instance(jint lh) {
    return (jint)lh > (jint)_lh_last_special_value;
  }
  static bool layout_helper_is_array(jint lh) {
    return (jint)lh < (jint)_lh_neutral_value;
  }
  static bool layout_helper_is_typeArray(jint lh) {
    return (juint)lh >= (juint)((juint)_lh_array_tag_primitive_value << _lh_array_tag_shift);
  }
  static bool layout_helper_is_objArray(jint lh) {
    return ((juint)lh >> _lh_array_tag_shift) == (juint)_lh_array_tag_object_value;
  }
  static int layout_helper_header_size(jint lh) {
    assert(layout_helper_is_array(lh), "must be array");
    return (lh >> _lh_array_base_shift) & _lh_array_base_mask;
  }

  static int layout_helper_log2_element_size(jint lh) {
    assert(lh < (jint)_lh_neutral_value, "must be array");
    int l2esz = (lh >> _lh_array_index_shift_shift) & _lh_array_index_shift_mask;
    assert(l2esz <= LogBytesPerLong,
           "sanity. l2esz: 0x%x for lh: 0x%x", (uint)l2esz, (uint)lh);
    return l2esz;
  }
  static int layout_helper_to_size_helper(jint lh) {
    assert(layout_helper_is_instance(lh), "must be instance");
    return lh >> LogBytesPerWord;
  }

  // subclass check
  bool is_subclass_of(const Klass* k) const { return is_subtype_of(k); }
  // subtype check: true if is_subclass_of, or if k is interface and receiver implements it
  bool is_subtype_of(const Klass *other) const;

 public:
  bool is_instance_klass()              const { return kind() <= InstanceStackChunkKlassKind; }
  bool is_reference_instance_klass()    const { return kind() == InstanceRefKlassKind; }
  // NOTE (chaeubl): is_array_klass(), is_objArray_klass(), and is_typeArray_klass() are potentially
  // dangerous as they return false for hybrid objects. This can be misleading but it usually does not
  // cause any problems because the array part of a hybrid object can only contain primitive data at
  // the moment.
  bool is_array_klass()                 const { return kind() >= TypeArrayKlassKind; }
  bool is_pod_instance_klass()          const { return kind() == InstancePodKlassKind; }
  bool is_stack_chunk_instance_klass()  const { return kind() == InstanceStackChunkKlassKind; }
  bool is_objArray_klass()              const { return kind() == ObjArrayKlassKind; }
  bool is_typeArray_klass()             const { return kind() == TypeArrayKlassKind; }

 public:
  void print_value_on(outputStream* st) const;

  void oop_print_value_on(oop obj, outputStream* st);
  void oop_print_on      (oop obj, outputStream* st);

  const char* internal_name() const;
};

#endif // SHARE_OOPS_KLASS_HPP
