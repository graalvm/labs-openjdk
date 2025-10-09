/*
 * Copyright (c) 2022, 2024, Oracle and/or its affiliates. All rights reserved.
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
 * @modules java.base/jdk.internal.vm
 *          java.base/jdk.internal.misc
 * @run testng/othervm
 *      jdk.internal.vm.test.TestTranslatedException
 */
package jdk.internal.vm.test;

import java.lang.reflect.InvocationTargetException;
import java.nio.charset.StandardCharsets;

import org.testng.Assert;
import org.testng.annotations.Test;

import jdk.internal.misc.Unsafe;
import jdk.internal.vm.TranslatedException;
import jdk.internal.vm.VMSupport;

public class TestTranslatedException {
    public static class Untranslatable extends RuntimeException {
        public Untranslatable(String message, Throwable cause) {
            super(message, cause);
        }
    }

    @Test
    public void encodeDecodeTest() {
        Throwable throwable = new ExceptionInInitializerError(new InvocationTargetException(new Untranslatable("test exception", new NullPointerException()), "invoke"));
        for (int i = 0; i < 10; i++) {
            throwable = new ExceptionInInitializerError(new InvocationTargetException(new RuntimeException(String.valueOf(i), throwable), "invoke"));
        }
        encodeDecode(throwable);

        try {
            VMSupport.decodeAndThrowThrowable(0, 0L, true, false);
            throw new AssertionError("expected decodeAndThrowThrowable to throw an exception");
        } catch (NullPointerException decoded) {
            // Expected
        } catch (Throwable decoded) {
            throw new AssertionError("unexpected exception: " + decoded);
        }

        try {
            VMSupport.decodeAndThrowThrowable(1, 0L, true, false);
            throw new AssertionError("expected decodeAndThrowThrowable to throw an exception");
        } catch (InternalError decoded) {
            if (!decoded.getMessage().startsWith("native buffer could not be allocated")) {
                throw new AssertionError("unexpected exception: " + decoded);
            }
        } catch (Throwable decoded) {
            throw new AssertionError("unexpected exception: " + decoded);
        }

        try {
            VMSupport.decodeAndThrowThrowable(2, 0L, true, false);
            throw new AssertionError("expected decodeAndThrowThrowable to throw an exception");
        } catch (OutOfMemoryError decoded) {
            // Expected
        } catch (Throwable decoded) {
            throw new AssertionError("unexpected exception: " + decoded);
        }

        try {
            VMSupport.decodeAndThrowThrowable(3, 0L, true, false);
            throw new AssertionError("expected decodeAndThrowThrowable to throw an exception");
        } catch (InternalError decoded) {
            // Expected
        } catch (Throwable decoded) {
            throw new AssertionError("unexpected exception: " + decoded);
        }

        try {
            VMSupport.decodeAndThrowThrowable(4, 0L, true, false);
            throw new AssertionError("expected decodeAndThrowThrowable to throw an exception");
        } catch (OutOfMemoryError decoded) {
            // Expected
        } catch (Throwable decoded) {
            throw new AssertionError("unexpected exception: " + decoded);
        }

        try {
            VMSupport.decodeAndThrowThrowable(5, 0L, true, false);
            throw new AssertionError("expected decodeAndThrowThrowable to throw an exception");
        } catch (InternalError decoded) {
            // Expected
        } catch (Throwable decoded) {
            throw new AssertionError("unexpected exception: " + decoded);
        }

        Unsafe unsafe = Unsafe.getUnsafe();
        byte[] problem = "very unlikely problem".getBytes(StandardCharsets.UTF_8);
        long buffer = unsafe.allocateMemory(problem.length + 4);
        try {
            unsafe.putInt(buffer, problem.length);
            unsafe.copyMemory(problem, Unsafe.ARRAY_BYTE_BASE_OFFSET, null, buffer + 4, problem.length);
            VMSupport.decodeAndThrowThrowable(3, buffer, true, false);
            throw new AssertionError("expected decodeAndThrowThrowable to throw an exception");
        } catch (InternalError decoded) {
            String msg = decoded.getMessage();
            if (!msg.endsWith("very unlikely problem")) {
                throw new AssertionError("unexpected exception: " + decoded);
            }
        } catch (Throwable decoded) {
            throw new AssertionError("unexpected exception: " + decoded);
        } finally {
            unsafe.freeMemory(buffer);
        }
    }

    private void encodeDecode(Throwable throwable) {
        Unsafe unsafe = Unsafe.getUnsafe();
        int bufferSize = 512;
        int format = 0;
        while (true) {
            long buffer = unsafe.allocateMemory(bufferSize);
            try {
                int res = VMSupport.encodeThrowable(throwable, buffer, bufferSize);
                if (res < 0) {
                    bufferSize = -res;
                } else {
                    try {
                        VMSupport.decodeAndThrowThrowable(format, buffer, true, false);
                        throw new AssertionError("expected decodeAndThrowThrowable to throw an exception");
                    } catch (Throwable decoded) {
                        assertThrowableEquals(throwable, decoded);
                    }
                    return;
                }
            } finally {
                unsafe.freeMemory(buffer);
            }
        }
    }

    private static void assertThrowableEquals(Throwable originalIn, Throwable decodedIn) {
        Throwable original = originalIn;
        Throwable decoded = decodedIn;
        try {
            Assert.assertEquals(original == null, decoded == null);
            while (original != null) {
                if (Untranslatable.class.equals(original.getClass())) {
                    Assert.assertEquals(decoded.getClass().getName(), "java.lang.InternalError");
                    Assert.assertEquals(decoded.toString(), "java.lang.InternalError: test exception [jdk.internal.vm.test.TestTranslatedException$Untranslatable]");
                    Assert.assertEquals(original.getMessage(), "test exception");
                } else {
                    Assert.assertEquals(decoded.getClass().getName(), original.getClass().getName());
                    Assert.assertEquals(decoded.getMessage(), original.getMessage());
                }
                StackTraceElement[] originalStack = original.getStackTrace();
                StackTraceElement[] decodedStack = decoded.getStackTrace();
                int decodedStartIndex;
                if (decoded == decodedIn) {
                    Assert.assertEquals(decodedStack[0], TranslatedException.TRANSLATED_MARKER);
                    Assert.assertEquals(decodedStack.length, originalStack.length + 1);
                    decodedStartIndex = 1;
                } else {
                    Assert.assertEquals(decodedStack.length, originalStack.length);
                    decodedStartIndex = 0;
                }
                for (int i = 0; i < originalStack.length; ++i) {
                    StackTraceElement originalStackElement = originalStack[i];
                    StackTraceElement decodedStackElement = decodedStack[i + decodedStartIndex];
                    Assert.assertEquals(decodedStackElement.getClassLoaderName(), originalStackElement.getClassLoaderName());
                    Assert.assertEquals(decodedStackElement.getModuleName(), originalStackElement.getModuleName());
                    Assert.assertEquals(decodedStackElement.getClassName(), originalStackElement.getClassName());
                    Assert.assertEquals(decodedStackElement.getMethodName(), originalStackElement.getMethodName());
                    Assert.assertEquals(decodedStackElement.getFileName(), originalStackElement.getFileName());
                    Assert.assertEquals(decodedStackElement.getLineNumber(), originalStackElement.getLineNumber());
                }
                original = original.getCause();
                decoded = decoded.getCause();
            }
        } catch (AssertionError e) {
            System.err.println("original:[");
            originalIn.printStackTrace(System.err);
            System.err.println("]");
            System.err.println("decoded:[");
            decodedIn.printStackTrace(System.err);
            System.err.println("]");
            throw e;
        }
    }
}
