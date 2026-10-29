/*
 * Copyright (c) 2012, 2019, Oracle and/or its affiliates. All rights reserved.
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

#ifndef SHARE_MEMORY_REFERENCETYPE_HPP
#define SHARE_MEMORY_REFERENCETYPE_HPP

#include "utilities/debug.hpp"

// ReferenceType is used to distinguish between java/lang/ref/Reference subclasses
// NOTE (chaeubl): these integer values must match the SVM-side because the reference type is stored in the DynamicHub.

namespace svm_gc {

enum ReferenceType {
  REF_NONE    = 0,   // Regular class
  REF_SOFT    = 1,   // Subclass of java/lang/ref/SoftReference
  REF_WEAK    = 2,   // Subclass of java/lang/ref/WeakReference
  // No DynamicHub will ever contain REF_FINAL - we only keep that value to minimize the diff against HotSpot.
  // If we happen to encounter a FinalReference in the heap, then its hub would contain the value REF_WEAK.
  REF_FINAL   = 3,
  REF_PHANTOM = 4    // Subclass of java/lang/ref/PhantomReference
};


} // namespace svm_gc

#endif // SHARE_MEMORY_REFERENCETYPE_HPP
