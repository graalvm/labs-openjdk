/*
 * Copyright (c) 2009, 2021, Oracle and/or its affiliates. All rights reserved.
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

/**
 * Represents a constant (boxed) value, such as an integer, floating point number, or object
 * reference, within the compiler and across the compiler/runtime interface. Exports a set of
 * {@code JavaConstant} instances that represent frequently used constant values, such as
 * {@link #NULL_POINTER}.
 */
public interface JavaConstant extends Constant, JavaValue {
    /*
     * Using a larger cache for integers leads to only a slight increase in cache hit ratio which is
     * not enough to justify the impact on startup time.
     */
    JavaConstant NULL_POINTER = new NullConstant();
    PrimitiveConstant INT_MINUS_1 = new PrimitiveConstant(JavaKind.Int, -1);
    PrimitiveConstant INT_0 = new PrimitiveConstant(JavaKind.Int, 0);
    PrimitiveConstant INT_1 = new PrimitiveConstant(JavaKind.Int, 1);
    PrimitiveConstant INT_2 = new PrimitiveConstant(JavaKind.Int, 2);
    PrimitiveConstant LONG_0 = new PrimitiveConstant(JavaKind.Long, 0L);
    PrimitiveConstant LONG_1 = new PrimitiveConstant(JavaKind.Long, 1L);
    PrimitiveConstant FLOAT_0 = new PrimitiveConstant(JavaKind.Float, Float.floatToRawIntBits(0.0F));
    PrimitiveConstant FLOAT_1 = new PrimitiveConstant(JavaKind.Float, Float.floatToRawIntBits(1.0F));
    PrimitiveConstant DOUBLE_0 = new PrimitiveConstant(JavaKind.Double, Double.doubleToRawLongBits(0.0D));
    PrimitiveConstant DOUBLE_1 = new PrimitiveConstant(JavaKind.Double, Double.doubleToRawLongBits(1.0D));
    PrimitiveConstant TRUE = new PrimitiveConstant(JavaKind.Boolean, 1L);
    PrimitiveConstant FALSE = new PrimitiveConstant(JavaKind.Boolean, 0L);
    PrimitiveConstant ILLEGAL = new PrimitiveConstant(JavaKind.Illegal, 0);

    /**
     * Returns the Java kind of this constant.
     */
    JavaKind getJavaKind();

    /**
     * Compares if the value boxed by this object is identical to the value boxed by
     * {@code other}. If {@code other} is not a JavaConstant, this method returns false.
     * If {@code other.getJavaKind() == JavaKind.Object}, the boxed object references
     * are compared for identity (i.e., {@code ==}), not structural equality (i.e.,
     * {@code equals()}). In JVMCI implementations supporting compressed objects references
     * (e.g., {@code -XX:+UseCompressedOops} on HotSpot), comparing JavaConstants for objects
     * also takes into account whether the references are compressed (i.e., a
     * JavaConstant that boxes a compressed reference is not equal to another
     * JavaConstant that boxes an uncompressed reference to the same object).
     */
    @Override
    boolean equals(Object other);

    /**
     * Gets the hash code based on the boxed value. If the boxed value is an object,
     * the returned value is based on the {@linkplain System#identityHashCode(Object)
     * identity hash code} of the boxed object.
     */
    @Override
    int hashCode();

    /**
     * Checks whether this constant is null.
     *
     * @return {@code true} if this constant is the null constant
     */
    boolean isNull();

    static boolean isNull(Constant c) {
        if (c instanceof JavaConstant) {
            return ((JavaConstant) c).isNull();
        } else {
            return false;
        }
    }

    /**
     * Checks whether this constant is non-null.
     *
     * @return {@code true} if this constant is a primitive, or an object constant that is not null
     */
    default boolean isNonNull() {
        return !isNull();
    }

    /**
     * Checks whether this constant is the default value for its kind (null, 0, 0.0, false).
     *
     * @return {@code true} if this constant is the default value for its kind
     */
    @Override
    boolean isDefaultForKind();

    /**
     * Returns the value of this constant as a boxed Java value.
     *
     * @return the value of this constant
     */
    Object asBoxedPrimitive();

    /**
     * Returns the primitive int value this constant represents. The constant must have a
     * {@link JavaKind#getStackKind()} of {@link JavaKind#Int}.
     *
     * @return the constant value
     */
    int asInt();

    /**
     * Returns the primitive boolean value this constant represents. The constant must have kind
     * {@link JavaKind#Boolean}.
     *
     * @return the constant value
     */
    boolean asBoolean();

    /**
     * Returns the primitive long value this constant represents. The constant must have kind
     * {@link JavaKind#Long}, a {@link JavaKind#getStackKind()} of {@link JavaKind#Int}.
     *
     * @return the constant value
     */
    long asLong();

    /**
     * Returns the primitive float value this constant represents. The constant must have kind
     * {@link JavaKind#Float}.
     *
     * @return the constant value
     */
    float asFloat();

    /**
     * Returns the primitive double value this constant represents. The constant must have kind
     * {@link JavaKind#Double}.
     *
     * @return the constant value
     */
    double asDouble();

    @Override
    default String toValueString() {
        if (getJavaKind() == JavaKind.Illegal) {
            return "illegal";
        } else {
            return getJavaKind().format(asBoxedPrimitive());
        }
    }

    static String toString(JavaConstant constant) {
        if (constant.getJavaKind() == JavaKind.Illegal) {
            return "illegal";
        } else {
            return constant.getJavaKind().getJavaName() + "[" + constant.toValueString() + "]";
        }
    }

    /**
     * Creates a boxed double constant.
     *
     * @param d the double value to box
     * @return a boxed copy of {@code value}
     */
    static PrimitiveConstant forDouble(double d) {
        if (Double.compare(0.0D, d) == 0) {
            return DOUBLE_0;
        }
        if (Double.compare(d, 1.0D) == 0) {
            return DOUBLE_1;
        }
        return new PrimitiveConstant(JavaKind.Double, Double.doubleToRawLongBits(d));
    }

    /**
     * Creates a boxed float constant.
     *
     * @param f the float value to box
     * @return a boxed copy of {@code value}
     */
    static PrimitiveConstant forFloat(float f) {
        if (Float.compare(f, 0.0F) == 0) {
            return FLOAT_0;
        }
        if (Float.compare(f, 1.0F) == 0) {
            return FLOAT_1;
        }
        return new PrimitiveConstant(JavaKind.Float, Float.floatToRawIntBits(f));
    }

    /**
     * Creates a boxed long constant.
     *
     * @param i the long value to box
     * @return a boxed copy of {@code value}
     */
    static PrimitiveConstant forLong(long i) {
        if (i == 0) {
            return LONG_0;
        } else if (i == 1) {
            return LONG_1;
        } else {
            return new PrimitiveConstant(JavaKind.Long, i);
        }
    }

    /**
     * Creates a boxed integer constant.
     *
     * @param i the integer value to box
     * @return a boxed copy of {@code value}
     */
    static PrimitiveConstant forInt(int i) {
        return switch (i) {
            case -1 -> INT_MINUS_1;
            case 0 -> INT_0;
            case 1 -> INT_1;
            case 2 -> INT_2;
            default -> new PrimitiveConstant(JavaKind.Int, i);
        };
    }

    /**
     * Creates a boxed byte constant.
     *
     * @param i the byte value to box
     * @return a boxed copy of {@code value}
     */
    static PrimitiveConstant forByte(byte i) {
        return new PrimitiveConstant(JavaKind.Byte, i);
    }

    /**
     * Creates a boxed boolean constant.
     *
     * @param i the boolean value to box
     * @return a boxed copy of {@code value}
     */
    static PrimitiveConstant forBoolean(boolean i) {
        return i ? TRUE : FALSE;
    }

    /**
     * Creates a boxed char constant.
     *
     * @param i the char value to box
     * @return a boxed copy of {@code value}
     */
    static PrimitiveConstant forChar(char i) {
        return new PrimitiveConstant(JavaKind.Char, i);
    }

    /**
     * Creates a boxed short constant.
     *
     * @param i the short value to box
     * @return a boxed copy of {@code value}
     */
    static PrimitiveConstant forShort(short i) {
        return new PrimitiveConstant(JavaKind.Short, i);
    }

    @VMEntryPoint
    static PrimitiveConstant forPrimitive(char typeChar, long rawValue) {
        return forPrimitive(JavaKind.fromPrimitiveOrVoidTypeChar(typeChar), rawValue);
    }

    static PrimitiveConstant forPrimitive(JavaKind kind, long rawValue) {
        return switch (kind) {
            case Boolean -> JavaConstant.forBoolean(rawValue != 0);
            case Byte -> JavaConstant.forByte((byte) rawValue);
            case Char -> JavaConstant.forChar((char) rawValue);
            case Short -> JavaConstant.forShort((short) rawValue);
            case Int -> JavaConstant.forInt((int) rawValue);
            case Long -> JavaConstant.forLong(rawValue);
            case Float -> JavaConstant.forFloat(Float.intBitsToFloat((int) rawValue));
            case Double -> JavaConstant.forDouble(Double.longBitsToDouble(rawValue));
            default -> throw new IllegalArgumentException("Unsupported kind: " + kind);
        };
    }

    /**
     * Creates a boxed constant for the given boxed primitive value.
     *
     * @param value the Java boxed value
     * @return the primitive constant holding the {@code value}
     */
    static PrimitiveConstant forBoxedPrimitive(Object value) {
        return switch (value) {
            case Boolean b -> forBoolean(b);
            case Byte b -> forByte(b);
            case Character c -> forChar(c);
            case Short i -> forShort(i);
            case Integer i -> forInt(i);
            case Long l -> forLong(l);
            case Float v -> forFloat(v);
            case Double v -> forDouble(v);
            case null, default -> null;
        };
    }
}
