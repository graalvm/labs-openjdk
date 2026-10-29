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

#ifndef SHARE_OPTO_COMPILE_HPP
#define SHARE_OPTO_COMPILE_HPP

#include "asm/codeBuffer.hpp"
#include "ci/compilerInterface.hpp"
#include "code/debugInfoRec.hpp"
#include "compiler/compiler_globals.hpp"
#include "compiler/compileBroker.hpp"
#include "compiler/compilerEvent.hpp"
#include "compiler/cHeapStringHolder.hpp"
#include "libadt/dict.hpp"
#include "libadt/vectset.hpp"
#include "memory/resourceArea.hpp"
#include "oops/methodData.hpp"
#include "opto/idealGraphPrinter.hpp"
#include "opto/phasetype.hpp"
#include "opto/phase.hpp"
#include "opto/regmask.hpp"
#include "runtime/deoptimization.hpp"
#include "runtime/sharedRuntime.hpp"
#include "runtime/timerTrace.hpp"
#include "runtime/vmThread.hpp"
#include "utilities/ticks.hpp"
#include "utilities/vmEnums.hpp"
#include "opto/printinlining.hpp"

#endif // SHARE_OPTO_COMPILE_HPP
