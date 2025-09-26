/*
 * Copyright (c) 2001, 2025, Oracle and/or its affiliates. All rights reserved.
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

#include "classfile/vmSymbols.hpp"
#include "jvm.h"
#include "logging/log.hpp"
#include "memory/allocation.inline.hpp"
#include "oops/oop.inline.hpp"
#include "runtime/arguments.hpp"
#include "runtime/handles.inline.hpp"
#include "runtime/java.hpp"
#include "runtime/javaCalls.hpp"
#include "runtime/mutex.hpp"
#include "runtime/mutexLocker.hpp"
#include "runtime/os.hpp"
#include "runtime/perfData.inline.hpp"
#include "utilities/exceptions.hpp"
#include "utilities/globalCounter.inline.hpp"
#include "utilities/globalDefinitions.hpp"

oop load_oop(void* base, ptrdiff_t offset) {
  return RawAccess<>::oop_load_at((oop)base, offset);
}

void PerfStringVariable::set_value(const char* s2) {
  typeArrayOop byte_arr = (typeArrayOop)load_oop(this, SVMGlobalData::_offsets._perf_string_variable._null_terminated_value);
  char *byte_arr_base = (char*)byte_arr->base();
  int length = byte_arr->length();

  // copy n bytes of the string, assuring the null string is copied if s2 == NULL.
  strncpy(byte_arr_base, s2 == nullptr ? "" : s2, length);

  // assure the string is null terminated when strlen(s2) >= _length
  byte_arr_base[length] = '\0';
}
