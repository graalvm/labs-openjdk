/*
 * Copyright (c) 2023, 2025, Oracle and/or its affiliates. All rights reserved.
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

# NOTE (chaeubl): this file doesn't exist on HotSpot
#
# For spin_wait, HotSpot uses the instruction "yield" on AArch64 by default (see OnSpinWaitInst
# in globals_aarch64.hpp). However, this instruction is a NOP on certain platforms (e.g., Apple
# silicon). So, we always use the instruction "isb" instead.
#
# Relevant HotSpot information:
# - assembler routine used in creation of stub -> macroAssembler_aarch64.cpp: spin_wait()
# - creation of stub: stubGenerator_aarch64.cpp -> generate_spin_wait()
# - logic to set SpinWait configuration -> vm_version_aarch64.cpp: get_spin_wait_desc()

        .text

        .globl aarch64_custom_spin_wait
        .align 5
aarch64_custom_spin_wait:
        isb
        ret
