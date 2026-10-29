/*
 * Copyright (c) 2019, 2026, Oracle and/or its affiliates. All rights reserved.
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
 *
 */
/* AUTOMATICALLY GENERATED FILE - DO NOT EDIT */

#ifndef JFRFILES_JFRTYPES_HPP
#define JFRFILES_JFRTYPES_HPP

#include "utilities/macros.hpp"
#if INCLUDE_JFR

#include <string.h>
#include "memory/allocation.hpp"


namespace svm_gc {

enum JfrTypeId {
  TYPE_VIRTUALSPACE = 172,
  TYPE_OBJECTSPACE = 173,
  TYPE_METASPACESIZES = 174,
  TYPE_COPYFAILED = 175,
  TYPE_G1EVACUATIONSTATISTICS = 176,
  TYPE_CALLEEMETHOD = 177,
  TYPE_SHENANDOAHHEAPREGIONSTATE = 178,
  TYPE_DEOPTIMIZATIONREASON = 179,
  TYPE_DEOPTIMIZATIONACTION = 180,
  TYPE_VMTHREADSTATE = 181,
  TYPE_BYTECODE = 182,
  TYPE_COMPILERTYPE = 183,
  TYPE_ZPAGETYPETYPE = 184,
  TYPE_ZSTATISTICSCOUNTERTYPE = 185,
  TYPE_ZSTATISTICSSAMPLERTYPE = 186,
  TYPE_NETWORKINTERFACENAME = 187,
  TYPE_THREAD = 188,
  TYPE_THREADGROUP = 189,
  TYPE_CLASS = 190,
  TYPE_CLASSLOADER = 191,
  TYPE_METHOD = 192,
  TYPE_SYMBOL = 193,
  TYPE_THREADSTATE = 194,
  TYPE_GCNAME = 195,
  TYPE_GCCAUSE = 196,
  TYPE_GCWHEN = 197,
  TYPE_G1HEAPREGIONTYPE = 198,
  TYPE_G1YCTYPE = 199,
  TYPE_GCTHRESHOLDUPDATER = 200,
  TYPE_REFERENCETYPE = 201,
  TYPE_METADATATYPE = 202,
  TYPE_METASPACEOBJECTTYPE = 203,
  TYPE_NARROWOOPMODE = 204,
  TYPE_VMOPERATIONTYPE = 205,
  TYPE_COMPILERPHASETYPE = 206,
  TYPE_FLAGVALUEORIGIN = 207,
  TYPE_CODEBLOBTYPE = 208,
  TYPE_INFLATECAUSE = 209,
  TYPE_MODULE = 210,
  TYPE_PACKAGE = 211,
  TYPE_STACKTRACE = 212,
  TYPE_FRAMETYPE = 213,
  TYPE_OLDOBJECTROOTSYSTEM = 214,
  TYPE_OLDOBJECTROOTTYPE = 215,
  TYPE_OLDOBJECTGCROOT = 216,
  TYPE_OLDOBJECTARRAY = 217,
  TYPE_OLDOBJECTFIELD = 218,
  TYPE_OLDOBJECT = 219,
  TYPE_REFERENCE = 220,
  TYPE_STACKFRAME = 221,
  TYPE_CHUNKHEADER = 222,
  TYPE_NMTTYPE = 223,
  TYPE_JAVAMONITORADDRESS = 224,
  TYPE_SAFEPOINTID = 225,
  TYPE_GCID = 226,
  TYPE_COMPILEID = 227,
  TYPE_SWEEPID = 228,
  TYPE_FLUSHID = 229,
  TYPE_CLASSREDEFINITIONID = 230,
  TYPE_LONG = 231,
  TYPE_INT = 232,
  TYPE_SHORT = 233,
  TYPE_BYTE = 234,
  TYPE_DOUBLE = 235,
  TYPE_FLOAT = 236,
  TYPE_BOOLEAN = 237,
  TYPE_CHAR = 238,
  TYPE_STRING = 239,
};

static const JfrTypeId FIRST_TYPE_ID = TYPE_VIRTUALSPACE;
static const JfrTypeId LAST_TYPE_ID = TYPE_STRING;

class JfrType : public AllStatic {
 public:
  static jlong name_to_id(const char* type_name) {
    if (strcmp(type_name, "java.lang.Class") == 0) {
      return TYPE_CLASS;
    }
    if (strcmp(type_name, "java.lang.Thread") == 0) {
      return TYPE_THREAD;
    }
    if (strcmp(type_name, "long") == 0) {
      return TYPE_LONG;
    }
    if (strcmp(type_name, "int") == 0) {
      return TYPE_INT;
    }
    if (strcmp(type_name, "short") == 0) {
      return TYPE_SHORT;
    }
    if (strcmp(type_name, "byte") == 0) {
      return TYPE_BYTE;
    }
    if (strcmp(type_name, "double") == 0) {
      return TYPE_DOUBLE;
    }
    if (strcmp(type_name, "float") == 0) {
      return TYPE_FLOAT;
    }
    if (strcmp(type_name, "boolean") == 0) {
      return TYPE_BOOLEAN;
    }
    if (strcmp(type_name, "char") == 0) {
      return TYPE_CHAR;
    }
    if (strcmp(type_name, "java.lang.String") == 0) {
      return TYPE_STRING;
    }
    if (strcmp(type_name, "jdk.types.StackTrace") == 0) {
      return TYPE_STACKTRACE;
    }
    return -1;
  }
};


} // namespace svm_gc

#endif // INCLUDE_JFR
#endif // JFRFILES_JFRTYPES_HPP
