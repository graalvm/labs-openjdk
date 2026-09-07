/*
 * Copyright (c) 2014, 2024, Oracle and/or its affiliates. All rights reserved.
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

#ifndef SHARE_GC_G1_G1HEAPREGIONTYPE_HPP
#define SHARE_GC_G1_G1HEAPREGIONTYPE_HPP

#include "gc/g1/g1HeapRegionTraceType.hpp"
#include "utilities/globalDefinitions.hpp"

#define hrt_assert_is_valid(tag) \
  assert(is_valid((tag)), "invalid HR type: %u", (uint) (tag))

class G1HeapRegionType {
friend class VMStructs;

private:
  // Free:                                   0000000 (0)
  // Young mask:                             0000010 (2)
  // Eden:                                   0000010 (2)
  // Survivor:                               0000011 (3)
  // Humongous type mask:                    0000101 (5)
  // Starts humongous:                       0000100 (4)
  // Continues humongous:                    0000101 (5)
  // Old:                                    0001000 (8)
  // Image heap type mask:                   0110000 (48)
  // Closed image heap:                      0100000 (16)
  // Closed image heap, starts humongous:    0101100 (20)
  // Closed image heap, continues humongous: 0101101 (21)
  // Open image heap:                        0010000 (32)
  // Open image heap, starts humongous:      0011100 (36)
  // Open image heap, continues humongous:   0011101 (37)
  //
  // Naming convention:
  // - tag:  use usable type
  // - bit:  a single bit, can be used as a bitmask
  // - mask: multiple bits, can be used as a bitmask
  typedef enum {
    FreeTag                              = 0,

    YoungMask                            = 2,
    EdenTag                              = YoungMask,
    SurvTag                              = YoungMask + 1,

    HumongousBit                         = 4,
    StartsHumongousTag                   = HumongousBit,
    ContinuesHumongousTag                = HumongousBit + 1,
    HumongousTypeMask                    = ContinuesHumongousTag,

    OldBit                               = 8,
    OldTag                               = OldBit,

    ClosedImageHeapBit                   = 16,
    ClosedImageHeapTag                   = ClosedImageHeapBit,
    ClosedImageHeapStartsHumongousTag    = ClosedImageHeapTag | StartsHumongousTag,
    ClosedImageHeapContinuesHumongousTag = ClosedImageHeapTag | ContinuesHumongousTag,

    OpenImageHeapBit                     = 32,
    OpenImageHeapTag                     = OpenImageHeapBit,
    OpenImageHeapStartsHumongousTag      = OpenImageHeapTag | StartsHumongousTag,
    OpenImageHeapContinuesHumongousTag   = OpenImageHeapTag | ContinuesHumongousTag,

    ImageHeapTypeMask                    = ClosedImageHeapTag | OpenImageHeapTag
  } Tag;

  volatile Tag _tag;

  static bool is_valid(Tag tag);

 public:
  Tag get() const {
    hrt_assert_is_valid(_tag);
    return _tag;
  }

 private:
  // Sets the type to 'tag'.
  void set(Tag tag) {
    hrt_assert_is_valid(tag);
    hrt_assert_is_valid(_tag);
    _tag = tag;
  }

  // Sets the type to 'tag', expecting the type to be 'before'. This
  // is available for when we want to add sanity checking to the type
  // transition.
  void set_from(Tag tag, Tag before) {
    hrt_assert_is_valid(tag);
    hrt_assert_is_valid(before);
    hrt_assert_is_valid(_tag);
    assert(_tag == before, "HR tag: %u, expected: %u new tag; %u", _tag, before, tag);
    _tag = tag;
  }

  // Private constructor used for static constants
  G1HeapRegionType(Tag t) : _tag(t) { hrt_assert_is_valid(_tag); }

public:
  // Queries

  bool is_free()                                const { return get() == FreeTag; }

  bool is_young()                               const { return (get() & YoungMask) != 0; }
  bool is_eden()                                const { return get() == EdenTag;  }
  bool is_survivor()                            const { return get() == SurvTag;  }

  bool is_humongous()                           const { return (get() & HumongousBit) != 0;   }
  bool is_starts_humongous()                    const { return (get() & HumongousTypeMask) == StartsHumongousTag;    }
  bool is_continues_humongous()                 const { return (get() & HumongousTypeMask) == ContinuesHumongousTag; }

  bool is_image_heap()                          const { return (get() & ImageHeapTypeMask) != 0; }
  bool is_open_image_heap()                     const { return (get() & ImageHeapTypeMask) == OpenImageHeapTag; }
  bool is_closed_image_heap()                   const { return (get() & ImageHeapTypeMask) == ClosedImageHeapTag; }

  bool is_old()                                 const { return (get() & OldBit) != 0; }
  bool is_old_or_humongous_or_open_image_heap() const {
    // NOTE (chaeubl): we filter humongous closed image heap regions to avoid false positives
    return (get() & (OldBit | HumongousBit | OpenImageHeapBit)) != 0 && !is_closed_image_heap();
  }

  // Setters

  void set_free() { set(FreeTag); }

  void set_eden()        { set_from(EdenTag, FreeTag); }
  void set_eden_pre_gc() { set_from(EdenTag, SurvTag); }
  void set_survivor()    { set_from(SurvTag, FreeTag); }

  void set_starts_humongous()    { set_from(StartsHumongousTag,    FreeTag); }
  void set_continues_humongous() { set_from(ContinuesHumongousTag, FreeTag); }

  void set_old() { set(OldTag); }

  // Change the current region type to be of an old region type if not already done so.
  // Returns whether the region type has been changed or not.
  bool relabel_as_old() {
    //assert(!is_free(), "Should not try to move Free region");
    assert(!is_humongous(), "Should not try to move Humongous region");
    if (is_old()) {
      return false;
    }
    if (is_eden()) {
      set_from(OldTag, EdenTag);
      return true;
    } else if (is_free()) {
      set_from(OldTag, FreeTag);
      return true;
    } else {
      set_from(OldTag, SurvTag);
      return true;
    }
  }

  void set_raw(jbyte tag) { set((G1HeapRegionType::Tag) tag); }

  // Misc

  const char* get_str() const;
  const char* get_short_str() const;
  G1HeapRegionTraceType::Type get_trace_type();

  G1HeapRegionType() : _tag(FreeTag) { hrt_assert_is_valid(_tag); }

  static const G1HeapRegionType Eden;
  static const G1HeapRegionType Survivor;
  static const G1HeapRegionType Old;
  static const G1HeapRegionType Humongous;
};

#endif // SHARE_GC_G1_G1HEAPREGIONTYPE_HPP
