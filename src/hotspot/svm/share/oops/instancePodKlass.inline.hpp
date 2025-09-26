/*
 * Copyright (c) 2022, 2025, Oracle and/or its affiliates. All rights reserved.
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

#ifndef SHARE_OOPS_INSTANCEPODKLASS_INLINE_HPP
#define SHARE_OOPS_INSTANCEPODKLASS_INLINE_HPP

#include "oops/instancePodKlass.hpp"

#include "memory/iterator.inline.hpp"
#include "oops/arrayOop.inline.hpp"
#include "oops/instanceKlass.inline.hpp"
#include "oops/klass.hpp"
#include "oops/oop.inline.hpp"
#include "oops/podOop.inline.hpp"

// NOTE (chaeubl): see SVM class Pod (this file doesn't exist on HotSpot).

template <typename T, class OopClosureType>
void InstancePodKlass::oop_oop_iterate(oop obj, OopClosureType* closure) {
  // Use the reference map of the super class to visit the inherited object fields.
  InstanceKlass* k = (InstanceKlass*)this;
  k->oop_oop_iterate<T>(obj, closure);

  // Use the embedded reference map to visit the dynamically defined object fields.
  podOop pod = podOopDesc::cast(obj);
  unsigned char* reference_map = pod->reference_map_end();
  address pos = (address) pod->base();

  unsigned char nrefs;
  unsigned char gap;
  do {
      nrefs = *(--reference_map);
      gap = *(--reference_map);

      for (int i = 0; i < nrefs; i++) {
        T* p = (T*) pos;
        Devirtualizer::do_oop(closure, p);
        pos += heapOopSize;
      }

      pos += (heapOopSize * gap);
  } while (gap != 0 || nrefs == 0xff);
}

template <typename T, class OopClosureType>
void InstancePodKlass::oop_oop_iterate_reverse(oop obj, OopClosureType* closure) {
  // Iterating in reverse order would be slow due to the structure of the reference map, so lets always iterate forwards.
  oop_oop_iterate<T>(obj, closure);
}

template <typename T, class OopClosureType>
void InstancePodKlass::oop_oop_iterate_bounded(oop obj, OopClosureType* closure, MemRegion mr) {
  OopIterateFilterClosure filter_closure(closure, mr);
  oop_oop_iterate<T>(obj, &filter_closure);
}

unsigned char* InstancePodKlass::reference_map_end(oop obj) {
  // This method may be called before the object header is fully initialized, therefore the address computation is a bit more complex.
  return  ((address)obj) + base_offset_in_bytes() + ((arrayOop)obj)->length();
}

#endif // SHARE_OOPS_INSTANCEPODKLASS_INLINE_HPP
