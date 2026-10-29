/*
 * Copyright (c) 2018, 2024, Oracle and/or its affiliates. All rights reserved.
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

#ifndef SHARE_GC_SHARED_VMSTRUCTS_GC_HPP
#define SHARE_GC_SHARED_VMSTRUCTS_GC_HPP

#include "gc/shared/ageTable.hpp"
#include "gc/shared/cardTable.hpp"
#include "gc/shared/collectedHeap.hpp"
#include "gc/shared/oopStorage.hpp"
#include "gc/shared/space.hpp"
#if INCLUDE_EPSILONGC
#include "gc/epsilon/vmStructs_epsilon.hpp"
#endif
#if INCLUDE_G1GC
#include "gc/g1/vmStructs_g1.hpp"
#endif
#if INCLUDE_PARALLELGC
#include "gc/parallel/vmStructs_parallelgc.hpp"
#endif
#if INCLUDE_SERIALGC
#include "gc/serial/vmStructs_serial.hpp"
#endif
#if INCLUDE_SHENANDOAHGC
#include "gc/shenandoah/vmStructs_shenandoah.hpp"
#endif
#if INCLUDE_ZGC
#include "gc/z/vmStructs_z.hpp"
#endif

#endif // SHARE_GC_SHARED_VMSTRUCTS_GC_HPP
