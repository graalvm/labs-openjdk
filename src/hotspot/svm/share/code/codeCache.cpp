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

#include "code/codeBlob.hpp"
#include "code/codeCache.hpp"
#include "code/codeHeapState.hpp"
#include "code/compiledIC.hpp"
#include "code/dependencies.hpp"
#include "code/dependencyContext.hpp"
#include "code/nmethod.hpp"
#include "code/pcDesc.hpp"
#include "compiler/compilationPolicy.hpp"
#include "compiler/compileBroker.hpp"
#include "compiler/compilerDefinitions.inline.hpp"
#include "compiler/oopMap.hpp"
#include "gc/shared/barrierSetNMethod.hpp"
#include "gc/shared/classUnloadingContext.hpp"
#include "gc/shared/collectedHeap.hpp"
#include "jfr/jfrEvents.hpp"
#include "jvm_io.h"
#include "logging/log.hpp"
#include "logging/logStream.hpp"
#include "memory/allocation.inline.hpp"
#include "memory/iterator.hpp"
#include "memory/memoryReserver.hpp"
#include "memory/resourceArea.hpp"
#include "memory/universe.hpp"
#include "oops/method.inline.hpp"
#include "oops/objArrayOop.hpp"
#include "oops/oop.inline.hpp"
#include "oops/verifyOopClosure.hpp"
#include "runtime/arguments.hpp"
#include "runtime/atomic.hpp"
#include "runtime/deoptimization.hpp"
#include "runtime/globals_extension.hpp"
#include "runtime/handles.inline.hpp"
#include "runtime/icache.hpp"
#include "runtime/init.hpp"
#include "runtime/java.hpp"
#include "runtime/mutexLocker.hpp"
#include "runtime/os.inline.hpp"
#include "runtime/safepointVerifiers.hpp"
#include "runtime/vmThread.hpp"
#include "sanitizers/leak.hpp"
#include "services/memoryService.hpp"
#include "utilities/align.hpp"
#include "utilities/vmError.hpp"
#include "utilities/xmlstream.hpp"
#ifdef COMPILER1
#include "c1/c1_Compilation.hpp"
#include "c1/c1_Compiler.hpp"
#endif
#ifdef COMPILER2
#include "opto/c2compiler.hpp"
#include "opto/compile.hpp"
#include "opto/node.hpp"
#endif


void CodeCache::nmethods_do(NMethodClosure* cl) {
  assert(SafepointSynchronize::is_at_safepoint() || SVMIsolateData::_during_teardown, "must be");
  typeArrayOop code_info_table = CodeCache::code_info_table();
  if (code_info_table != nullptr) {
    for (int i = 0; i < code_info_table->length(); i++) {
      nmethod *nm = (nmethod*)code_info_table->word_at(i);
      if (nm != nullptr) {
        cl->do_nmethod(nm);
      }
    }
  }
}

void CodeCache::verify_oops() {
  assert(SafepointSynchronize::is_at_safepoint() || SVMIsolateData::_during_teardown, "must be");
  typeArrayOop code_info_table = CodeCache::code_info_table();
  if (code_info_table != nullptr) {
    VerifyOopClosure voc;
    for (int i = 0; i < code_info_table->length(); i++) {
      nmethod *nm = (nmethod*)code_info_table->word_at(i);
      if (nm != nullptr && !nm->will_be_freed()) {
        nm->oops_do(&voc);
      }
    }
  }
}

typeArrayOop CodeCache::code_info_table() {
  // As the code info table is a Word (NonmovableArray) and not really an oop, we must not use any of the Access classes (Words are never compressed).
  return *((typeArrayOop*)(((char*)SVMIsolateData::_runtime_code_info_memory) + SVMGlobalData::_offsets._runtime_code_info_memory._table));
}
