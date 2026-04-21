/*
 * Copyright (c) 2014, 2026, Oracle and/or its affiliates. All rights reserved.
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
 */
package jdk.vm.ci.hotspot;

import java.util.Arrays;

import jdk.vm.ci.code.stack.InspectedFrame;
import jdk.vm.ci.meta.ResolvedJavaMethod;

/**
 * HotSpot implementation of {@link InspectedFrame}.
 * <p>
 * HotSpot exposes locals here as object, primitive, or unavailable storage. Primitive access is
 * intentionally storage-based: {@link #getLocalInt(int)} and {@link #getLocalFloat(int)} are 32-bit
 * views over the same slot payload, while {@link #getLocalLong(int)} and {@link #getLocalDouble(int)}
 * read an adjacent-slot 64-bit view when the following slot is also primitive. HotSpot does not
 * attempt to recover precise Java primitive kinds for every slot. The VM captures the frame metadata
 * and local arrays eagerly when constructing this object, so subsequent accesses read a stable
 * snapshot of that captured frame state.
 */
public class HotSpotStackFrameReference implements InspectedFrame {
    private static final byte LOCAL_KIND_UNAVAILABLE = 0;
    private static final byte LOCAL_KIND_OBJECT = 1;
    private static final byte LOCAL_KIND_PRIMITIVE = 2;

    private CompilerToVM compilerToVM;
    // set in the VM when materializeVirtualObjects is called
    @SuppressWarnings("unused") private boolean objectsMaterialized;

    // information used to find the stack frame
    private long stackPointer;
    private int frameNumber;

    // information about the stack frame's contents
    private int bci;
    private HotSpotResolvedJavaMethod method;
    private boolean compiledFrame;
    private boolean deoptimizedFrame;
    private Object[] localReferences;
    private boolean[] localIsVirtual;
    private byte[] localKinds;
    private long[] localPrimitiveValues;

    public long getStackPointer() {
        return stackPointer;
    }

    public int getFrameNumber() {
        return frameNumber;
    }

    /**
     * Returns whether this frame was captured from HotSpot compiled code.
     */
    public boolean isCompiledFrame() {
        return compiledFrame;
    }

    /**
     * Returns whether this compiled frame was observed in HotSpot's deoptimized frame shape.
     */
    public boolean isDeoptimizedFrame() {
        return deoptimizedFrame;
    }

    @Override
    public Object getLocal(int index) {
        requireLocalKind(index, LOCAL_KIND_OBJECT, "object");
        return localReferences[index];
    }

    @Override
    public boolean isVirtual(int index) {
        return localIsVirtual == null ? false : localIsVirtual[index];
    }

    private byte getLocalKind(int index) {
        if (localKinds == null) {
            return LOCAL_KIND_UNAVAILABLE;
        }
        return localKinds[index];
    }

    private static String localKindName(byte localKind) {
        return switch (localKind) {
            case LOCAL_KIND_OBJECT -> "object";
            case LOCAL_KIND_PRIMITIVE -> "primitive";
            default -> "unavailable";
        };
    }

    private void requireLocalKind(int index, byte expectedKind, String accessKind) {
        byte actualKind = getLocalKind(index);
        if (actualKind != expectedKind) {
            throw new UnsupportedOperationException("local " + index + " is " + localKindName(actualKind) + ", not " + accessKind);
        }
    }

    // Primitive access is storage-based: a slot is either object, primitive, or unavailable from
    // the current frame state.
    private int primitiveIndex(int index) {
        return index << 1;
    }

    private int requirePrimitive32Value(int index) {
        requireLocalKind(index, LOCAL_KIND_PRIMITIVE, "32-bit primitive storage");
        if (localPrimitiveValues == null) {
            throw new UnsupportedOperationException("primitive local " + index + " is unavailable");
        }
        return (int) localPrimitiveValues[primitiveIndex(index)];
    }

    private long requirePrimitive64Value(int index) {
        requireLocalKind(index, LOCAL_KIND_PRIMITIVE, "64-bit primitive storage");
        if (localKinds == null || index + 1 >= localKinds.length || getLocalKind(index + 1) != LOCAL_KIND_PRIMITIVE) {
            throw new UnsupportedOperationException("local " + index + " does not have a trailing primitive slot for 64-bit primitive storage");
        }
        if (localPrimitiveValues == null) {
            throw new UnsupportedOperationException("wide primitive local " + index + " is unavailable");
        }
        return localPrimitiveValues[primitiveIndex(index) + 1];
    }

    @Override
    public int getLocalInt(int index) {
        return requirePrimitive32Value(index);
    }

    @Override
    public long getLocalLong(int index) {
        return requirePrimitive64Value(index);
    }

    @Override
    public float getLocalFloat(int index) {
        return Float.intBitsToFloat(requirePrimitive32Value(index));
    }

    @Override
    public double getLocalDouble(int index) {
        return Double.longBitsToDouble(requirePrimitive64Value(index));
    }

    @Override
    public void materializeVirtualObjects(boolean invalidateCode) {
        if (Thread.currentThread().isVirtual()) {
            throw new IllegalArgumentException("cannot materialize frames of a virtual thread");
        }
        compilerToVM.materializeVirtualObjects(this, invalidateCode);
    }

    @Override
    public int getBytecodeIndex() {
        return bci;
    }

    @Override
    public boolean isMethod(ResolvedJavaMethod otherMethod) {
        return method.equals(otherMethod);
    }

    @Override
    public boolean hasVirtualObjects() {
        return localIsVirtual != null;
    }

    @Override
    public String toString() {
        return "HotSpotStackFrameReference [stackPointer=" + stackPointer + ", frameNumber=" + frameNumber + ", bci=" + bci + ", method=" + method + ", compiledFrame=" + compiledFrame +
                        ", deoptimizedFrame=" + deoptimizedFrame + ", localReferences=" + Arrays.toString(localReferences) +
                        ", localIsVirtual=" + Arrays.toString(localIsVirtual) + ", localKinds=" + Arrays.toString(localKinds) + ", localPrimitiveValues=" + Arrays.toString(localPrimitiveValues) + "]";
    }
}
