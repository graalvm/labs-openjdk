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

#include "classfile/classLoaderDataGraph.hpp"
#include "classfile/stringTable.hpp"
#include "classfile/symbolTable.hpp"
#include "classfile/vmSymbols.hpp"
#include "code/codeCache.hpp"
#include "compiler/compileBroker.hpp"
#include "gc/shared/collectedHeap.hpp"
#include "gc/shared/isGCActiveMark.hpp"
#include "logging/log.hpp"
#include "logging/logStream.hpp"
#include "logging/logConfiguration.hpp"
#include "memory/heapInspection.hpp"
#include "memory/metaspace/metaspaceReporter.hpp"
#include "memory/resourceArea.hpp"
#include "memory/universe.hpp"
#include "oops/symbol.hpp"
#include "runtime/arguments.hpp"
#include "runtime/deoptimization.hpp"
#include "runtime/frame.inline.hpp"
#include "runtime/interfaceSupport.inline.hpp"
#include "runtime/javaThread.inline.hpp"
#include "runtime/jniHandles.hpp"
#include "runtime/objectMonitor.inline.hpp"
#include "runtime/stackFrameStream.inline.hpp"
#include "runtime/synchronizer.hpp"
#include "runtime/threads.hpp"
#include "runtime/threadSMR.inline.hpp"
#include "runtime/vmOperations.hpp"
#include "services/threadService.hpp"
#include "utilities/ticks.hpp"

#define VM_OP_NAME_INITIALIZE(name) #name,

VM_Operation::VM_Operation() {
  Copy::fill_to_bytes(&_data, sizeof(VM_OperationData), 0);
}

VM_Operation* VM_OperationData::vm_operation() {
  return (VM_Operation*) (((address)this) - in_bytes(VM_Operation::data_offset()));
}

void VM_Operation::evaluate() {
  ResourceMark rm;
  doit();
}
