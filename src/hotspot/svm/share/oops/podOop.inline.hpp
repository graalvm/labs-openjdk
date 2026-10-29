/*
 * Copyright (c) 2022, 2026, Oracle and/or its affiliates. All rights reserved.
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

#ifndef SHARE_OOPS_PODOOP_INLINE_HPP
#define SHARE_OOPS_PODOOP_INLINE_HPP

#include "oops/podOop.hpp"

#include "oops/instancePodKlass.inline.hpp"
#include "utilities/macros.hpp"

// NOTE (chaeubl): see SVM class Pod (this file doesn't exist on HotSpot).

inline podOop podOopDesc::cast(oop obj) {
  assert(obj == nullptr || obj->is_pod(), "Wrong type");
  return podOop(obj);
}

inline unsigned char* podOopDesc::reference_map_end() {
  return ((InstancePodKlass*)klass())->reference_map_end(this);
}

#endif // SHARE_OOPS_PODOOP_INLINE_HPP
