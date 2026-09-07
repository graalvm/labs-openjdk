/*
 * Copyright (c) 2019, 2024, Oracle and/or its affiliates. All rights reserved.
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

#ifndef SHARE_OOPS_COMPRESSEDOOPS_HPP
#define SHARE_OOPS_COMPRESSEDOOPS_HPP

#include "memory/allStatic.hpp"
#include "memory/memRegion.hpp"
#include "oops/oopsHierarchy.hpp"
#include "utilities/globalDefinitions.hpp"
#include <type_traits>
#ifdef SVM
#include "svmGlobalData.hpp"
#endif // SVM


namespace svm_gc {

class outputStream;
class ReservedHeapSpace;

class CompressedOops : public AllStatic {
  friend class VMStructs;

  // The address range of the heap
  static MemRegion _heap_address_range;

public:
  static void initialize();

  static address  base()                     { return (address)SVMIsolateData::_heap_base; }
  static address  begin()                    { return (address)_heap_address_range.start(); }
  static address  end()                      { return (address)_heap_address_range.end(); }
  static bool     is_base(void* addr)        { return (base() == (address)addr); }
  static int      shift()                    { return CompressedOopShift; }

  static bool is_in(void* addr);
  static bool is_in(MemRegion mr);

  static bool is_null(oop v)       { return v == nullptr; }
  static bool is_null(narrowOop v) { return v == narrowOop::null; }

  static inline oop decode_raw_not_null(narrowOop v);
  static inline oop decode_raw(narrowOop v);
  static inline oop decode_not_null(narrowOop v);
  static inline oop decode(narrowOop v);
  static inline narrowOop encode_not_null(oop v);
  static inline narrowOop encode(oop v);

  // No conversions needed for these overloads
  static inline oop decode_raw_not_null(oop v);
  static inline oop decode_not_null(oop v);
  static inline oop decode(oop v);
  static inline narrowOop encode_not_null(narrowOop v);
  static inline narrowOop encode(narrowOop v);

  template<typename T>
  static inline narrowOop narrow_oop_cast(T i);
};


} // namespace svm_gc

#endif // SHARE_OOPS_COMPRESSEDOOPS_HPP
