/*
 * Copyright (c) 2026, Oracle and/or its affiliates. All rights reserved.
 * ...
 */

// ファイルパス: src/jdk.internal.vm.ci/share/classes/jdk/vm/ci/arm/ARMKind.java

package jdk.vm.ci.arm;

import jdk.vm.ci.meta.PlatformKind;

public enum ARMKind implements PlatformKind {
    BYTE(1),
    WORD(2),
    DWORD(4),     // 32-bit int / float / pointer
    QWORD(8),     // 64-bit via register pair (r0:r1) or VFP d0
    SINGLE(4),    // VFP single-precision float (s registers)
    DOUBLE(8),    // VFP double-precision float (d registers)
    V128_WORD(16); // NEON 128-bit

    private final int size;
    private final EnumKey key = new EnumKey(this);

    ARMKind(int size) {
        this.size = size;
    }

    @Override
    public String name() {
        return name();
    }

    @Override
    public Key getKey() {
        return key;
    }

    @Override
    public int getSizeInBytes() {
        return size;
    }

    @Override
    public int getVectorLength() {
        return 1;
    }

    @Override
    public char getTypeChar() {
        return switch (this) {
            case BYTE    -> 'b';
            case WORD    -> 'w';
            case DWORD   -> 'd';
            case QWORD   -> 'q';
            case SINGLE  -> 'S';
            case DOUBLE  -> 'D';
            case V128_WORD -> 'V';
        };
    }

    public boolean isInteger() {
        return this == BYTE || this == WORD || this == DWORD || this == QWORD;
    }

    public boolean isFP() {
        return this == SINGLE || this == DOUBLE;
    }
}
