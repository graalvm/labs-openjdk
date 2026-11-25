/*
 * Copyright (c) 2011, 2026, Oracle and/or its affiliates. All rights reserved.
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

import java.lang.invoke.MethodHandleInfo;
import java.util.AbstractList;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

import jdk.vm.ci.common.JVMCIError;
import static jdk.vm.ci.hotspot.CompilerToVM.compilerToVM;
import static jdk.vm.ci.hotspot.HotSpotJVMCIRuntime.runtime;
import static jdk.vm.ci.hotspot.HotSpotVMConfig.config;
import static jdk.vm.ci.hotspot.UnsafeAccess.UNSAFE;
import jdk.vm.ci.meta.ConstantPool;
import jdk.vm.ci.meta.JavaConstant;
import jdk.vm.ci.meta.JavaField;
import jdk.vm.ci.meta.JavaMethod;
import jdk.vm.ci.meta.JavaType;
import jdk.vm.ci.meta.PrimitiveConstant;
import jdk.vm.ci.meta.ResolvedJavaMethod;
import jdk.vm.ci.meta.ResolvedJavaType;
import jdk.vm.ci.meta.Signature;
import jdk.vm.ci.meta.UnresolvedJavaField;
import jdk.vm.ci.meta.UnresolvedJavaMethod;
import jdk.vm.ci.meta.UnresolvedJavaType;

/**
 * Implementation of {@link ConstantPool} for HotSpot.
 *
 * The following convention is used in the jdk.vm.ci.hotspot package when accessing the ConstantPool with an index:
 * <ul>
 * <li>rawIndex - Index in the bytecode stream after the opcode (could be rewritten for some opcodes)</li>
 * <li>cpi -      The constant pool index (as specified in JVM Spec)</li>
 * <li>cpci -     The constant pool cache index, used only by the four bytecodes INVOKE{VIRTUAL,SPECIAL,STATIC,INTERFACE}.
 *                It's the same as {@code rawIndex}. </li>
 * <li>which -    May be either a {@code rawIndex} or a {@code cpci}.</li>
 * </ul>
 *
 * Note that {@code cpci} and {@code which} are used only in the HotSpot-specific implementation. They
 * are not used by the public interface in jdk.vm.ci.meta.*.
 * After JDK-8301993, all uses of {@code cpci} and {@code which} will be replaced with {@code rawIndex}.
 */
public final class HotSpotConstantPool implements ConstantPool, MetaspaceHandleObject {

    /**
     * Subset of JVM bytecode opcodes used by {@link HotSpotConstantPool}.
     */
    public static class Bytecodes {
        public static final int LDC = 18; // 0x12
        public static final int LDC_W = 19; // 0x13
        public static final int LDC2_W = 20; // 0x14
        public static final int GETSTATIC = 178; // 0xB2
        public static final int PUTSTATIC = 179; // 0xB3
        public static final int GETFIELD = 180; // 0xB4
        public static final int PUTFIELD = 181; // 0xB5
        public static final int INVOKEVIRTUAL = 182; // 0xB6
        public static final int INVOKESPECIAL = 183; // 0xB7
        public static final int INVOKESTATIC = 184; // 0xB8
        public static final int INVOKEINTERFACE = 185; // 0xB9
        public static final int INVOKEDYNAMIC = 186; // 0xBA
        public static final int NEW = 187; // 0xBB
        public static final int NEWARRAY = 188; // 0xBC
        public static final int ANEWARRAY = 189; // 0xBD
        public static final int CHECKCAST = 192; // 0xC0
        public static final int INSTANCEOF = 193; // 0xC1
        public static final int MULTIANEWARRAY = 197; // 0xC5

        static boolean isInvoke(int opcode) {
            switch (opcode) {
                case INVOKEVIRTUAL:
                case INVOKESPECIAL:
                case INVOKESTATIC:
                case INVOKEINTERFACE:
                case INVOKEDYNAMIC:
                    return true;
                default:
                    return false;
            }
        }

        /**
         * See: {@code Rewriter::maybe_rewrite_invokehandle}.
         */
        static boolean isInvokeHandleAlias(int opcode) {
            switch (opcode) {
                case INVOKEVIRTUAL:
                case INVOKESPECIAL:
                    return true;
                default:
                    return false;
            }
        }
    }

    static final class JvmConstant {
        private final int tag;
        private final String name;
        private final Tag tagEnum;

        JvmConstant(int tag, String name, Tag tagEnum) {
            this.tag = tag;
            this.name = name;
            this.tagEnum = tagEnum;
        }

        @Override
        public String toString() {
            return name;
        }
    }

    /**
     * {@code JVM_CONSTANT} constants used in the VM including both public and internal ones.
     */
    static final class JvmConstants {

        private final HotSpotVMConfig c = config();
        private final int externalMax = c.jvmConstantExternalMax;
        private final int internalMax = c.jvmConstantInternalMax;
        private final int internalMin = c.jvmConstantInternalMin;
        private final JvmConstant[] table = new JvmConstant[externalMax + 1 + (internalMax - internalMin) + 1];

        final JvmConstant jvmUtf8 = add(new JvmConstant(c.jvmConstantUtf8, "Utf8", Tag.UTF8));
        final JvmConstant jvmInteger = add(new JvmConstant(c.jvmConstantInteger, "Integer", Tag.INTEGER));
        final JvmConstant jvmLong = add(new JvmConstant(c.jvmConstantLong, "Long", Tag.LONG));
        final JvmConstant jvmFloat = add(new JvmConstant(c.jvmConstantFloat, "Float", Tag.FLOAT));
        final JvmConstant jvmDouble = add(new JvmConstant(c.jvmConstantDouble, "Double", Tag.DOUBLE));
        final JvmConstant jvmClass = add(new JvmConstant(c.jvmConstantClass, "Class", Tag.CLASS));
        final JvmConstant jvmUnresolvedClass = add(new JvmConstant(c.jvmConstantUnresolvedClass, "UnresolvedClass", Tag.CLASS));
        final JvmConstant jvmUnresolvedClassInError = add(new JvmConstant(c.jvmConstantUnresolvedClassInError, "UnresolvedClassInError", Tag.CLASS));
        final JvmConstant jvmString = add(new JvmConstant(c.jvmConstantString, "String", Tag.STRING));
        final JvmConstant jvmFieldref = add(new JvmConstant(c.jvmConstantFieldref, "Fieldref", Tag.FIELD_REF));
        final JvmConstant jvmMethodref = add(new JvmConstant(c.jvmConstantMethodref, "Methodref", Tag.METHOD_REF));
        final JvmConstant jvmInterfaceMethodref = add(new JvmConstant(c.jvmConstantInterfaceMethodref, "InterfaceMethodref", Tag.INTERFACE_METHOD_REF));
        final JvmConstant jvmNameAndType = add(new JvmConstant(c.jvmConstantNameAndType, "NameAndType", Tag.NAME_AND_TYPE));
        final JvmConstant jvmMethodHandle = add(new JvmConstant(c.jvmConstantMethodHandle, "MethodHandle", Tag.METHODHANDLE));
        final JvmConstant jvmMethodHandleInError = add(new JvmConstant(c.jvmConstantMethodHandleInError, "MethodHandleInError", Tag.METHODHANDLE));
        final JvmConstant jvmMethodType = add(new JvmConstant(c.jvmConstantMethodType, "MethodType", Tag.METHODTYPE));
        final JvmConstant jvmMethodTypeInError = add(new JvmConstant(c.jvmConstantMethodTypeInError, "MethodTypeInError", Tag.METHODTYPE));
        final JvmConstant jvmInvokeDynamic = add(new JvmConstant(c.jvmConstantInvokeDynamic, "InvokeDynamic", Tag.INVOKEDYNAMIC));
        final JvmConstant jvmDynamic = add(new JvmConstant(c.jvmConstantDynamic, "Dynamic", Tag.DYNAMIC));
        final JvmConstant jvmDynamicInError = add(new JvmConstant(c.jvmConstantDynamicInError, "DynamicInError", Tag.DYNAMIC));

        private JvmConstant add(JvmConstant constant) {
            table[indexOf(constant.tag)] = constant;
            return constant;
        }

        private int indexOf(int tag) {
            if (tag >= internalMin) {
                return tag - internalMin + externalMax + 1;
            } else {
                assert tag <= externalMax;
            }
            return tag;
        }

        JvmConstant get(int tag) {
            JvmConstant res = table[indexOf(tag)];
            if (res != null) {
                return res;
            }
            throw new JVMCIError("Unknown JvmConstant tag %s", tag);
        }

        private static volatile JvmConstants instance;

        static JvmConstants instance() {
            JvmConstants result = instance;
            if (result == null) {
                synchronized (JvmConstants.class) {
                    result = instance;
                    if (result == null) {
                        instance = result = new JvmConstants();
                    }
                }
            }
            return result;
        }
    }

    private static class LookupTypeCacheElement {
        int lastCpi = Integer.MIN_VALUE;
        JavaType javaType;

        LookupTypeCacheElement(int lastCpi, JavaType javaType) {
            super();
            this.lastCpi = lastCpi;
            this.javaType = javaType;
        }
    }

    /**
     * A {@code jmetadata} value that is a handle to {@code ConstantPool*} value.
     */
    private final long constantPoolHandle;

    private volatile LookupTypeCacheElement lastLookupType;
    private final JvmConstants constants;

    /**
     * Cache for {@link #getHolder()}.
     */
    private HotSpotResolvedObjectTypeImpl holder;

    /**
     * Gets the JVMCI mirror from a HotSpot constant pool. The VM is responsible for ensuring that
     * the ConstantPool is kept alive for the duration of this call and the
     * {@link HotSpotJVMCIRuntime} keeps it alive after that.
     *
     * Called from the VM.
     *
     * @param constantPoolHandle a {@code jmetaspace} handle to a raw {@code ConstantPool*} value
     * @return the {@link HotSpotConstantPool} corresponding to {@code constantPoolHandle}
     */
    @SuppressWarnings("unused")
    @VMEntryPoint
    private static HotSpotConstantPool fromMetaspace(long constantPoolHandle) {
        return new HotSpotConstantPool(constantPoolHandle);
    }

    private HotSpotConstantPool(long constantPoolHandle) {
        this.constantPoolHandle = constantPoolHandle;
        this.constants = JvmConstants.instance();
        HandleCleaner.create(this, constantPoolHandle);
    }

    /**
     * Gets the holder for this constant pool as {@link HotSpotResolvedObjectTypeImpl}.
     *
     * @return holder for this constant pool
     */
    private HotSpotResolvedObjectType getHolder() {
        if (holder == null) {
            holder = compilerToVM().getResolvedJavaType(this, config().constantPoolHolderOffset);
        }
        return holder;
    }

    /**
     * Gets the raw {@code ConstantPool*} value for the this constant pool.
     */
    long getConstantPoolPointer() {
        return getMetaspacePointer();
    }

    @Override
    public long getMetadataHandle() {
        return constantPoolHandle;
    }

    /**
     * Gets the constant pool tag at index {@code index}.
     *
     * @param index constant pool index
     * @return constant pool tag
     */
    private JvmConstant getTagAt(int index) {
        checkBounds(index);
        HotSpotVMConfig config = config();
        final long metaspaceConstantPoolTags = UNSAFE.getAddress(getConstantPoolPointer() + config.constantPoolTagsOffset);
        final int tag = UNSAFE.getByteVolatile(null, metaspaceConstantPoolTags + config.arrayU1DataOffset + index);
        if (tag == 0) {
            return null;
        }
        return constants.get(tag);
    }

    /**
     * Gets the constant pool entry at index {@code index}.
     *
     * @param index constant pool index
     * @return constant pool entry
     */
    long getEntryAt(int index) {
        checkBounds(index);
        int offset = index * runtime().getHostJVMCIBackend().getTarget().wordSize;
        return UNSAFE.getAddress(getConstantPoolPointer() + config().constantPoolSize + offset);
    }

    /**
     * Gets the raw 32-bit value stored in a constant pool entry.
     *
     * @param index constant pool index
     * @return raw constant pool entry value
     */
    private int getEntryValueAt(int index) {
        int offset = index * runtime().getHostJVMCIBackend().getTarget().wordSize;
        return UNSAFE.getInt(getConstantPoolPointer() + config().constantPoolSize + offset);
    }

    /**
     * Gets the integer constant pool entry at index {@code index}.
     *
     * @param index constant pool index
     * @return integer constant pool entry at index
     */
    private int getIntAt(int index) {
        checkTag(index, constants.jvmInteger);
        return getEntryValueAt(index);
    }

    /**
     * Gets the long constant pool entry at index {@code index}.
     *
     * @param index constant pool index
     * @return long constant pool entry
     */
    private long getLongAt(int index) {
        checkTag(index, constants.jvmLong);
        int offset = index * runtime().getHostJVMCIBackend().getTarget().wordSize;
        return UNSAFE.getLong(getConstantPoolPointer() + config().constantPoolSize + offset);
    }

    /**
     * Gets the float constant pool entry at index {@code index}.
     *
     * @param index constant pool index
     * @return float constant pool entry
     */
    private float getFloatAt(int index) {
        checkTag(index, constants.jvmFloat);
        int offset = index * runtime().getHostJVMCIBackend().getTarget().wordSize;
        return UNSAFE.getFloat(getConstantPoolPointer() + config().constantPoolSize + offset);
    }

    /**
     * Gets the double constant pool entry at index {@code index}.
     *
     * @param index constant pool index
     * @return float constant pool entry
     */
    private double getDoubleAt(int index) {
        checkTag(index, constants.jvmDouble);
        int offset = index * runtime().getHostJVMCIBackend().getTarget().wordSize;
        return UNSAFE.getDouble(getConstantPoolPointer() + config().constantPoolSize + offset);
    }

    /**
     * Gets the {@code JVM_CONSTANT_NameAndType} constant pool entry at index {@code index}.
     *
     * @param index constant pool index
     * @return {@code JVM_CONSTANT_NameAndType} constant pool entry
     */
    private int getNameAndTypeAt(int index) {
        checkTag(index, constants.jvmNameAndType);
        return getEntryValueAt(index);
    }

    /**
     * Gets the {@code JVM_CONSTANT_NameAndType} reference index constant pool entry at index
     * {@code index}.
     *
     * @param rawIndex rewritten index in the bytecode stream
     * @param opcode the opcode of the instruction for which the lookup is being performed
     * @return {@code JVM_CONSTANT_NameAndType} reference constant pool entry
     */
    private int getNameAndTypeRefIndexAt(int rawIndex, int opcode) {
        return compilerToVM().lookupNameAndTypeRefIndexInPool(this, rawIndex, opcode);
    }

    /**
     * Gets the name reference index of a {@code JVM_CONSTANT_NameAndType} constant pool entry at
     * index {@code index}.
     *
     * @param index constant pool index
     * @return name reference index
     */
    private int getNameRefIndexAt(int index) {
        final int refIndex = getNameAndTypeAt(index);
        // name ref index is in the low 16-bits.
        return refIndex & 0xFFFF;
    }


    /**
     * Gets the signature reference index of a {@code JVM_CONSTANT_NameAndType} constant pool entry
     * at index {@code index}.
     *
     * @param index constant pool index
     * @return signature reference index
     */
    private int getSignatureRefIndexAt(int index) {
        final int refIndex = getNameAndTypeAt(index);
        // signature ref index is in the high 16-bits.
        return refIndex >>> 16;
    }

    /**
     * Gets the NameAndType index stored in a field, method, Dynamic, or InvokeDynamic entry at
     * {@code cpi} without applying bytecode-index decoding.
     */
    private int getNameAndTypeRefIndexAt(int cpi) {
        Tag tag = getTag(cpi);
        if (tag != Tag.FIELD_REF && tag != Tag.METHOD_REF && tag != Tag.INTERFACE_METHOD_REF && tag != Tag.DYNAMIC && tag != Tag.INVOKEDYNAMIC) {
            throw new IllegalArgumentException("constant pool tag at index " + cpi + " is " + tag + " but has no NameAndType entry");
        }
        return getEntryValueAt(cpi) >>> 16;
    }

    /**
     * Gets the raw value stored in a {@code JVM_CONSTANT_MethodHandle} entry at {@code cpi}.
     */
    private int getMethodHandleAt(int cpi) {
        Tag tag = getTag(cpi);
        if (tag != Tag.METHODHANDLE) {
            throw new IllegalArgumentException("constant pool tag at index " + cpi + " is " + tag + " but expected METHODHANDLE");
        }
        return getEntryValueAt(cpi);
    }

    /**
     * Gets the referenced constant pool index stored in a {@code JVM_CONSTANT_MethodHandle} entry
     * at {@code cpi}.
     */
    private int getMethodHandleRefIndexAt(int cpi) {
        return getMethodHandleAt(cpi) >>> 16;
    }

    /**
     * Gets the reference kind stored in a {@code JVM_CONSTANT_MethodHandle} entry at {@code cpi}.
     */
    private int getMethodHandleReferenceKindAt(int cpi) {
        return getMethodHandleAt(cpi) & 0xFFFF;
    }

    /**
     * Gets the UTF8 index stored in a {@code JVM_CONSTANT_MethodType} entry at {@code cpi}.
     */
    private int getMethodTypeSignatureIndexAt(int cpi) {
        if (getTag(cpi) != Tag.METHODTYPE) {
            throw new IllegalArgumentException("constant pool tag at index " + cpi + " is " + getTag(cpi) + " but expected METHODTYPE");
        }
        return getEntryValueAt(cpi);
    }

    /**
     * Gets the klass reference index constant pool entry at index {@code index}.
     *
     * @param which  for INVOKE{VIRTUAL,SPECIAL,STATIC,INTERFACE}, must be {@code cpci}. For all other bytecodes,
     *               must be {@code rawIndex}
     * @param opcode the opcode of the instruction for which the lookup is being performed
     * @return klass reference index
     */
    private int getKlassRefIndexAt(int which, int opcode) {
        return compilerToVM().lookupKlassRefIndexInPool(this, which, opcode);
    }

    /**
     * Gets the uncached klass reference index constant pool entry at index {@code index}. See:
     * {@code ConstantPool::uncached_klass_ref_index_at}.
     *
     * @param index constant pool index
     * @return klass reference index
     */
    private int getUncachedKlassRefIndexAt(int index) {
        checkTagIsFieldOrMethod(index);
        int offset = index * runtime().getHostJVMCIBackend().getTarget().wordSize;
        final int refIndex = UNSAFE.getInt(getConstantPoolPointer() + config().constantPoolSize + offset);
        // klass ref index is in the low 16-bits.
        return refIndex & 0xFFFF;
    }

    /**
     * Checks that the constant pool index {@code index} is in the bounds of the constant pool.
     *
     * @param index constant pool index
     * @throws IndexOutOfBoundsException if the check fails
     */
    private void checkBounds(int index) {
        if (index < 1 || index >= length()) {
            throw new IndexOutOfBoundsException("index " + index + " not between 1 and " + length());
        }
    }

    /**
     * Checks that the constant pool tag at index {@code index} is equal to {@code tag}.
     *
     * @param index constant pool index
     * @param tag expected tag
     * @throws IllegalArgumentException if the check fails
     */
    private void checkTag(int index, JvmConstant tag) {
        final JvmConstant tagAt = getTagAt(index);
        if (tagAt != tag) {
            throw new IllegalArgumentException("constant pool tag at index " + index + " is " + tagAt + " but expected " + tag);
        }
    }

    /**
     * Asserts that the constant pool tag at index {@code index} is a
     * {@link JvmConstants#jvmFieldref}, or a {@link JvmConstants#jvmMethodref}, or a
     * {@link JvmConstants#jvmInterfaceMethodref}.
     *
     * @param index constant pool index
     * @throws IllegalArgumentException if the check fails
     */
    private void checkTagIsFieldOrMethod(int index) {
        final JvmConstant tagAt = getTagAt(index);
        if (tagAt != constants.jvmFieldref && tagAt != constants.jvmMethodref && tagAt != constants.jvmInterfaceMethodref) {
            throw new IllegalArgumentException("constant pool tag at index " + index + " is " + tagAt);
        }
    }

    @Override
    public int length() {
        return UNSAFE.getInt(getConstantPoolPointer() + config().constantPoolLengthOffset);
    }

    public boolean hasDynamicConstant() {
        return (flags() & config().constantPoolHasDynamicConstant) != 0;
    }

    private int flags() {
        return UNSAFE.getInt(getConstantPoolPointer() + config().constantPoolFlagsOffset);
    }

    /**
     * A lazily populated list of static arguments for a {@link BootstrapMethodInvocation}. The
     * constant pool indices come from
     * {@link BootstrapMethodInvocation#getStaticArgumentIndices()}; each argument is looked up
     * without forcing resolution and cached when requested. If an argument cannot be looked up,
     * its constant pool index is returned as a {@link PrimitiveConstant}.
     */
    static class CachedBSMArgs extends AbstractList<JavaConstant> {
        private final JavaConstant[] cache;
        private final HotSpotConstantPool cp;
        private final int[] argumentIndices;

        CachedBSMArgs(HotSpotConstantPool cp, int[] argumentIndices) {
            this.cp = cp;
            this.argumentIndices = argumentIndices;
            this.cache = new JavaConstant[argumentIndices.length];
        }

        /**
         * Looks up and caches the argument at the given index and returns it. The method
         * {@link ConstantPool.BootstrapMethodInvocation#getStaticArgumentIndices()} supplies the
         * constant pool index of the entry and {@link ConstantPool#lookupConstant} looks it up.
         * If the constant is unresolved, the index is returned as a {@link PrimitiveConstant}.
         *
         * @param index index of the element to return
         * @return A {@link JavaConstant} corresponding to the static argument requested. A return
         * value of type {@link PrimitiveConstant} represents an unresolved constant pool entry
         */
        @Override
        public JavaConstant get(int index) {
            JavaConstant res = cache[index];
            if (res == null) {
                int argCpi = argumentIndices[index];
                Object object = cp.lookupConstant(argCpi, false);
                res = switch (object) {
                    case PrimitiveConstant primitiveConstant ->
                            runtime().getReflection().boxPrimitive(primitiveConstant);
                    case JavaConstant javaConstant -> javaConstant;
                    case HotSpotResolvedJavaType hsType -> hsType.getJavaMirror();
                    case null, default -> JavaConstant.forInt(argCpi);
                };
                cache[index] = res;
            }
            return res;
        }

        @Override
        public int size() {
            return cache.length;
        }
    }

    class BootstrapMethodInvocationImpl implements BootstrapMethodInvocation {
        private final boolean indy;
        private final ResolvedJavaMethod method;
        private final String name;
        private final JavaConstant type;
        private final List<JavaConstant> staticArguments;
        private final int[] staticArgumentIndices;
        private final int cpiOrIndyIndex;
        private final int methodHandleIndex;

        BootstrapMethodInvocationImpl(boolean indy, ResolvedJavaMethod method, String name, JavaConstant type,
                        List<JavaConstant> staticArguments, int[] staticArgumentIndices, int cpiOrIndyIndex, int methodHandleIndex) {
            this.indy = indy;
            this.method = method;
            this.name = name;
            this.type = type;
            this.staticArguments = staticArguments;
            this.staticArgumentIndices = staticArgumentIndices;
            this.cpiOrIndyIndex = cpiOrIndyIndex;
            this.methodHandleIndex = methodHandleIndex;
        }

        @Override
        public ResolvedJavaMethod getMethod() {
            return method;
        }

        @Override
        public int getMethodHandleIndex() {
            return methodHandleIndex;
        }

        @Override
        public String getName() {
            return name;
        }

        @Override
        public boolean isInvokeDynamic() {
            return indy;
        }

        @Override
        public JavaConstant getType() {
            return type;
        }

        @Override
        public List<JavaConstant> getStaticArguments() {
            if (staticArguments == null) {
                throw new IllegalStateException("Static arguments are unavailable for a non-resolving bootstrap method invocation");
            }
            return staticArguments;
        }

        @Override
        public int[] getStaticArgumentIndices() {
            return staticArgumentIndices.clone();
        }

        @Override
        public void resolve() {
            if (isInvokeDynamic()) {
                if (cpiOrIndyIndex < 0) {
                    throw new IllegalStateException("No rewritten invokedynamic index is available");
                }
                loadReferencedType(cpiOrIndyIndex, Bytecodes.INVOKEDYNAMIC);
            } else {
                lookupConstant(cpiOrIndyIndex, true);
            }
        }

        @Override
        public JavaConstant lookup() {
            if (isInvokeDynamic()) {
                if (cpiOrIndyIndex < 0) {
                    throw new IllegalStateException("No rewritten invokedynamic index is available");
                }
                return lookupAppendix(cpiOrIndyIndex, Bytecodes.INVOKEDYNAMIC);
            } else {
                return (JavaConstant) lookupConstant(cpiOrIndyIndex, false);
            }
        }

        @Override
        public String toString() {
            String staticArgumentsString = staticArguments == null ? "<unavailable>" :
                            staticArguments.stream().map(BootstrapMethodInvocationImpl::argumentAsString).collect(Collectors.joining(", ", "[", "]"));
            return "BootstrapMethod[" + (indy ? "indy" : "condy") +
                            ", method:" + (method == null ? "<unavailable @ " + methodHandleIndex + ">" : method.format("%H.%n(%p)")) +
                            ", name: " + name +
                            ", type: " + (type == null ? "<unavailable>" : type.toValueString()) +
                            ", static arguments:" + staticArgumentsString + "]";
        }

        private static String argumentAsString(JavaConstant arg) {
            String type = arg.getJavaKind().getJavaName();
            String value = arg.toValueString();
            return type + ":" + value;
        }
    }

    @Override
    public BootstrapMethodInvocation lookupBootstrapMethodInvocation(int index, int opcode) {
        return lookupBootstrapMethodInvocation(index, opcode, true);
    }

    @Override
    public BootstrapMethodInvocation lookupBootstrapMethodInvocation(int index, int opcode, boolean resolve) {
        int cpi = bootstrapMethodInvocationConstantPoolIndex(index, opcode);
        final JvmConstant tagAt = getTagAt(cpi);
        if (tagAt == null) {
            return null;
        }
        final Tag tag = tagAt.tagEnum;
        switch (tag) {
            case INVOKEDYNAMIC:
            case DYNAMIC:
                int indyIndex = opcode == Bytecodes.INVOKEDYNAMIC && index >= 0 ? index : -1;
                Object[] bsmi = compilerToVM().lookupBootstrapMethodInvocation(this, cpi, resolve, indyIndex);
                ResolvedJavaMethod method = (ResolvedJavaMethod) bsmi[0];
                String name = (String) bsmi[1];
                JavaConstant type = (JavaConstant) bsmi[2];
                Object staticArguments = bsmi[3];
                int[] staticArgumentIndices = (int[]) bsmi[4];
                int methodHandleIndex = (Integer) bsmi[5];
                List<JavaConstant> staticArgumentsList;
                if (!resolve) {
                    staticArgumentsList = null;
                } else if (staticArguments == null) {
                    staticArgumentsList = List.of();
                } else if (staticArguments instanceof JavaConstant) {
                    staticArgumentsList = List.of((JavaConstant) staticArguments);
                } else if (staticArguments instanceof JavaConstant[]) {
                    staticArgumentsList = List.of((JavaConstant[]) staticArguments);
                } else {
                    staticArgumentsList = new CachedBSMArgs(this, staticArgumentIndices);
                }
                boolean isIndy = tag == Tag.INVOKEDYNAMIC;
                int lookupIndex = isIndy ? indyIndex : cpi;
                return new BootstrapMethodInvocationImpl(isIndy, method, name, type, staticArgumentsList,
                                staticArgumentIndices, lookupIndex, methodHandleIndex);
            default:
                return null;
        }
    }

    private boolean isDynamicEntry(int cpi) {
        JvmConstant tagAt = getTagAt(cpi);
        return tagAt != null && tagAt.tagEnum == Tag.DYNAMIC;
    }

    @Override
    public List<BootstrapMethodInvocation> lookupBootstrapMethodInvocations(boolean invokeDynamic){
        if (invokeDynamic) {
            int numIndys = compilerToVM().getNumIndyEntries(this);
            if (numIndys == 0) {
                return List.of();
            }
            return IntStream.range(0, numIndys)
                            .mapToObj(i -> lookupBootstrapMethodInvocation(i, Bytecodes.INVOKEDYNAMIC))
                            .toList();
        } else {
            return IntStream.range(1, length())
                            .filter(this::isDynamicEntry)
                            .mapToObj(cpi -> lookupBootstrapMethodInvocation(cpi, -1))
                            .toList();
        }
    }

    /**
     * Gets the {@link JavaConstant} for the {@code ConstantValue} attribute of a field.
     */
    JavaConstant getStaticFieldConstantValue(int cpi) {
        final JvmConstant tag = getTagAt(cpi);
        switch (tag.name) {
            case "Integer":
                return JavaConstant.forInt(getIntAt(cpi));
            case "Long":
                return JavaConstant.forLong(getLongAt(cpi));
            case "Float":
                return JavaConstant.forFloat(getFloatAt(cpi));
            case "Double":
                return JavaConstant.forDouble(getDoubleAt(cpi));
            case "String":
                return compilerToVM().getUncachedStringInPool(this, cpi);
            default:
                throw new IllegalArgumentException("Illegal entry for a ConstantValue attribute:" + tag);
        }
    }

    @Override
    public Object lookupConstant(int cpi) {
        return lookupConstant(cpi, true);
    }

    @Override
    public Object lookupConstant(int cpi, boolean resolve) {
        final JvmConstant tag = getTagAt(cpi);
        switch (tag.name) {
            case "Integer":
                return JavaConstant.forInt(getIntAt(cpi));
            case "Long":
                return JavaConstant.forLong(getLongAt(cpi));
            case "Float":
                return JavaConstant.forFloat(getFloatAt(cpi));
            case "Double":
                return JavaConstant.forDouble(getDoubleAt(cpi));
            case "Class":
            case "UnresolvedClass":
            case "UnresolvedClassInError":
                final int opcode = -1;  // opcode is not used
                return lookupType(cpi, opcode);
            case "String":
                /*
                 * Normally, we would expect a String here, but unsafe anonymous classes can have
                 * "pseudo strings" (arbitrary live objects) patched into a String entry. Such
                 * entries do not have a symbol in the constant pool slot.
                 */
                return compilerToVM().lookupConstantInPool(this, cpi, true);
            case "MethodHandle":
            case "MethodHandleInError":
            case "MethodType":
            case "MethodTypeInError":
            case "Dynamic":
            case "DynamicInError":
                return compilerToVM().lookupConstantInPool(this, cpi, resolve);
            default:
                throw new JVMCIError("Unknown constant pool tag %s", tag);
        }

    }

    @Override
    public String lookupUtf8(int cpi) {
        checkTag(cpi, constants.jvmUtf8);
        return compilerToVM().getSymbol(getEntryAt(cpi));
    }

    @Override
    public String lookupName(int rawIndex, int opcode) {
        switch (opcode) {
            case -1:
            case Bytecodes.LDC:
            case Bytecodes.LDC_W:
            case Bytecodes.LDC2_W:
                return lookupNameAtCPI(rawIndex);
            case Bytecodes.GETSTATIC:
            case Bytecodes.PUTSTATIC:
            case Bytecodes.GETFIELD:
            case Bytecodes.PUTFIELD:
            case Bytecodes.INVOKEVIRTUAL:
            case Bytecodes.INVOKESPECIAL:
            case Bytecodes.INVOKESTATIC:
            case Bytecodes.INVOKEINTERFACE:
            case Bytecodes.INVOKEDYNAMIC:
                return lookupNameAt(rawIndex, opcode);
            default:
                throw new IllegalArgumentException("unexpected opcode " + opcode + " at " + rawIndex);
        }
    }

    private String lookupNameAt(int rawIndex, int opcode) {
        int nameAndTypeIndex = getNameAndTypeRefIndexAt(rawIndex, opcode);
        return lookupNameAtNameAndType(nameAndTypeIndex);
    }

    private String lookupNameAtNameAndType(int nameAndTypeIndex) {
        return lookupUtf8(getNameRefIndexAt(nameAndTypeIndex));
    }

    private String lookupNameAtCPI(int cpi) {
        switch (getTag(cpi)) {
            case FIELD_REF:
            case METHOD_REF:
            case INTERFACE_METHOD_REF:
            case DYNAMIC:
            case INVOKEDYNAMIC:
                return lookupNameAtNameAndType(getNameAndTypeRefIndexAt(cpi));
            case METHODHANDLE:
                return lookupNameAtCPI(getMethodHandleRefIndexAt(cpi));
            default:
                throw new IllegalArgumentException("constant pool tag at index " + cpi + " is " + getTag(cpi) + " but has no symbolic name");
        }
    }

    @Override
    public Signature lookupSignature(int rawIndex, int opcode) {
        switch (opcode) {
            case -1:
            case Bytecodes.LDC:
            case Bytecodes.LDC_W:
            case Bytecodes.LDC2_W:
                return lookupSignatureAtCPI(rawIndex);
            case Bytecodes.INVOKEVIRTUAL:
            case Bytecodes.INVOKESPECIAL:
            case Bytecodes.INVOKESTATIC:
            case Bytecodes.INVOKEINTERFACE:
            case Bytecodes.INVOKEDYNAMIC:
                return lookupSignatureAt(rawIndex, opcode);
            default:
                throw new IllegalArgumentException("unexpected opcode " + opcode + " at " + rawIndex);
        }
    }

    private Signature lookupSignatureAt(int rawIndex, int opcode) {
        int nameAndTypeIndex = getNameAndTypeRefIndexAt(rawIndex, opcode);
        return new HotSpotSignature(runtime(), lookupUtf8(getSignatureRefIndexAt(nameAndTypeIndex)));
    }

    private Signature lookupSignatureAtCPI(int cpi) {
        int signatureIndex;
        switch (getTag(cpi)) {
            case UTF8:
                signatureIndex = cpi;
                break;
            case METHOD_REF:
            case INTERFACE_METHOD_REF:
                signatureIndex = getSignatureRefIndexAt(getNameAndTypeRefIndexAt(cpi));
                break;
            case METHODHANDLE:
                if (lookupMethodHandleReferenceKind(cpi) <= MethodHandleInfo.REF_putStatic) {
                    throw new IllegalArgumentException("field method handle at constant pool index " + cpi + " has no method signature");
                }
                return lookupSignatureAtCPI(getMethodHandleRefIndexAt(cpi));
            case METHODTYPE:
                signatureIndex = getMethodTypeSignatureIndexAt(cpi);
                break;
            case INVOKEDYNAMIC:
                signatureIndex = getSignatureRefIndexAt(getNameAndTypeRefIndexAt(cpi));
                break;
            default:
                throw new IllegalArgumentException("constant pool tag at index " + cpi + " is " + getTag(cpi) + " but has no method signature");
        }
        return new HotSpotSignature(runtime(), lookupUtf8(signatureIndex));
    }

    @Override
    public int lookupMethodHandleReferenceKind(int cpi) {
        return getMethodHandleReferenceKindAt(cpi);
    }

    @Override
    public int lookupMethodHandleReferenceIndex(int cpi) {
        return getMethodHandleRefIndexAt(cpi);
    }

    @Override
    public JavaConstant lookupAppendix(int rawIndex, int opcode) {
        if (!Bytecodes.isInvoke(opcode)) {
            throw new IllegalArgumentException("expected an invoke bytecode for " + rawIndex + ", got " + opcode);
        }
        return compilerToVM().lookupAppendixInPool(this, rawIndex, opcode);
    }

    /**
     * Gets a {@link JavaType} corresponding a given resolved or unresolved type.
     *
     * @param type either a ResolvedJavaType or a String naming a unresolved type.
     */
    private static JavaType getJavaType(final Object type) {
        if (type instanceof String) {
            String name = (String) type;
            return UnresolvedJavaType.create("L" + name + ";");
        } else {
            return (JavaType) type;
        }
    }

    @Override
    public JavaMethod lookupMethod(int rawIndex, int opcode, ResolvedJavaMethod caller) {
        if (opcode == -1 || opcode == Bytecodes.LDC || opcode == Bytecodes.LDC_W) {
            if (getTag(rawIndex) != Tag.METHODHANDLE) {
                throw new IllegalArgumentException("constant pool tag at index " + rawIndex + " is " + getTag(rawIndex) + " but expected METHODHANDLE for an ldc opcode");
            }
            int referenceKind = lookupMethodHandleReferenceKind(rawIndex);
            if (referenceKind < MethodHandleInfo.REF_invokeVirtual || referenceKind > MethodHandleInfo.REF_invokeInterface) {
                throw new IllegalArgumentException("method handle at constant pool index " + rawIndex + " has field reference kind " + referenceKind);
            }
            int methodHandleOpcode = opcode == -1 ? Bytecodes.LDC : opcode;
            final HotSpotResolvedJavaMethod method = compilerToVM().lookupMethodInPool(this, rawIndex, (byte) methodHandleOpcode, (HotSpotResolvedJavaMethodImpl) caller);
            if (method != null) {
                return method;
            }
            int memberCpi = getMethodHandleRefIndexAt(rawIndex);
            int holderCpi = getUncachedKlassRefIndexAt(memberCpi);
            return new UnresolvedJavaMethod(lookupName(memberCpi, -1), lookupSignature(memberCpi), lookupType(holderCpi, -1));
        }
        final HotSpotResolvedJavaMethod method = compilerToVM().lookupMethodInPool(this, rawIndex, (byte) opcode, (HotSpotResolvedJavaMethodImpl) caller);
        if (method != null) {
            return method;
        } else {
            // Get the method's name and signature.
            Object[] nameAndSignature = compilerToVM().lookupNameAndSignatureInPool(this, rawIndex, opcode);
            String name = (String) nameAndSignature[0];
            Signature signature = new HotSpotSignature(runtime(), (String) nameAndSignature[1]);
            if (opcode == Bytecodes.INVOKEDYNAMIC) {
                return new UnresolvedJavaMethod(name, signature, runtime().getMethodHandleClass());
            } else {
                final int klassIndex = getKlassRefIndexAt(rawIndex, opcode);
                final Object type = compilerToVM().lookupKlassInPool(this, klassIndex);
                return new UnresolvedJavaMethod(name, signature, getJavaType(type));
            }
        }
    }

    @Override
    public JavaType lookupType(int cpi, int opcode) {
        final LookupTypeCacheElement elem = this.lastLookupType;
        if (elem != null && elem.lastCpi == cpi) {
            return elem.javaType;
        } else {
            final Object type = compilerToVM().lookupKlassInPool(this, cpi);
            JavaType result = getJavaType(type);
            if (result instanceof ResolvedJavaType) {
                this.lastLookupType = new LookupTypeCacheElement(cpi, result);
            }
            return result;
        }
    }

    @Override
    public JavaType lookupReferencedType(int rawIndex, int opcode) {
        int cpi;
        switch (opcode) {
            case -1: {
                Tag tag = getTag(rawIndex);
                if (tag == Tag.DYNAMIC) {
                    int nameAndTypeIndex = getNameAndTypeRefIndexAt(rawIndex);
                    String typeName = lookupUtf8(getSignatureRefIndexAt(nameAndTypeIndex));
                    return runtime().lookupType(typeName, getHolder(), false);
                }
                cpi = switch (tag) {
                    case CLASS -> rawIndex;
                    case FIELD_REF, METHOD_REF, INTERFACE_METHOD_REF -> getUncachedKlassRefIndexAt(rawIndex);
                    case METHODHANDLE -> getUncachedKlassRefIndexAt(getMethodHandleRefIndexAt(rawIndex));
                    default ->
                        throw JVMCIError.shouldNotReachHere("Unexpected constant pool tag: " + tag);
                };
                break;
            }
            case Bytecodes.CHECKCAST:
            case Bytecodes.INSTANCEOF:
            case Bytecodes.NEW:
            case Bytecodes.ANEWARRAY:
            case Bytecodes.MULTIANEWARRAY:
                cpi = rawIndex;
                break;
            case Bytecodes.LDC:
            case Bytecodes.LDC_W:
            case Bytecodes.LDC2_W:
                if (getTag(rawIndex) == Tag.DYNAMIC) {
                    int nameAndTypeIndex = getNameAndTypeRefIndexAt(rawIndex);
                    String typeName = lookupUtf8(getSignatureRefIndexAt(nameAndTypeIndex));
                    return runtime().lookupType(typeName, getHolder(), false);
                }
                cpi = rawIndex;
                break;
            case Bytecodes.GETSTATIC:
            case Bytecodes.PUTSTATIC:
            case Bytecodes.GETFIELD:
            case Bytecodes.PUTFIELD:
                cpi = getKlassRefIndexAt(rawIndex, opcode);
                break;
            case Bytecodes.INVOKEVIRTUAL:
            case Bytecodes.INVOKESPECIAL:
            case Bytecodes.INVOKESTATIC:
            case Bytecodes.INVOKEINTERFACE: {
                cpi = getKlassRefIndexAt(rawIndex, opcode);
                break;
            }
            default:
                throw JVMCIError.shouldNotReachHere("Unexpected opcode " + opcode);
        }
        final Object type = compilerToVM().lookupKlassInPool(this, cpi);
        return getJavaType(type);
    }

    @Override
    public JavaField lookupField(int rawIndex, ResolvedJavaMethod method, int opcode) {
        if (!(method instanceof HotSpotResolvedJavaMethodImpl accessingMethod)) {
            throw new IllegalArgumentException("Require a non-null method from this JVMCI implementation");
        }
        int nameAndTypeIndex;
        int holderIndex;
        if (opcode == -1 || opcode == Bytecodes.LDC || opcode == Bytecodes.LDC_W) {
            if (getTag(rawIndex) != Tag.METHODHANDLE) {
                throw new IllegalArgumentException("constant pool tag at index " + rawIndex + " is " + getTag(rawIndex) + " but expected METHODHANDLE for an ldc opcode");
            }
            int referenceKind = lookupMethodHandleReferenceKind(rawIndex);
            if (referenceKind < MethodHandleInfo.REF_getField || referenceKind > MethodHandleInfo.REF_putStatic) {
                throw new IllegalArgumentException("method handle at constant pool index " + rawIndex + " has method reference kind " + referenceKind);
            }
            int memberCpi = getMethodHandleRefIndexAt(rawIndex);
            nameAndTypeIndex = getNameAndTypeRefIndexAt(memberCpi);
            holderIndex = getUncachedKlassRefIndexAt(memberCpi);
        } else {
            nameAndTypeIndex = compilerToVM().lookupNameAndTypeRefIndexInPool(this, rawIndex, opcode);
            holderIndex = getKlassRefIndexAt(rawIndex, opcode);
        }
        String typeName = lookupUtf8(getSignatureRefIndexAt(nameAndTypeIndex));
        JavaType type = runtime().lookupType(typeName, getHolder(), false);
        JavaType fieldHolder = lookupType(holderIndex, opcode);
        if (fieldHolder instanceof HotSpotResolvedObjectTypeImpl) {
            int[] info = new int[4];
            HotSpotResolvedObjectTypeImpl resolvedHolder;
            try {
                int resolutionOpcode = opcode == -1 ? Bytecodes.LDC : opcode;
                resolvedHolder = compilerToVM().resolveFieldInPool(this, rawIndex, accessingMethod, (byte) resolutionOpcode, info);
            } catch (Throwable cause) {
                String name = lookupNameAtNameAndType(nameAndTypeIndex);
                return new UnresolvedJavaField(fieldHolder, name, type, cause);
            }
            if (resolvedHolder == null) {
                String name = lookupNameAtNameAndType(nameAndTypeIndex);
                return new UnresolvedJavaField(fieldHolder, name, type);
            }
            final int flags = info[0];
            final int offset = info[1];
            final int fieldIndex = info[2];
            final int fieldFlags = info[3];
            return resolvedHolder.createField(type, offset, flags, fieldFlags, fieldIndex);
        } else {
            String name = lookupNameAtNameAndType(nameAndTypeIndex);
            return new UnresolvedJavaField(fieldHolder, name, type);
        }
    }

    /**
     * Converts an index for a bytecode that can reference a dynamic constant or call site to a
     * constant pool index.
     */
    private int bootstrapMethodInvocationConstantPoolIndex(int index, int opcode) {
        return switch (opcode) {
            case -1, Bytecodes.LDC, Bytecodes.LDC_W, Bytecodes.LDC2_W -> index;
            case Bytecodes.INVOKEDYNAMIC -> compilerToVM().decodeIndyIndexToCPIndex(this, index, false);
            default -> throw new IllegalArgumentException("unexpected opcode " + opcode + " at " + index);
        };
    }

    @Override
    public void loadReferencedType(int rawIndex, int opcode) {
        loadReferencedType(rawIndex, opcode, true /* initialize */);
    }

    @Override
    @SuppressWarnings("fallthrough")
    public void loadReferencedType(int rawIndex, int opcode, boolean initialize) {
        int cpi = decodeIndex(rawIndex, opcode, true);
        final JvmConstant tag = getTagAt(cpi);
        if (tag == null) {
            assert getTagAt(cpi - 1) == constants.jvmDouble || getTagAt(cpi - 1) == constants.jvmLong;
            return;
        }
        switch (tag.name) {
            case "Methodref":
            case "Fieldref":
            case "InterfaceMethodref":
                cpi = getUncachedKlassRefIndexAt(cpi);
                // Read the tag only once because it could change between multiple reads.
                final JvmConstant klassTag = getTagAt(cpi);
                assert klassTag == constants.jvmClass || klassTag == constants.jvmUnresolvedClass || klassTag == constants.jvmUnresolvedClassInError : klassTag;
                // fall through
            case "Class":
            case "UnresolvedClass":
            case "UnresolvedClassInError":
                final HotSpotResolvedObjectTypeImpl type = compilerToVM().resolveTypeInPool(this, cpi);
                if (initialize && !type.isPrimitive() && !type.isArray()) {
                    type.ensureInitialized();
                }
                if (tag == constants.jvmMethodref) {
                    if (Bytecodes.isInvokeHandleAlias(opcode) && isSignaturePolymorphicHolder(type)) {
                        checkTag(compilerToVM().decodeMethodIndexToCPIndex(this, rawIndex), constants.jvmMethodref);
                        compilerToVM().resolveInvokeHandleInPool(this, rawIndex);
                    }
                }

                break;
            case "InvokeDynamic":
                // Already resolved by decodeIndex.
                break;
            default:
                // nothing
                break;
        }
    }

    @Override
    public int decodeIndex(int rawIndex, int opcode) {
        return decodeIndex(rawIndex, opcode, false);
    }

    private int decodeIndex(int rawIndex, int opcode, boolean resolveIndy) {
        return switch (opcode) {
            case Bytecodes.CHECKCAST, Bytecodes.INSTANCEOF, Bytecodes.NEW, Bytecodes.ANEWARRAY,
                 Bytecodes.MULTIANEWARRAY, Bytecodes.LDC, Bytecodes.LDC_W, Bytecodes.LDC2_W -> rawIndex;
            case Bytecodes.INVOKEDYNAMIC ->
                    compilerToVM().decodeIndyIndexToCPIndex(this, rawIndex, resolveIndy);
            case Bytecodes.GETSTATIC, Bytecodes.PUTSTATIC, Bytecodes.GETFIELD, Bytecodes.PUTFIELD ->
                    compilerToVM().decodeFieldIndexToCPIndex(this, rawIndex);
            case Bytecodes.INVOKEVIRTUAL, Bytecodes.INVOKESPECIAL, Bytecodes.INVOKESTATIC, Bytecodes.INVOKEINTERFACE ->
                    compilerToVM().decodeMethodIndexToCPIndex(this, rawIndex);
            default -> throw JVMCIError.shouldNotReachHere("Unexpected opcode " + opcode);
        };
    }

    @Override
    public Tag getTag(int cpi) {
        JvmConstant tag = getTagAt(cpi);
        if (tag == null) {
            return Tag.INVALID;
        }
        return tag.tagEnum;
    }

    // Lazily initialized.
    private static String[] signaturePolymorphicHolders;

    /**
     * Determines if {@code type} contains signature polymorphic methods.
     */
    @SuppressFBWarnings(value = "LI_LAZY_INIT_STATIC", justification = "signaturePolymorphicHolders is a cache, not a singleton that must be constructed exactly once" +
                    "and compiler re-ordering is not an issue due to the VM call")
    static boolean isSignaturePolymorphicHolder(final ResolvedJavaType type) {
        String name = type.getName();
        if (signaturePolymorphicHolders == null) {
            signaturePolymorphicHolders = compilerToVM().getSignaturePolymorphicHolders();
        }
        for (String holder : signaturePolymorphicHolders) {
            if (name.equals(holder)) {
                return true;
            }
        }
        return false;
    }

    public String getSourceFileName() {
        final int sourceFileNameIndex = UNSAFE.getChar(getConstantPoolPointer() + config().constantPoolSourceFileNameIndexOffset);
        if (sourceFileNameIndex == 0) {
            return null;
        }
        return lookupUtf8(sourceFileNameIndex);
    }

    @Override
    public String toString() {
        return "HotSpotConstantPool<" + getHolder().toJavaName() + ">";
    }
}
