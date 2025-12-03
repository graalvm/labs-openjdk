/*
 * Copyright (c) 2012, 2025, Oracle and/or its affiliates. All rights reserved.
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

/**
 * @test
 * @requires vm.jvmci
 * @library ../../../../../ /test/lib
 * @build jdk.test.lib.RandomFactory
 * @modules jdk.internal.vm.ci/jdk.vm.ci.meta
 *          jdk.internal.vm.ci/jdk.vm.ci.runtime
 *          java.base/jdk.internal.misc
 * @run junit/othervm -XX:+UnlockExperimentalVMOptions -XX:+EnableJVMCI -XX:-UseJVMCICompiler jdk.vm.ci.runtime.test.TestConstantReflectionProvider
 */

package jdk.vm.ci.runtime.test;

import jdk.test.lib.RandomFactory;
import jdk.vm.ci.meta.ConstantReflectionProvider;
import jdk.vm.ci.meta.JavaConstant;
import jdk.vm.ci.meta.JavaKind;
import org.junit.Assert;
import org.junit.Test;

import java.lang.reflect.Array;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.IdentityHashMap;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Random;
import java.util.TreeMap;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertThrows;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

/**
 * Tests for {@link ConstantReflectionProvider}. It assumes an implementation of the interface that
 * actually returns non-null results for access operations that are possible, i.e., the tests will
 * fail for an implementation that spuriously returns null (which is allowed by the specification).
 */
public class TestConstantReflectionProvider extends TypeUniverse {

    @Test
    public void constantEqualsTest() {
        for (ConstantValue c1 : constants()) {
            for (ConstantValue c2 : constants()) {
                // test symmetry
                assertEquals(constantReflection.constantEquals(c1.value, c2.value), constantReflection.constantEquals(c2.value, c1.value));
                if (c1.value.getJavaKind() != JavaKind.Object && c2.value.getJavaKind() != JavaKind.Object) {
                    assertEquals(c1.value.equals(c2.value), constantReflection.constantEquals(c2.value, c1.value));
                }
            }
        }
    }

    @Test
    public void readArrayLengthTest() {
        for (ConstantValue cv : constants()) {
            JavaConstant c = cv.value;
            Integer actual = constantReflection.readArrayLength(c);
            if (c.getJavaKind() != JavaKind.Object || c.isNull() || !cv.boxed.getClass().isArray()) {
                assertNull(actual);
            } else {
                assertNotNull(actual);
                int actualInt = actual;
                assertEquals(Array.getLength(cv.boxed), actualInt);
            }
        }
    }

    static class PrimitiveConstants {
        static final long LONG_CONST = 42;
        static final int INT_CONST = 66;
        static final byte BYTE_CONST = 123;
        static final boolean BOOL_CONST = true;
    }

    static class BoxedConstants {
        static final Long LONG_CONST = 42L;
        static final Integer INT_CONST = 66;
        static final Byte BYTE_CONST = 123;
        static final Boolean BOOL_CONST = true;
    }

    @Test
    public void identityHashCodeTest() {
        try {
            constantReflection.identityHashCode(null);
            fail("Expected NullPointerException");
        } catch (NullPointerException e) {
            // Expected
        }
        assertEquals(0, constantReflection.identityHashCode(JavaConstant.NULL_POINTER));
        for (ConstantValue cv : constants()) {
            if (cv.value.getJavaKind() != JavaKind.Object) {
                try {
                    constantReflection.identityHashCode(cv.value);
                    fail("Expected IllegalArgumentException for " + cv);
                } catch (IllegalArgumentException e) {
                    // Expected
                }
            } else if (cv.boxed != cv.value) {
                int expect = System.identityHashCode(cv.boxed);
                int actual = constantReflection.identityHashCode(cv.value);
                assertEquals(cv.toString(), expect, actual);
            }
        }
    }

    @Test
    public void makeIdentityHashCodeTest() throws Exception {
        try {
            constantReflection.makeIdentityHashCode(null, 0);
            fail("Expected NullPointerException");
        } catch (NullPointerException e) {
            // Expected
        }
        try {
            constantReflection.makeIdentityHashCode(JavaConstant.NULL_POINTER, 0);
            fail("Expected NullPointerException");
        } catch (NullPointerException e) {
            // Expected
        }
        try {
            JavaConstant constant = constantReflection.forString("fresh value");
            int illegalHotSpotHashCode = -1;
            constantReflection.makeIdentityHashCode(constant, illegalHotSpotHashCode);
            fail("Expected IllegalArgumentException");
        } catch (IllegalArgumentException e) {
            // Expected
        }
        for (ConstantValue cv : constants()) {
            if (cv.value.getJavaKind() != JavaKind.Object) {
                try {
                    constantReflection.makeIdentityHashCode(cv.value, 0);
                    fail("Expected IllegalArgumentException for " + cv);
                } catch (IllegalArgumentException e) {
                    // Expected
                }
            } else if (cv.boxed != cv.value) {
                int expect = System.identityHashCode(cv.boxed);
                // Check that changing an initialized identity hash code fails
                int differentThanExpect = expect == Integer.MAX_VALUE ? 1 : expect + 1;
                int actual = constantReflection.makeIdentityHashCode(cv.value, differentThanExpect);
                assertEquals(cv.toString(), expect, actual);
            }
        }

        class ConstantsWithUninitializedIdentityHashCodes {
            static final Object CONST1 = new ArrayList<>();
            static final Object CONST2 = new ArrayList<>();
            static final Object CONST3 = new IdentityHashMap<>();
            static final Object CONST4 = new LinkedHashMap<>();
            static final Object CONST5 = new TreeMap<>();
            static final Object CONST6 = new ArrayDeque<>();
            static final Object CONST7 = new LinkedList<>();
            static final Object CONST8 = new String[] {"string1", "string2", "string3"};
            static final Object CONST9 = new Object();
            static final Object CONST10 = new Object() {};
        }

        Random random = RandomFactory.getRandom();
        for (ConstantValue cv : readConstants(ConstantsWithUninitializedIdentityHashCodes.class)) {
            // Get a random value in the range [1 .. MAX_VALUE]
            int expect = random.nextInt(Integer.MAX_VALUE) + 1;
            int actual = constantReflection.makeIdentityHashCode(cv.value, expect);
            Assert.assertEquals(cv.name, expect, actual);

            // Check that changing an initialized identity hash code fails
            int differentThanExpect = expect == Integer.MAX_VALUE ? 1 : expect + 1;
            actual = constantReflection.makeIdentityHashCode(cv.value, differentThanExpect);
            assertEquals(cv.toString(), expect, actual);
        }
    }

    @Test
    public void boxTest() {
        for (ConstantValue cv : constants()) {
            JavaConstant c = cv.value;
            JavaConstant boxed = constantReflection.boxPrimitive(c);
            if (boxed != null && c.getJavaKind().isPrimitive()) {
                assertTrue(boxed.getJavaKind().isObject());
                assertFalse(boxed.isNull());
            }
        }

        List<ConstantValue> primitiveConstants = readConstants(PrimitiveConstants.class);
        List<ConstantValue> boxedConstants = readConstants(BoxedConstants.class);
        for (int i = 0; i < primitiveConstants.size(); i++) {
            ConstantValue prim = primitiveConstants.get(i);
            ConstantValue box = boxedConstants.get(i);
            assertEquals(box.value, constantReflection.boxPrimitive(prim.value));
        }

        assertNull(constantReflection.boxPrimitive(JavaConstant.NULL_POINTER));
    }

    @Test
    public void unboxTest() {
        for (ConstantValue cv : constants()) {
            JavaConstant c = cv.value;
            JavaConstant unboxed = c.isNull() ? null : constantReflection.unboxPrimitive(c);
            if (unboxed != null) {
                assertFalse(unboxed.getJavaKind().isObject());
            }
        }
        List<ConstantValue> primitiveConstants = readConstants(PrimitiveConstants.class);
        List<ConstantValue> boxedConstants = readConstants(BoxedConstants.class);
        for (int i = 0; i < primitiveConstants.size(); i++) {
            ConstantValue prim = primitiveConstants.get(i);
            ConstantValue box = boxedConstants.get(i);
            assert prim.getSimpleName().equals(box.getSimpleName());
            assertEquals(prim.value, constantReflection.unboxPrimitive(box.value));
        }

        assertNull(constantReflection.unboxPrimitive(JavaConstant.NULL_POINTER));
    }

    static class ArrayConstants {
        static final byte[] BYTE_ARRAY_CONST = new byte[]{0};
        static final Object[] OBJECT_ARRAY_CONST = new Object[]{null};
    }

    @Test
    public void readOnePastLastArrayElementTest() {
        for (ConstantValue cv : readConstants(ArrayConstants.class)) {
            if (cv.boxed != null && cv.boxed.getClass().isArray()) {
                JavaKind kind = metaAccess.lookupJavaType(cv.value).getComponentType().getJavaKind();
                long offset = metaAccess.getArrayBaseOffset(kind) + (long) metaAccess.getArrayIndexScale(kind) * Array.getLength(cv.boxed);
                // read array[array.length]
                assertThrows(IllegalArgumentException.class, () -> {
                    if (kind == JavaKind.Object) {
                        constantReflection.getMemoryAccessProvider().readObjectConstant(cv.value, offset);
                    } else {
                        constantReflection.getMemoryAccessProvider().readPrimitiveConstant(kind, cv.value, offset, kind.getBitCount());
                    }
                });
            }
        }
    }

    static class IntArrayConstants {
        static final int[] INT_ARRAY_CONST = new int[]{0};
    }

    @Test
    public void readPartiallyOutOfBoundsTest() {
        for (ConstantValue cv : readConstants(IntArrayConstants.class)) {
            JavaKind kind = metaAccess.lookupJavaType(cv.value).getComponentType().getJavaKind();
            long offset = metaAccess.getArrayBaseOffset(kind) + (long) metaAccess.getArrayIndexScale(kind) * (Array.getLength(cv.boxed) - 1);
            // read a long from array[array.length - 1], which is partially out of bounds
            JavaKind accessKind = JavaKind.Long;
            assertThrows(IllegalArgumentException.class, () -> constantReflection.getMemoryAccessProvider().readPrimitiveConstant(accessKind, cv.value, offset, accessKind.getBitCount()));
        }
    }
}
