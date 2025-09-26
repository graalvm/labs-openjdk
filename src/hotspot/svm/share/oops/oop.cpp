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

#include "cds/cdsConfig.hpp"
#include "classfile/altHashing.hpp"
#include "classfile/javaClasses.inline.hpp"
#include "gc/shared/collectedHeap.inline.hpp"
#include "gc/shared/gc_globals.hpp"
#include "memory/resourceArea.hpp"
#include "memory/universe.hpp"
#include "oops/access.inline.hpp"
#include "oops/compressedKlass.inline.hpp"
#include "oops/compressedOops.inline.hpp"
#include "oops/oop.inline.hpp"
#include "oops/verifyOopClosure.hpp"
#include "runtime/handles.inline.hpp"
#include "runtime/javaThread.hpp"
#include "runtime/synchronizer.hpp"
#include "utilities/macros.hpp"
#ifdef SVM
#include "svmImageHeap.hpp"
#endif // SVM

void oopDesc::print_on(outputStream* st) const {
  if (*((juint*)this) == badHeapWordVal) {
    st->print_cr("BAD WORD");
  } else {
    klass()->oop_print_on(cast_to_oop(this), st);
  }
}

void oopDesc::print_address_on(outputStream* st) const {
  st->print("{" PTR_FORMAT "}", p2i(this));

}

void oopDesc::print()         { print_on(tty);         }

void oopDesc::print_value_on(outputStream* st) const {
  oop obj = const_cast<oopDesc*>(this);
  klass()->oop_print_value_on(obj, st);
}

// used only for asserts and guarantees
bool oopDesc::is_oop(oop obj, bool ignore_mark_word) {
  if (!Universe::heap()->is_oop(obj)) {
    return false;
  }
  return true;
}

// used only for asserts and guarantees
bool oopDesc::is_oop_or_null(oop obj, bool ignore_mark_word) {
  return obj == nullptr ? true : is_oop(obj, ignore_mark_word);
}

VerifyOopClosure VerifyOopClosure::verify_oop;

template <class T> void VerifyOopClosure::do_oop_work(T* p) {
  oop obj = RawAccess<>::oop_load(p);
  guarantee(oopDesc::is_oop_or_null(obj), "invalid oop: " PTR_FORMAT, p2i((oopDesc*) obj));
}

void VerifyOopClosure::do_oop(oop* p)       { VerifyOopClosure::do_oop_work(p); }
void VerifyOopClosure::do_oop(narrowOop* p) { VerifyOopClosure::do_oop_work(p); }

void oopDesc::obj_field_put_raw(int offset, oop value)                { RawAccess<>::oop_store_at(as_oop(), offset, value); }
