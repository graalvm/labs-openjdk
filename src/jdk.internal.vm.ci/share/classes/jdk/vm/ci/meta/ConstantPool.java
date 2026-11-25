/*
 * Copyright (c) 2009, 2026, Oracle and/or its affiliates. All rights reserved.
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
package jdk.vm.ci.meta;

import java.lang.invoke.MethodType;
import java.util.List;

/**
 * Represents the runtime representation of the constant pool that is used by the compiler when
 * parsing bytecode. Provides methods to look up a constant pool entry without performing
 * resolution. They are used during compilation.
 *
 * The following convention is used when accessing the ConstantPool with an index:
 * <ul>
 * <li>rawIndex - index in the bytecode stream after the opcode (could be rewritten for some opcodes)</li>
 * <li>cpi - the constant pool index (as specified in JVM Spec)</li>
 * </ul>
 *
 * Some of the methods are currently not using the convention correctly. That will be addressed in JDK-8314172.
 */
public interface ConstantPool {

    /**
     * Returns the number of entries the constant pool.
     *
     * @return number of entries in the constant pool
     */
    int length();

    /**
     * Ensures that the type referenced by the specified constant pool entry is loaded and
     * initialized. This can be used to compile time resolve a type. It works for field, method, or
     * type constant pool entries.
     *
     * @param rawIndex index in the bytecode stream after the {@code opcode} (could be rewritten for some opcodes)
     * @param opcode the opcode of the instruction that references the type
     */
    void loadReferencedType(int rawIndex, int opcode);

    /**
     * Ensures that the type referenced by the specified constant pool entry is loaded. This can be
     * used to compile time resolve a type. It works for field, method, or type constant pool
     * entries.
     *
     * @param rawIndex index in the bytecode stream after the {@code opcode} (could be rewritten for some opcodes)
     * @param opcode the opcode of the instruction that references the type
     * @param initialize if {@code true}, the referenced type is either guaranteed to be initialized
     *            upon return or an initialization exception is thrown
     */
    default void loadReferencedType(int rawIndex, int opcode, boolean initialize) {
        if (initialize) {
            loadReferencedType(rawIndex, opcode);
        } else {
            throw new UnsupportedOperationException();
        }
    }

    /**
     * Looks up the type referenced by the {@code rawIndex}.
     * If {@code opcode} is {@code -1}, {@code rawIndex} is a constant pool index denoting a
     * class, field reference, method reference, interface method reference, method handle, or
     * dynamic constant. For a member reference or method handle, returns the symbolic holder
     * of the member reference, which may differ from the member's declaring class. For a
     * dynamic constant, returns its declared type. This lookup does not load classes or resolve
     * method handles; a holder that is not already loaded is returned as an unresolved type.
     *
     * @param rawIndex index in the bytecode stream after the {@code opcode} (could be rewritten
     *            for some opcodes), or a constant pool index if {@code opcode} is {@code -1}
     * @param opcode the opcode of the instruction with {@code rawIndex} as an operand, or
     *            {@code -1} for symbolic inspection by constant pool index
     * @return a reference to the compiler interface type
     */
    JavaType lookupReferencedType(int rawIndex, int opcode);

    /**
     * Looks up a reference to a field. Resolution checks
     * specific to the bytecode it denotes are performed if the field is already resolved. Checks
     * for some bytecodes require the method that contains the bytecode to be specified. Should any
     * of these checks fail, an unresolved field reference is returned.
     * If {@code opcode} is {@code -1}, {@code ldc}, or {@code ldc_w} and {@code rawIndex} denotes
     * a {@link Tag#METHODHANDLE} with a field reference kind, the underlying field is returned.
     * This lookup does not resolve the method handle or load its member holder. It can return a
     * resolved field based on the symbolic member reference even if the method handle is
     * unresolved, has failed, or will fail to resolve. Otherwise, an unresolved symbolic field is
     * returned. When the holder is already loaded, the VM field resolution step may resolve its
     * constant-pool entry if the current thread can run Java.
     *
     * @param rawIndex rewritten index in the bytecode stream after the {@code opcode}, or a
     *            constant pool index when {@code opcode} is {@code -1}, {@code ldc}, or
     *            {@code ldc_w}
     * @param opcode the opcode of the instruction for which the lookup is being performed, or
     *            {@code -1}
     * @param method the method for which the lookup is being performed
     * @return a reference to the field at {@code rawIndex} in this pool
     * @throws IllegalArgumentException if {@code opcode} is {@code -1}, {@code ldc}, or
     *             {@code ldc_w} and {@code rawIndex} does not denote a method handle that
     *             references a field
     * @throws IllegalArgumentException if {@code method} is null or is not a method from this
     *             JVMCI implementation
     */
    JavaField lookupField(int rawIndex, ResolvedJavaMethod method, int opcode);

    /**
     * Looks up a reference to a method. If {@code opcode} is non-negative, then resolution checks
     * specific to the bytecode it denotes are performed if the method is already resolved. Should
     * any of these checks fail, an unresolved method reference is returned.
     * If {@code opcode} is {@code -1}, {@code ldc}, or {@code ldc_w} and {@code cpi} denotes a
     * {@link Tag#METHODHANDLE} with a method reference kind, the underlying method is returned.
     * This lookup does not resolve the method handle or load its member holder. It can return a
     * resolved method based on the symbolic member reference even if the method handle is
     * unresolved, has failed, or will fail to resolve. Otherwise, an unresolved symbolic method
     * is returned. In particular, signature-polymorphic method handles may remain unresolved
     * because their invocation descriptor is not the descriptor of a single declared method.
     *
     * @param cpi rewritten index in the bytecode stream after the {@code opcode}, or a constant
     *            pool index when {@code opcode} is {@code -1}, {@code ldc}, or {@code ldc_w}
     * @param opcode the opcode of the instruction for which the lookup is being performed or
     *            {@code -1}
     * @return a reference to the method at {@code cpi} in this pool
     * @throws ClassFormatError if the entry at {@code cpi} is not a method
     * @throws IllegalArgumentException if {@code opcode} is {@code -1}, {@code ldc}, or
     *             {@code ldc_w} and {@code cpi} does not denote a method handle that references a
     *             method
     */
    default JavaMethod lookupMethod(int cpi, int opcode) {
        return lookupMethod(cpi, opcode, null);
    }

    /**
     * Looks up a reference to a method. If {@code opcode} is non-negative, then resolution checks
     * specific to the bytecode it denotes are performed if the method is already resolved. Should
     * any of these checks fail, an unresolved method reference is returned.
     *
     * @param cpi rewritten index in the bytecode stream after the {@code opcode}, or a constant
     *            pool index when {@code opcode} is {@code -1}, {@code ldc}, or {@code ldc_w}
     * @param opcode the opcode of the instruction for which the lookup is being performed or
     *            {@code -1}
     * @param caller if non-null, do access checks in the context of {@code caller} calling the
     *            looked up method
     * @return a reference to the method at {@code cpi} in this pool
     * @throws ClassFormatError if the entry at {@code cpi} is not a method
     * @throws IllegalAccessError if {@code caller} is non-null and it cannot link against the
     *             looked up method
     * @throws IllegalArgumentException if {@code opcode} is {@code -1}, {@code ldc}, or
     *             {@code ldc_w} and {@code cpi} does not denote a method handle that references a
     *             method
     */
    JavaMethod lookupMethod(int cpi, int opcode, ResolvedJavaMethod caller);

    /**
     * The details for invoking a bootstrap method associated with a {@code CONSTANT_Dynamic_info}
     * or {@code CONSTANT_InvokeDynamic_info} pool entry.
     *
     * @jvms 4.4.10 The {@code CONSTANT_Dynamic_info} and {@code CONSTANT_InvokeDynamic_info}
     *       Structures
     * @jvms 4.7.23 The {@code BootstrapMethods} Attribute
     */
    interface BootstrapMethodInvocation {
        /**
         * Gets the bootstrap method that will be invoked, or {@code null} if this invocation was
         * looked up without resolution and its method handle was not already resolved, or if the
         * method handle cannot be represented by a {@link ResolvedJavaMethod}. A {@code null}
         * result does not necessarily mean that the method handle is unresolved; an implementation
         * may resolve the handle without being able to associate it with a
         * {@link ResolvedJavaMethod}.
         * {@link #getMethodHandleIndex()} can be used to inspect the symbolic bootstrap method
         * handle in either case.
         */
        ResolvedJavaMethod getMethod();

        /**
         * Gets the constant pool index of the {@link Tag#METHODHANDLE} entry denoting the
         * bootstrap method. This index can be used with the symbolic lookup methods on
         * {@link ConstantPool} to inspect the method handle and its referenced member, including
         * {@link ConstantPool#lookupMethodHandleReferenceKind(int)},
         * {@link ConstantPool#lookupMethodHandleReferenceIndex(int)},
         * {@link ConstantPool#lookupMethod(int, int)}, and
         * {@link ConstantPool#lookupField(int, ResolvedJavaMethod, int)}.
         */
        int getMethodHandleIndex();

        /**
         * Returns {@code true} if this bootstrap method invocation is for a
         * {@code CONSTANT_InvokeDynamic_info} pool entry, {@code false} if it is for a
         * {@code CONSTANT_Dynamic_info} entry.
         */
        boolean isInvokeDynamic();

        /**
         * Gets the name of the pool entry.
         */
        String getName();

        /**
         * Returns a reference to the {@link MethodType} ({@code this.isInvokeDynamic() == true}) or
         * {@link Class} ({@code this.isInvokeDynamic() == false}) resolved for the descriptor of
         * the pool entry, or {@code null} if this invocation was looked up without resolution.
         */
        JavaConstant getType();

        /**
         * Gets the constant pool indices of the static arguments, in the order in which they
         * occur in the {@code BootstrapMethods} attribute. The returned array is a copy.
         *
         * @throws UnsupportedOperationException if the implementation does not support retrieving
         *             static argument indices
         */
        default int[] getStaticArgumentIndices() {
            throw new UnsupportedOperationException();
        }

        /// Gets the static arguments with which the bootstrap method will be invoked.
        ///
        /// The [kind][JavaConstant#getJavaKind()] of each argument will be
        /// [JavaKind#Object] or [JavaKind#Int]. The latter represents an
        /// unresolved `CONSTANT_Dynamic_info` entry. To resolve this entry, the
        /// corresponding bootstrap method has to be called first:
        /// ```
        /// List<JavaConstant> args = bmi.getStaticArguments();
        /// List<JavaConstant> resolvedArgs = new ArrayList<>(args.size());
        /// for (JavaConstant c : args) {
        ///     JavaConstant r = c;
        ///     if (c.getJavaKind() == JavaKind.Int) {
        ///         // If needed, access corresponding BootstrapMethodInvocation using
        ///         // cp.lookupBootstrapMethodInvocation(pc.asInt(), -1)
        ///         r = cp.lookupConstant(c.asInt(), true);
        ///     } else {
        ///         assert c.getJavaKind() == JavaKind.Object;
        ///     }
        ///     resolvedArgs.append(r);
        /// }
        /// ```
        /// The other types of entries are already resolved and can be used directly.
        ///
        /// @throws IllegalStateException if this invocation was looked up without resolution;
        ///         use `getStaticArgumentIndices()` instead
        /// @jvms 5.4.3.6
        List<JavaConstant> getStaticArguments();

        /**
         * Resolves the element represented by this bootstrap method invocation. If
         * {@code isInvokeDynamic()}, this resolves the associated call site, invoking its
         * bootstrap method as needed. If {@code !isInvokeDynamic()}, this resolves the dynamic
         * constant, invoking its bootstrap method as needed. This may execute arbitrary Java code
         * and may throw an {@link Error}, including a {@link BootstrapMethodError}.
         * <p>
         * Resolving an invokedynamic call site requires an index associated with an invokedynamic
         * instruction. An invocation looked up by constant pool index alone does not identify a
         * call site, so this method throws {@link IllegalStateException} in that case.
         *
         * @jvms 5.4.3.6
         */
        void resolve();

        /**
         * If {@code isInvokeDynamic()}, then this method looks up the corresponding
         * invoke dynamic's appendix. If {@code !isInvokeDynamic()}, then this will
         * return the constant pool entry's value. An invokedynamic entry looked up by constant
         * pool index alone does not identify a call site; calling this method in that case throws
         * {@link IllegalStateException}.
         */
        JavaConstant lookup();
    }

    /**
     * Gets the details for invoking a bootstrap method associated with the
     * {@code CONSTANT_Dynamic_info} or {@code CONSTANT_InvokeDynamic_info} pool entry
     * in the constant pool.
     * This is equivalent to calling the
     * {@link #lookupBootstrapMethodInvocation(int, int, boolean)} overload with {@code resolve}
     * set to {@code true}.
     *
     * @param index the operand of the bytecode specified by {@code opcode}, or a constant pool
     *            index if {@code opcode} is -1
     * @param opcode must be {@code Bytecodes.INVOKEDYNAMIC}, {@code Bytecodes.LDC},
     *            {@code Bytecodes.LDC_W}, or {@code Bytecodes.LDC2_W}; or -1 if {@code index} was
     *            not decoded from a bytecode stream
     * @return the bootstrap method invocation details or {@code null} if the entry specified by
     *         {@code index} is not a {@code CONSTANT_Dynamic_info} or
     *         {@code CONSTANT_InvokeDynamic_info}
     * @jvms 4.7.23 The {@code BootstrapMethods} Attribute
     */
    default BootstrapMethodInvocation lookupBootstrapMethodInvocation(int index, int opcode) {
        return lookupBootstrapMethodInvocation(index, opcode, true);
    }

    /**
     * Gets the details for invoking a bootstrap method. When {@code resolve} is false, this
     * method does not cause classes to be loaded or arbitrary Java code to be called. The method
     * can be {@code null} when it is not already safely available. The type is {@code null} when
     * {@code resolve} is false. When {@code resolve} is false, static argument indices are available
     * through {@link BootstrapMethodInvocation#getStaticArgumentIndices()}. The bootstrap method
     * handle entry can be inspected symbolically even if its resolution has failed or will fail.
     * An invokedynamic entry looked up by constant pool index alone provides bootstrap method
     * details but does not identify a call site for {@link BootstrapMethodInvocation#resolve()}
     * or {@link BootstrapMethodInvocation#lookup()}.
     *
     * @param index as for {@link #lookupBootstrapMethodInvocation(int, int)}
     * @param opcode as for {@link #lookupBootstrapMethodInvocation(int, int)}
     * @param resolve whether bootstrap method details may be resolved
     * @return the bootstrap method invocation details or null if the entry is neither a
     *         {@code CONSTANT_Dynamic_info} nor a {@code CONSTANT_InvokeDynamic_info}
     */
    default BootstrapMethodInvocation lookupBootstrapMethodInvocation(int index, int opcode, boolean resolve) {
        throw new UnsupportedOperationException();
    }

    /**
     * Returns either the BootstrapMethodInvocation instances for all invokedynamic
     * bytecodes which reference this constant pool, or all
     * {@code CONSTANT_Dynamic_info} BootstrapMethodInvocations within this constant
     * pool. The returned List is unmodifiable; calls to any mutator method will
     * always cause {@code UnsupportedOperationException} to be thrown.
     *
     * @param invokeDynamic when true, return all invokedynamic
     *                      BootstrapMethodInvocations; otherwise, return all
     *                      {@code CONSTANT_Dynamic_info}
     *                      BootstrapMethodInvocations.
     */
    List<BootstrapMethodInvocation> lookupBootstrapMethodInvocations(boolean invokeDynamic);

    /**
     * Looks up a reference to a type. If {@code opcode} is non-negative, then resolution checks
     * specific to the bytecode it denotes are performed if the type is already resolved. Should any
     * of these checks fail, an unresolved type reference is returned.
     *
     * @param cpi the constant pool index
     * @param opcode the opcode of the instruction for which the lookup is being performed or
     *            {@code -1}
     * @return a reference to the compiler interface type
     */
    JavaType lookupType(int cpi, int opcode);

    /**
     * Looks up an Utf8 string.
     *
     * @param cpi the constant pool index
     * @return the Utf8 string at index {@code cpi} in this constant pool
     */
    String lookupUtf8(int cpi);

    /**
     * Looks up the name of a constant pool entry without resolving the entry.
     *
     * This method supports {@link Tag#FIELD_REF}, {@link Tag#METHOD_REF},
     * {@link Tag#INTERFACE_METHOD_REF}, {@link Tag#METHODHANDLE}, {@link Tag#DYNAMIC}, and
     * {@link Tag#INVOKEDYNAMIC} entries.
     *
     * @param rawIndex index in the bytecode stream after the {@code opcode} (could be rewritten
     *            for some opcodes), or a constant pool index if {@code opcode} is {@code -1}
     * @param opcode the opcode of the instruction for which the lookup is being performed, or
     *            {@code -1} if {@code rawIndex} is a constant pool index
     * @return the name at the entry referenced by {@code rawIndex}
     * @throws IllegalArgumentException if the entry referenced by {@code rawIndex} has no name
     */
    String lookupName(int rawIndex, int opcode);

    /**
     * Looks up a method signature.
     *
     * In addition to a {@link Tag#UTF8} entry, this method supports {@link Tag#METHOD_REF},
     * {@link Tag#INTERFACE_METHOD_REF}, method-related {@link Tag#METHODHANDLE},
     * {@link Tag#METHODTYPE}, and {@link Tag#INVOKEDYNAMIC} entries. The result is obtained from
     * the constant pool without resolving the entry. For a method handle, this is the descriptor
     * of its referenced member, not the adapted method handle type.
     *
     * @param rawIndex index in the bytecode stream after the {@code opcode} (could be rewritten
     *            for some opcodes), or a constant pool index if {@code opcode} is {@code -1}
     * @param opcode the opcode of the instruction for which the lookup is being performed, or
     *            {@code -1} if {@code rawIndex} is a constant pool index
     * @return the method signature at the entry referenced by {@code rawIndex}
     * @throws IllegalArgumentException if the entry referenced by {@code rawIndex} has no method
     *             signature, or if {@code opcode} denotes a field access
     */
    Signature lookupSignature(int rawIndex, int opcode);

    /**
     * Looks up a method signature using a constant pool index.
     *
     * @param cpi the constant pool index
     * @return the method signature at the entry referenced by {@code cpi}
     */
    default Signature lookupSignature(int cpi) {
        return lookupSignature(cpi, -1);
    }

    /**
     * Looks up the reference kind of a {@link Tag#METHODHANDLE} entry without resolving the
     * entry. The returned value is one of the {@code REF_*} constants in
     * {@link java.lang.invoke.MethodHandleInfo}.
     *
     * @param cpi the constant pool index
     * @return the method handle reference kind at {@code cpi}
     * @throws IllegalArgumentException if the entry at {@code cpi} is not a method handle
     */
    int lookupMethodHandleReferenceKind(int cpi);

    /**
     * Looks up the reference index of a {@link Tag#METHODHANDLE} entry without resolving the
     * entry. The returned index denotes a {@link Tag#FIELD_REF}, {@link Tag#METHOD_REF}, or
     * {@link Tag#INTERFACE_METHOD_REF} entry, as determined by the method handle's reference kind.
     *
     * @param cpi the constant pool index
     * @return the constant pool index of the member reference at {@code cpi}
     * @throws IllegalArgumentException if the entry at {@code cpi} is not a method handle
     */
    int lookupMethodHandleReferenceIndex(int cpi);

    /**
     * Looks up a constant at the specified index.
     *
     * @param cpi the constant pool index
     * @return the {@code Constant} or {@code JavaType} instance representing the constant pool
     *         entry
     */
    Object lookupConstant(int cpi);

    /**
     * Looks up a constant at the specified index.
     *
     * If {@code resolve == false} and the denoted constant is of type
     * {@code JVM_CONSTANT_Dynamic}, {@code JVM_CONSTANT_MethodHandle} or
     * {@code JVM_CONSTANT_MethodType} and it's not yet resolved then
     * {@code null} is returned.
     *
     * @param cpi the constant pool index
     * @return the {@code Constant} or {@code JavaType} instance representing the constant pool
     *         entry
     */
    Object lookupConstant(int cpi, boolean resolve);

    /**
     * Looks up the appendix at the specified index.
     *
     * @param rawIndex index in the bytecode stream after the {@code opcode} (could be rewritten for some opcodes)
     * @param opcode the opcode of the instruction for which the lookup is being performed
     * @return the appendix if it exists and is resolved or {@code null}
     */
    JavaConstant lookupAppendix(int rawIndex, int opcode);

    /**
     * Decodes a bytecode operand to the index of its entry in this constant pool. The operand
     * may have been rewritten by the VM. For instructions whose operands are constant pool
     * indices already, returns {@code rawIndex} unchanged. This does not resolve the referenced
     * entry or call site, or invoke a bootstrap method.
     *
     * @param rawIndex the operand as returned in {@link ResolvedJavaMethod#getCode()}
     * @param opcode an opcode that has a constant pool index operand
     * @return the constant pool index of the referenced entry
     */
    int decodeIndex(int rawIndex, int opcode);

    /**
     * Gets the tag of the constant pool entry at {@code cpi}.
     *
     * @param cpi the constant pool index
     * @return the tag of the entry at {@code cpi}, or {@link Tag#INVALID} if {@code cpi} is the
     *         unusable slot following a {@link Tag#LONG} or {@link Tag#DOUBLE} entry
     * @throws IndexOutOfBoundsException if {@code cpi} is not a valid constant pool index
     */
    Tag getTag(int cpi);

    /**
     * The tags for entries in a class file constant pool.
     *
     * {@link #INVALID} represents an unusable constant pool slot following a {@link #LONG} or
     * {@link #DOUBLE} entry.
     *
     * @jvms 4.4 The {@code Constant Pool}
     */
    enum Tag {
        INVALID(0),
        UTF8(1),
        INTEGER(3),
        FLOAT(4),
        LONG(5),
        DOUBLE(6),
        CLASS(7),
        STRING(8),
        FIELD_REF(9),
        METHOD_REF(10),
        INTERFACE_METHOD_REF(11),
        NAME_AND_TYPE(12),
        METHODHANDLE(15),
        METHODTYPE(16),
        DYNAMIC(17),
        INVOKEDYNAMIC(18),
        MODULE(19),
        PACKAGE(20);

        private final byte value;

        Tag(int value) {
            assert (byte) value == value;
            this.value = (byte) value;
        }

        /**
         * Gets the numeric value of this tag.
         *
         * @return the numeric value of this tag
         */
        public final byte getValue() {
            return value;
        }

        /**
         * Gets the tag with a given numeric value.
         *
         * @param value a numeric constant pool tag value
         * @return the tag with the given value
         * @throws IllegalArgumentException if {@code value} is not a recognized constant-pool tag
         */
        public static Tag fromValue(int value) {
            Tag tag = switch (value) {
                case 0 -> INVALID;
                case 1 -> UTF8;
                case 3 -> INTEGER;
                case 4 -> FLOAT;
                case 5 -> LONG;
                case 6 -> DOUBLE;
                case 7 -> CLASS;
                case 8 -> STRING;
                case 9 -> FIELD_REF;
                case 10 -> METHOD_REF;
                case 11 -> INTERFACE_METHOD_REF;
                case 12 -> NAME_AND_TYPE;
                case 15 -> METHODHANDLE;
                case 16 -> METHODTYPE;
                case 17 -> DYNAMIC;
                case 18 -> INVOKEDYNAMIC;
                case 19 -> MODULE;
                case 20 -> PACKAGE;
                default -> throw new IllegalArgumentException("Unknown tag value: " + value);
            };
            assert tag.value == value;
            return tag;
        }
    }
}
