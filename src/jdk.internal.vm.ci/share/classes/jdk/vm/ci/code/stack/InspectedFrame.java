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
package jdk.vm.ci.code.stack;

import jdk.vm.ci.meta.ResolvedJavaMethod;

/**
 * Access to the reference and primitive locals in a stack frame.
 * <p>
 * Local inspection is intentionally low level. This API is for trusted runtime code that already
 * knows which slot it wants to inspect. Implementations expose what the current frame state can
 * reliably recover as object storage, primitive storage, or unavailable storage. They may surface
 * stronger implementation-specific information, but callers must not assume that primitive locals
 * can be recovered with Java-level type precision.
 * <p>
 * An {@link InspectedFrame} is a handle to a frame observed during stack iteration.
 * Implementations are not required to snapshot the full frame state when the frame is reported to
 * the visitor. A retained {@code InspectedFrame} may be used after
 * {@link InspectedFrameVisitor#visitFrame(InspectedFrame)} returns only for operations that the
 * implementation can perform safely, either from preserved state or by revalidating the
 * corresponding live stack frame.
 * <p>
 * If an operation cannot be performed safely because the corresponding frame can no longer be
 * found, verified, or read in a compatible state, the operation must fail rather than read or
 * mutate an unrelated frame. Such failures are reported with {@link IllegalStateException}. Local
 * accessors may also throw {@link UnsupportedOperationException} when the requested local is
 * unavailable or not recoverable in the observed frame state.
 * <p>
 * Local availability is part of the observed frame state. For example, a slot may be unavailable
 * because the compiled frame state no longer carries a value for a dead local, or an object accessor
 * may be unsupported for a slot whose recoverable value is primitive storage. An unavailable local
 * continues to throw {@link UnsupportedOperationException} for accesses that are not supported by
 * that frame state.
 */
public interface InspectedFrame {

    /**
     * Returns the value of the reference local at {@code index}. This value is a copy iff
     * {@link #isVirtual(int)} is true.
     *
     * @throws UnsupportedOperationException if the local is unavailable or not recoverable as object
     *             storage on this frame
     */
    Object getLocal(int index);

    /**
     * Returns the raw 32-bit primitive storage at {@code index}. Callers are responsible for
     * interpreting subword values such as {@code boolean}, {@code byte}, {@code short}, and
     * {@code char}. This method is a storage-level accessor: if a 32-bit primitive payload is
     * available, callers may rely on this method even when the source-level primitive type cannot be
     * recovered precisely. Implementations may still report a primitive local as unavailable, for
     * example when compiled-frame state no longer carries a usable value.
     *
     * @throws UnsupportedOperationException if the local is unavailable or not recoverable as 32-bit
     *             primitive storage on this frame
     */
    int getLocalInt(int index);

    /**
     * Returns the raw 64-bit primitive storage starting at local {@code index}.
     * <p>
     * This is a storage-level accessor. Implementations may recover the value structurally from
     * adjacent primitive slots instead of tracking whether the source-level type was {@code long} or
     * {@code double}. Stronger implementations may use more precise metadata, but callers must not
     * depend on that precision.
     *
     * @throws UnsupportedOperationException if the local is unavailable or not recoverable as 64-bit
     *             primitive storage on this frame
     */
    long getLocalLong(int index);

    /**
     * Returns the raw 32-bit primitive storage at {@code index} interpreted as a float. This is a
     * convenience view over the same storage exposed by {@link #getLocalInt(int)}.
     *
     * @throws UnsupportedOperationException if the local is unavailable or not recoverable as 32-bit
     *             primitive storage on this frame
     */
    float getLocalFloat(int index);

    /**
     * Returns the raw 64-bit primitive storage starting at local {@code index} interpreted as a
     * double. This is a convenience view over the same storage exposed by
     * {@link #getLocalLong(int)}.
     *
     * @throws UnsupportedOperationException if the local is unavailable or not recoverable as 64-bit
     *             primitive storage on this frame
     */
    double getLocalDouble(int index);

    /**
     * Returns whether the local at {@code index} is a virtual object, and therefore the reference
     * returned by {@link #getLocal(int)} is a copy.
     */
    boolean isVirtual(int index);

    /**
     * Returns true if the stack frame is a compiled stack frame and there are virtual objects
     * anywhere in the current state of the compiled method. This can return true even if
     * {@link #isVirtual(int)} return false for all locals.
     */
    boolean hasVirtualObjects();

    /**
     * This method will materialize all virtual objects, deoptimize the stack frame and make sure
     * that subsequent execution of the deoptimized frame uses the materialized values.
     *
     * @see StackIntrospection#canMaterializeVirtualObjects
     * @throws IllegalArgumentException if stack introspection does not support
     *             materialization of virtual objects for this frame
     * @throws IllegalStateException if the corresponding frame can no longer be found or verified on
     *             the stack
     */
    void materializeVirtualObjects(boolean invalidateCode);

    /**
     * @return the current bytecode index
     */
    int getBytecodeIndex();

    /**
     * Checks if the current method is equal to the given method.
     */
    boolean isMethod(ResolvedJavaMethod method);
}
