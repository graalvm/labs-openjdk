/*
 * Copyright (c) 2014, 2024, Oracle and/or its affiliates. All rights reserved.
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

#ifndef SHARE_GC_G1_G1PARSCANTHREADSTATE_HPP
#define SHARE_GC_G1_G1PARSCANTHREADSTATE_HPP

#include "gc/g1/g1CollectedHeap.hpp"
#include "gc/g1/g1OopClosures.hpp"
#include "gc/g1/g1RedirtyCardsQueue.hpp"
#include "gc/g1/g1YoungGCAllocationFailureInjector.hpp"
#include "gc/shared/ageTable.hpp"
#include "gc/shared/copyFailedInfo.hpp"
#include "gc/shared/gc_globals.hpp"
#include "gc/shared/partialArraySplitter.hpp"
#include "gc/shared/partialArrayState.hpp"
#include "gc/shared/stringdedup/stringDedup.hpp"
#include "gc/shared/taskqueue.hpp"
#include "memory/allocation.hpp"
#include "oops/oop.hpp"
#include "utilities/ticks.hpp"

#endif // SHARE_GC_G1_G1PARSCANTHREADSTATE_HPP
