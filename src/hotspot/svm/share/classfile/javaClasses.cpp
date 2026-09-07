/*
 * Copyright (c) 1997, 2025, Oracle and/or its affiliates. All rights reserved.
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

#include "cds/aotReferenceObjSupport.hpp"
#include "cds/archiveBuilder.hpp"
#include "cds/archiveHeapLoader.hpp"
#include "cds/cdsConfig.hpp"
#include "cds/heapShared.hpp"
#include "cds/metaspaceShared.hpp"
#include "classfile/altHashing.hpp"
#include "classfile/classLoaderData.inline.hpp"
#include "classfile/javaClasses.inline.hpp"
#include "classfile/javaClassesImpl.hpp"
#include "classfile/javaThreadStatus.hpp"
#include "classfile/moduleEntry.hpp"
#include "classfile/stringTable.hpp"
#include "classfile/symbolTable.hpp"
#include "classfile/systemDictionary.hpp"
#include "classfile/vmClasses.hpp"
#include "classfile/vmSymbols.hpp"
#include "code/debugInfo.hpp"
#include "code/dependencyContext.hpp"
#include "code/pcDesc.hpp"
#include "gc/shared/collectedHeap.inline.hpp"
#include "interpreter/interpreter.hpp"
#include "interpreter/linkResolver.hpp"
#include "jvm.h"
#include "logging/log.hpp"
#include "logging/logStream.hpp"
#include "memory/oopFactory.hpp"
#include "memory/resourceArea.hpp"
#include "memory/universe.hpp"
#include "oops/fieldInfo.hpp"
#include "oops/fieldStreams.inline.hpp"
#include "oops/instanceKlass.inline.hpp"
#include "oops/instanceMirrorKlass.hpp"
#include "oops/klass.hpp"
#include "oops/klass.inline.hpp"
#include "oops/method.inline.hpp"
#include "oops/objArrayKlass.hpp"
#include "oops/objArrayOop.inline.hpp"
#include "oops/oopCast.inline.hpp"
#include "oops/oop.inline.hpp"
#include "oops/symbol.hpp"
#include "oops/recordComponent.hpp"
#include "oops/typeArrayOop.inline.hpp"
#include "prims/jvmtiExport.hpp"
#include "prims/methodHandles.hpp"
#include "prims/resolvedMethodTable.hpp"
#include "runtime/continuationEntry.inline.hpp"
#include "runtime/continuationJavaClasses.inline.hpp"
#include "runtime/fieldDescriptor.inline.hpp"
#include "runtime/frame.inline.hpp"
#include "runtime/handles.inline.hpp"
#include "runtime/handshake.hpp"
#include "runtime/init.hpp"
#include "runtime/interfaceSupport.inline.hpp"
#include "runtime/java.hpp"
#include "runtime/javaCalls.hpp"
#include "runtime/javaThread.hpp"
#include "runtime/jniHandles.inline.hpp"
#include "runtime/reflectionUtils.hpp"
#include "runtime/safepoint.hpp"
#include "runtime/safepointVerifiers.hpp"
#include "runtime/threadSMR.hpp"
#include "runtime/vframe.inline.hpp"
#include "runtime/vm_version.hpp"
#include "utilities/align.hpp"
#include "utilities/globalDefinitions.hpp"
#include "utilities/growableArray.hpp"
#include "utilities/preserveException.hpp"
#include "utilities/utf8.hpp"
#if INCLUDE_JVMCI
#include "jvmci/jvmciJavaClasses.hpp"
#endif

bool java_lang_String::equals(oop java_string, const char* chars) {
  assert(java_string->klass() == vmClasses::String_klass(),
         "must be java_string");
  int length = java_lang_String::length(java_string);
  int len = strlen(chars);
  if (length != len) {
    return false;
  }
  bool is_latin1 = java_lang_String::is_latin1(java_string);
  if (!is_latin1) {
    jchar *src = (jchar*) java_lang_String::base(java_string);
    for (int i = 0; i < len; i++) {
      if (src[i] != chars[i]) {
        return false;
      }
    }
  } else {
    jbyte *src = java_lang_String::base(java_string);
    for (int i = 0; i < len; i++) {
      if ((((jchar) src[i]) & 0xff) != chars[i]) {
        return false;
      }
    }
  }
  return true;
}

// NOTE (chaeubl): this code is not a proper implementation (no UTF-16 support and it may leak memory).
// It should only be used for debugging or fatal errors.
char* java_lang_String::as_latin1(oop string) {
  if (string == nullptr) {
    return nullptr;
  }

  int length = java_lang_String::length(string);
  char *result = new char[length + 1];
  if (!is_latin1(string)) {
    jchar *src = (jchar*) java_lang_String::base(string);
    for (int i = 0; i < length; i++) {
      result[i] = (char)src[i];
    }
  } else {
    jbyte *src = java_lang_String::base(string);
    for (int i = 0; i < length; i++) {
      result[i] = src[i];
    }
  }

  result[length] = '\0';
  return result;
}

// Support for java_lang_ref_Reference

bool java_lang_ref_Reference::is_referent_field(oop obj, ptrdiff_t offset) {
  assert(obj != nullptr, "sanity");
  return offset == SVMGlobalData::_offsets._reference._referent && obj->klass()->reference_type() != REF_NONE;
}

// Support for java_lang_ref_SoftReference
//

jlong java_lang_ref_SoftReference::timestamp(oop ref) {
  return ref->long_field(SVMGlobalData::_offsets._soft_reference._timestamp);
}


void java_lang_ref_SoftReference::set_clock(jlong value) {
  return RawAccess<>::store_at((oop)SVMIsolateData::_static_primitive_fields, (ptrdiff_t)SVMGlobalData::_offsets._soft_reference._static_clock, value);
}
