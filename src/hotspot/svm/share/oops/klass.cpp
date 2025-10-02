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

#include "cds/archiveHeapLoader.hpp"
#include "cds/cdsConfig.hpp"
#include "cds/heapShared.hpp"
#include "classfile/classLoader.hpp"
#include "classfile/classLoaderData.inline.hpp"
#include "classfile/classLoaderDataGraph.inline.hpp"
#include "classfile/javaClasses.inline.hpp"
#include "classfile/moduleEntry.hpp"
#include "classfile/systemDictionary.hpp"
#include "classfile/systemDictionaryShared.hpp"
#include "classfile/vmClasses.hpp"
#include "classfile/vmSymbols.hpp"
#include "gc/shared/collectedHeap.inline.hpp"
#include "jvm_io.h"
#include "logging/log.hpp"
#include "memory/metadataFactory.hpp"
#include "memory/metaspaceClosure.hpp"
#include "memory/oopFactory.hpp"
#include "memory/resourceArea.hpp"
#include "memory/universe.hpp"
#include "oops/compressedKlass.inline.hpp"
#include "oops/compressedOops.inline.hpp"
#include "oops/instanceKlass.hpp"
#include "oops/klass.inline.hpp"
#include "oops/objArrayKlass.hpp"
#include "oops/oop.inline.hpp"
#include "oops/oopHandle.inline.hpp"
#include "prims/jvmtiExport.hpp"
#include "runtime/atomic.hpp"
#include "runtime/handles.inline.hpp"
#include "runtime/perfData.hpp"
#include "utilities/macros.hpp"
#include "utilities/powerOfTwo.hpp"
#include "utilities/rotate_bits.hpp"
#include "utilities/stack.inline.hpp"
#ifdef SVM
#include "svmImageHeap.hpp"
#endif // SVM


namespace svm_gc {

bool Klass::is_klass() const {
  return SVMImageHeap::is_image_heap_object((oop)this) && ((oop)this)->klass_without_asserts() == Universe::_dynamic_hub_klass;
}

juint load_jshort_as_juint(const Klass* base, int offset) {
  jshort value = RawAccess<>::load_at((oop)base, offset);
  return ((juint)value) & 0xffff;
}

jint load_jint(const Klass* base, int offset) {
  return RawAccess<>::load_at((oop)base, offset);
}

jshort load_jshort(const Klass* base, int offset) {
  return RawAccess<>::load_at((oop)base, offset);
}

oop load_oop(const Klass* base, int offset) {
  return RawAccess<>::oop_load_at((oop)base, offset);
}

// type == this, checkedHub == other
bool Klass::is_subtype_of(const Klass* other) const {
  if (SVMGlobalData::_closed_type_world) {
    juint type_check_start = load_jshort_as_juint(this, SVMGlobalData::_offsets._dynamicHub._closed_type_world_type_check_start);
    juint type_check_range = load_jshort_as_juint(this, SVMGlobalData::_offsets._dynamicHub._closed_type_world_type_check_range);
    juint type_check_slot = load_jshort_as_juint(this, SVMGlobalData::_offsets._dynamicHub._closed_type_world_type_check_slot) * 2;

    juint checked_type_id = load_jshort_as_juint(other, SVMGlobalData::_offsets._hybrid_layout._closed_type_world_type_check_slots + type_check_slot);
    return checked_type_id - type_check_start < type_check_range;
  } else {
    // open type world
    jshort type_id_depth = load_jshort(this, SVMGlobalData::_offsets._dynamicHub._open_type_world_type_id_depth);
    jshort checked_num_class_types = load_jshort(other, SVMGlobalData::_offsets._dynamicHub._open_type_world_num_class_types);
    if (type_id_depth >= 0) {
      // perform class check
      if (type_id_depth >= checked_num_class_types) {
        return false;
      }
      jint type_id = load_jint(this, SVMGlobalData::_offsets._dynamicHub._open_type_world_type_id);
      typeArrayOop checked_type_ids = (typeArrayOop)load_oop(other, SVMGlobalData::_offsets._dynamicHub._open_type_world_type_check_slots);
      jint checked_class_id = checked_type_ids->int_at(type_id_depth);
      return checked_class_id == type_id;
    } else {
      uint32_t interface_id = static_cast<uint32_t>(load_jint(this, SVMGlobalData::_offsets._dynamicHub._open_type_world_interface_id));
      if (SVMGlobalData::_use_interface_hashing && interface_id <= SVMGlobalData::_interface_hashing_max_id) {
        // perform interface hashing check
        uint32_t hash_param = static_cast<uint32_t>(load_jint(other, SVMGlobalData::_offsets._dynamicHub._open_type_world_interface_hash_param));
        int shift = hash_param >> DynamicHubHashingShiftOffset;

        typeArrayOop hash_table = (typeArrayOop)load_oop(other, SVMGlobalData::_offsets._dynamicHub._open_type_world_interface_hash_table);
        int hash = (interface_id >> shift) & hash_param;

        jint hash_table_entry = hash_table->int_at(hash);
        return (hash_table_entry & DynamicHubHashingInterfaceMask) == interface_id;
      } else {
        // perform iterative interface check
        jshort checked_num_iterable_interface_types = load_jshort(other, SVMGlobalData::_offsets._dynamicHub._open_type_world_num_iterable_interface_types);
        typeArrayOop checked_interface_ids = (typeArrayOop)load_oop(other, SVMGlobalData::_offsets._dynamicHub._open_type_world_type_check_slots);

        for (int i = 0; i < checked_num_iterable_interface_types; i++) {
          jint checked_interface_id = checked_interface_ids->int_at(checked_num_class_types + (i * 2));
          if (checked_interface_id == interface_id) {
            return true;
          }
        }
        return false;
      }
    }
  }
}

// Printing

// NOTE (chaeubl): replaces the inherited method Metadata::print_value_on(...)
void Klass::print_value_on(outputStream* st) const {
  st->print_cr("%s", internal_name());
}

#define BULLET  " - "

// Caller needs ResourceMark
void Klass::oop_print_on(oop obj, outputStream* st) {
  // print title
  st->print_cr("%s ", internal_name());
  obj->print_address_on(st);

  // print class
  st->print(BULLET"klass: ");
  obj->klass()->print_value_on(st);
  st->cr();
}

void Klass::oop_print_value_on(oop obj, outputStream* st) {
  // print title
  ResourceMark rm;              // Cannot print in debug mode without this
  st->print("%s", internal_name());
  obj->print_address_on(st);
}

const char* Klass::internal_name() const {
  oop string = RawAccess<>::oop_load_at((oop)this, SVMGlobalData::_offsets._dynamicHub._name);
  return java_lang_String::as_latin1(string);
}

} // namespace svm_gc

