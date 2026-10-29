/*
 * Copyright (c) 2022, Oracle and/or its affiliates. All rights reserved.
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

#ifndef SHARE_GC_SHARED_CONTINUATIONGCSUPPORT_HPP
#define SHARE_GC_SHARED_CONTINUATIONGCSUPPORT_HPP

#include "memory/allStatic.hpp"
#include "oops/oopsHierarchy.hpp"


namespace svm_gc {

class ContinuationGCSupport : public AllStatic {
public:
  // Relativize and transform to use a bitmap for future oop iteration for the
  // given oop if it is a stack chunk.
  static void transform_stack_chunk(oop obj) {
    // NOTE (chaeubl): On HotSpot, this method does the following:
    // - Relativize derived oops (i.e., reduce them to their offset) so that the GC does not need visit/adjust them.
    //   This is necessary because derived pointers would cause issues for the bitmap that is computed, see below.
    //   On ZGC/Shenandoah derived pointers also cause issues for the read barriers, so for those GCs, the pointers
    //   are relativized while freezing the continuation.
    // - Convert the oops in the stack chunk to compressed references.
    // - Build a bitmap so that the GC can iterate over all the reference more easily (1 bit per 4 or 8 bytes,
    //   depending on heapOopSize).
    //
    // For Native Image, none of that is needed at the moment. The bitmap would be beneficial for continuations that are
    // large or that live a long time. For small, short-lived continuations it is just an overhead. All those
    // transformations also make thawing a continuation more expensive because it is necessary to undo the transformations
    // before execution can continue in the continuation.
  }
};


} // namespace svm_gc

#endif // SHARE_GC_SHARED_CONTINUATIONGCSUPPORT_HPP
