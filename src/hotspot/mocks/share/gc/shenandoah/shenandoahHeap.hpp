/*
 * Copyright (c) 2023, 2025, Oracle and/or its affiliates. All rights reserved.
 * Copyright (c) 2013, 2021, Red Hat, Inc. All rights reserved.
 * Copyright Amazon.com Inc. or its affiliates. All Rights Reserved.
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

#ifndef SHARE_GC_SHENANDOAH_SHENANDOAHHEAP_HPP
#define SHARE_GC_SHENANDOAH_SHENANDOAHHEAP_HPP

#include "gc/shared/collectedHeap.hpp"
#include "gc/shared/markBitMap.hpp"
#include "gc/shared/softRefPolicy.hpp"
#include "gc/shenandoah/mode/shenandoahMode.hpp"
#include "gc/shenandoah/shenandoahAllocRequest.hpp"
#include "gc/shenandoah/shenandoahAsserts.hpp"
#include "gc/shenandoah/shenandoahController.hpp"
#include "gc/shenandoah/shenandoahEvacOOMHandler.hpp"
#include "gc/shenandoah/shenandoahEvacTracker.hpp"
#include "gc/shenandoah/shenandoahGenerationSizer.hpp"
#include "gc/shenandoah/shenandoahGenerationType.hpp"
#include "gc/shenandoah/shenandoahLock.hpp"
#include "gc/shenandoah/shenandoahMmuTracker.hpp"
#include "gc/shenandoah/shenandoahPadding.hpp"
#include "gc/shenandoah/shenandoahSharedVariables.hpp"
#include "gc/shenandoah/shenandoahUnload.hpp"
#include "memory/metaspace.hpp"
#include "services/memoryManager.hpp"
#include "utilities/globalDefinitions.hpp"
#include "utilities/stack.hpp"

#endif // SHARE_GC_SHENANDOAH_SHENANDOAHHEAP_HPP
