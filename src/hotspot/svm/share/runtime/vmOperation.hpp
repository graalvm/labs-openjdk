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

#ifndef SHARE_RUNTIME_VMOPERATION_HPP
#define SHARE_RUNTIME_VMOPERATION_HPP

#include "memory/allocation.hpp"

// The following classes are used for operations
// initiated by a Java thread but that must
// take place in the VMThread.

#define VM_OP_ENUM(type)   VMOp_##type,

// Note: When new VM_XXX comes up, add 'XXX' to the template table.
#define VM_OPS_DO(template)                       \
  template(Verify)                                \

#ifdef SVM_COMPRESSED_REFERENCES

namespace svm_gc {

typedef char VMOpDataBlackbox[29];
typedef char VMOpWrapperDataBlackbox[44];

} // namespace svm_gc

#else

namespace svm_gc {

typedef char VMOpDataBlackbox[33];
typedef char VMOpWrapperDataBlackbox[52];

} // namespace svm_gc

#endif


namespace svm_gc {

class VM_Operation;

// The layout of this class must match the NativeGCVMOperationData on the Java-side.
// It must always be allocated as a part of a VM_Operation (i.e., it must never exist on its own)
// and it must not contain a vtable. The blackbox fields are only accessed on native-image side.
class VM_OperationData {
 private:
  VMOpDataBlackbox _data;

 public:
  VM_Operation* vm_operation();
};

// This data structure is allocated on the C++ side but only accessed on the SVM side.
class VM_OperationWrapperData {
 private:
  VMOpWrapperDataBlackbox _data;
};

class VM_Operation : public StackObj {
 public:
  enum VMOp_Type {
    VM_OPS_DO(VM_OP_ENUM)
    VMOp_Terminating
  };

  // must be in sync with SVM
  enum VM_OpExecutionStatus {
    BLOCK_VM_THREAD = 0,
    EXECUTE_PROLOGUE = BLOCK_VM_THREAD + 1,
    EXECUTE_VM_OPERATION = EXECUTE_PROLOGUE + 1,
    CANCELLED = EXECUTE_VM_OPERATION + 1,
    ADJUST_THREAD_STATUS = CANCELLED + 1,
    FINISHED = ADJUST_THREAD_STATUS + 1
  };

 private:
  VM_OperationData _data;

 public:
  VM_Operation();

  // Called by VM thread - does in turn invoke doit(). Do not override this
  void evaluate();

  // evaluate() is called by the VMThread and in turn calls doit().
  // If the thread invoking VMThread::execute((VM_Operation*) is a JavaThread,
  // doit_prologue() is called in that thread before transferring control to
  // the VMThread.
  // If doit_prologue() returns true the VM operation will proceed, and
  // doit_epilogue() will be called by the JavaThread once the VM operation
  // completes. If doit_prologue() returns false the VM operation is cancelled.
  virtual void doit()                            = 0;
  virtual bool doit_prologue()                   { return true; };
  virtual void doit_epilogue()                   {};

  VM_OperationData* data()                       { return &_data; }
  static ByteSize data_offset()                  { return byte_offset_of(VM_Operation, _data); }

  // Configuration. Override these appropriately in subclasses.
  virtual VMOp_Type type() const = 0;
  virtual bool allow_nested_vm_operations() const { return false; }
};


} // namespace svm_gc

#endif // SHARE_RUNTIME_VMOPERATION_HPP
