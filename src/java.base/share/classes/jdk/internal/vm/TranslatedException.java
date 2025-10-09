/*
 * Copyright (c) 2018, 2024, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Oracle designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Oracle in the LICENSE file that accompanied this code.
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
package jdk.internal.vm;

import jdk.internal.misc.VM;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.zip.GZIPInputStream;
import java.util.zip.GZIPOutputStream;

/**
 * Support for translating exceptions between the HotSpot heap and libjvmci heap.
 * <p>
 * Apart from {@link OutOfMemoryError}s, successfully translated exceptions have
 * {@link #TRANSLATED_MARKER} as the first element in their stack trace. This
 * allows a caller to distinguish between a translated exception and an error
 * that arose during translation.
 */
@SuppressWarnings("serial")
public final class TranslatedException extends Exception {

    /**
     * Marker frame prepended to outermost stack of a translated exception.
     */
    public static final StackTraceElement TRANSLATED_MARKER = new StackTraceElement(TranslatedException.class.getName(), "translated", null, -2);

    /**
     * The value returned by {@link #encodeThrowable(Throwable)} when encoding
     * fails due to an {@link OutOfMemoryError}.
     */
    private static final byte[] FALLBACK_ENCODED_OUTOFMEMORYERROR_BYTES;

    /**
     * The value returned by {@link #encodeThrowable(Throwable)} when encoding
     * fails for any reason other than {@link OutOfMemoryError}.
     */
    private static final byte[] FALLBACK_ENCODED_THROWABLE_BYTES;
    static {
        maybeFailClinit();
        try {
            FALLBACK_ENCODED_THROWABLE_BYTES =
                encodeThrowable(translationFailure("error during encoding"), false);
            FALLBACK_ENCODED_OUTOFMEMORYERROR_BYTES =
                encodeThrowable(translationFailure("OutOfMemoryError during encoding"), false);
        } catch (IOException e) {
            throw new InternalError(e);
        }
    }

    private static InternalError translationFailure(String messageFormat, Object... messageArgs) {
        return new InternalError(messageFormat.formatted(messageArgs));
    }

    /**
     * Helper to test exception translation.
     */
    private static void maybeFailClinit() {
        String className = VM.getSavedProperty("test.jvmci.TranslatedException.clinit.throw");
        if (className != null) {
            try {
                throw (Throwable) Class.forName(className).getDeclaredConstructor().newInstance();
            } catch (RuntimeException | Error e) {
                throw e;
            } catch (Throwable e) {
                throw new InternalError(e);
            }
        }
    }

    TranslatedException(Throwable translated) {
        super(translated);
    }

    /**
     * No need to record an initial stack trace since
     * it will be manually overwritten.
     */
    @SuppressWarnings("sync-override")
    @Override
    public Throwable fillInStackTrace() {
        return this;
    }

    /**
     * Prints a stack trace for {@code throwable} if the system property
     * {@code "jdk.internal.vm.TranslatedException.debug"} is true.
     */
    private static void debugPrintStackTrace(Throwable throwable, boolean debug) {
        if (debug) {
            System.err.print("DEBUG: ");
            throwable.printStackTrace(System.err);
        }
    }

    private static Throwable initCause(Throwable throwable, Throwable cause, boolean debug) {
        if (cause != null) {
            try {
                throwable.initCause(cause);
            } catch (IllegalStateException e) {
                // Cause could not be set or overwritten.
                debugPrintStackTrace(e, debug);
            }
        }
        return throwable;
    }

    /**
     * Creates an exception if {@code className} is one of the supported
     * core exceptions for translation.
     *
     * @param className class name of exception to create
     * @param message the detailed message for the exception
     * @return {@code null} if {@code className} is unsupported
     */
    private static Throwable newThrowable(String className, String message) {
        return switch (className) {
            // Exceptions
            case "java.lang.ArithmeticException" -> new ArithmeticException(message);
            case "java.lang.ArrayIndexOutOfBoundsException" -> new ArrayIndexOutOfBoundsException(message);
            case "java.lang.ArrayStoreException" -> new ArrayStoreException(message);
            case "java.lang.ClassCastException" -> new ClassCastException(message);
            case "java.lang.ClassNotFoundException" -> new ClassNotFoundException(message);
            case "java.lang.CloneNotSupportedException" -> new CloneNotSupportedException(message);
            case "java.lang.IllegalAccessException" -> new IllegalAccessException(message);
            case "java.lang.IllegalArgumentException" -> new IllegalArgumentException(message);
            case "java.lang.IndexOutOfBoundsException" -> new IndexOutOfBoundsException(message);
            case "java.lang.InstantiationException" -> new InstantiationException(message);
            case "java.lang.NegativeArraySizeException" -> new NegativeArraySizeException(message);
            case "java.lang.NoSuchFieldException" -> new NoSuchFieldException(message);
            case "java.lang.NoSuchMethodException" -> new NoSuchMethodException(message);
            case "java.lang.NullPointerException" -> new NullPointerException(message);
            case "java.lang.RuntimeException" -> new RuntimeException(message);
            case "java.lang.StringIndexOutOfBoundsException" -> new StringIndexOutOfBoundsException(message);
            case "java.lang.UnsupportedOperationException" -> new UnsupportedOperationException(message);

            // Errors
            case "java.lang.AbstractMethodError" -> new AbstractMethodError(message);
            case "java.lang.BootstrapMethodError" -> new BootstrapMethodError(message);
            case "java.lang.ClassCircularityError" -> new ClassCircularityError(message);
            case "java.lang.ClassFormatError" -> new ClassFormatError(message);
            case "java.lang.IllegalAccessError" -> new IllegalAccessError(message);
            case "java.lang.IncompatibleClassChangeError" -> new IncompatibleClassChangeError(message);
            case "java.lang.InstantiationError" -> new InstantiationError(message);
            case "java.lang.InternalError" -> new InternalError(message);
            case "java.lang.LinkageError" -> new LinkageError(message);
            case "java.lang.NoClassDefFoundError" -> new NoClassDefFoundError(message);
            case "java.lang.NoSuchFieldError" -> new NoSuchFieldError(message);
            case "java.lang.NoSuchMethodError" -> new NoSuchMethodError(message);
            case "java.lang.OutOfMemoryError" -> new OutOfMemoryError(message);
            case "java.lang.StackOverflowError" -> new StackOverflowError(message);
            case "java.lang.UnsatisfiedLinkError" -> new UnsatisfiedLinkError(message);
            default -> null;
        };
    }

    private static Throwable create(String className, String message, Throwable cause, boolean debug) {
        try {
            if (className.equals(InvocationTargetException.class.getName())) {
                return new InvocationTargetException(cause, message);
            }
            if (className.equals(ExceptionInInitializerError.class.getName())) {
                return new ExceptionInInitializerError(cause);
            }
            if (className.equals(AssertionError.class.getName())) {
                return initCause(new AssertionError(cause), cause,debug);
            }
            Throwable throwable = newThrowable(className, message);
            if (throwable != null) {
                return initCause(throwable, cause,debug);
            }
            return initCause(translationFailure("%s [%s]", message, className), cause, debug);
        } catch (Throwable translationFailure) {
            debugPrintStackTrace(translationFailure, debug);
            return initCause(translationFailure("%s [%s]", message, className), cause, debug);
        }
    }

    private static String emptyIfNull(String value) {
        return value == null ? "" : value;
    }

    private static String emptyAsNull(String value) {
        return value.isEmpty() ? null : value;
    }

    /**
     * Encodes {@code throwable} including its stack and causes as a {@linkplain GZIPOutputStream
     * compressed} byte array that can be decoded by {@link #decodeThrowable}.
     */
    static byte[] encodeThrowable(Throwable throwable) {
        try {
            return encodeThrowable(throwable, true);
        } catch (OutOfMemoryError e) {
            return FALLBACK_ENCODED_OUTOFMEMORYERROR_BYTES;
        } catch (Throwable e) {
            return FALLBACK_ENCODED_THROWABLE_BYTES;
        }
    }

    private static byte[] encodeThrowable(Throwable throwable,
                                          boolean withCauseAndStack) throws IOException {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        try (DataOutputStream dos = new DataOutputStream(new GZIPOutputStream(baos))) {
            List<Throwable> throwables = new ArrayList<>();
            for (Throwable current = throwable; current != null; current = current.getCause()) {
                throwables.add(current);
                if (!withCauseAndStack) {
                    break;
                }
            }

            // Encode from innermost cause outwards
            Collections.reverse(throwables);

            for (Throwable current : throwables) {
                dos.writeUTF(current.getClass().getName());
                dos.writeUTF(emptyIfNull(current.getMessage()));
                StackTraceElement[] stackTrace = withCauseAndStack ? current.getStackTrace() : null;
                if (stackTrace == null) {
                    stackTrace = new StackTraceElement[0];
                }
                dos.writeInt(stackTrace.length);
                for (StackTraceElement frame : stackTrace) {
                    if (frame != null) {
                        dos.writeUTF(emptyIfNull(frame.getClassLoaderName()));
                        dos.writeUTF(emptyIfNull(frame.getModuleName()));
                        dos.writeUTF(emptyIfNull(frame.getModuleVersion()));
                        dos.writeUTF(emptyIfNull(frame.getClassName()));
                        dos.writeUTF(emptyIfNull(frame.getMethodName()));
                        dos.writeUTF(emptyIfNull(frame.getFileName()));
                        dos.writeInt(frame.getLineNumber());
                    }
                }
            }
        }
        return baos.toByteArray();
    }

    /**
     * Gets the stack of the current thread as of the first native method. The chopped
     * frames are for the VM call to {@link VMSupport#decodeAndThrowThrowable}.
     */
    private static StackTraceElement[] getMyStackTrace() {
        Exception ex = new Exception();
        StackTraceElement[] stack = ex.getStackTrace();
        for (int i = 0; i < stack.length; i++) {
            StackTraceElement e = stack[i];
            if (e.isNativeMethod()) {
                return Arrays.copyOfRange(stack, i, stack.length);
            }
        }
        // This should never happen but since this is exception handling
        // code, be defensive instead raising a nested exception.
        return new StackTraceElement[0];
    }

    /**
     * Decodes {@code encodedThrowable} into a {@link TranslatedException}.
     *
     * @param encodedThrowable an encoded exception in the format specified by
     *            {@link #encodeThrowable}
     */
    static Throwable decodeThrowable(byte[] encodedThrowable, boolean debug) {
        ByteArrayInputStream bais = new ByteArrayInputStream(encodedThrowable);
        try (DataInputStream dis = new DataInputStream(new GZIPInputStream(bais))) {
            Throwable cause = null;
            Throwable throwable = null;
            StackTraceElement[] myStack = getMyStackTrace();
            while (dis.available() != 0) {
                String exceptionClassName = dis.readUTF();
                String exceptionMessage = emptyAsNull(dis.readUTF());
                throwable = create(exceptionClassName, exceptionMessage, cause, debug);
                int stackTraceDepth = dis.readInt();
                StackTraceElement[] stackTrace = new StackTraceElement[stackTraceDepth + myStack.length];
                int stackTraceIndex = 0;
                int myStackIndex = 0;
                for (int j = 0; j < stackTraceDepth; j++) {
                    String classLoaderName = emptyAsNull(dis.readUTF());
                    String moduleName = emptyAsNull(dis.readUTF());
                    String moduleVersion = emptyAsNull(dis.readUTF());
                    String className = emptyAsNull(dis.readUTF());
                    String methodName = emptyAsNull(dis.readUTF());
                    String fileName = emptyAsNull(dis.readUTF());
                    int lineNumber = dis.readInt();
                    StackTraceElement ste = new StackTraceElement(classLoaderName,
                                                                  moduleName,
                                                                  moduleVersion,
                                                                  className,
                                                                  methodName,
                                                                  fileName,
                                                                  lineNumber);

                    if (ste.isNativeMethod()) {
                        // Best effort attempt to weave stack traces from two heaps into
                        // a single stack trace using native method frames as stitching points.
                        // This is not 100% reliable as there's no guarantee that native method
                        // frames only exist for calls between HotSpot and libjvmci.
                        while (myStackIndex < myStack.length) {
                            StackTraceElement suffixSTE = myStack[myStackIndex++];
                            if (suffixSTE.isNativeMethod()) {
                                break;
                            }
                            stackTrace[stackTraceIndex++] = suffixSTE;
                        }
                    }
                    stackTrace[stackTraceIndex++] = ste;
                }
                while (myStackIndex < myStack.length) {
                    stackTrace[stackTraceIndex++] = myStack[myStackIndex++];
                }
                if (stackTraceIndex != stackTrace.length) {
                    // Remove null entries at end of stackTrace
                    stackTrace = Arrays.copyOf(stackTrace, stackTraceIndex);
                }
                if (dis.available() == 0) {
                    // Prepend the marker frame to the outermost stack trace
                    StackTraceElement[] newStackTrace = new StackTraceElement[stackTrace.length + 1];
                    System.arraycopy(stackTrace, 0, newStackTrace, 1, stackTrace.length);
                    newStackTrace[0] = TRANSLATED_MARKER;
                    stackTrace = newStackTrace;
                }
                throwable.setStackTrace(stackTrace);
                cause = throwable;
            }
            return throwable;
        } catch (Throwable translationFailure) {
            debugPrintStackTrace(translationFailure, debug);
            return translationFailure("error decoding exception: %s", Arrays.toString(encodedThrowable));
        }
    }
}
