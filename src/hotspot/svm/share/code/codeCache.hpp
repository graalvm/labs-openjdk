/*
 * Copyright (c) 1997, 2024, Oracle and/or its affiliates. All rights reserved.
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

#ifndef SHARE_CODE_CODECACHE_HPP
#define SHARE_CODE_CODECACHE_HPP

#include "asm/codeBuffer.hpp"
#include "compiler/compilerDefinitions.hpp"
#include "compiler/oopMap.hpp"
#include "runtime/javaFrameAnchor.hpp"
#include "runtime/frame.hpp"
#include "runtime/handles.hpp"
#include "utilities/align.hpp"
#include "utilities/macros.hpp"

// The GC only needs read access to the code cache, so all more complex operations such as adding or removing code
// only exist on the SVM-side. The GC may only access the code cache at a safepoint or during teardown because it
// cannot use the mutual exclusion mechanism that lives on the SVM-side. This is a major difference to HotSpot,
// where it is sufficient to hold the CodeCache_lock to guarantee mutual exclusion.

namespace svm_gc {

class CodeCache : AllStatic {
public:
  static void nmethods_do(NMethodClosure* cl);             // iterates over all nmethods
  static void verify_oops();
  static typeArrayOop code_info_table();

  static bool is_gc_marking_cycle_active() { return false; }
  static void on_gc_marking_cycle_start()  {}
  static void on_gc_marking_cycle_finish() {}
  static void arm_all_nmethods()           {}
};


} // namespace svm_gc

#endif // SHARE_CODE_CODECACHE_HPP
