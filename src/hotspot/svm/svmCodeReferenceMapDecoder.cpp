/*
 * Copyright (c) 2020, 2026, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Oracle designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Oracle in the LICENSE file that accompanied this code.
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
 */

#include "code/compressedStream.hpp"
#include "compiler/oopMap.hpp"
#include "exports/sharedGCStructs.h"
#include "svmCodeReferenceMapDecoder.hpp"

/**
 * Code reference maps in SVM support negative offsets and use a form of delta-encoding where the distance (gap) between
 * reference map entries is encoded. n references that are adjacent to each other and that have the same size are encoded
 * as one run with a count of n.
 *
 * - The reference map [8, 16, 32, 40, 64] gets encoded as follows, assuming that it only contains uncompressed references
 *   and no derived references:
 *   - 8, 16 will be encoded as a run with gap=8 and count=2
 *   - 32, 40 will be encoded as a run with gap=8 and count=2
 *   - 64 will be encoded as a run with gap=16 and count=1
 * - If a reference is used as a base for derived references then that base reference gets encoded in a separate run
 *   together with all derived offsets. However, the encoding for such a run is different from the normal encoding.
 */
void SVMCodeReferenceMapDecoder::walk_offsets_from_pointer(u_char *base_address, u_char *encoded_reference_map, jlong reference_map_index, OopClosure *f) {
  assert(encoded_reference_map != nullptr, "must be");
  assert(reference_map_index >= 0, "must be");
  assert(f != nullptr, "must be");

  int uncompressed_size = oopSize;
  int compressed_size = heapOopSize;

  // the logic below was ported from the Java method CodeReferenceMapDecoder.walkOffsetsFromPointer
  CompressedReadStream stream(encoded_reference_map, reference_map_index);
  u_char *obj_ref = base_address;
  bool first_run = true;
  while (true) {
    // Size of gap in bytes (negative means the next pointer has derived pointers)
    jlong gap = stream.read_signed_int();
    // Number of pointers (sign distinguishes between compression and uncompression)
    jlong count = stream.read_signed_int();

    if (gap == 0 && count == 0) {
      break; // reached end of table
    }

    bool derived = false;
    if (!first_run && gap < 0) {
      /* Derived pointer run */
      gap = -(gap + 1);
      derived = true;
    }
    first_run = false;

    obj_ref += (size_t)gap;
    bool compressed = (count < 0);
    size_t ref_size = compressed ? compressed_size : uncompressed_size;
    count = (count < 0) ? -count : count;

    if (derived) {
      /*
       * To correctly relocate a derived pointer, we need to know the value pointed to by
       * the base reference and the derived reference before either one is relocated. This
       * allows us to compute the inner offset, i.e. how much into the actual object does
       * the derived reference point to.
       */
      u_char *base_ptr = base_address == nullptr ? obj_ref : *((u_char**)obj_ref);

      if (compressed) {
        f->do_oop((narrowOop*)obj_ref);
      } else {
        f->do_oop((oop*)obj_ref);
      }

      // NOTE (chaeubl): Derived references are not visited right away. Instead, they are added to a separate table that is
      // processed at a later point in time. This handling is normally done in a deriveOopClosure but the code that calls this
      // closure is deeply embedded into the HotSpot frame internals that we don't use. So, we just hardcode this behavior here.
      // If the derived pointer table is not active, then there is no need to visit derived pointers and they will be ignored.
      if (DerivedPointerTable::is_active()) {
        /* count in this case is the number of derived references for this base pointer */
        for (size_t d = 0; d < count; d++) {
          /* Offset in words from the base reference to the derived reference */
          jlong ref_offset = stream.read_signed_int();

          u_char *derived_ref;
          if (ref_offset >= 0) {
            derived_ref = obj_ref + ((size_t)ref_offset) * ref_size;
          } else {
            derived_ref = obj_ref - ((size_t)-ref_offset) * ref_size;
          }

          guarantee(!compressed, "Derived references must not be compressed.");
          DerivedPointerTable::add((derived_pointer*)derived_ref, (derived_base*)base_ptr);
        }
      }
      obj_ref += ref_size;
    } else {
      if (compressed) {
        for (size_t c = 0; c < count; c += 1) {
          f->do_oop((narrowOop*)obj_ref);
          obj_ref += ref_size;
        }
      } else {
        for (size_t c = 0; c < count; c += 1) {
          f->do_oop((oop*)obj_ref);
          obj_ref += ref_size;
        }
      }
    }
  }
}
