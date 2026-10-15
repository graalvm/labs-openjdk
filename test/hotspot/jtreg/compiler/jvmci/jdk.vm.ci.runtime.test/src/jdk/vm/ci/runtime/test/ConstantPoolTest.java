/*
 * Copyright (c) 2020, 2026, Oracle and/or its affiliates. All rights reserved.
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
 * @modules jdk.internal.vm.ci/jdk.vm.ci.hotspot:open
 *          jdk.internal.vm.ci/jdk.vm.ci.runtime
 *          jdk.internal.vm.ci/jdk.vm.ci.meta
 *          jdk.internal.vm.ci/jdk.vm.ci.code
 *          jdk.internal.vm.ci/jdk.vm.ci.common
 * @library /compiler/jvmci/jdk.vm.ci.hotspot.test/src
 *          /compiler/jvmci/jdk.vm.ci.code.test/src
 *          /testlibrary/asm
 * @run testng/othervm
 *      -XX:+UnlockExperimentalVMOptions -XX:+EnableJVMCI -XX:-UseJVMCICompiler
 *      jdk.vm.ci.runtime.test.ConstantPoolTest
 */
package jdk.vm.ci.runtime.test;

import java.io.IOException;
import java.io.InputStream;
import java.lang.classfile.ClassFile;
import java.lang.classfile.constantpool.PoolEntry;
import java.lang.invoke.MethodHandle;
import java.lang.invoke.MethodHandleInfo;
import java.lang.invoke.MethodHandles;
import java.lang.invoke.MethodHandles.Lookup;
import java.lang.invoke.MethodType;

import org.testng.Assert;
import org.testng.annotations.Test;
import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.Handle;
import org.objectweb.asm.MethodVisitor;
import org.objectweb.asm.Opcodes;

import jdk.vm.ci.meta.ConstantPool;
import jdk.vm.ci.meta.ConstantPool.Tag;
import jdk.vm.ci.meta.JavaConstant;
import jdk.vm.ci.meta.JavaField;
import jdk.vm.ci.meta.JavaMethod;
import jdk.vm.ci.meta.MetaAccessProvider;
import jdk.vm.ci.meta.ResolvedJavaField;
import jdk.vm.ci.meta.ResolvedJavaMethod;
import jdk.vm.ci.meta.ResolvedJavaType;
import jdk.vm.ci.meta.Signature;
import jdk.vm.ci.meta.UnresolvedJavaField;
import jdk.vm.ci.meta.UnresolvedJavaMethod;
import jdk.vm.ci.meta.UnresolvedJavaType;
import jdk.vm.ci.runtime.JVMCI;

public class ConstantPoolTest {

    static Object cloneByteArray(byte[] arr) {
        return arr.clone();
    }

    static Object cloneCharArray(char[] arr) {
        return arr.clone();
    }

    static Object cloneShortArray(short[] arr) {
        return arr.clone();
    }

    static Object cloneIntArray(int[] arr) {
        return arr.clone();
    }

    static Object cloneFloatArray(float[] arr) {
        return arr.clone();
    }

    static Object cloneLongArray(long[] arr) {
        return arr.clone();
    }

    static Object cloneDoubleArray(double[] arr) {
        return arr.clone();
    }

    static Object cloneObjectArray(Object[] arr) {
        return arr.clone();
    }

    public static final int ICONST_0 = 3;
    public static final int LCONST_0 = 9;
    public static final int ALOAD_0 = 42;
    public static final int ALOAD_1 = 43;
    public static final int IRETURN = 172;
    public static final int RETURN = 177;
    public static final int LDC = 18;
    public static final int LDC_W = 19;
    public static final int LDC2_W = 20;
    public static final int GETSTATIC = 178;
    public static final int INVOKEVIRTUAL = 182;
    public static final int INVOKEDYNAMIC = 186;
    private static final float TAG_TEST_FLOAT = 123.5f;
    private static final long TAG_TEST_LONG = 0x1234_5678_9abc_def0L;
    private static final double TAG_TEST_DOUBLE = 12345.6789d;

    public static int beU2(byte[] data, int bci) {
        return ((data[bci] & 0xff) << 8) | (data[bci + 1] & 0xff);
    }

    public static int beU1(byte[] data, int bci) {
        return data[bci] & 0xff;
    }

    public static int beS4(byte[] data, int bci) {
        return (data[bci] << 24) | ((data[bci + 1] & 0xff) << 16) | ((data[bci + 2] & 0xff) << 8) | (data[bci + 3] & 0xff);
    }

    @Test
    public void constantPoolTagFromValueTest() {
        for (Tag tag : Tag.values()) {
            Assert.assertSame(Tag.fromValue(tag.getValue()), tag, "tag does not round trip: " + tag);
        }
        for (int value : new int[]{2, 13, 14, 21}) {
            final int invalidValue = value;
            Assert.assertThrows(IllegalArgumentException.class, () -> Tag.fromValue(invalidValue));
        }
    }

    @Test
    public void constantPoolGetTagTest() {
        MetaAccessProvider metaAccess = JVMCI.getRuntime().getHostJVMCIBackend().getMetaAccess();
        ResolvedJavaType type = metaAccess.lookupJavaType(ConstantPoolTest.class);
        ConstantPool constantPool = type.getDeclaredMethods()[0].getConstantPool();
        boolean sawLong = false;
        boolean sawDouble = false;

        for (int cpi = 1; cpi < constantPool.length(); cpi++) {
            Tag tag = constantPool.getTag(cpi);
            Assert.assertNotNull(tag, "null tag at constant pool index " + cpi);
            if (tag == Tag.INVALID) {
                Assert.assertTrue(cpi > 1, "invalid tag at the first constant pool index");
                Tag previousTag = constantPool.getTag(cpi - 1);
                Assert.assertTrue(previousTag == Tag.LONG || previousTag == Tag.DOUBLE,
                                "invalid tag at constant pool index " + cpi + " does not follow a long or double");
            } else if (tag == Tag.LONG || tag == Tag.DOUBLE) {
                Assert.assertSame(constantPool.getTag(cpi + 1), Tag.INVALID,
                                "long or double at constant pool index " + cpi + " is not followed by INVALID");
                sawLong |= tag == Tag.LONG;
                sawDouble |= tag == Tag.DOUBLE;
            }
        }

        Assert.assertTrue(sawLong, "constant pool does not contain the long test constant");
        Assert.assertTrue(sawDouble, "constant pool does not contain the double test constant");
    }

    @Test
    public void constantPoolGetTagConsistencyTest() throws IOException {
        MetaAccessProvider metaAccess = JVMCI.getRuntime().getHostJVMCIBackend().getMetaAccess();
        ResolvedJavaType type = metaAccess.lookupJavaType(ConstantPoolTest.class);
        ConstantPool constantPool = type.getDeclaredMethods()[0].getConstantPool();
        InputStream stream = ConstantPoolTest.class.getResourceAsStream("ConstantPoolTest.class");
        Assert.assertNotNull(stream, "class file not found");

        java.lang.classfile.constantpool.ConstantPool classFileConstantPool;
        try (stream) {
            classFileConstantPool = ClassFile.of().parse(stream.readAllBytes()).constantPool();
        }

        Assert.assertEquals(constantPool.length(), classFileConstantPool.size(), "wrong constant pool length");
        for (PoolEntry entry : classFileConstantPool) {
            Tag expectedTag = Tag.fromValue(entry.tag());
            Assert.assertSame(constantPool.getTag(entry.index()), expectedTag,
                            "wrong tag at constant pool index " + entry.index());
        }
    }

    @Test
    public void lookupArrayCloneMethodTest() throws Exception {
        MetaAccessProvider metaAccess = JVMCI.getRuntime().getHostJVMCIBackend().getMetaAccess();
        ResolvedJavaType type = metaAccess.lookupJavaType(ConstantPoolTest.class);
        for (ResolvedJavaMethod m : type.getDeclaredMethods()) {
            if (m.getName().startsWith("clone")) {
                byte[] bytecode = m.getCode();
                Assert.assertNotNull(bytecode, m.toString());
                Assert.assertEquals(5, bytecode.length, m.toString());
                Assert.assertEquals(ALOAD_0, beU1(bytecode, 0), m.toString());
                Assert.assertEquals(INVOKEVIRTUAL, beU1(bytecode, 1), m.toString());
                int cpi = beU2(bytecode, 2);
                JavaMethod callee = m.getConstantPool().lookupMethod(cpi, INVOKEVIRTUAL);
                Assert.assertTrue(callee instanceof ResolvedJavaMethod, callee.toString());
            }
        }
    }

    static int someStaticField = 1;
    static int getStaticField() {
        return someStaticField;
    }

    @Test
    public void lookupFieldTest() throws Exception {
        MetaAccessProvider metaAccess = JVMCI.getRuntime().getHostJVMCIBackend().getMetaAccess();
        ResolvedJavaType type = metaAccess.lookupJavaType(ConstantPoolTest.class);

        String methodName = "getStaticField";
        Signature methodSig = metaAccess.parseMethodDescriptor("()I");
        ResolvedJavaMethod m = type.findMethod(methodName, methodSig);
        Assert.assertNotNull(m);

        // Expected:
        // 0: getstatic "someStaticField":"I";
        // 3: ireturn;
        byte[] bytecode = m.getCode();
        Assert.assertNotNull(bytecode);
        Assert.assertEquals(4, bytecode.length);
        Assert.assertEquals(GETSTATIC, beU1(bytecode, 0));
        int rawIndex = beU2(bytecode, 1);
        JavaField field =  m.getConstantPool().lookupField(rawIndex, m, GETSTATIC);
        Assert.assertEquals("someStaticField", field.getName(), "Wrong field name; rawIndex = " + rawIndex + ";");
        Assert.assertEquals(m.getConstantPool().lookupName(rawIndex, GETSTATIC), "someStaticField");
    }

    static String concatString1(String a, String b) {
        return a + b;
    }

    static String concatString2(String a, String b) {
        return a + b;
    }

    // We never call this method, so its indy is never resolved.
    static String concatString3_never_call(String a, String b) {
        return a + b;
    }

    static void invokeHandle(MethodHandle mh) throws Throwable  {
        mh.invokeExact(0);
    }

    // We never call this method, so its signature-polymorphic invoke site is never resolved.
    static void invokeHandle_never_call(MethodHandle mh) throws Throwable {
        mh.invokeExact(0L);
    }

    static byte invokeHandle_byte(MethodHandle mh) throws Throwable {
        return (byte) mh.invokeExact((byte) 0);
    }

    static boolean invokeHandle_boolean(MethodHandle mh) throws Throwable {
        return (boolean) mh.invokeExact(false);
    }

    static short invokeHandle_short(MethodHandle mh) throws Throwable {
        return (short) mh.invokeExact((short) 0);
    }

    static char invokeHandle_char(MethodHandle mh) throws Throwable {
        return (char) mh.invokeExact((char) 0);
    }

    static void intFunc(int t) {}

    public static class MethodHandleBase {
        public int inheritedField;
    }

    static Runnable createLambda() {
        return () -> {
        };
    }

    @Test
    public void lookupSymbolicInfoTest() throws ReflectiveOperationException {
        MetaAccessProvider metaAccess = JVMCI.getRuntime().getHostJVMCIBackend().getMetaAccess();
        ResolvedJavaType type = metaAccess.lookupJavaType(ConstantPoolTest.class);
        ResolvedJavaMethod constantPoolMethod = type.getDeclaredMethods()[0];
        ConstantPool constantPool = constantPoolMethod.getConstantPool();
        boolean sawMethodHandle = false;
        boolean sawMethodType = false;
        boolean sawInvokeDynamic = false;
        int nonMethodHandleCpi = -1;
        int invalidNameCpi = -1;

        for (int cpi = 1; cpi < constantPool.length(); cpi++) {
            Tag tag = constantPool.getTag(cpi);
            if (nonMethodHandleCpi == -1 && tag != Tag.METHODHANDLE && tag != Tag.INVALID) {
                nonMethodHandleCpi = cpi;
            }
            if (invalidNameCpi == -1 && tag != Tag.FIELD_REF && tag != Tag.METHOD_REF && tag != Tag.INTERFACE_METHOD_REF &&
                            tag != Tag.METHODHANDLE && tag != Tag.DYNAMIC && tag != Tag.INVOKEDYNAMIC && tag != Tag.INVALID) {
                invalidNameCpi = cpi;
            }
            switch (tag) {
                case FIELD_REF:
                    Assert.assertFalse(constantPool.lookupName(cpi, -1).isEmpty());
                    break;
                case METHOD_REF:
                case INTERFACE_METHOD_REF:
                    Assert.assertFalse(constantPool.lookupName(cpi, -1).isEmpty());
                    Assert.assertNotNull(constantPool.lookupSignature(cpi));
                    break;
                case METHODHANDLE: {
                    int referenceKind = constantPool.lookupMethodHandleReferenceKind(cpi);
                    Assert.assertTrue(referenceKind >= MethodHandleInfo.REF_getField && referenceKind <= MethodHandleInfo.REF_invokeInterface,
                                    "invalid reference kind: " + referenceKind);
                    int referenceIndex = constantPool.lookupMethodHandleReferenceIndex(cpi);
                    Tag referenceTag = constantPool.getTag(referenceIndex);
                    if (referenceKind >= MethodHandleInfo.REF_getField && referenceKind <= MethodHandleInfo.REF_putStatic) {
                        Assert.assertSame(referenceTag, Tag.FIELD_REF);
                    } else {
                        Assert.assertTrue(referenceTag == Tag.METHOD_REF || referenceTag == Tag.INTERFACE_METHOD_REF,
                                        "invalid method handle reference tag: " + referenceTag);
                    }
                    Assert.assertFalse(constantPool.lookupName(cpi, -1).isEmpty());
                    if (referenceKind >= MethodHandleInfo.REF_getField && referenceKind <= MethodHandleInfo.REF_putStatic) {
                        JavaField field = constantPool.lookupField(cpi, constantPoolMethod, -1);
                        Assert.assertEquals(field.getName(), constantPool.lookupName(cpi, -1));
                    } else {
                        JavaMethod method = constantPool.lookupMethod(cpi, -1);
                        Assert.assertEquals(method.getName(), constantPool.lookupName(cpi, -1));
                        Assert.assertEquals(method.getSignature().toMethodDescriptor(), constantPool.lookupSignature(cpi).toMethodDescriptor());
                    }
                    sawMethodHandle = true;
                    break;
                }
                case METHODTYPE:
                    Assert.assertNotNull(constantPool.lookupSignature(cpi));
                    sawMethodType = true;
                    break;
                case INVOKEDYNAMIC:
                    Assert.assertFalse(constantPool.lookupName(cpi, -1).isEmpty());
                    Assert.assertNotNull(constantPool.lookupSignature(cpi));
                    sawInvokeDynamic = true;
                    break;
                default:
                    break;
            }
        }

        Assert.assertTrue(sawMethodHandle, "constant pool has no MethodHandle entry");
        Assert.assertTrue(sawMethodType, "constant pool has no MethodType entry");
        Assert.assertTrue(sawInvokeDynamic, "constant pool has no InvokeDynamic entry");
        Assert.assertTrue(nonMethodHandleCpi > 0, "constant pool has no non-method-handle entry");
        Assert.assertTrue(invalidNameCpi > 0, "constant pool has no entry without a symbolic name");
        final int invalidMethodHandleCpi = nonMethodHandleCpi;
        Assert.assertThrows(IllegalArgumentException.class, () -> constantPool.lookupMethodHandleReferenceIndex(invalidMethodHandleCpi));
        final int invalidNameEntryCpi = invalidNameCpi;
        Assert.assertThrows(IllegalArgumentException.class, () -> constantPool.lookupName(invalidNameEntryCpi, -1));

        Class<?> fixture = defineMethodHandleFixture();
        ResolvedJavaMethod fixtureMethod = metaAccess.lookupJavaMethod(fixture.getDeclaredMethod("method"));
        ConstantPool fixturePool = fixtureMethod.getConstantPool();
        for (int cpi = 1; cpi < fixturePool.length(); cpi++) {
            if (fixturePool.getTag(cpi) == Tag.METHODHANDLE && fixturePool.lookupMethodHandleReferenceKind(cpi) == MethodHandleInfo.REF_getStatic) {
                final int fieldMethodHandleCpi = cpi;
                int referenceIndex = fixturePool.lookupMethodHandleReferenceIndex(cpi);
                Assert.assertSame(fixturePool.getTag(referenceIndex), Tag.FIELD_REF);
                Assert.assertEquals(fixturePool.lookupName(referenceIndex, -1), "field");
                JavaField field = fixturePool.lookupField(cpi, fixtureMethod, -1);
                Assert.assertEquals(field.getName(), "field");
                Assert.assertThrows(IllegalArgumentException.class, () -> fixturePool.lookupSignature(fieldMethodHandleCpi));
                Assert.assertThrows(IllegalArgumentException.class, () -> fixturePool.lookupField(fieldMethodHandleCpi, null, -1));
                return;
            }
        }
        Assert.fail("fixture constant pool has no REF_getStatic MethodHandle entry");
    }

    private static Class<?> defineMethodHandleFixture() throws IllegalAccessException {
        ClassWriter writer = new ClassWriter(0);
        String name = ConstantPoolTest.class.getName().replace('.', '/') + "$MethodHandleFixture";
        String superName = MethodHandleBase.class.getName().replace('.', '/');
        writer.visit(Opcodes.V17, Opcodes.ACC_PUBLIC | Opcodes.ACC_SUPER, name, null, superName, null);
        writer.visitField(Opcodes.ACC_PUBLIC | Opcodes.ACC_STATIC, "field", "I", null, null).visitEnd();
        writer.visitField(Opcodes.ACC_PUBLIC, "instanceField", "I", null, null).visitEnd();
        MethodVisitor constructor = writer.visitMethod(Opcodes.ACC_PUBLIC, "<init>", "()V", null, null);
        constructor.visitVarInsn(Opcodes.ALOAD, 0);
        constructor.visitMethodInsn(Opcodes.INVOKESPECIAL, superName, "<init>", "()V", false);
        constructor.visitInsn(Opcodes.RETURN);
        constructor.visitMaxs(1, 1);
        constructor.visitEnd();
        MethodVisitor target = writer.visitMethod(Opcodes.ACC_PUBLIC | Opcodes.ACC_STATIC, "target", "()V", null, null);
        target.visitInsn(Opcodes.RETURN);
        target.visitMaxs(0, 0);
        target.visitEnd();
        MethodVisitor instanceTarget = writer.visitMethod(Opcodes.ACC_PUBLIC, "instanceTarget", "()V", null, null);
        instanceTarget.visitInsn(Opcodes.RETURN);
        instanceTarget.visitMaxs(0, 1);
        instanceTarget.visitEnd();
        MethodVisitor varargs = writer.visitMethod(Opcodes.ACC_PUBLIC | Opcodes.ACC_STATIC | Opcodes.ACC_VARARGS,
                        "varargs", "([Ljava/lang/Object;)V", null, null);
        varargs.visitInsn(Opcodes.RETURN);
        varargs.visitMaxs(0, 1);
        varargs.visitEnd();
        MethodVisitor method = writer.visitMethod(Opcodes.ACC_PUBLIC | Opcodes.ACC_STATIC, "method", "()V", null, null);
        method.visitLdcInsn(new Handle(Opcodes.H_INVOKESTATIC, name, "target", "()V", false));
        method.visitInsn(Opcodes.POP);
        method.visitLdcInsn(new Handle(Opcodes.H_INVOKESTATIC, name, "varargs", "([Ljava/lang/Object;)V", false));
        method.visitInsn(Opcodes.POP);
        method.visitLdcInsn(new Handle(Opcodes.H_INVOKEVIRTUAL, name, "instanceTarget", "()V", false));
        method.visitInsn(Opcodes.POP);
        method.visitLdcInsn(new Handle(Opcodes.H_INVOKESPECIAL, name, "instanceTarget", "()V", false));
        method.visitInsn(Opcodes.POP);
        method.visitLdcInsn(new Handle(Opcodes.H_NEWINVOKESPECIAL, name, "<init>", "()V", false));
        method.visitInsn(Opcodes.POP);
        method.visitLdcInsn(new Handle(Opcodes.H_INVOKEINTERFACE, "java/lang/Runnable", "run", "()V", true));
        method.visitInsn(Opcodes.POP);
        method.visitLdcInsn(new Handle(Opcodes.H_GETSTATIC, name, "field", "I", false));
        method.visitInsn(Opcodes.POP);
        method.visitLdcInsn(new Handle(Opcodes.H_PUTSTATIC, name, "field", "I", false));
        method.visitInsn(Opcodes.POP);
        method.visitLdcInsn(new Handle(Opcodes.H_GETFIELD, name, "instanceField", "I", false));
        method.visitInsn(Opcodes.POP);
        method.visitLdcInsn(new Handle(Opcodes.H_PUTFIELD, name, "instanceField", "I", false));
        method.visitInsn(Opcodes.POP);
        method.visitLdcInsn(new Handle(Opcodes.H_GETFIELD, name, "inheritedField", "I", false));
        method.visitInsn(Opcodes.POP);
        method.visitInsn(Opcodes.RETURN);
        method.visitMaxs(1, 0);
        method.visitEnd();
        writer.visitEnd();
        return MethodHandles.lookup().defineHiddenClass(writer.toByteArray(), false).lookupClass();
    }

    @Test
    public void lookupMethodHandleTest() throws Exception {
        MetaAccessProvider metaAccess = JVMCI.getRuntime().getHostJVMCIBackend().getMetaAccess();
        Class<?> fixture = defineMethodHandleFixture();
        // The symbolic holder classes are loaded before looking up the unresolved handles.
        metaAccess.lookupJavaType(fixture);
        metaAccess.lookupJavaType(Runnable.class);
        metaAccess.lookupJavaType(MethodHandleBase.class);
        ResolvedJavaMethod fixtureMethod = metaAccess.lookupJavaMethod(fixture.getDeclaredMethod("method"));
        ConstantPool fixturePool = fixtureMethod.getConstantPool();
        int methodHandleCpi = findMethodHandle(fixturePool, MethodHandleInfo.REF_invokeStatic, "target");
        int varargsMethodHandleCpi = findMethodHandle(fixturePool, MethodHandleInfo.REF_invokeStatic, "varargs");
        int virtualMethodHandleCpi = findMethodHandle(fixturePool, MethodHandleInfo.REF_invokeVirtual, "instanceTarget");
        int specialMethodHandleCpi = findMethodHandle(fixturePool, MethodHandleInfo.REF_invokeSpecial, "instanceTarget");
        int constructorMethodHandleCpi = findMethodHandle(fixturePool, MethodHandleInfo.REF_newInvokeSpecial, "<init>");
        int interfaceMethodHandleCpi = findMethodHandle(fixturePool, MethodHandleInfo.REF_invokeInterface, "run");
        int fieldHandleCpi = findMethodHandle(fixturePool, MethodHandleInfo.REF_getStatic, "field");
        int putStaticFieldHandleCpi = findMethodHandle(fixturePool, MethodHandleInfo.REF_putStatic, "field");
        int instanceFieldHandleCpi = findMethodHandle(fixturePool, MethodHandleInfo.REF_getField, "instanceField");
        int putInstanceFieldHandleCpi = findMethodHandle(fixturePool, MethodHandleInfo.REF_putField, "instanceField");
        int inheritedFieldHandleCpi = findMethodHandle(fixturePool, MethodHandleInfo.REF_getField, "inheritedField");

        int[] methodHandles = {methodHandleCpi, varargsMethodHandleCpi, virtualMethodHandleCpi,
                        specialMethodHandleCpi, constructorMethodHandleCpi, interfaceMethodHandleCpi};
        String[] methodNames = {"target", "varargs", "instanceTarget", "instanceTarget", "<init>", "run"};
        int[] fieldHandles = {fieldHandleCpi, putStaticFieldHandleCpi, instanceFieldHandleCpi,
                        putInstanceFieldHandleCpi, inheritedFieldHandleCpi};
        String[] fieldNames = {"field", "field", "instanceField", "instanceField", "inheritedField"};
        for (int cpi : methodHandles) {
            Assert.assertTrue(cpi > 0, "fixture constant pool is missing a method handle");
        }
        for (int cpi : fieldHandles) {
            Assert.assertTrue(cpi > 0, "fixture constant pool is missing a field handle");
        }
        Assert.assertEquals(fixturePool.lookupSignature(fixturePool.lookupMethodHandleReferenceIndex(methodHandleCpi)).toMethodDescriptor(), "()V");

        for (int cpi : methodHandles) {
            Assert.assertNull(fixturePool.lookupConstant(cpi, false), "method handle resolved before lookup: " + cpi);
        }
        for (int cpi : fieldHandles) {
            Assert.assertNull(fixturePool.lookupConstant(cpi, false), "method handle resolved before lookup: " + cpi);
        }

        for (int opcode : new int[]{-1, LDC, LDC_W}) {
            for (int i = 0; i < methodHandles.length; i++) {
                int cpi = methodHandles[i];
                JavaMethod method = fixturePool.lookupMethod(cpi, opcode);
                Assert.assertTrue(method instanceof ResolvedJavaMethod, opcode + " " + methodNames[i] + ": " + method);
                Assert.assertEquals(method.getName(), methodNames[i]);
            }
        }
        for (int opcode : new int[]{-1, LDC, LDC_W}) {
            for (int i = 0; i < fieldHandles.length; i++) {
                int cpi = fieldHandles[i];
                JavaField field = fixturePool.lookupField(cpi, fixtureMethod, opcode);
                Assert.assertTrue(field instanceof ResolvedJavaField, opcode + " " + fieldNames[i] + ": " + field);
                Assert.assertEquals(field.getName(), fieldNames[i]);
            }
        }
        for (int cpi : methodHandles) {
            Assert.assertNull(fixturePool.lookupConstant(cpi, false), "method lookup resolved method handle: " + cpi);
        }
        for (int cpi : fieldHandles) {
            Assert.assertNull(fixturePool.lookupConstant(cpi, false), "field lookup resolved method handle: " + cpi);
        }

        for (int cpi : methodHandles) {
            fixturePool.lookupConstant(cpi);
        }
        for (int cpi : fieldHandles) {
            fixturePool.lookupConstant(cpi);
        }

        for (int opcode : new int[]{-1, LDC, LDC_W}) {
            for (int i = 0; i < methodHandles.length; i++) {
                JavaMethod method = fixturePool.lookupMethod(methodHandles[i], opcode);
                Assert.assertTrue(method instanceof ResolvedJavaMethod, opcode + " " + methodNames[i] + ": " + method);
                Assert.assertEquals(method.getName(), methodNames[i]);
            }
        }
        for (int opcode : new int[]{-1, LDC, LDC_W}) {
            for (int i = 0; i < fieldHandles.length; i++) {
                JavaField field = fixturePool.lookupField(fieldHandles[i], fixtureMethod, opcode);
                Assert.assertTrue(field instanceof ResolvedJavaField, opcode + " " + fieldNames[i] + " kind=" +
                                fixturePool.lookupMethodHandleReferenceKind(fieldHandles[i]) + ": " + field);
                Assert.assertEquals(field.getName(), fieldNames[i]);
            }
        }
    }

    private static Class<?> defineUnresolvedHolderMethodHandleFixture() throws IllegalAccessException {
        ClassWriter writer = new ClassWriter(0);
        String name = ConstantPoolTest.class.getName().replace('.', '/') + "$UnresolvedHolderMethodHandleFixture";
        String missingHolder = ConstantPoolTest.class.getName().replace('.', '/') + "$MissingMethodHandleHolder";
        writer.visit(Opcodes.V17, Opcodes.ACC_PUBLIC | Opcodes.ACC_SUPER, name, null, "java/lang/Object", null);
        MethodVisitor method = writer.visitMethod(Opcodes.ACC_PUBLIC | Opcodes.ACC_STATIC, "method", "()V", null, null);
        method.visitLdcInsn(new Handle(Opcodes.H_INVOKESTATIC, missingHolder, "missingTarget", "()V", false));
        method.visitInsn(Opcodes.POP);
        method.visitLdcInsn(new Handle(Opcodes.H_GETSTATIC, missingHolder, "missingField", "I", false));
        method.visitInsn(Opcodes.POP);
        method.visitInsn(Opcodes.RETURN);
        method.visitMaxs(1, 0);
        method.visitEnd();
        writer.visitEnd();
        return MethodHandles.lookup().defineHiddenClass(writer.toByteArray(), false).lookupClass();
    }

    @Test
    public void lookupMethodHandleUnresolvedHolderTest() throws Exception {
        MetaAccessProvider metaAccess = JVMCI.getRuntime().getHostJVMCIBackend().getMetaAccess();
        Class<?> fixture = defineUnresolvedHolderMethodHandleFixture();
        ResolvedJavaMethod fixtureMethod = metaAccess.lookupJavaMethod(fixture.getDeclaredMethod("method"));
        ConstantPool fixturePool = fixtureMethod.getConstantPool();
        int methodHandleCpi = findMethodHandle(fixturePool, MethodHandleInfo.REF_invokeStatic, "missingTarget");
        int fieldHandleCpi = findMethodHandle(fixturePool, MethodHandleInfo.REF_getStatic, "missingField");
        Assert.assertTrue(methodHandleCpi > 0, "fixture constant pool is missing the unresolved method handle");
        Assert.assertTrue(fieldHandleCpi > 0, "fixture constant pool is missing the unresolved field handle");

        for (int opcode : new int[]{-1, LDC, LDC_W}) {
            Assert.assertNull(fixturePool.lookupConstant(methodHandleCpi, false));
            JavaMethod method = fixturePool.lookupMethod(methodHandleCpi, opcode);
            Assert.assertTrue(method instanceof UnresolvedJavaMethod, opcode + ": " + method);
            Assert.assertTrue(method.getDeclaringClass() instanceof UnresolvedJavaType, opcode + ": " + method.getDeclaringClass());
            Assert.assertEquals(method.getName(), "missingTarget");
        }
        Assert.assertNull(fixturePool.lookupConstant(methodHandleCpi, false), "lookup resolved the method handle");

        for (int opcode : new int[]{-1, LDC, LDC_W}) {
            Assert.assertNull(fixturePool.lookupConstant(fieldHandleCpi, false));
            JavaField field = fixturePool.lookupField(fieldHandleCpi, fixtureMethod, opcode);
            Assert.assertTrue(field instanceof UnresolvedJavaField, opcode + ": " + field);
            Assert.assertTrue(field.getDeclaringClass() instanceof UnresolvedJavaType, opcode + ": " + field.getDeclaringClass());
            Assert.assertEquals(field.getName(), "missingField");
        }
        Assert.assertNull(fixturePool.lookupConstant(fieldHandleCpi, false), "lookup resolved the field handle");

        // Force HotSpot to record linkage failures for both handles. Symbolic inspection must
        // remain possible after the failure and must not surface the saved linkage exception.
        final int failedMethodHandleCpi = methodHandleCpi;
        final int failedFieldHandleCpi = fieldHandleCpi;
        Assert.assertThrows(LinkageError.class, () -> fixturePool.lookupConstant(failedMethodHandleCpi, true));
        Assert.assertThrows(LinkageError.class, () -> fixturePool.lookupConstant(failedFieldHandleCpi, true));
        Assert.assertSame(fixturePool.getTag(methodHandleCpi), Tag.METHODHANDLE);
        Assert.assertSame(fixturePool.getTag(fieldHandleCpi), Tag.METHODHANDLE);
        Assert.assertEquals(fixturePool.lookupName(methodHandleCpi, -1), "missingTarget");
        Assert.assertEquals(fixturePool.lookupSignature(methodHandleCpi).toMethodDescriptor(), "()V");
        JavaMethod failedMethod = fixturePool.lookupMethod(methodHandleCpi, -1);
        Assert.assertTrue(failedMethod instanceof UnresolvedJavaMethod, String.valueOf(failedMethod));
        Assert.assertEquals(failedMethod.getName(), "missingTarget");
        Assert.assertEquals(fixturePool.lookupName(fieldHandleCpi, -1), "missingField");
        JavaField failedField = fixturePool.lookupField(fieldHandleCpi, fixtureMethod, -1);
        Assert.assertTrue(failedField instanceof UnresolvedJavaField, String.valueOf(failedField));
        Assert.assertEquals(failedField.getName(), "missingField");
    }

    private static int findMethodHandle(ConstantPool constantPool, int referenceKind, String name) {
        for (int cpi = 1; cpi < constantPool.length(); cpi++) {
            if (constantPool.getTag(cpi) == Tag.METHODHANDLE && constantPool.lookupMethodHandleReferenceKind(cpi) == referenceKind &&
                            constantPool.lookupName(constantPool.lookupMethodHandleReferenceIndex(cpi), -1).equals(name)) {
                return cpi;
            }
        }
        return -1;
    }

    @Test
    public void lookupAppendixTest() throws Throwable {
        // We want at least two indy bytecodes -- with a single indy, the rawIndex is -1,
        // or 0xffffffff. Even if we load it with the wrong endianness, it will still come
        // "correctly" out as -1.
        concatString1("aaa", "bbb"); // force the indy to be resolved
        concatString2("aaa", "bbb"); // force the indy to be resolved

        MethodHandles.Lookup lookup = MethodHandles.lookup();
        MethodType mt = MethodType.methodType(void.class, int.class);
        MethodHandle mh = lookup.findStatic(ConstantPoolTest.class, "intFunc", mt);
        invokeHandle(mh);

        lookupAppendixTest_dynamic("concatString1");
        lookupAppendixTest_dynamic("concatString2");
        lookupAppendixTest_virtual();
    }

    private void lookupAppendixTest_dynamic(String methodName) throws Exception {
        MetaAccessProvider metaAccess = JVMCI.getRuntime().getHostJVMCIBackend().getMetaAccess();
        ResolvedJavaType type = metaAccess.lookupJavaType(ConstantPoolTest.class);
        Signature methodSig = metaAccess.parseMethodDescriptor("(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
        ResolvedJavaMethod m = type.findMethod(methodName, methodSig);
        Assert.assertNotNull(m);

        // Expected:
        // aload_0;
        // aload_1;
        // invokedynamic ...StringConcatFactory.makeConcatWithConstants...
        byte[] bytecode = m.getCode();
        Assert.assertNotNull(bytecode);
        Assert.assertEquals(8, bytecode.length);
        Assert.assertEquals(ALOAD_0, beU1(bytecode, 0));
        Assert.assertEquals(ALOAD_1, beU1(bytecode, 1));
        Assert.assertEquals(INVOKEDYNAMIC, beU1(bytecode, 2));

        // Note: internally HotSpot stores the indy index as a native int32, but m.getCode() byte-swaps all such
        // indices so they appear to be big-endian.
        int rawIndex = beS4(bytecode, 3);
        JavaConstant constant = m.getConstantPool().lookupAppendix(rawIndex, INVOKEDYNAMIC);
        Assert.assertTrue(constant.toString().startsWith("Object["), "wrong appendix: " + constant);
    }

    private void lookupAppendixTest_virtual() throws Exception {
        MetaAccessProvider metaAccess = JVMCI.getRuntime().getHostJVMCIBackend().getMetaAccess();
        ResolvedJavaType type = metaAccess.lookupJavaType(ConstantPoolTest.class);
        Signature methodSig = metaAccess.parseMethodDescriptor("(Ljava/lang/invoke/MethodHandle;)V");
        ResolvedJavaMethod m = type.findMethod("invokeHandle", methodSig);
        Assert.assertNotNull(m);

        // Expected
        // aload_0
        // iconst_0
        // invokevirtual #rawIndex // Method java/lang/invoke/MethodHandle.invokeExact:(I)V
        byte[] bytecode = m.getCode();
        Assert.assertNotNull(bytecode);
        Assert.assertEquals(6, bytecode.length);
        Assert.assertEquals(ALOAD_0, beU1(bytecode, 0));
        Assert.assertEquals(ICONST_0, beU1(bytecode, 1));
        Assert.assertEquals(INVOKEVIRTUAL, beU1(bytecode, 2));

        int rawIndex = beU2(bytecode, 3);
        //System.out.println("rawIndex = " + rawIndex);
        JavaConstant constant = m.getConstantPool().lookupAppendix(rawIndex, INVOKEVIRTUAL);
        //System.out.println("constant = " + constant);
        Assert.assertTrue(constant.toString().startsWith("Object["), "wrong appendix: " + constant);
    }

    static void invokeVirtual(Object o) {
        o.hashCode();
    }

    @Test
    public void lookupMethodTest_dynamic() throws Exception {
        concatString1("aaa", "bbb"); // force the indy to be resolved

        MetaAccessProvider metaAccess = JVMCI.getRuntime().getHostJVMCIBackend().getMetaAccess();
        ResolvedJavaType type = metaAccess.lookupJavaType(ConstantPoolTest.class);
        Signature methodSig = metaAccess.parseMethodDescriptor("(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
        ResolvedJavaMethod m = type.findMethod("concatString1", methodSig);
        Assert.assertNotNull(m);

        // Expected:
        // aload_0;
        // aload_1;
        // invokedynamic ...StringConcatFactory.makeConcatWithConstants...
        byte[] bytecode = m.getCode();
        Assert.assertNotNull(bytecode);
        Assert.assertEquals(8, bytecode.length);
        Assert.assertEquals(ALOAD_0, beU1(bytecode, 0));
        Assert.assertEquals(ALOAD_1, beU1(bytecode, 1));
        Assert.assertEquals(INVOKEDYNAMIC, beU1(bytecode, 2));

        // Note: internally HotSpot stores the indy index as a native int32, but m.getCode() byte-swaps all such
        // indices so they appear to be big-endian.
        int rawIndex = beS4(bytecode, 3);
        System.out.println("rawIndex = " + rawIndex);
        JavaMethod callee = m.getConstantPool().lookupMethod(rawIndex, INVOKEDYNAMIC, /*caller=*/m);
        System.out.println("callee = " + callee);
        Assert.assertTrue(callee.toString().equals("HotSpotMethod<Invokers$Holder.linkToTargetMethod(Object, Object, Object)>"),
                          "wrong method: " + callee);
    }

    @Test
    public void lookupMethodTest_dynamic_unresolved() throws Exception {
        MetaAccessProvider metaAccess = JVMCI.getRuntime().getHostJVMCIBackend().getMetaAccess();
        ResolvedJavaType type = metaAccess.lookupJavaType(ConstantPoolTest.class);
        Signature methodSig = metaAccess.parseMethodDescriptor("(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
        ResolvedJavaMethod m = type.findMethod("concatString3_never_call", methodSig);
        Assert.assertNotNull(m);

        // Expected:
        // aload_0;
        // aload_1;
        // invokedynamic ...StringConcatFactory.makeConcatWithConstants...
        byte[] bytecode = m.getCode();
        Assert.assertNotNull(bytecode);
        Assert.assertEquals(8, bytecode.length);
        Assert.assertEquals(ALOAD_0, beU1(bytecode, 0));
        Assert.assertEquals(ALOAD_1, beU1(bytecode, 1));
        Assert.assertEquals(INVOKEDYNAMIC, beU1(bytecode, 2));

        // Note: internally HotSpot stores the indy index as a native int32, but m.getCode() byte-swaps all such
        // indices so they appear to be big-endian.
        int rawIndex = beS4(bytecode, 3);
        System.out.println("rawIndex = " + rawIndex);
        JavaMethod callee = m.getConstantPool().lookupMethod(rawIndex, INVOKEDYNAMIC, /*caller=*/m);
        System.out.println("callee = " + callee);
        Assert.assertTrue(callee.toString().startsWith("jdk.vm.ci.meta.UnresolvedJavaMethod"),
                          "wrong method: " + callee);
    }

    @Test
    public void lookupMethodTest_virtual() throws Exception {
        MetaAccessProvider metaAccess = JVMCI.getRuntime().getHostJVMCIBackend().getMetaAccess();
        ResolvedJavaType type = metaAccess.lookupJavaType(ConstantPoolTest.class);
        Signature methodSig = metaAccess.parseMethodDescriptor("(Ljava/lang/Object;)V");
        ResolvedJavaMethod m = type.findMethod("invokeVirtual", methodSig);
        Assert.assertNotNull(m);

        // Expected
        // 0: aload_0
        // 1: invokevirtual #rawIndex // Method Method java/lang/Object.hashCode:()I
        byte[] bytecode = m.getCode();
        Assert.assertNotNull(bytecode);
        Assert.assertEquals(ALOAD_0, beU1(bytecode, 0));
        Assert.assertEquals(INVOKEVIRTUAL, beU1(bytecode, 1));
        int rawIndex = beU2(bytecode, 2);
        System.out.println("rawIndex = " + rawIndex);
        JavaMethod callee = m.getConstantPool().lookupMethod(rawIndex, INVOKEVIRTUAL, /*caller=*/m);
        System.out.println("callee = " + callee);
        Assert.assertTrue(callee.toString().equals("HotSpotMethod<Object.hashCode()>"),
                          "wrong method: " + callee);
        Assert.assertEquals(m.getConstantPool().lookupName(rawIndex, INVOKEVIRTUAL), "hashCode");
        Assert.assertEquals(m.getConstantPool().lookupSignature(rawIndex, INVOKEVIRTUAL).toMethodDescriptor(), "()I");
    }

    @Test
    public void lookupMethodTest_signaturePolymorphic_resolved() throws Throwable {
        MethodHandles.Lookup lookup = MethodHandles.lookup();
        MethodType mt = MethodType.methodType(void.class, int.class);
        MethodHandle mh = lookup.findStatic(ConstantPoolTest.class, "intFunc", mt);
        invokeHandle(mh);

        ResolvedJavaMethod m = findInvokeExactCaller("invokeHandle");
        int rawIndex = getInvokeExactIndex(m, ICONST_0, "(I)V");
        JavaMethod callee = m.getConstantPool().lookupMethod(rawIndex, INVOKEVIRTUAL, m);
        assertResolvedInvokeExactMethod(callee, "(Ljava/lang/Object;ILjava/lang/Object;)V");
    }

    @Test
    public void lookupMethodTest_signaturePolymorphic_unresolved() throws Exception {
        ResolvedJavaMethod m = findInvokeExactCaller("invokeHandle_never_call");
        int rawIndex = getInvokeExactIndex(m, LCONST_0, "(J)V");
        JavaMethod callee = m.getConstantPool().lookupMethod(rawIndex, INVOKEVIRTUAL, m);
        Assert.assertTrue(callee instanceof UnresolvedJavaMethod, "unresolved invokeExact site returned " + callee);
        Assert.assertEquals(callee.getName(), "invokeExact");
        Assert.assertEquals(callee.getSignature().toMethodDescriptor(), "(J)V");
        Assert.assertEquals(callee.getDeclaringClass().getName(), "Ljava/lang/invoke/MethodHandle;");
    }

    @Test
    public void lookupMethodTest_signaturePolymorphic_subInt() throws Throwable {
        invokeHandle_byte(MethodHandles.identity(byte.class));
        invokeHandle_boolean(MethodHandles.identity(boolean.class));
        invokeHandle_short(MethodHandles.identity(short.class));
        invokeHandle_char(MethodHandles.identity(char.class));

        String[] methodNames = {"invokeHandle_byte", "invokeHandle_boolean", "invokeHandle_short", "invokeHandle_char"};
        String[] descriptors = {"(B)B", "(Z)Z", "(S)S", "(C)C"};
        for (int i = 0; i < methodNames.length; i++) {
            String returnDescriptor = descriptors[i].substring(descriptors[i].indexOf(')') + 1);
            ResolvedJavaMethod m = findInvokeExactCaller(methodNames[i], returnDescriptor);
            int rawIndex = getInvokeExactIndex(m, ICONST_0, descriptors[i]);
            JavaMethod callee = m.getConstantPool().lookupMethod(rawIndex, INVOKEVIRTUAL, m);
            assertResolvedInvokeExactMethod(callee, "(Ljava/lang/Object;ILjava/lang/Object;)I");
        }
    }

    private void assertResolvedInvokeExactMethod(JavaMethod method, String descriptor) {
        Assert.assertTrue(method instanceof ResolvedJavaMethod, "resolved invokeExact site returned " + method);
        Assert.assertEquals(method.getName(), "invokeExact_MT");
        Assert.assertEquals(method.getSignature().toMethodDescriptor(), descriptor);
        Assert.assertTrue(method.getDeclaringClass() instanceof ResolvedJavaType, "unresolved declaring class: " + method);
        Assert.assertTrue(method.getDeclaringClass().getName().startsWith("Ljava/lang/invoke/LambdaForm$MH"),
                        "wrong declaring class: " + method.getDeclaringClass());
    }

    private ResolvedJavaMethod findInvokeExactCaller(String methodName) {
        return findInvokeExactCaller(methodName, "V");
    }

    private ResolvedJavaMethod findInvokeExactCaller(String methodName, String returnDescriptor) {
        MetaAccessProvider metaAccess = JVMCI.getRuntime().getHostJVMCIBackend().getMetaAccess();
        ResolvedJavaType type = metaAccess.lookupJavaType(ConstantPoolTest.class);
        Signature methodSig = metaAccess.parseMethodDescriptor("(Ljava/lang/invoke/MethodHandle;)" + returnDescriptor);
        ResolvedJavaMethod method = type.findMethod(methodName, methodSig);
        Assert.assertNotNull(method, "missing method " + methodName);
        return method;
    }

    private int getInvokeExactIndex(ResolvedJavaMethod method, int argumentOpcode, String descriptor) {
        byte[] bytecode = method.getCode();
        Assert.assertNotNull(bytecode, method.toString());
        Assert.assertEquals(bytecode.length, 6, method.toString());
        Assert.assertEquals(beU1(bytecode, 0), ALOAD_0, method.toString());
        Assert.assertEquals(beU1(bytecode, 1), argumentOpcode, method.toString());
        Assert.assertEquals(beU1(bytecode, 2), INVOKEVIRTUAL, method.toString());
        Assert.assertEquals(beU1(bytecode, 5), descriptor.endsWith("V") ? RETURN : IRETURN, method.toString());
        int rawIndex = beU2(bytecode, 3);
        Assert.assertEquals(method.getConstantPool().lookupName(rawIndex, INVOKEVIRTUAL), "invokeExact");
        Assert.assertEquals(method.getConstantPool().lookupSignature(rawIndex, INVOKEVIRTUAL).toMethodDescriptor(), descriptor);
        Assert.assertEquals(method.getConstantPool().lookupReferencedType(rawIndex, INVOKEVIRTUAL).getName(), "Ljava/lang/invoke/MethodHandle;");
        return rawIndex;
    }
}
