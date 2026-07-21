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
 * @requires vm.jvmci
 * @summary Test CONSTANT_Dynamic resolution by HotSpotConstantPool.
 * @library /testlibrary/asm
 * @modules jdk.internal.vm.ci/jdk.vm.ci.hotspot:+open
 *          jdk.internal.vm.ci/jdk.vm.ci.runtime
 *          jdk.internal.vm.ci/jdk.vm.ci.meta
 * @run testng/othervm
 *      -XX:+UnlockExperimentalVMOptions -XX:+EnableJVMCI -XX:-UseJVMCICompiler
 *      jdk.vm.ci.hotspot.test.TestDynamicConstant
 * @run testng/othervm
 *      -XX:+UnlockExperimentalVMOptions -XX:+EnableJVMCI -XX:-UseJVMCICompiler -XX:+UnlockDiagnosticVMOptions -XX:UseBootstrapCallInfo=3
 *      jdk.vm.ci.hotspot.test.TestDynamicConstant
 */

package jdk.vm.ci.hotspot.test;

import java.io.File;
import java.io.IOException;
import java.lang.invoke.CallSite;
import java.lang.invoke.ConstantBootstraps;
import java.lang.invoke.MethodHandle;
import java.lang.invoke.MethodHandles;
import java.lang.invoke.MethodType;
import java.lang.invoke.StringConcatFactory;
import java.lang.reflect.Method;
import java.nio.file.Files;
import java.util.List;
import java.util.Map;

import org.testng.Assert;
import org.testng.annotations.Test;

import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.ConstantDynamic;
import org.objectweb.asm.Handle;
import org.objectweb.asm.MethodVisitor;
import org.objectweb.asm.Opcodes;
import org.objectweb.asm.Type;
import jdk.vm.ci.hotspot.HotSpotObjectConstant;
import jdk.vm.ci.hotspot.HotSpotConstantPool;
import jdk.vm.ci.hotspot.HotSpotResolvedObjectType;
import jdk.vm.ci.meta.ConstantPool;
import jdk.vm.ci.meta.ConstantPool.BootstrapMethodInvocation;
import jdk.vm.ci.meta.JavaConstant;
import jdk.vm.ci.meta.JavaMethod;
import jdk.vm.ci.meta.JavaType;
import jdk.vm.ci.meta.MetaAccessProvider;
import jdk.vm.ci.meta.PrimitiveConstant;
import jdk.vm.ci.meta.ResolvedJavaMethod;
import jdk.vm.ci.meta.UnresolvedJavaMethod;
import jdk.vm.ci.runtime.JVMCI;

/**
 * Tests support for Dynamic constants and {@link jdk.vm.ci.meta.ConstantPool#lookupBootstrapMethodInvocation}.
 *
 * @see "https://openjdk.org/jeps/309"
 * @see "https://bugs.openjdk.org/browse/JDK-8177279"
 */
public class TestDynamicConstant implements Opcodes {

    private static final int PUBLIC_STATIC = ACC_PUBLIC | ACC_STATIC;

    static final String testClassInternalName = Type.getInternalName(TestDynamicConstant.class);
    static final String constantBootstrapsClassInternalName = Type.getInternalName(ConstantBootstraps.class);

    enum CondyType {
        /**
         * Condy whose bootstrap method is one of the {@code TestDynamicConstant.get<type>BSM()}
         * methods.
         */
        CALL_DIRECT_BSM,

        /**
         * Condy whose bootstrap method is one of the {@code TestDynamicConstant.get<type>BSM(<type> constant, int i)}
         * methods with one condy arg and one int arg.
         */
        CALL_DIRECT_WITH_ARGS_BSM,

        /**
         * Condy whose bootstrap method is the varargs {@link ConstantBootstraps#invoke} method
         * that invokes one of the {@code TestDynamicConstant.get<type>()} methods.
         */
        CALL_INDIRECT_BSM,

        /**
         * Condy whose bootstrap method is the varargs {@link ConstantBootstraps#invoke} method
         * that invokes one of the {@code TestDynamicConstant.get<type>(<type> p1, <type> p2)}
         * methods with args that are condys themselves.
         */
        CALL_INDIRECT_WITH_ARGS_BSM
    }

    /**
     * Generates a class with a static {@code run} method that returns a value loaded from
     * CONSTANT_Dynamic constant pool entry.
     */
    static class TestGenerator {

        /**
         * Type of value returned by the generated {@code run} method.
         */
        final Type type;

        /**
         * Type of condy used to produce the returned value.
         */
        final CondyType condyType;

        /**
         * Base name of the static {@code TestDynamicConstant.get<type>} method(s) invoked from
         * condys in the generated class.
         */
        final String getter;

        /**
         * Name of the generated class.
         */
        final String className;

        /**
         * Whether to add an invokedynamic whose bootstrap method is a
         * signature-polymorphic MethodHandle method.
         */
        final boolean includeNonDirectBootstrapMethod;

        /**
         * Whether to add a dynamic constant whose bootstrap method fails.
         */
        final boolean includeFailingDynamicConstant;

        TestGenerator(Class<?> type, CondyType condyType) {
            this(type, condyType, false, false);
        }

        TestGenerator(Class<?> type, CondyType condyType, boolean includeNonDirectBootstrapMethod) {
            this(type, condyType, includeNonDirectBootstrapMethod, false);
        }

        TestGenerator(Class<?> type, CondyType condyType, boolean includeNonDirectBootstrapMethod, boolean includeFailingDynamicConstant) {
            String typeName = type.getSimpleName();
            this.type = Type.getType(type);
            this.condyType = condyType;
            this.getter = "get" + typeName.substring(0, 1).toUpperCase() + typeName.substring(1);
            this.includeNonDirectBootstrapMethod = includeNonDirectBootstrapMethod;
            this.includeFailingDynamicConstant = includeFailingDynamicConstant;
            this.className = TestDynamicConstant.class.getName() + "$" + typeName + '_' + condyType +
                            (includeNonDirectBootstrapMethod ? "_NonDirectBSM" : "") +
                            (includeFailingDynamicConstant ? "_FailingCondy" : "");
        }

        Class<?> generateClass() throws ClassNotFoundException {
            TestCL cl = new TestCL(getClass().getClassLoader());
            return cl.findClass(className);
        }

        byte[] generateClassfile() {
            ClassWriter cw = new ClassWriter(ClassWriter.COMPUTE_MAXS | ClassWriter.COMPUTE_FRAMES);
            cw.visit(V16, ACC_SUPER | ACC_PUBLIC, className.replace('.', '/'), null, "java/lang/Object", null);

            // @formatter:off
            // Object ConstantBootstraps.invoke(MethodHandles.Lookup lookup, String name, Class<?> type, MethodHandle handle, Object... args)
            // @formatter:on
            String invokeSig = "(Ljava/lang/invoke/MethodHandles$Lookup;Ljava/lang/String;Ljava/lang/Class;Ljava/lang/invoke/MethodHandle;[Ljava/lang/Object;)Ljava/lang/Object;";
            Handle invokeHandle = new Handle(H_INVOKESTATIC, constantBootstrapsClassInternalName, "invoke", invokeSig, false);

            String desc = type.getDescriptor();
            ConstantDynamic condy;
            if (condyType == CondyType.CALL_DIRECT_BSM) {
                // Example: int TestDynamicConstant.getIntBSM(MethodHandles.Lookup l, String name,
                // Class<?> type)
                String sig = "(Ljava/lang/invoke/MethodHandles$Lookup;Ljava/lang/String;Ljava/lang/Class;)" + desc;
                Handle handle = new Handle(H_INVOKESTATIC, testClassInternalName, getter + "BSM", sig, false);

                condy = new ConstantDynamic("const", desc, handle);
                MethodVisitor run = cw.visitMethod(PUBLIC_STATIC, "run", "()" + desc, null, null);
                run.visitLdcInsn(condy);
                run.visitInsn(type.getOpcode(IRETURN));
                run.visitMaxs(0, 0);
                run.visitEnd();
             } else if (condyType == CondyType.CALL_DIRECT_WITH_ARGS_BSM) {
                // Example: int TestDynamicConstant.getIntBSM(MethodHandles.Lookup l, String name,
                // Class<?> type, int constant, int i)
                String sig1 = "(Ljava/lang/invoke/MethodHandles$Lookup;Ljava/lang/String;Ljava/lang/Class;)" + desc;
                String sig2 = "(Ljava/lang/invoke/MethodHandles$Lookup;Ljava/lang/String;Ljava/lang/Class;" + desc + "I)" + desc;

                Handle handle1 = new Handle(H_INVOKESTATIC, testClassInternalName, getter + "BSM", sig1, false);
                Handle handle2 = new Handle(H_INVOKESTATIC, testClassInternalName, getter + "BSM", sig2, false);

                ConstantDynamic condy1 = new ConstantDynamic("const1", desc, handle1);
                ConstantDynamic condy2 = new ConstantDynamic("const2", desc, handle2, condy1, Integer.MAX_VALUE);

                condy = condy2;
                MethodVisitor run = cw.visitMethod(PUBLIC_STATIC, "run", "()" + desc, null, null);
                run.visitLdcInsn(condy);
                run.visitInsn(type.getOpcode(IRETURN));
                run.visitMaxs(0, 0);
                run.visitEnd();
            } else if (condyType == CondyType.CALL_INDIRECT_BSM) {
                // Example: int TestDynamicConstant.getInt()
                Handle handle = new Handle(H_INVOKESTATIC, testClassInternalName, getter, "()" + desc, false);

                condy = new ConstantDynamic("const", desc, invokeHandle, handle);
                MethodVisitor run = cw.visitMethod(PUBLIC_STATIC, "run", "()" + desc, null, null);
                run.visitLdcInsn(condy);
                run.visitInsn(type.getOpcode(IRETURN));
                run.visitMaxs(0, 0);
                run.visitEnd();
            } else {
                assert condyType == CondyType.CALL_INDIRECT_WITH_ARGS_BSM;
                // Example: int TestDynamicConstant.getInt()
                Handle handle1 = new Handle(H_INVOKESTATIC, testClassInternalName, getter, "()" + desc, false);

                // Example: int TestDynamicConstant.getInt(int v1, int v2)
                Handle handle2 = new Handle(H_INVOKESTATIC, testClassInternalName, getter, "(" + desc + desc + ")" + desc, false);

                ConstantDynamic condy1 = new ConstantDynamic("const1", desc, invokeHandle, handle1);
                ConstantDynamic condy2 = new ConstantDynamic("const2", desc, invokeHandle, handle2, condy1, condy1);
                condy = condy2;

                MethodVisitor run = cw.visitMethod(PUBLIC_STATIC, "run", "()" + desc, null, null);
                run.visitLdcInsn(condy2);
                run.visitInsn(type.getOpcode(IRETURN));
                run.visitMaxs(0, 0);
                run.visitEnd();
            }

            if (includeFailingDynamicConstant) {
                String failingSignature = "(Ljava/lang/invoke/MethodHandles$Lookup;Ljava/lang/String;Ljava/lang/Class;)Ljava/lang/Object;";
                Handle failingHandle = new Handle(H_INVOKESTATIC, testClassInternalName, "throwingBSM", failingSignature, false);
                ConstantDynamic failingCondy = new ConstantDynamic("failed", "Ljava/lang/Object;", failingHandle);
                MethodVisitor failing = cw.visitMethod(PUBLIC_STATIC, "failed", "()Ljava/lang/Object;", null, null);
                failing.visitLdcInsn(failingCondy);
                failing.visitInsn(ARETURN);
                failing.visitMaxs(0, 0);
                failing.visitEnd();
            }

            MethodVisitor concat = cw.visitMethod(PUBLIC_STATIC, "concat", "()Ljava/lang/String;", null, null);
            String sig = "(Ljava/lang/invoke/MethodHandles$Lookup;Ljava/lang/String;Ljava/lang/invoke/MethodType;Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/invoke/CallSite;";
            Handle handle = new Handle(H_INVOKESTATIC, Type.getInternalName(StringConcatFactory.class), "makeConcatWithConstants", sig, false);
            String recipe = "var arg=\u0001, const arg=\u0002";
            Object[] bsmArgs = {recipe, condy};
            concat.visitLdcInsn(condy);
            concat.visitInvokeDynamicInsn("do_concat", "(" + desc + ")Ljava/lang/String;", handle, bsmArgs);
            concat.visitInsn(ARETURN);
            concat.visitMaxs(0, 0);
            concat.visitEnd();

            MethodVisitor shouldNotBeCalled = cw.visitMethod(PUBLIC_STATIC, "shouldNotBeCalled", "()V", null, null);
            sig = "(Ljava/lang/invoke/MethodHandles$Lookup;Ljava/lang/String;Ljava/lang/invoke/MethodType;)Ljava/lang/invoke/CallSite;";
            handle = new Handle(H_INVOKESTATIC, testClassInternalName, "shouldNotBeCalledBSM", sig, false);
            shouldNotBeCalled.visitInvokeDynamicInsn("do_shouldNotBeCalled", "()V", handle);
            shouldNotBeCalled.visitInsn(RETURN);
            shouldNotBeCalled.visitMaxs(0, 0);
            shouldNotBeCalled.visitEnd();

            if (includeNonDirectBootstrapMethod) {
                /*
                 * Resolving CONSTANT_MethodHandles for signature-polymorphic and caller-sensitive
                 * methods can create non-DirectMethodHandle adapters.
                 * These entries are inspection fixtures, not usable bootstrap methods: their
                 * target handles cannot accept the required Lookup, name, and MethodType arguments.
                 * The test inspects the resolved handles and metadata without linking the call sites.
                 */
                String invokeSignature = "(Ljava/lang/invoke/MethodHandles$Lookup;Ljava/lang/String;Ljava/lang/Class;)Ljava/lang/Object;";
                addNonDirectBootstrapMethod(cw, "nonDirectMethodHandleInvoke", H_INVOKEVIRTUAL, "java/lang/invoke/MethodHandle", "invoke", invokeSignature);
                addNonDirectBootstrapMethod(cw, "nonDirectMethodHandleInvokeExact", H_INVOKEVIRTUAL, "java/lang/invoke/MethodHandle", "invokeExact", invokeSignature);
                addNonDirectBootstrapMethod(cw, "nonDirectVarHandleGet", H_INVOKEVIRTUAL, "java/lang/invoke/VarHandle", "get", "()Ljava/lang/Object;");
                addNonDirectBootstrapMethod(cw, "nonDirectThreadGetContextClassLoader", H_INVOKEVIRTUAL, "java/lang/Thread", "getContextClassLoader", "()Ljava/lang/ClassLoader;");
                addNonDirectBootstrapMethod(cw, "nonDirectClassForName", H_INVOKESTATIC, "java/lang/Class", "forName", "(Ljava/lang/String;)Ljava/lang/Class;");
            }

            cw.visitEnd();
            return cw.toByteArray();
        }

        private static void addNonDirectBootstrapMethod(ClassWriter cw, String methodName, int tag, String owner, String name, String descriptor) {
            MethodVisitor method = cw.visitMethod(PUBLIC_STATIC, methodName, "()V", null, null);
            Handle handle = new Handle(tag, owner, name, descriptor, false);
            method.visitInvokeDynamicInsn("nonDirect", "()V", handle);
            method.visitInsn(RETURN);
            method.visitMaxs(0, 0);
            method.visitEnd();
        }

        private final class TestCL extends ClassLoader {
            String saveClassfilesDir = System.getProperty("save.classfiles.dir");

            private TestCL(ClassLoader parent) {
                super(parent);
            }

            @Override
            protected Class<?> findClass(String name) throws ClassNotFoundException {
                if (name.equals(className)) {
                    byte[] classfileBytes = generateClassfile();
                    if (saveClassfilesDir != null) {
                        try {
                            File classfile = new File(saveClassfilesDir, name.replace('.', File.separatorChar) + ".class");
                            File classfileDir = classfile.getParentFile();
                            classfileDir.mkdirs();
                            Files.write(classfile.toPath(), classfileBytes);
                            System.out.println("Wrote: " + classfile.getAbsolutePath());
                        } catch (IOException cause) {
                            Assert.fail("Error saving class file for " + name, cause);
                        }
                    }
                    return defineClass(name, classfileBytes, 0, classfileBytes.length);
                } else {
                    return super.findClass(name);
                }
            }
        }
    }

    private static Class<?> defineUnlinkedDynamicFixture() {
        ClassWriter writer = new ClassWriter(0);
        String name = testClassInternalName + "$UnlinkedDynamicFixture";
        writer.visit(V17, ACC_PUBLIC | ACC_SUPER, name, null, "java/lang/Object", null);
        Handle bootstrapMethod = new Handle(H_INVOKESTATIC, constantBootstrapsClassInternalName, "nullConstant",
                        "(Ljava/lang/invoke/MethodHandles$Lookup;Ljava/lang/String;Ljava/lang/Class;)Ljava/lang/Object;", false);
        MethodVisitor method = writer.visitMethod(PUBLIC_STATIC, "method", "()Ljava/lang/Object;", null, null);
        method.visitLdcInsn(new ConstantDynamic("dynamic", "Ljava/lang/Object;", bootstrapMethod));
        method.visitInsn(POP);
        Handle indyBootstrapMethod = new Handle(H_INVOKESTATIC, Type.getInternalName(StringConcatFactory.class), "makeConcat",
                        "(Ljava/lang/invoke/MethodHandles$Lookup;Ljava/lang/String;Ljava/lang/invoke/MethodType;)Ljava/lang/invoke/CallSite;", false);
        method.visitInvokeDynamicInsn("concat", "()Ljava/lang/String;", indyBootstrapMethod);
        method.visitInsn(ARETURN);
        method.visitMaxs(1, 0);
        method.visitEnd();
        writer.visitEnd();
        byte[] bytes = writer.toByteArray();
        class ByteArrayClassLoader extends ClassLoader {
            ByteArrayClassLoader() {
                super(TestDynamicConstant.class.getClassLoader());
            }

            Class<?> define() {
                return defineClass(null, bytes, 0, bytes.length);
            }
        }
        return new ByteArrayClassLoader().define();
    }

    @Test
    public void testBootstrapInspectionOfUnlinkedHolder() {
        MetaAccessProvider metaAccess = JVMCI.getRuntime().getHostJVMCIBackend().getMetaAccess();
        HotSpotResolvedObjectType holder = (HotSpotResolvedObjectType) metaAccess.lookupJavaType(defineUnlinkedDynamicFixture());
        // Access the constant pool through the HotSpot type: Class.getDeclaredMethod links the
        // holder, which would prevent this test from exercising inspection without a CP cache.
        ConstantPool cp = holder.getConstantPool();
        Assert.assertFalse(holder.isLinked(), "fixture must remain unlinked before inspection");
        int dynamicCpi = -1;
        int indyCpi = -1;
        for (int cpi = 1; cpi < cp.length(); cpi++) {
            if (cp.getTag(cpi) == ConstantPool.Tag.DYNAMIC) {
                dynamicCpi = cpi;
            } else if (cp.getTag(cpi) == ConstantPool.Tag.INVOKEDYNAMIC) {
                indyCpi = cpi;
            }
        }
        Assert.assertTrue(dynamicCpi > 0, "fixture constant pool has no dynamic entry");
        Assert.assertTrue(indyCpi > 0, "fixture constant pool has no invokedynamic entry");

        BootstrapMethodInvocation condy = cp.lookupBootstrapMethodInvocation(dynamicCpi, -1, false);
        Assert.assertNotNull(condy);
        Assert.assertFalse(condy.isInvokeDynamic());
        Assert.assertNull(condy.getMethod(), "unlinked holder must not resolve the bootstrap method");
        Assert.assertEquals(condy.getName(), "dynamic");
        Assert.assertNull(condy.getType(), "non-resolving lookup must not resolve the type");
        Assert.assertEquals(condy.getStaticArgumentIndices().length, 0);
        Assert.assertEquals(cp.getTag(condy.getMethodHandleIndex()), ConstantPool.Tag.METHODHANDLE);
        Assert.assertEquals(cp.lookupName(condy.getMethodHandleIndex(), -1), "nullConstant");

        BootstrapMethodInvocation indy = cp.lookupBootstrapMethodInvocation(indyCpi, -1, false);
        Assert.assertNotNull(indy);
        Assert.assertTrue(indy.isInvokeDynamic());
        Assert.assertNull(indy.getMethod());
        Assert.assertEquals(indy.getName(), "concat");
        Assert.assertNull(indy.getType());
        Assert.assertThrows(IllegalStateException.class, indy::lookup);
        Assert.assertThrows(IllegalStateException.class, indy::resolve);
        Assert.assertFalse(holder.isLinked(), "bootstrap inspection must not link the holder");

        holder.link();
        // A constant pool index does not identify a call site, even after linking.
        BootstrapMethodInvocation linkedIndy = cp.lookupBootstrapMethodInvocation(indyCpi, -1, false);
        Assert.assertNotNull(linkedIndy);
        Assert.assertThrows(IllegalStateException.class, linkedIndy::lookup);
        Assert.assertThrows(IllegalStateException.class, linkedIndy::resolve);
        Assert.assertNull(cp.lookupBootstrapMethodInvocation(dynamicCpi, -1, false).getMethod());
    }

    public static Object throwingBSM(MethodHandles.Lookup lookup, String name, Class<?> type) {
        throw new IllegalStateException("expected bootstrap failure");
    }

    @Test
    public void testFailedDynamicConstantInspection() throws Exception {
        MetaAccessProvider metaAccess = JVMCI.getRuntime().getHostJVMCIBackend().getMetaAccess();
        Class<?> testClass = new TestGenerator(Object.class, CondyType.CALL_DIRECT_BSM, false, true).generateClass();
        ResolvedJavaMethod failed = metaAccess.lookupJavaMethod(testClass.getDeclaredMethod("failed"));
        ConstantPool cp = failed.getConstantPool();
        int cpi = -1;
        for (int i = 1; i < cp.length(); i++) {
            if (cp.getTag(i) == ConstantPool.Tag.DYNAMIC && cp.lookupName(i, -1).equals("failed")) {
                cpi = i;
                break;
            }
        }
        Assert.assertTrue(cpi > 0, "failed dynamic constant not found");

        BootstrapMethodInvocation before = cp.lookupBootstrapMethodInvocation(cpi, -1, false);
        Assert.assertNotNull(before);
        Assert.assertEquals(cp.getTag(before.getMethodHandleIndex()), ConstantPool.Tag.METHODHANDLE);

        final int failedCpi = cpi;
        Assert.assertThrows(BootstrapMethodError.class, () -> cp.lookupConstant(failedCpi, true));
        Assert.assertEquals(cp.getTag(cpi), ConstantPool.Tag.DYNAMIC);
        BootstrapMethodInvocation after = cp.lookupBootstrapMethodInvocation(cpi, -1, false);
        Assert.assertNotNull(after, "failed dynamic constant lost its symbolic bootstrap information");
        Assert.assertEquals(after.getName(), "failed");
    }

    private static Class<?> generateClassWithoutInvokeDynamic(String suffix, boolean includeCondy,
                    boolean includeUnusedIndy) throws IllegalAccessException {
        String className = TestDynamicConstant.class.getName() + "$" + suffix;
        ClassWriter cw = new ClassWriter(ClassWriter.COMPUTE_MAXS | ClassWriter.COMPUTE_FRAMES);
        cw.visit(V17, ACC_PUBLIC | ACC_SUPER, className.replace('.', '/'), null, "java/lang/Object", null);
        Handle nullConstant = new Handle(H_INVOKESTATIC, constantBootstrapsClassInternalName, "nullConstant",
                        "(Ljava/lang/invoke/MethodHandles$Lookup;Ljava/lang/String;Ljava/lang/Class;)Ljava/lang/Object;", false);
        if (includeCondy) {
            MethodVisitor method = cw.visitMethod(ACC_PUBLIC | ACC_STATIC, "condy", "()Ljava/lang/Object;", null, null);
            method.visitLdcInsn(new ConstantDynamic("value", "Ljava/lang/Object;", nullConstant));
            method.visitInsn(ARETURN);
            method.visitMaxs(0, 0);
            method.visitEnd();
        }
        if (includeUnusedIndy) {
            // Add the CP entry and BootstrapMethods record without emitting an invokedynamic
            // instruction.
            cw.newInvokeDynamic("unused", "()V", nullConstant);
        }
        if (!includeCondy) {
            MethodVisitor marker = cw.visitMethod(ACC_PUBLIC | ACC_STATIC, "marker", "()V", null, null);
            marker.visitLdcInsn("plain string constant");
            marker.visitInsn(POP);
            marker.visitLdcInsn(42L);
            marker.visitInsn(POP2);
            marker.visitInsn(RETURN);
            marker.visitMaxs(0, 0);
            marker.visitEnd();
        }
        cw.visitEnd();
        return MethodHandles.lookup().defineHiddenClass(cw.toByteArray(), false).lookupClass();
    }

    @Test
    public void testBootstrapInspectionWithoutIndyCache() throws Exception {
        MetaAccessProvider metaAccess = JVMCI.getRuntime().getHostJVMCIBackend().getMetaAccess();
        // A CONSTANT_Dynamic without an invokedynamic instruction has no resolved indy entry array.
        // Bootstrap inspection must not unconditionally dereference
        // cp->cache()->resolved_indy_entries().
        Class<?> condyOnlyClass = generateClassWithoutInvokeDynamic("CondyOnly", true, false);
        ResolvedJavaMethod condyMethod = metaAccess.lookupJavaMethod(condyOnlyClass.getDeclaredMethod("condy"));
        ConstantPool condyPool = condyMethod.getConstantPool();
        int condyCpi = -1;
        for (int cpi = 1; cpi < condyPool.length(); cpi++) {
            if (condyPool.getTag(cpi) == ConstantPool.Tag.DYNAMIC) {
                condyCpi = cpi;
                break;
            }
        }
        Assert.assertTrue(condyCpi > 0, "condy-only class has no dynamic constant");
        BootstrapMethodInvocation condy = condyPool.lookupBootstrapMethodInvocation(condyCpi, LDC, false);
        Assert.assertNotNull(condy);
        Assert.assertFalse(condy.isInvokeDynamic());
        Assert.assertEquals(condy.getName(), "value");

        Class<?> unusedIndyClass = generateClassWithoutInvokeDynamic("UnusedIndy", false, true);
        ResolvedJavaMethod marker = metaAccess.lookupJavaMethod(unusedIndyClass.getDeclaredMethod("marker"));
        ConstantPool unusedIndyPool = marker.getConstantPool();
        int indyCpi = -1;
        for (int cpi = 1; cpi < unusedIndyPool.length(); cpi++) {
            if (unusedIndyPool.getTag(cpi) == ConstantPool.Tag.INVOKEDYNAMIC) {
                indyCpi = cpi;
                break;
            }
        }
        Assert.assertTrue(indyCpi > 0, "class has no unused invokedynamic entry");
        final int unusedIndyCpi = indyCpi;
        BootstrapMethodInvocation unusedIndy = unusedIndyPool.lookupBootstrapMethodInvocation(unusedIndyCpi, -1, false);
        Assert.assertNotNull(unusedIndy);
        Assert.assertTrue(unusedIndy.isInvokeDynamic());
        Assert.assertEquals(unusedIndy.getName(), "unused");

        byte[] markerCode = marker.getCode();
        int markerLdcOpcode = beU1(markerCode, 0);
        int markerLdcCpi = switch (markerLdcOpcode) {
            case LDC -> beU1(markerCode, 1);
            case LDC_W -> beU2(markerCode, 1);
            default -> throw new AssertionError("Unexpected marker opcode: " + markerLdcOpcode);
        };
        Assert.assertEquals(unusedIndyPool.getTag(markerLdcCpi), ConstantPool.Tag.STRING);
        Assert.assertNull(unusedIndyPool.lookupBootstrapMethodInvocation(markerLdcCpi, markerLdcOpcode, false));
        Assert.assertNull(unusedIndyPool.lookupBootstrapMethodInvocation(markerLdcCpi, LDC_W, false));

        int markerLdc2WCpi = -1;
        for (int cpi = 1; cpi < unusedIndyPool.length(); cpi++) {
            if (unusedIndyPool.getTag(cpi) == ConstantPool.Tag.LONG) {
                markerLdc2WCpi = cpi;
                break;
            }
        }
        Assert.assertTrue(markerLdc2WCpi > 0, "marker method has no long constant");
        Assert.assertNull(unusedIndyPool.lookupBootstrapMethodInvocation(markerLdc2WCpi, LDC2_W, false));
    }

    @Test
    public void testRawInvokeDynamicBootstrapLifecycle() throws Exception {
        MetaAccessProvider metaAccess = JVMCI.getRuntime().getHostJVMCIBackend().getMetaAccess();
        Class<?> testClass = new TestGenerator(String.class, CondyType.CALL_DIRECT_BSM).generateClass();
        ResolvedJavaMethod concat = metaAccess.lookupJavaMethod(testClass.getDeclaredMethod("concat"));
        ConstantPool cp = concat.getConstantPool();
        int cpi = -1;
        for (int i = 1; i < cp.length(); i++) {
            if (cp.getTag(i) == ConstantPool.Tag.INVOKEDYNAMIC && cp.lookupName(i, -1).equals("do_concat")) {
                cpi = i;
                break;
            }
        }
        Assert.assertTrue(cpi > 0, "concatenation invokedynamic entry not found");

        BootstrapMethodInvocation bsmi = cp.lookupBootstrapMethodInvocation(cpi, -1, false);
        Assert.assertNotNull(bsmi);
        Assert.assertTrue(bsmi.isInvokeDynamic());
        Assert.assertThrows(IllegalStateException.class, bsmi::resolve);
        Assert.assertThrows(IllegalStateException.class, bsmi::lookup);
        BootstrapMethodInvocation site = cp.lookupBootstrapMethodInvocation(getFirstInvokedynamicOperand(concat), INVOKEDYNAMIC, false);
        Assert.assertNull(site.lookup());
        BootstrapMethodInvocation rawResolved = cp.lookupBootstrapMethodInvocation(cpi, -1, true);
        Assert.assertEquals(rawResolved.getName(), site.getName());
        Assert.assertNotNull(rawResolved.getMethod());
        MethodType expectedType = MethodType.methodType(String.class, String.class);
        Assert.assertEquals(((HotSpotObjectConstant) rawResolved.getType()).asObject(MethodType.class), expectedType);
        Assert.assertEquals(rawResolved.getStaticArgumentIndices(), site.getStaticArgumentIndices());
        Assert.assertNull(site.lookup(), "resolving bootstrap details must not link the call site");
        Assert.assertThrows(IllegalStateException.class, rawResolved::resolve);
        Assert.assertThrows(IllegalStateException.class, rawResolved::lookup);

        site.resolve();
        Assert.assertNotNull(site.lookup());
        BootstrapMethodInvocation after = cp.lookupBootstrapMethodInvocation(cpi, -1, true);
        Assert.assertEquals(after.getMethod(), rawResolved.getMethod());
        Assert.assertEquals(((HotSpotObjectConstant) after.getType()).asObject(MethodType.class), expectedType);
        Assert.assertEquals(after.getStaticArgumentIndices(), rawResolved.getStaticArgumentIndices());
        Assert.assertThrows(IllegalStateException.class, after::resolve);
        Assert.assertThrows(IllegalStateException.class, after::lookup);
    }

    @Test
    public void testSharedInvokeDynamicConstantPoolEntry() throws Exception {
        ClassWriter writer = new ClassWriter(ClassWriter.COMPUTE_MAXS | ClassWriter.COMPUTE_FRAMES);
        writer.visit(V17, ACC_PUBLIC | ACC_SUPER, testClassInternalName + "$SharedIndy", null, "java/lang/Object", null);
        Handle bootstrapMethod = new Handle(H_INVOKESTATIC, Type.getInternalName(StringConcatFactory.class), "makeConcat",
                        "(Ljava/lang/invoke/MethodHandles$Lookup;Ljava/lang/String;Ljava/lang/invoke/MethodType;)Ljava/lang/invoke/CallSite;", false);
        for (String name : List.of("first", "second")) {
            MethodVisitor method = writer.visitMethod(PUBLIC_STATIC, name, "()Ljava/lang/String;", null, null);
            method.visitLdcInsn("shared");
            // ASM reuses the same constant pool entry for both instructions.
            method.visitInvokeDynamicInsn("concat", "(Ljava/lang/String;)Ljava/lang/String;", bootstrapMethod);
            method.visitInsn(ARETURN);
            method.visitMaxs(0, 0);
            method.visitEnd();
        }
        writer.visitEnd();
        Class<?> testClass = MethodHandles.lookup().defineHiddenClass(writer.toByteArray(), false).lookupClass();
        MetaAccessProvider metaAccess = JVMCI.getRuntime().getHostJVMCIBackend().getMetaAccess();
        ResolvedJavaMethod first = metaAccess.lookupJavaMethod(testClass.getDeclaredMethod("first"));
        ResolvedJavaMethod second = metaAccess.lookupJavaMethod(testClass.getDeclaredMethod("second"));
        ConstantPool cp = first.getConstantPool();
        int firstOperand = getFirstInvokedynamicOperand(first);
        int secondOperand = getFirstInvokedynamicOperand(second);
        Assert.assertNotEquals(firstOperand, secondOperand, "each instruction must have its own rewritten index");
        int cpi = cp.decodeIndex(firstOperand, INVOKEDYNAMIC);
        Assert.assertEquals(cp.decodeIndex(secondOperand, INVOKEDYNAMIC), cpi, "instructions must share a constant pool entry");
        BootstrapMethodInvocation firstSite = cp.lookupBootstrapMethodInvocation(firstOperand, INVOKEDYNAMIC, false);
        BootstrapMethodInvocation secondSite = cp.lookupBootstrapMethodInvocation(secondOperand, INVOKEDYNAMIC, false);
        Assert.assertNull(firstSite.lookup());
        Assert.assertNull(secondSite.lookup());

        assertNoCallSiteForInvokeDynamicEntry(cp, cpi);
        Assert.assertNull(firstSite.lookup(), "bootstrap inspection must not link the first call site");
        Assert.assertNull(secondSite.lookup(), "bootstrap inspection must not link the second call site");

        firstSite.resolve();
        Assert.assertNotNull(firstSite.lookup());
        Assert.assertNull(secondSite.lookup(), "linking the first call site must not link the second");
        assertNoCallSiteForInvokeDynamicEntry(cp, cpi);
        Assert.assertNull(secondSite.lookup(), "bootstrap inspection must not link the second call site");

        secondSite.resolve();
        Assert.assertNotNull(secondSite.lookup());
        Assert.assertNotNull(firstSite.lookup());
        assertNoCallSiteForInvokeDynamicEntry(cp, cpi);
    }

    private static void assertNoCallSiteForInvokeDynamicEntry(ConstantPool cp, int cpi) {
        for (boolean resolve : new boolean[]{false, true}) {
            BootstrapMethodInvocation raw = cp.lookupBootstrapMethodInvocation(cpi, -1, resolve);
            Assert.assertTrue(raw.isInvokeDynamic());
            Assert.assertThrows(IllegalStateException.class, raw::resolve);
            Assert.assertThrows(IllegalStateException.class, raw::lookup);
        }
    }

    /**
     * Asserts that {@link ConstantPool#lookupConstant(int, boolean)} with {@code resolve == false}
     * returns null for all resolvable constant entries.
     */
    private static void assertNoEagerConstantResolution(Class<?> testClass, ConstantPool cp) {
        for (int cpi = 1; cpi < cp.length(); cpi++) {
            ConstantPool.Tag tag = cp.getTag(cpi);
            switch (tag) {
                case METHODHANDLE:
                case METHODTYPE:
                case DYNAMIC: {
                    Object con = cp.lookupConstant(cpi, false);
                    Assert.assertNull(con, "Unexpected eager resolution in " + testClass + " at cpi " + cpi + " (tag: " + tag + ")");
                    break;
                }
            }
        }
    }

    /**
     * Ensures {@link ConstantPool#lookupBootstrapMethodInvocation} does not invoke the associated bootstrap method.
     */
    private static void assertLookupBMIDoesNotInvokeBM(MetaAccessProvider metaAccess, Class<?> testClass) throws Exception {
        ResolvedJavaMethod shouldNotBeCalled = metaAccess.lookupJavaMethod(testClass.getDeclaredMethod("shouldNotBeCalled"));
        ConstantPool cp = shouldNotBeCalled.getConstantPool();
        int rawIndex = getFirstInvokedynamicOperand(shouldNotBeCalled);
        BootstrapMethodInvocation bmi = cp.lookupBootstrapMethodInvocation(rawIndex, INVOKEDYNAMIC);
        Assert.assertEquals(bmi.getName(), "do_shouldNotBeCalled");
        Assert.assertEquals(bmi.getMethod().getName(), "shouldNotBeCalledBSM");
        getBootstrapMethodHandle(bmi, cp);

        // The resolving lookup above caches the bootstrap method handle. The non-resolving form
        // can now safely expose the corresponding direct method, while leaving type and arguments
        // unresolved.
        BootstrapMethodInvocation nonResolvingBmi = cp.lookupBootstrapMethodInvocation(rawIndex, INVOKEDYNAMIC, false);
        Assert.assertEquals(nonResolvingBmi.getMethod().getName(), "shouldNotBeCalledBSM");
        Assert.assertNull(nonResolvingBmi.getType());
        Assert.assertThrows(IllegalStateException.class, nonResolvingBmi::getStaticArguments);
    }

    @SuppressWarnings("try")
    @Test
    public void test() throws Throwable {
        MetaAccessProvider metaAccess = JVMCI.getRuntime().getHostJVMCIBackend().getMetaAccess();
        Class<?>[] types = {
                        boolean.class,
                        byte.class,
                        short.class,
                        char.class,
                        int.class,
                        float.class,
                        long.class,
                        double.class,
                        String.class,
                        Object.class,
                        List.class
        };

        for (Class<?> type : types) {
            for (CondyType condyType : CondyType.values()) {
                TestGenerator e = new TestGenerator(type, condyType);
                Class<?> testClass = e.generateClass();
                Method m = testClass.getDeclaredMethod("run");
                ResolvedJavaMethod run = metaAccess.lookupJavaMethod(m);
                ConstantPool cp = run.getConstantPool();

                assertNoEagerConstantResolution(testClass, cp);
                assertNonResolvingBootstrapMethodInspection(testClass, metaAccess, cp, condyType);
                assertLdcBootstrapMethodInspection(run, cp);
                for (int cpi = 1; cpi < cp.length(); cpi++) {
                    if (cp.getTag(cpi) == ConstantPool.Tag.DYNAMIC) {
                        Assert.assertTrue(cp.lookupName(cpi, -1).startsWith("const"));
                        JavaType dynamicType = cp.lookupReferencedType(cpi, LDC);
                        Assert.assertEquals(dynamicType.getName(), e.type.getDescriptor());
                    }
                }
                assertNoEagerConstantResolution(testClass, cp);
                assertLookupBMIDoesNotInvokeBM(metaAccess, testClass);

                if (type != Object.class) {
                    testLookupBootstrapMethodInvocation(condyType, metaAccess, testClass);
                } else {
                    // StringConcatFactoryStringConcatFactory cannot accept null constants
                }

                Object lastConstant = null;
                for (int cpi = 1; cpi < cp.length(); cpi++) {
                    if (cp.getTag(cpi) == ConstantPool.Tag.DYNAMIC) {
                        lastConstant = cp.lookupConstant(cpi);
                    }
                }
                Assert.assertTrue(lastConstant != null, "No Dynamic entries in constant pool of " + testClass.getName());

                // Execute code to resolve condy by execution and compare
                // with condy resolved via ConstantPool
                Object expect = m.invoke(null);
                Object actual;
                if (lastConstant == PrimitiveConstant.NULL_POINTER) {
                    actual = null;
                } else if (lastConstant instanceof PrimitiveConstant) {
                    actual = ((PrimitiveConstant) lastConstant).asBoxedPrimitive();
                } else {
                    actual = ((HotSpotObjectConstant) lastConstant).asObject(type);
                }
                Assert.assertEquals(actual, expect, m + ":");
            }
        }
    }

    /**
     * Checks that the non-resolving form exposes the BootstrapMethods structure without causing
     * method handles, method types, or dynamic constants to be resolved.
     */
    private static void assertNonResolvingBootstrapMethodInspection(Class<?> testClass, MetaAccessProvider metaAccess, ConstantPool cp, CondyType condyType) throws Exception {
        for (int cpi = 1; cpi < cp.length(); cpi++) {
            ConstantPool.Tag tag = cp.getTag(cpi);
            BootstrapMethodInvocation bsmi = cp.lookupBootstrapMethodInvocation(cpi, -1, false);
            if (tag != ConstantPool.Tag.INVOKEDYNAMIC && tag != ConstantPool.Tag.DYNAMIC) {
                Assert.assertNull(bsmi, testClass + ": " + cpi);
                continue;
            }
            Assert.assertNotNull(bsmi);
            Assert.assertEquals(bsmi.isInvokeDynamic(), tag == ConstantPool.Tag.INVOKEDYNAMIC);
            Assert.assertNotNull(bsmi.getName());
            Assert.assertEquals(cp.getTag(bsmi.getMethodHandleIndex()), ConstantPool.Tag.METHODHANDLE);
            Assert.assertNull(bsmi.getMethod());
            Assert.assertNull(bsmi.getType());
            Assert.assertThrows(IllegalStateException.class, bsmi::getStaticArguments);

            int[] indices = bsmi.getStaticArgumentIndices();
            assertStaticArgumentIndices(bsmi, cp, condyType, indices);
            int[] copy = bsmi.getStaticArgumentIndices();
            if (copy.length != 0) {
                copy[0] = -1;
                Assert.assertNotEquals(bsmi.getStaticArgumentIndices()[0], -1);
            }
        }

        ResolvedJavaMethod concat = metaAccess.lookupJavaMethod(testClass.getDeclaredMethod("concat"));
        int rawIndex = getFirstInvokedynamicOperand(concat);
        BootstrapMethodInvocation raw = cp.lookupBootstrapMethodInvocation(rawIndex, INVOKEDYNAMIC, false);
        Assert.assertTrue(raw.isInvokeDynamic());
        Assert.assertEquals(cp.lookupName(rawIndex, INVOKEDYNAMIC), raw.getName());
        Assert.assertNotNull(cp.lookupSignature(rawIndex, INVOKEDYNAMIC));
        assertNoEagerConstantResolution(testClass, cp);
    }

    /**
     * Verifies the symbolic static-argument layout generated for each bootstrap method.
     */
    private static void assertStaticArgumentIndices(BootstrapMethodInvocation bsmi, ConstantPool cp, CondyType condyType, int[] indices) {
        ConstantPool.Tag[] expectedTags;
        if (bsmi.isInvokeDynamic()) {
            expectedTags = switch (bsmi.getName()) {
                case "do_concat" -> new ConstantPool.Tag[]{ConstantPool.Tag.STRING, ConstantPool.Tag.DYNAMIC};
                case "do_shouldNotBeCalled" -> new ConstantPool.Tag[0];
                default -> throw new AssertionError("Unexpected invokedynamic: " + bsmi.getName());
            };
        } else {
            expectedTags = switch (condyType) {
                case CALL_DIRECT_BSM -> new ConstantPool.Tag[0];
                case CALL_DIRECT_WITH_ARGS_BSM -> switch (bsmi.getName()) {
                    case "const1" -> new ConstantPool.Tag[0];
                    case "const2" -> new ConstantPool.Tag[]{ConstantPool.Tag.DYNAMIC, ConstantPool.Tag.INTEGER};
                    default -> throw new AssertionError("Unexpected dynamic constant: " + bsmi.getName());
                };
                case CALL_INDIRECT_BSM -> new ConstantPool.Tag[]{ConstantPool.Tag.METHODHANDLE};
                case CALL_INDIRECT_WITH_ARGS_BSM -> switch (bsmi.getName()) {
                    case "const1" -> new ConstantPool.Tag[]{ConstantPool.Tag.METHODHANDLE};
                    case "const2" -> new ConstantPool.Tag[]{ConstantPool.Tag.METHODHANDLE, ConstantPool.Tag.DYNAMIC, ConstantPool.Tag.DYNAMIC};
                    default -> throw new AssertionError("Unexpected dynamic constant: " + bsmi.getName());
                };
            };
        }
        Assert.assertEquals(indices.length, expectedTags.length, bsmi.toString());
        for (int i = 0; i < indices.length; i++) {
            Assert.assertEquals(cp.getTag(indices[i]), expectedTags[i], bsmi.toString());
        }
    }

    /**
     * Checks that bootstrap method lookup accepts the bytecodes that load dynamic constants.
     */
    private static void assertLdcBootstrapMethodInspection(ResolvedJavaMethod method, ConstantPool cp) {
        byte[] code = method.getCode();
        int opcode = beU1(code, 0);
        int cpi = switch (opcode) {
            case LDC -> beU1(code, 1);
            case LDC_W, LDC2_W -> beU2(code, 1);
            default -> throw new AssertionError("Unexpected constant-loading opcode: " + opcode);
        };
        BootstrapMethodInvocation bsmi = cp.lookupBootstrapMethodInvocation(cpi, opcode, false);
        Assert.assertNotNull(bsmi);
        Assert.assertFalse(bsmi.isInvokeDynamic());

        // A small constant-pool index is also a valid operand of LDC_W. Exercise that form even
        // when ASM selected the more compact LDC encoding.
        if (opcode == LDC) {
            BootstrapMethodInvocation wideBsmi = cp.lookupBootstrapMethodInvocation(cpi, LDC_W, false);
            Assert.assertEquals(wideBsmi.getName(), bsmi.getName());
            Assert.assertEquals(wideBsmi.getStaticArgumentIndices(), bsmi.getStaticArgumentIndices());
        }
    }

    /**
     * Tests inspection of non-direct bootstrap method handles. These generated entries are not
     * usable bootstrap methods: their target handles cannot accept the required
     * {@code (Lookup, String, MethodType)} arguments. The test does not call
     * {@link BootstrapMethodInvocation#resolve()}, which would try to invoke them.
     */
    @Test
    public void testNonDirectBootstrapMethod() throws Exception {
        MetaAccessProvider metaAccess = JVMCI.getRuntime().getHostJVMCIBackend().getMetaAccess();
        Class<?> testClass = new TestGenerator(int.class, CondyType.CALL_DIRECT_BSM, true).generateClass();
        Class<?> directMethodHandleClass = Class.forName("java.lang.invoke.DirectMethodHandle");
        List<String> methodNames = List.of(
                        "nonDirectMethodHandleInvoke",
                        "nonDirectMethodHandleInvokeExact",
                        "nonDirectVarHandleGet",
                        "nonDirectThreadGetContextClassLoader",
                        "nonDirectClassForName");
        List<String> referencedNames = List.of("invoke", "invokeExact", "get", "getContextClassLoader", "forName");
        for (int i = 0; i < methodNames.size(); i++) {
            String methodName = methodNames.get(i);
            ResolvedJavaMethod method = metaAccess.lookupJavaMethod(testClass.getDeclaredMethod(methodName));
            ConstantPool cp = method.getConstantPool();

            BootstrapMethodInvocation bsmi = cp.lookupBootstrapMethodInvocation(getFirstInvokedynamicOperand(method), INVOKEDYNAMIC);
            Assert.assertEquals(bsmi.getName(), "nonDirect");
            Assert.assertEquals(cp.lookupName(bsmi.getMethodHandleIndex(), -1), referencedNames.get(i));
            if (i < 3) {
                // Unlike a linked signature-polymorphic bytecode invocation, whose cached adapter
                // lookupMethod can return, this starts from the BSM's symbolic MethodHandle entry.
                // That lookup has no linked invocation and appendix to identify the adapter. The
                // MethodHandle constant itself is still resolved.
                Assert.assertNull(bsmi.getMethod());
                int methodHandleCpi = bsmi.getMethodHandleIndex();
                Assert.assertTrue(cp.lookupConstant(methodHandleCpi, false) instanceof HotSpotObjectConstant,
                                "bootstrap method handle was not resolved");
                JavaMethod symbolicMethod = cp.lookupMethod(methodHandleCpi, -1, method);
                Assert.assertTrue(symbolicMethod instanceof UnresolvedJavaMethod, String.valueOf(symbolicMethod));
                Assert.assertEquals(symbolicMethod.getName(), referencedNames.get(i));
                String expectedHolder = i < 2 ? "Ljava/lang/invoke/MethodHandle;" : "Ljava/lang/invoke/VarHandle;";
                Assert.assertEquals(symbolicMethod.getDeclaringClass().getName(), expectedHolder);
                Assert.assertEquals(symbolicMethod.getSignature().toMethodDescriptor(), cp.lookupSignature(methodHandleCpi).toMethodDescriptor());
            } else {
                Assert.assertNotNull(bsmi.getMethod());
                Assert.assertEquals(bsmi.getMethod().getName(), referencedNames.get(i));
            }
            MethodHandle methodHandle = getBootstrapMethodHandle(bsmi, cp);
            Assert.assertFalse(directMethodHandleClass.isInstance(methodHandle), methodName + ": " + methodHandle.getClass().getName());
        }
    }

    /**
     * Resolves the bootstrap method handle referenced by {@code bsmi} and verifies that its index
     * identifies a {@code CONSTANT_MethodHandle_info} entry.
     */
    private static MethodHandle getBootstrapMethodHandle(BootstrapMethodInvocation bsmi, ConstantPool cp) {
        int methodHandleIndex = bsmi.getMethodHandleIndex();
        Assert.assertEquals(cp.getTag(methodHandleIndex), ConstantPool.Tag.METHODHANDLE);
        Object methodHandleConstant = cp.lookupConstant(methodHandleIndex);
        Assert.assertTrue(methodHandleConstant instanceof HotSpotObjectConstant, String.valueOf(methodHandleConstant));
        return ((HotSpotObjectConstant) methodHandleConstant).asObject(MethodHandle.class);
    }

    /**
     * Tests {@link jdk.vm.ci.meta.ConstantPool#lookupBootstrapMethodInvocation}.
     */
    private void testLookupBootstrapMethodInvocation(CondyType condyType, MetaAccessProvider metaAccess, Class<?> testClass) throws Throwable {
        Method m = testClass.getDeclaredMethod("concat");
        ResolvedJavaMethod concat = metaAccess.lookupJavaMethod(m);
        ConstantPool cp = concat.getConstantPool();

        // Contains a map of (bootstrap method names, resolvable) values.
        Map<String, Boolean> expectedIndyBSMs = Map.of(
            "jdk.vm.ci.hotspot.test.TestDynamicConstant.shouldNotBeCalledBSM", false,
            "java.lang.invoke.StringConcatFactory.makeConcatWithConstants", true
        );

        for (int cpi = 1; cpi < cp.length(); cpi++) {
            ConstantPool.Tag tag = cp.getTag(cpi);
            BootstrapMethodInvocation bsmi = cp.lookupBootstrapMethodInvocation(cpi, -1);
            if (tag == ConstantPool.Tag.INVOKEDYNAMIC || tag == ConstantPool.Tag.DYNAMIC) {
                Assert.assertNotNull(bsmi);
                MethodHandle methodHandle = getBootstrapMethodHandle(bsmi, cp);
                String bsm = bsmi.getMethod().format("%H.%n");
                if (bsmi.getMethod().isVarArgs()) {
                    Assert.assertTrue(methodHandle.isVarargsCollector(), bsm);
                }
                if (tag == ConstantPool.Tag.INVOKEDYNAMIC) {
                    Assert.assertTrue(bsmi.isInvokeDynamic());
                    Assert.assertTrue(expectedIndyBSMs.containsKey(bsm), expectedIndyBSMs.toString());
                } else {
                    Assert.assertFalse(bsmi.isInvokeDynamic());
                    Assert.assertNull(bsmi.lookup());
                    checkBsmName(condyType, bsm);
                    if (condyType == CondyType.CALL_INDIRECT_BSM || condyType == CondyType.CALL_INDIRECT_WITH_ARGS_BSM) {
                        Assert.assertTrue(bsmi.getMethod().isVarArgs(), bsm);
                    }
                    List<JavaConstant> staticArguments = bsmi.getStaticArguments();
                    for (int i = 0; i < staticArguments.size(); ++i) {
                        JavaConstant constant = staticArguments.get(i);
                        if (constant instanceof PrimitiveConstant) {
                            ConstantPool.Tag innerTag = cp.getTag(constant.asInt());
                            if (condyType == CondyType.CALL_DIRECT_WITH_ARGS_BSM) {
                                Assert.assertEquals(i, 0);
                                Assert.assertEquals(innerTag, ConstantPool.Tag.DYNAMIC);
                            }
                            if (innerTag == ConstantPool.Tag.DYNAMIC) {
                                BootstrapMethodInvocation innerBsmi = cp.lookupBootstrapMethodInvocation(constant.asInt(), -1);
                                String innerBsm = innerBsmi.getMethod().format("%H.%n");
                                checkBsmName(condyType, innerBsm);
                                MethodHandle innerMethodHandle = getBootstrapMethodHandle(innerBsmi, cp);
                                if (condyType == CondyType.CALL_INDIRECT_BSM || condyType == CondyType.CALL_INDIRECT_WITH_ARGS_BSM) {
                                    Assert.assertTrue(innerBsmi.getMethod().isVarArgs(), innerBsm);
                                    Assert.assertTrue(innerMethodHandle.isVarargsCollector(), innerBsm);
                                }
                            } else {
                                Assert.assertEquals(innerTag, ConstantPool.Tag.METHODHANDLE);
                            }
                        } else {
                            if (condyType == CondyType.CALL_DIRECT_WITH_ARGS_BSM) {
                                Assert.assertEquals(i, 1);
                            }
                            Assert.assertTrue(staticArguments.get(i) instanceof HotSpotObjectConstant);
                        }
                    }
                }
            } else {
                Assert.assertNull(bsmi, String.valueOf(bsmi));
            }
        }

        testLoadReferencedType(concat, cp);

        testLookupBootstrapMethodInvocations(condyType, cp, expectedIndyBSMs);
    }

    private static void testLookupBootstrapMethodInvocations(CondyType condyType, ConstantPool cp, Map<String, Boolean> expectedIndyBSMs) {
        List<BootstrapMethodInvocation> indyBSMs = cp.lookupBootstrapMethodInvocations(true);
        Assert.assertEquals(indyBSMs.size(), 2);
        for (var bsmi : indyBSMs) {
            String bsm = bsmi.getMethod().format("%H.%n");
            Assert.assertTrue(expectedIndyBSMs.containsKey(bsm), expectedIndyBSMs.toString());
            Assert.assertTrue(bsmi.isInvokeDynamic());
            if (expectedIndyBSMs.get(bsm)) {
                bsmi.resolve();
                Assert.assertNotNull(bsmi.lookup());
            } else {
                try {
                    bsmi.resolve();
                } catch (BootstrapMethodError bme) {
                    // expected error
                }
                Assert.assertNull(bsmi.lookup());
            }
        }

        List<BootstrapMethodInvocation> condyBSMs = cp.lookupBootstrapMethodInvocations(false);
        int expectedNumCondys = switch(condyType) {
            case CALL_DIRECT_BSM, CALL_INDIRECT_BSM -> 1;
            case CALL_DIRECT_WITH_ARGS_BSM, CALL_INDIRECT_WITH_ARGS_BSM -> 2;
        };
        Assert.assertEquals(condyBSMs.size(), expectedNumCondys);
        for (var bsmi : condyBSMs) {
            Assert.assertTrue(!bsmi.isInvokeDynamic());
            if (condyType == CondyType.CALL_INDIRECT_BSM || condyType == CondyType.CALL_INDIRECT_WITH_ARGS_BSM) {
                Assert.assertTrue(bsmi.getMethod().isVarArgs(), bsmi.getMethod().format("%H.%n"));
            }
            bsmi.resolve();
            Assert.assertNotNull(bsmi.lookup());
        }
    }

    private static void checkBsmName(CondyType condyType, String bsm) {
        if (condyType == CondyType.CALL_DIRECT_BSM || condyType == CondyType.CALL_DIRECT_WITH_ARGS_BSM) {
            Assert.assertTrue(bsm.startsWith("jdk.vm.ci.hotspot.test.TestDynamicConstant.get") && bsm.endsWith("BSM"), bsm);
        } else {
            Assert.assertEquals(bsm, "java.lang.invoke.ConstantBootstraps.invoke");
        }
    }

    private static int beS4(byte[] data, int bci) {
        return (data[bci] << 24) | ((data[bci + 1] & 0xff) << 16) | ((data[bci + 2] & 0xff) << 8) | (data[bci + 3] & 0xff);
    }

    private static int beU1(byte[] data, int bci) {
        return data[bci] & 0xff;
    }
    private static int beU2(byte[] data, int bci) {
        return (beU1(data, bci) << 8) | beU1(data, bci + 1);
    }
    private static final int LDC_W = 19;
    private static final int LDC2_W = 20;

    /**
     * Gets the operand of the first invokedynamic in {@code method}. This
     * assumes that the bytecode of {@code method} is an INVOKEDYNAMIC instruction,
     * possibly preceded by an LDC instruction.
     */
    private static int getFirstInvokedynamicOperand(ResolvedJavaMethod method) {
        byte[] code = method.getCode();
        int opcode = beU1(code, 0);
        if (opcode == INVOKEDYNAMIC) {
            return beS4(code, 1);
        }
        Assert.assertTrue(opcode == LDC || opcode == LDC2_W, String.valueOf(opcode));
        int bci = opcode == LDC ? 2 : 3;
        Assert.assertEquals(beU1(code, bci), INVOKEDYNAMIC);
        return beS4(code, bci + 1);
    }

    /**
     * Ensures that loadReferencedType for an invokedynamic call site does not throw an exception.
     */
    private static void testLoadReferencedType(ResolvedJavaMethod method, ConstantPool cp) {
        int rawIndex = getFirstInvokedynamicOperand(method);
        cp.loadReferencedType(rawIndex, INVOKEDYNAMIC, false);
    }

    // @formatter:off
    @SuppressWarnings("unused") public static boolean getBooleanBSM(MethodHandles.Lookup l, String name, Class<?> type) { return true; }
    @SuppressWarnings("unused") public static char    getCharBSM   (MethodHandles.Lookup l, String name, Class<?> type) { return '*'; }
    @SuppressWarnings("unused") public static short   getShortBSM  (MethodHandles.Lookup l, String name, Class<?> type) { return Short.MAX_VALUE; }
    @SuppressWarnings("unused") public static byte    getByteBSM   (MethodHandles.Lookup l, String name, Class<?> type) { return Byte.MAX_VALUE; }
    @SuppressWarnings("unused") public static int     getIntBSM    (MethodHandles.Lookup l, String name, Class<?> type) { return Integer.MAX_VALUE; }
    @SuppressWarnings("unused") public static float   getFloatBSM  (MethodHandles.Lookup l, String name, Class<?> type) { return Float.MAX_VALUE; }
    @SuppressWarnings("unused") public static long    getLongBSM   (MethodHandles.Lookup l, String name, Class<?> type) { return Long.MAX_VALUE; }
    @SuppressWarnings("unused") public static double  getDoubleBSM (MethodHandles.Lookup l, String name, Class<?> type) { return Double.MAX_VALUE; }
    @SuppressWarnings("unused") public static String  getStringBSM (MethodHandles.Lookup l, String name, Class<?> type) { return "a string"; }
    @SuppressWarnings("unused") public static Object  getObjectBSM (MethodHandles.Lookup l, String name, Class<?> type) { return null; }
    @SuppressWarnings("unused") public static List<?> getListBSM   (MethodHandles.Lookup l, String name, Class<?> type) { return List.of("element"); }

    @SuppressWarnings("unused") public static boolean getBooleanBSM(MethodHandles.Lookup l, String name, Class<?> type, boolean constant, int i) { return true; }
    @SuppressWarnings("unused") public static char    getCharBSM   (MethodHandles.Lookup l, String name, Class<?> type, char constant, int i) { return '*'; }
    @SuppressWarnings("unused") public static short   getShortBSM  (MethodHandles.Lookup l, String name, Class<?> type, short constant, int i) { return Short.MAX_VALUE; }
    @SuppressWarnings("unused") public static byte    getByteBSM   (MethodHandles.Lookup l, String name, Class<?> type, byte constant, int i) { return Byte.MAX_VALUE; }
    @SuppressWarnings("unused") public static int     getIntBSM    (MethodHandles.Lookup l, String name, Class<?> type, int constant, int i) { return Integer.MAX_VALUE; }
    @SuppressWarnings("unused") public static float   getFloatBSM  (MethodHandles.Lookup l, String name, Class<?> type, float constant, int i) { return Float.MAX_VALUE; }
    @SuppressWarnings("unused") public static long    getLongBSM   (MethodHandles.Lookup l, String name, Class<?> type, long constant, int i) { return Long.MAX_VALUE; }
    @SuppressWarnings("unused") public static double  getDoubleBSM (MethodHandles.Lookup l, String name, Class<?> type, double constant, int i) { return Double.MAX_VALUE; }
    @SuppressWarnings("unused") public static String  getStringBSM (MethodHandles.Lookup l, String name, Class<?> type, String constant, int i) { return "a string"; }
    @SuppressWarnings("unused") public static Object  getObjectBSM (MethodHandles.Lookup l, String name, Class<?> type, Object constant, int i) { return null; }
    @SuppressWarnings("unused") public static List<?> getListBSM   (MethodHandles.Lookup l, String name, Class<?> type, List<?> constant, int i) { return List.of("element"); }


    public static boolean getBoolean() { return true; }
    public static char    getChar   () { return '*'; }
    public static short   getShort  () { return Short.MAX_VALUE; }
    public static byte    getByte   () { return Byte.MAX_VALUE; }
    public static int     getInt    () { return Integer.MAX_VALUE; }
    public static float   getFloat  () { return Float.MAX_VALUE; }
    public static long    getLong   () { return Long.MAX_VALUE; }
    public static double  getDouble () { return Double.MAX_VALUE; }
    public static String  getString () { return "a string"; }
    public static Object  getObject () { return null; }
    public static List<?> getList   () { return List.of("element"); }

    public static boolean getBoolean(boolean v1, boolean v2) { return v1 || v2; }
    public static char    getChar   (char v1, char v2)       { return (char)(v1 ^ v2); }
    public static short   getShort  (short v1, short v2)     { return (short)(v1 ^ v2); }
    public static byte    getByte   (byte v1,   byte v2)     { return (byte)(v1 ^ v2); }
    public static int     getInt    (int v1, int v2)         { return v1 ^ v2; }
    public static float   getFloat  (float v1, float v2)     { return v1 * v2; }
    public static long    getLong   (long v1, long v2)       { return v1 ^ v2; }
    public static double  getDouble (double v1, double v2)   { return v1 * v2; }
    public static String  getString (String v1, String v2)   { return v1 + v2; }
    public static Object  getObject (Object v1, Object v2)   { return null; }
    public static List<?> getList   (List<?> v1, List<?> v2) { return List.of(v1, v2); }
    // @formatter:on

    // A bootstrap method that should never be called
    public static CallSite shouldNotBeCalledBSM(MethodHandles.Lookup caller, String name, MethodType type) throws Exception {
        throw new RuntimeException("should not be called");
    }
}
