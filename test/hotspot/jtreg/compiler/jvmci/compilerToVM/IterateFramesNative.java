/*
 * Copyright (c) 2021, 2026, Oracle and/or its affiliates. All rights reserved.
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

/*
 * @test
 * @bug 8269592
 *
 * @requires vm.jvmci
 *
 * @library / /test/lib
 * @library ../common/patches
 * @library /testlibrary/asm
 * @modules java.base/jdk.internal.misc
 * @modules jdk.internal.vm.ci/jdk.vm.ci.hotspot
 *          jdk.internal.vm.ci/jdk.vm.ci.code
 *          jdk.internal.vm.ci/jdk.vm.ci.code.stack
 *          jdk.internal.vm.ci/jdk.vm.ci.meta
 *
 * @build jdk.internal.vm.ci/jdk.vm.ci.hotspot.CompilerToVMHelper jdk.test.whitebox.WhiteBox
 * @run driver jdk.test.lib.helpers.ClassFileInstaller jdk.test.whitebox.WhiteBox
 * @run main/othervm -Xbatch -Xbootclasspath/a:.
 *                   -XX:CompileCommand=dontinline,compiler.jvmci.compilerToVM.IterateFramesNative::testNativeFrameCallback
 *                   -XX:CompileCommand=dontinline,compiler.jvmci.compilerToVM.IterateFramesNative::deadBytecodeIndexCarrier
 *                   -XX:CompileCommand=dontinline,compiler.jvmci.compilerToVM.IterateFramesNative::liveBytecodeIndexCarrier
 *                   -XX:CompileCommand=dontinline,compiler.jvmci.compilerToVM.IterateFramesNative::fencedBytecodeIndexCarrier
 *                   -XX:CompileCommand=dontinline,compiler.jvmci.compilerToVM.IterateFramesNative::inspectBytecodeIndexCarrier
 *                   -XX:+UnlockDiagnosticVMOptions -XX:+WhiteBoxAPI
 *                   -XX:+UnlockExperimentalVMOptions -XX:+EnableJVMCI
 *                   compiler.jvmci.compilerToVM.IterateFramesNative
 * @run main/othervm -Xbatch -Xbootclasspath/a:.
 *                   -XX:CompileCommand=dontinline,compiler.jvmci.compilerToVM.IterateFramesNative::testNativeFrameCallback
 *                   -XX:CompileCommand=dontinline,compiler.jvmci.compilerToVM.IterateFramesNative::deadBytecodeIndexCarrier
 *                   -XX:CompileCommand=dontinline,compiler.jvmci.compilerToVM.IterateFramesNative::liveBytecodeIndexCarrier
 *                   -XX:CompileCommand=dontinline,compiler.jvmci.compilerToVM.IterateFramesNative::fencedBytecodeIndexCarrier
 *                   -XX:CompileCommand=dontinline,compiler.jvmci.compilerToVM.IterateFramesNative::inspectBytecodeIndexCarrier
 *                   -XX:+UnlockDiagnosticVMOptions -XX:+WhiteBoxAPI
 *                   -XX:+UnlockExperimentalVMOptions -XX:+EnableJVMCI
 *                   -Dcompiler.jvmci.compilerToVM.IterateFramesNative.checkCompiled=true
 *                   compiler.jvmci.compilerToVM.IterateFramesNative
 */

package compiler.jvmci.compilerToVM;

import compiler.jvmci.common.CTVMUtilities;
import compiler.whitebox.CompilerWhiteBoxTest;
import jdk.test.lib.Asserts;
import jdk.test.lib.Utils;
import jdk.vm.ci.code.stack.InspectedFrameVisitor;
import jdk.vm.ci.hotspot.CompilerToVMHelper;
import jdk.vm.ci.hotspot.HotSpotStackFrameReference;
import jdk.vm.ci.meta.ResolvedJavaMethod;
import jdk.test.whitebox.WhiteBox;

import java.lang.ref.Reference;
import java.lang.reflect.Method;
import java.util.concurrent.atomic.AtomicInteger;

public class IterateFramesNative {
    private static final int FULL_OPTIMIZATION_COMP_LEVEL = CompilerWhiteBoxTest.COMP_LEVEL_FULL_OPTIMIZATION;
    private static final int COMPILE_THRESHOLD = CompilerWhiteBoxTest.THRESHOLD;
    private static final WhiteBox WB;
    private static final Method NATIVE_METHOD;
    private static final Method NATIVE_CALLBACK_METHOD;
    private static final Method ITERATE_FRAMES_METHOD;
    private static final Method DEAD_BYTECODE_INDEX_CARRIER_METHOD;
    private static final Method LIVE_BYTECODE_INDEX_CARRIER_METHOD;
    private static final Method FENCED_BYTECODE_INDEX_CARRIER_METHOD;
    private static final Method INSPECT_BYTECODE_INDEX_CARRIER_METHOD;
    private static final ResolvedJavaMethod NATIVE_METHOD_RESOLVED;
    private static final ResolvedJavaMethod NATIVE_CALLBACK_METHOD_RESOLVED;
    private static final ResolvedJavaMethod ITERATE_FRAMES_METHOD_RESOLVED;
    private static final ResolvedJavaMethod DEAD_BYTECODE_INDEX_CARRIER_METHOD_RESOLVED;
    private static final ResolvedJavaMethod LIVE_BYTECODE_INDEX_CARRIER_METHOD_RESOLVED;
    private static final ResolvedJavaMethod FENCED_BYTECODE_INDEX_CARRIER_METHOD_RESOLVED;
    private static final ResolvedJavaMethod INSPECT_BYTECODE_INDEX_CARRIER_METHOD_RESOLVED;
    private static final boolean CHECK_COMPILED;
    private static final AtomicInteger BYTECODE_INDEX_SINK = new AtomicInteger();
    private static final int COMPILED_CARRIER_ATTEMPTS = 50;
    private static final long COMPILATION_WAIT_MS = 10_000L;

    private Helper expectedCarrierHelper;
    private int expectedCarrierBytecodeIndex;
    private boolean compiledAssertionsEnabled;

    private record CarrierFrameState(boolean compiledFrame, boolean deoptimizedFrame, boolean bytecodeIndexAvailable, int bytecodeIndexValue) {
    }

    static {
        Method nativeCallbackMethod;
        Method deadBytecodeIndexCarrierMethod;
        Method liveBytecodeIndexCarrierMethod;
        Method fencedBytecodeIndexCarrierMethod;
        Method inspectBytecodeIndexCarrierMethod;
        WB = WhiteBox.getWhiteBox();
        try {
            NATIVE_METHOD = IterateFramesNative.class.getDeclaredMethod("callerNative",
                    Runnable.class);
            nativeCallbackMethod = IterateFramesNative.class.getDeclaredMethod("testNativeFrameCallback",
                    Helper.class, String.class, boolean.class, byte.class, short.class, char.class, int.class, long.class, float.class, double.class);
            deadBytecodeIndexCarrierMethod = IterateFramesNative.class.getDeclaredMethod("deadBytecodeIndexCarrier",
                    Helper.class, int.class);
            liveBytecodeIndexCarrierMethod = IterateFramesNative.class.getDeclaredMethod("liveBytecodeIndexCarrier",
                    Helper.class, int.class);
            fencedBytecodeIndexCarrierMethod = IterateFramesNative.class.getDeclaredMethod("fencedBytecodeIndexCarrier",
                    Helper.class, int.class);
            inspectBytecodeIndexCarrierMethod = IterateFramesNative.class.getDeclaredMethod("inspectBytecodeIndexCarrier",
                    ResolvedJavaMethod.class);
            ITERATE_FRAMES_METHOD = CompilerToVMHelper.CompilerToVMClass().getDeclaredMethod(
                    "iterateFrames",
                    ResolvedJavaMethod[].class,
                    ResolvedJavaMethod[].class,
                    int.class,
                    InspectedFrameVisitor.class);
        } catch (NoSuchMethodException e) {
            throw new Error("Can't get executable for test method", e);
        }
        NATIVE_CALLBACK_METHOD = nativeCallbackMethod;
        NATIVE_METHOD_RESOLVED = CTVMUtilities.getResolvedMethod(NATIVE_METHOD);
        NATIVE_CALLBACK_METHOD_RESOLVED = CTVMUtilities.getResolvedMethod(nativeCallbackMethod);
        ITERATE_FRAMES_METHOD_RESOLVED = CTVMUtilities.getResolvedMethod(ITERATE_FRAMES_METHOD);
        DEAD_BYTECODE_INDEX_CARRIER_METHOD = deadBytecodeIndexCarrierMethod;
        LIVE_BYTECODE_INDEX_CARRIER_METHOD = liveBytecodeIndexCarrierMethod;
        FENCED_BYTECODE_INDEX_CARRIER_METHOD = fencedBytecodeIndexCarrierMethod;
        INSPECT_BYTECODE_INDEX_CARRIER_METHOD = inspectBytecodeIndexCarrierMethod;
        DEAD_BYTECODE_INDEX_CARRIER_METHOD_RESOLVED = CTVMUtilities.getResolvedMethod(deadBytecodeIndexCarrierMethod);
        LIVE_BYTECODE_INDEX_CARRIER_METHOD_RESOLVED = CTVMUtilities.getResolvedMethod(liveBytecodeIndexCarrierMethod);
        FENCED_BYTECODE_INDEX_CARRIER_METHOD_RESOLVED = CTVMUtilities.getResolvedMethod(fencedBytecodeIndexCarrierMethod);
        INSPECT_BYTECODE_INDEX_CARRIER_METHOD_RESOLVED = CTVMUtilities.getResolvedMethod(inspectBytecodeIndexCarrierMethod);
        CHECK_COMPILED = Boolean.getBoolean(
                "compiler.jvmci.compilerToVM.IterateFramesNative.checkCompiled");

        loadNativeLibrary();
    }

    public static void main(String[] args) {
        new IterateFramesNative().test();
    }

    private void test() {
        testInitialFrameName();
        for (int i = 0; i < (CHECK_COMPILED ? COMPILE_THRESHOLD : 10_000); i++) {
            testNativeFrame("someString", i);
            testBytecodeIndexCarrier(i);
        }

        if (CHECK_COMPILED) {
            ensureCompiled(NATIVE_METHOD);
            ensureCompiled(NATIVE_CALLBACK_METHOD);
            ensureCompiled(DEAD_BYTECODE_INDEX_CARRIER_METHOD);
            ensureCompiled(LIVE_BYTECODE_INDEX_CARRIER_METHOD);
            ensureCompiled(FENCED_BYTECODE_INDEX_CARRIER_METHOD);
            ensureCompiled(INSPECT_BYTECODE_INDEX_CARRIER_METHOD);

            Asserts.assertTrue(WB.isMethodCompiled(ITERATE_FRAMES_METHOD),
                "Expected iterateFrames to be compiled: " + ITERATE_FRAMES_METHOD);
            Asserts.assertTrue(WB.isMethodCompiled(NATIVE_METHOD),
                "Expected native method to be compiled: " + NATIVE_METHOD);
            Asserts.assertTrue(WB.isMethodCompiled(NATIVE_CALLBACK_METHOD),
                "Expected callback method to be compiled: " + NATIVE_CALLBACK_METHOD);
            Asserts.assertTrue(WB.isMethodCompiled(DEAD_BYTECODE_INDEX_CARRIER_METHOD),
                "Expected dead carrier method to be compiled: " + DEAD_BYTECODE_INDEX_CARRIER_METHOD);
            Asserts.assertTrue(WB.isMethodCompiled(LIVE_BYTECODE_INDEX_CARRIER_METHOD),
                "Expected live carrier method to be compiled: " + LIVE_BYTECODE_INDEX_CARRIER_METHOD);
            Asserts.assertTrue(WB.isMethodCompiled(FENCED_BYTECODE_INDEX_CARRIER_METHOD),
                "Expected fenced carrier method to be compiled: " + FENCED_BYTECODE_INDEX_CARRIER_METHOD);
            Asserts.assertTrue(WB.isMethodCompiled(INSPECT_BYTECODE_INDEX_CARRIER_METHOD),
                "Expected inspection method to be compiled: " + INSPECT_BYTECODE_INDEX_CARRIER_METHOD);

            compiledAssertionsEnabled = true;
            testNativeFrame("compiled", 17);
            testBytecodeIndexCarrier(17);
        }

        testEscapedFrameSnapshotRemainsReadable();
        testReturningFrameIsAllowed();
    }

    private static void ensureCompiled(Method method) {
        if (WB.isMethodCompiled(method)) {
            return;
        }
        boolean enqueued = WB.enqueueMethodForCompilation(method, FULL_OPTIMIZATION_COMP_LEVEL);
        Asserts.assertTrue(enqueued || WB.isMethodCompiled(method),
            "expected method to be queued or already compiled: " + method);
        Utils.waitForCondition(() -> WB.isMethodCompiled(method), COMPILATION_WAIT_MS);
        Asserts.assertTrue(WB.isMethodCompiled(method),
            "expected method to be compiled after explicit enqueue: " + method);
        if (!method.equals(NATIVE_METHOD)) {
            Asserts.assertEQ(WB.getMethodCompilationLevel(method), FULL_OPTIMIZATION_COMP_LEVEL,
                "expected full optimization compilation level for " + method);
        }
    }

    /**
    * Loads native library(libIterateFramesNative.so)
    */
    protected static void loadNativeLibrary() {
        System.loadLibrary("IterateFramesNative");
    }

    public static native void callerNative(Runnable runnable);

    private void testNativeFrame(String str, int iteration) {
        Helper innerHelper = new Helper(str);
        String nullableReference = null;
        boolean booleanValue = (iteration & 1) == 0;
        byte byteValue = (byte) (iteration - 17);
        short shortValue = (short) (1024 + iteration);
        char charValue = (char) ('A' + (iteration & 0x0f));
        int intValue = iteration;
        long longValue = 0x1234_5678_0000_0000L + iteration;
        float floatValue = iteration + 0.25f;
        double doubleValue = iteration + 0.5d;

        callerNative(() -> testNativeFrameCallback(innerHelper, nullableReference, booleanValue, byteValue, shortValue, charValue, intValue, longValue, floatValue, doubleValue));

        Asserts.assertEQ(innerHelper.string, NATIVE_METHOD_RESOLVED.getName(),
            "Native frame not found?: " + NATIVE_METHOD_RESOLVED.getName());
    }

    private void testInitialFrameName() {
        Boolean foundInitialFrame = CompilerToVMHelper.iterateFrames(
            null,
            null,
            0,
            f -> {
                HotSpotStackFrameReference frame = (HotSpotStackFrameReference) f;
                Asserts.assertNotNull(frame, "got null frame for initial walk");
                return frame.isMethod(ITERATE_FRAMES_METHOD_RESOLVED);
            });
        Asserts.assertNotNull(foundInitialFrame, "frame must not be null");
        Asserts.assertTrue(foundInitialFrame,
            "Expected initial frame method to be CompilerToVM.iterateFrames");
    }

    private void testBytecodeIndexCarrier(int iteration) {
        expectedCarrierHelper = new Helper("carrier-" + iteration);
        expectedCarrierBytecodeIndex = 0x5000 + iteration;

        if (CHECK_COMPILED) {
            if (!compiledAssertionsEnabled) {
                deadBytecodeIndexCarrier(expectedCarrierHelper, expectedCarrierBytecodeIndex);
                liveBytecodeIndexCarrier(expectedCarrierHelper, expectedCarrierBytecodeIndex);
                fencedBytecodeIndexCarrier(expectedCarrierHelper, expectedCarrierBytecodeIndex);
                return;
            }
            observeCompiledFailureMode(true);
            observeCompiledFailureMode(false);
            observeCompiledFencedCarrier();
            return;
        }
        deadBytecodeIndexCarrier(expectedCarrierHelper, expectedCarrierBytecodeIndex);
        CarrierFrameState liveCarrier = liveBytecodeIndexCarrier(expectedCarrierHelper, expectedCarrierBytecodeIndex);
        Asserts.assertTrue(liveCarrier.bytecodeIndexAvailable(),
            "expected live carrier bytecode index local to be available");
        Asserts.assertEQ(liveCarrier.bytecodeIndexValue(), expectedCarrierBytecodeIndex,
            "unexpected live carrier bytecode index local");
        CarrierFrameState fencedCarrier = fencedBytecodeIndexCarrier(expectedCarrierHelper, expectedCarrierBytecodeIndex);
        Asserts.assertTrue(fencedCarrier.bytecodeIndexAvailable(),
            "expected fenced carrier bytecode index local to be available");
        Asserts.assertEQ(fencedCarrier.bytecodeIndexValue(), expectedCarrierBytecodeIndex,
            "unexpected fenced carrier bytecode index local");
    }

    private static void expectUnsupported(String message, Runnable access) {
        try {
            access.run();
            throw new AssertionError(message);
        } catch (UnsupportedOperationException expected) {
            // expected
        }
    }

    // The fenced carrier mirrors the intended finally-based usage pattern: the slot should
    // remain visible once we are looking at compiled-frame debug info for that method.
    private void observeCompiledFencedCarrier() {
        CarrierFrameState lastState = null;
        boolean observedCompiledCarrierFrame = false;
        for (int attempt = 0; attempt < COMPILED_CARRIER_ATTEMPTS; attempt++) {
            lastState = fencedBytecodeIndexCarrier(expectedCarrierHelper, expectedCarrierBytecodeIndex);
            if (!lastState.compiledFrame()) {
                continue;
            }
            observedCompiledCarrierFrame = true;
            if (lastState.bytecodeIndexAvailable() && lastState.bytecodeIndexValue() == expectedCarrierBytecodeIndex) {
                return;
            }
        }
        if (!observedCompiledCarrierFrame) {
            throw new AssertionError("did not observe a compiled carrier frame after " + COMPILED_CARRIER_ATTEMPTS
                            + " attempts for fenced carrier"
                            + ": methodCompiled=" + WB.isMethodCompiled(FENCED_BYTECODE_INDEX_CARRIER_METHOD)
                            + ", compilationLevel=" + WB.getMethodCompilationLevel(FENCED_BYTECODE_INDEX_CARRIER_METHOD)
                            + ", lastState=" + lastState);
        }
        throw new AssertionError("did not observe compiled fenced-carrier bytecode index after " + COMPILED_CARRIER_ATTEMPTS
                        + " attempts"
                        + ": methodCompiled=" + WB.isMethodCompiled(FENCED_BYTECODE_INDEX_CARRIER_METHOD)
                        + ", compilationLevel=" + WB.getMethodCompilationLevel(FENCED_BYTECODE_INDEX_CARRIER_METHOD)
                        + ", lastState=" + lastState);
    }

    // The dead carrier demonstrates the failure mode: the source-level local exists, but
    // compiled debug info no longer carries the primitive value for slot 2.
    private void observeCompiledFailureMode(boolean deadCarrier) {
        Method carrierMethod = deadCarrier ? DEAD_BYTECODE_INDEX_CARRIER_METHOD : LIVE_BYTECODE_INDEX_CARRIER_METHOD;
        CarrierFrameState lastState = null;
        boolean observedCompiledCarrierFrame = false;
        for (int attempt = 0; attempt < COMPILED_CARRIER_ATTEMPTS; attempt++) {
            lastState = deadCarrier
                            ? deadBytecodeIndexCarrier(expectedCarrierHelper, expectedCarrierBytecodeIndex)
                            : liveBytecodeIndexCarrier(expectedCarrierHelper, expectedCarrierBytecodeIndex);
            if (!lastState.compiledFrame()) {
                continue;
            }
            observedCompiledCarrierFrame = true;
            if (deadCarrier && !lastState.bytecodeIndexAvailable()) {
                return;
            }
            if (!deadCarrier && lastState.bytecodeIndexAvailable() &&
                            lastState.bytecodeIndexValue() == expectedCarrierBytecodeIndex) {
                return;
            }
        }
        if (!observedCompiledCarrierFrame) {
            throw new AssertionError("did not observe a compiled carrier frame after " + COMPILED_CARRIER_ATTEMPTS
                            + " attempts for " + (deadCarrier ? "dead" : "live") + " carrier"
                            + ": methodCompiled=" + WB.isMethodCompiled(carrierMethod)
                            + ", compilationLevel=" + WB.getMethodCompilationLevel(carrierMethod)
                            + ", lastState=" + lastState);
        }
        throw new AssertionError("did not observe compiled-debug-info failure mode after " + COMPILED_CARRIER_ATTEMPTS
                        + " attempts for " + (deadCarrier ? "dead" : "live") + " carrier"
                        + ": methodCompiled=" + WB.isMethodCompiled(carrierMethod)
                        + ", compilationLevel=" + WB.getMethodCompilationLevel(carrierMethod)
                        + ", lastState=" + lastState);
    }

    private CarrierFrameState deadBytecodeIndexCarrier(Helper helper, int bytecodeIndex) {
        CarrierFrameState state = inspectBytecodeIndexCarrier(DEAD_BYTECODE_INDEX_CARRIER_METHOD_RESOLVED);
        // Keep the helper local live while leaving bytecodeIndex dead at the inspection point.
        Asserts.assertEQ(helper.string, expectedCarrierHelper.string,
            "unexpected helper string after dead carrier inspection");
        return state;
    }

    // These intentionally-empty helpers match the primitive finally-fence pattern under test.
    private static void primitiveReachabilityFence(boolean local) {
    }

    private static void primitiveReachabilityFence(byte local) {
    }

    private static void primitiveReachabilityFence(short local) {
    }

    private static void primitiveReachabilityFence(char local) {
    }

    private static void primitiveReachabilityFence(int local) {
    }

    private static void primitiveReachabilityFence(long local) {
    }

    private static void primitiveReachabilityFence(float local) {
    }

    private static void primitiveReachabilityFence(double local) {
    }

    private CarrierFrameState fencedBytecodeIndexCarrier(Helper helper, int bytecodeIndex) {
        try {
            CarrierFrameState state = inspectBytecodeIndexCarrier(FENCED_BYTECODE_INDEX_CARRIER_METHOD_RESOLVED);
            Asserts.assertEQ(helper.string, expectedCarrierHelper.string,
                "unexpected helper string after fenced carrier inspection");
            return state;
        } finally {
            Reference.reachabilityFence(helper);
            primitiveReachabilityFence(bytecodeIndex);
        }
    }

    private CarrierFrameState liveBytecodeIndexCarrier(Helper helper, int bytecodeIndex) {
        CarrierFrameState state = inspectBytecodeIndexCarrier(LIVE_BYTECODE_INDEX_CARRIER_METHOD_RESOLVED);
        Asserts.assertEQ(helper.string, expectedCarrierHelper.string,
            "unexpected helper string after live carrier inspection");
        BYTECODE_INDEX_SINK.lazySet(bytecodeIndex);
        return state;
    }

    private CarrierFrameState inspectBytecodeIndexCarrier(ResolvedJavaMethod carrierMethod) {
        CarrierFrameState[] state = new CarrierFrameState[1];
        AtomicInteger frameCounter = new AtomicInteger();
        ResolvedJavaMethod[] methods = new ResolvedJavaMethod[] {INSPECT_BYTECODE_INDEX_CARRIER_METHOD_RESOLVED, carrierMethod};
        CompilerToVMHelper.iterateFrames(
            methods,
            methods,
            0,
            f -> {
                HotSpotStackFrameReference frame = (HotSpotStackFrameReference) f;
                Asserts.assertNotNull(frame, "got null frame while inspecting carrier state");
                int index = frameCounter.getAndIncrement();
                if (index == 0) {
                    Asserts.assertTrue(frame.isMethod(INSPECT_BYTECODE_INDEX_CARRIER_METHOD_RESOLVED),
                        "unexpected frame at inspection entry");
                } else if (index == 1) {
                    Asserts.assertTrue(frame.isMethod(carrierMethod),
                        "unexpected carrier frame");
                    Asserts.assertEQ(((Helper) frame.getLocal(1)).string, expectedCarrierHelper.string,
                        "unexpected carrier helper local");
                    boolean bytecodeIndexAvailable = false;
                    int bytecodeIndexValue = Integer.MIN_VALUE;
                    try {
                        bytecodeIndexValue = frame.getLocalInt(2);
                        bytecodeIndexAvailable = true;
                    } catch (UnsupportedOperationException expected) {
                        // dead/unavailable primitive slot
                    }
                    expectUnsupported("expected carrier local 2 to reject object access",
                        () -> frame.getLocal(2));
                    state[0] = new CarrierFrameState(frame.isCompiledFrame(), frame.isDeoptimizedFrame(),
                                    bytecodeIndexAvailable, bytecodeIndexValue);
                    return Boolean.TRUE;
                }
                return null;
            });
        Asserts.assertNotNull(state[0], "expected to capture carrier frame state");
        return state[0];
    }

    private void testNativeFrameCallback(Helper helper, String nullableReference, boolean booleanValue, byte byteValue, short shortValue, char charValue, int intValue, long longValue, float floatValue, double doubleValue) {
        try {
            AtomicInteger frameCounter = new AtomicInteger();
            ResolvedJavaMethod[] methods = new ResolvedJavaMethod[] {NATIVE_METHOD_RESOLVED, NATIVE_CALLBACK_METHOD_RESOLVED};
            CompilerToVMHelper.iterateFrames(
                methods,
                methods,
                0,
                f -> {
                    HotSpotStackFrameReference frame = (HotSpotStackFrameReference) f;
                    Asserts.assertNotNull(frame, "got null frame for native method");
                    int index = frameCounter.getAndIncrement();
                    if (index == 0) {
                        Asserts.assertTrue(frame.isMethod(NATIVE_CALLBACK_METHOD_RESOLVED),
                            "unexpected callback frame");
                        Asserts.assertNotNull(frame.getLocal(0), "expected receiver local");
                        Asserts.assertEQ(((Helper) frame.getLocal(1)).string, helper.string,
                            "unexpected callback helper local");
                        Asserts.assertNull(frame.getLocal(2), "expected null reference local");
                        Asserts.assertEQ(frame.getLocal(2), nullableReference,
                            "unexpected callback nullable reference local");
                        Asserts.assertEQ(frame.getLocalInt(3) != 0, booleanValue,
                            "unexpected callback boolean local");
                        Asserts.assertEQ((byte) frame.getLocalInt(4), byteValue,
                            "unexpected callback byte local");
                        Asserts.assertEQ((short) frame.getLocalInt(5), shortValue,
                            "unexpected callback short local");
                        Asserts.assertEQ((char) frame.getLocalInt(6), charValue,
                            "unexpected callback char local");
                        Asserts.assertEQ(frame.getLocalInt(7), intValue,
                            "unexpected callback int local");
                        Asserts.assertEQ(frame.getLocalLong(8), longValue,
                            "unexpected callback long local");
                        Asserts.assertEQ(frame.getLocalFloat(10), floatValue,
                            "unexpected callback float local");
                        Asserts.assertEQ(frame.getLocalDouble(11), doubleValue,
                            "unexpected callback double local");
                        expectUnsupported("expected object local 1 to reject int access",
                            () -> frame.getLocalInt(1));
                        expectUnsupported("expected primitive local 3 to reject object access",
                            () -> frame.getLocal(3));
                        expectUnsupported("expected trailing primitive slot to reject wide primitive access",
                            () -> frame.getLocalLong(12));
                    } else if (index == 1) {
                        Asserts.assertTrue(frame.isMethod(NATIVE_METHOD_RESOLVED),
                            "unexpected native entry frame");
                        helper.string = NATIVE_METHOD_RESOLVED.getName();
                        Asserts.assertFalse(frame.hasVirtualObjects(),
                            "native frames do not have virtual objects");
                        return Boolean.TRUE; // stop
                    }
                    return null;
                });
        } finally {
            // Keep the callback arguments live at the stack-walk safepoint so the compiled-frame
            // assertions observe deliberate debug info instead of incidental remnants.
            Reference.reachabilityFence(this);
            Reference.reachabilityFence(helper);
            Reference.reachabilityFence(nullableReference);
            primitiveReachabilityFence(booleanValue);
            primitiveReachabilityFence(byteValue);
            primitiveReachabilityFence(shortValue);
            primitiveReachabilityFence(charValue);
            primitiveReachabilityFence(intValue);
            primitiveReachabilityFence(longValue);
            primitiveReachabilityFence(floatValue);
            primitiveReachabilityFence(doubleValue);
        }
    }

    private void testEscapedFrameSnapshotRemainsReadable() {
        HotSpotStackFrameReference[] escaped = new HotSpotStackFrameReference[1];
        Integer bytecodeIndex = CompilerToVMHelper.iterateFrames(
            null,
            null,
            0,
            f -> {
                HotSpotStackFrameReference frame = (HotSpotStackFrameReference) f;
                escaped[0] = frame;
                return frame.getBytecodeIndex();
            });
        Asserts.assertNotNull(bytecodeIndex, "expected a frame bytecode index");
        Asserts.assertNotNull(escaped[0], "expected an escaped frame reference");
        Asserts.assertTrue(escaped[0].isMethod(ITERATE_FRAMES_METHOD_RESOLVED),
            "escaped frame should preserve the captured method identity");
        Asserts.assertEQ(bytecodeIndex.intValue(), escaped[0].getBytecodeIndex());
        Asserts.assertGTE(escaped[0].getBytecodeIndex(), -1);
    }

    private void testReturningFrameIsAllowed() {
        HotSpotStackFrameReference escaped = (HotSpotStackFrameReference) CompilerToVMHelper.iterateFrames(null, null, 0, f -> f);
        Asserts.assertNotNull(escaped, "expected a returned inspected frame");
        Asserts.assertTrue(escaped.isMethod(ITERATE_FRAMES_METHOD_RESOLVED),
            "returned inspected frame should preserve the captured method identity");
    }

    private class Helper {
        public String string;

        public Helper(String s) {
            this.string = s;
        }
    }
}
