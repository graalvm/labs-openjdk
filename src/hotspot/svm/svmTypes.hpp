/*
 * Copyright (c) 2019, 2026, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Oracle designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Oracle in the LICENSE file that accompanied this code.
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
 */

#ifndef SVM_TYPES_HPP
#define SVM_TYPES_HPP

class IsolateThread;
struct StackFramesPerThread;
struct StackFrames;
struct CodeInfosPerThread;
class VM_OperationData;
class VM_OperationWrapperData;
class oopDesc;

typedef void(*queueVmOperationFunc)(address, IsolateThread*, VM_OperationData*, VM_OperationWrapperData*);
typedef void(*vmOperationStatusFunc)(address, IsolateThread*, VM_OperationWrapperData*, int);
typedef bool(*vmOperationDataFunc)(address, IsolateThread*, VM_OperationWrapperData*);
typedef StackFramesPerThread*(*fetchThreadStackFramesFunc)(address, IsolateThread*);
typedef void*(*freeThreadStackFramesFunc)(address, IsolateThread*, StackFramesPerThread*);
typedef StackFrames*(*fetchContinuationStackFramesFunc)(address, oopDesc*);
typedef void*(*freeContinuationStackFramesFunc)(address, StackFrames*);
typedef CodeInfosPerThread*(*fetchCodeInfosFunc)(address, IsolateThread*);
typedef void*(*freeCodeInfosFunc)(address, IsolateThread*, CodeInfosPerThread*);
typedef void*(*threadStateTransitionFunc)(IsolateThread*);
typedef bool*(*fastThreadStateTransitionFunc)(IsolateThread*);
typedef void(*cleanRuntimeCodeCacheFunc)(address, IsolateThread*);

#endif // SVM_TYPES_HPP
