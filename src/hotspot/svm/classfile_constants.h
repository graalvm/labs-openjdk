/*
 * Copyright (c) 2004, 2019, Oracle and/or its affiliates. All rights reserved.
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

#ifndef CLASSFILE_CONSTANTS_H
#define CLASSFILE_CONSTANTS_H

enum {
    JVM_T_BOOLEAN = 4,
    JVM_T_CHAR    = 5,
    JVM_T_FLOAT   = 6,
    JVM_T_DOUBLE  = 7,
    JVM_T_BYTE    = 8,
    JVM_T_SHORT   = 9,
    JVM_T_INT     = 10,
    JVM_T_LONG    = 11
};

/* Type signatures */

enum {
    JVM_SIGNATURE_SLASH         = '/',
    JVM_SIGNATURE_DOT           = '.',
    JVM_SIGNATURE_SPECIAL       = '<',
    JVM_SIGNATURE_ENDSPECIAL    = '>',
    JVM_SIGNATURE_ARRAY         = '[',
    JVM_SIGNATURE_BYTE          = 'B',
    JVM_SIGNATURE_CHAR          = 'C',
    JVM_SIGNATURE_CLASS         = 'L',
    JVM_SIGNATURE_ENDCLASS      = ';',
    JVM_SIGNATURE_ENUM          = 'E',
    JVM_SIGNATURE_FLOAT         = 'F',
    JVM_SIGNATURE_DOUBLE        = 'D',
    JVM_SIGNATURE_FUNC          = '(',
    JVM_SIGNATURE_ENDFUNC       = ')',
    JVM_SIGNATURE_INT           = 'I',
    JVM_SIGNATURE_LONG          = 'J',
    JVM_SIGNATURE_SHORT         = 'S',
    JVM_SIGNATURE_VOID          = 'V',
    JVM_SIGNATURE_BOOLEAN       = 'Z'
};

#endif /* CLASSFILE_CONSTANTS */
