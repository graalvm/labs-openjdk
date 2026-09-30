/*
 * Copyright (c) 2000, 2026, Oracle and/or its affiliates. All rights reserved.
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

#include "classfile/classLoaderData.hpp"
#include "classfile/javaClasses.inline.hpp"
#include "compiler/compilerDefinitions.hpp"
#include "gc/shared/gcConfig.hpp"
#include "jvm.h"
#include "jvmci/jvmci.hpp"
#include "jvmci/jvmci_globals.hpp"
#include "logging/log.hpp"
#include "oops/instanceKlass.hpp"
#include "oops/klass.inline.hpp"
#include "oops/markWord.hpp"
#include "oops/objArrayKlass.hpp"
#include "runtime/arguments.hpp"
#include "runtime/flags/jvmFlagAccess.hpp"
#include "runtime/globals_extension.hpp"
#include "runtime/os.hpp"
#include "runtime/synchronizer.hpp"
#include "utilities/bytes.hpp"
#include "utilities/byteswap.hpp"
#include "utilities/defaultStream.hpp"
#include "utilities/ostream.hpp"
#include "utilities/resourceHash.hpp"

struct JVMCIClassIdentityHashCodeKey {
  const char* loader_name;
  const char* class_name;
};

static unsigned jvmci_class_identity_hash_code_key_hash(const JVMCIClassIdentityHashCodeKey& key) {
  unsigned hash = 0;
  for (const char* p = key.loader_name; *p != '\0'; p++) {
    hash = 31 * hash + static_cast<unsigned char>(*p);
  }
  hash = 31 * hash + '\t';
  for (const char* p = key.class_name; *p != '\0'; p++) {
    hash = 31 * hash + static_cast<unsigned char>(*p);
  }
  return hash;
}

static bool jvmci_class_identity_hash_code_key_equals(const JVMCIClassIdentityHashCodeKey& a, const JVMCIClassIdentityHashCodeKey& b) {
  return strcmp(a.loader_name, b.loader_name) == 0 && strcmp(a.class_name, b.class_name) == 0;
}

using JVMCIClassIdentityHashCodeTable = ResourceHashtable<
  JVMCIClassIdentityHashCodeKey,
  intptr_t,
  10007,
  AnyObj::C_HEAP, mtJVMCI,
  jvmci_class_identity_hash_code_key_hash,
  jvmci_class_identity_hash_code_key_equals>;

fileStream* JVMCIGlobals::_jni_config_file = nullptr;
static JVMCIClassIdentityHashCodeTable* class_identity_hash_codes = nullptr;

template <typename T>
static bool read_big_endian(FILE* file, T* value) {
  if (fread(value, sizeof(T), 1, file) != 1) {
    return false;
  }
  if (Endian::is_Java_byte_ordering_different()) {
    *value = byteswap(*value);
  }
  return true;
}

static char* read_utf(FILE* file) {
  u2 length;
  if (!read_big_endian(file, &length) || length == 0) {
    return nullptr;
  }
  char* value = NEW_C_HEAP_ARRAY(char, length + 1, mtJVMCI);
  if (fread(value, length, 1, file) != 1) {
    FREE_C_HEAP_ARRAY(char, value);
    return nullptr;
  }
  value[length] = '\0';
  return value;
}

// JVMCI class identity hash codes
//
// JVMCIClassIdentityHashCodeFile specifies an input file containing hashes to
// install in newly created java.lang.Class mirrors. The file is read once at
// JVM startup; JVMCI does not generate or update it. The default is no file.
//
// JVMCIUseStableGeneratedClassIdentityHashCodes independently enables computed
// hashes for eligible hidden classes, including lambda implementation classes,
// and arrays with eligible hidden classes as their ultimate component type.
// It defaults to false and does not require an input file. Supplying a file
// does not implicitly enable it. Both options require EnableJVMCI and are
// experimental unless JVMCI product mode is enabled.
//
// Hashes are installed during mirror creation, before publication. These
// options do not overwrite existing hashes or modify mirrors restored from a
// CDS archive. Use -Xshare:off when all mirrors must use this creation path.
//
// File format
//
// All integers are unsigned and big-endian. There are no alignment or padding
// bytes. The file consists of:
//
//   u4 magic;                       // 0x4A434948 ("JCIH")
//   u4 entry_count;                 // 0 .. max_jint
//   {
//     u4 identity_hash_code;        // 1 .. markWord::hash_mask
//     u2 loader_name_length;        // encoded byte length, 1 .. 65535
//     u1 loader_name[loader_name_length];
//     u2 class_name_length;         // encoded byte length, 1 .. 65535
//     u1 class_name[class_name_length];
//   } entries[entry_count];
//
// The hash upper bound is 0x7fffffff on 64-bit builds. Strings use the modified
// UTF-8 encoding of DataOutputStream.writeUTF, not standard UTF-8, and have no
// trailing NUL byte in the file. Names are compared byte-for-byte without
// Unicode normalization. The reader checks the lengths, but does not validate
// the modified UTF-8 sequences; producers must supply correctly encoded names.
//
// A Java producer can write the header using writeInt(0x4A434948) followed by
// writeInt(entryCount), then each record using writeInt(hash),
// writeUTF(loaderName), and writeUTF(className). For example, the record
// (12345, "bootstrap", "java/lang/String") requests hash 12345 for String.class.
//
// There is no separate version field, and EOF must follow the last record.
// An empty mapping contains the magic and a zero count, not zero bytes.
// Record order is irrelevant. Duplicate (loader_name, class_name) pairs are
// rejected even when their hashes agree; different keys may share a hash.
//
// Failure to open the file fails JVM startup. An invalid magic, count, hash,
// zero-length string, truncated record, duplicate key, trailing data, or
// detected read error also fails startup, rather than ignoring the file.
//
// Lookup keys
//
// The bootstrap loader is named "bootstrap". Other loaders use the name
// returned by ClassLoaderData::loader_name(): their explicitly assigned name,
// or the loader class's fully qualified Java name if none was assigned.
// Loader names are not unique loader identities. Classes with the same name
// in two equally named loaders select the same entry.
//
// Class names use internal form, such as "java/lang/String". Arrays use
// descriptors, such as "[I" or "[Ljava/lang/String;", together with their
// defining loader's name. Primitive mirrors use "bootstrap" and one of
// "boolean", "byte", "char", "short", "int", "long", "float", "double", or "void".
//
// A class_name beginning with "@interfaces:" selects a fallback lookup by
// direct superinterfaces, paired with the defining loader's name. It is followed
// by the internal names of the direct superinterfaces, in declaration order,
// separated by semicolons. For example:
//
//   ("app", "@interfaces:java/lang/Runnable;java/io/Serializable")
//
// This supports ordinary proxy classes whose generated names can change
// between JVMs. The matching rule is not restricted to proxies: it applies to
// any instance class with nonempty direct superinterfaces in the same order.
// Direct subclasses of java.lang.reflect.Proxy also use this lookup when the
// interface list is empty, with class_name "@interfaces:". For an array, use
// its ultimate component class's interface key followed by "@array:" and the
// array rank in decimal. The fallback distinguishes defining loader names,
// but not class names or superclasses. Equally named loaders remain ambiguous.
// Producers must account for this broader matching when emitting such entries.
//
// Lookup precedence
//
// For a newly created non-primitive mirror:
//   1. If enabled and applicable, use stable_generated_class_identity_hash_code().
//      The computed hash takes precedence over all file entries.
//   2. Look up the exact (loader_name, class_name) pair.
//   3. Try the interface key described above, including the rank for arrays.
//   4. If no rule supplies a hash, leave normal identity-hash behavior unchanged.
//
// Primitive mirrors only use exact file lookup. Unused entries do not cause
// errors or force classes to load. The generated-class option does not handle
// proxy classes; those can use the "@interfaces" entries.
static bool initialize_class_identity_hash_codes() {
  if (JVMCIClassIdentityHashCodeFile == nullptr) {
    return true;
  }

  FILE* file = os::fopen(JVMCIClassIdentityHashCodeFile, "rb");
  if (file == nullptr) {
    jio_fprintf(defaultStream::error_stream(), "Could not open Class identity hash code file: %s\n", JVMCIClassIdentityHashCodeFile);
    return false;
  }

  JVMCIClassIdentityHashCodeTable* table = new (mtJVMCI) JVMCIClassIdentityHashCodeTable();
  u4 magic;
  u4 count;
  bool valid = read_big_endian(file, &magic) && magic == 0x4A434948 && read_big_endian(file, &count) && count <= max_jint;
  u4 entry = 0;
  for (; valid && entry < count; entry++) {
    u4 parsed_hash;
    char* loader_name = nullptr;
    char* class_name = nullptr;
    valid = read_big_endian(file, &parsed_hash) && parsed_hash > 0 && parsed_hash <= markWord::hash_mask &&
            (loader_name = read_utf(file)) != nullptr && (class_name = read_utf(file)) != nullptr;
    if (!valid) {
      FREE_C_HEAP_ARRAY(char, loader_name);
      FREE_C_HEAP_ARRAY(char, class_name);
      break;
    }

    JVMCIClassIdentityHashCodeKey parsed_key = { loader_name, class_name };
    if (!table->put(parsed_key, parsed_hash)) {
      valid = false;
      FREE_C_HEAP_ARRAY(char, loader_name);
      FREE_C_HEAP_ARRAY(char, class_name);
    }
  }

  valid = valid && fgetc(file) == EOF && !ferror(file);
  fclose(file);
  if (!valid) {
    jio_fprintf(defaultStream::error_stream(), "Invalid entry %u in Class identity hash code file: %s\n", entry, JVMCIClassIdentityHashCodeFile);
    delete table;
    return false;
  }
  class_identity_hash_codes = table;
  return true;
}

static intptr_t find_class_identity_hash_code(const char* loader_name, const char* class_name) {
  JVMCIClassIdentityHashCodeTable* table = class_identity_hash_codes;
  if (table == nullptr) {
    return 0;
  }
  JVMCIClassIdentityHashCodeKey key = { loader_name, class_name };
  intptr_t* value = table->get(key);
  return value == nullptr ? 0 : *value;
}

static void append_interfaces(InstanceKlass* klass, stringStream* key) {
  key->print_raw("@interfaces:");
  Array<InstanceKlass*>* interfaces = klass->local_interfaces();
  for (int i = 0; i < interfaces->length(); i++) {
    if (i != 0) {
      key->print_raw(";");
    }
    key->print_raw(interfaces->at(i)->name()->as_C_string());
  }
}

// Computes a deterministic hash for a hidden instance class whose name contains
// "+0x" or "/0x". Use the first "+0x" occurrence, or the first "/0x" occurrence
// if there is no "+0x". Remove that suffix and everything after it, then remove
// trailing ASCII digits and one immediately preceding '$' from the name.
//
// Hash these bytes, in order (the prefixes are specific to this hash computation):
//   1. "@generated", followed by a tab byte.
//   2. The remaining class name.
//   3. "@super:" and the superclass's internal name, if there is a superclass.
//   4. "@interfaces:" and the list of direct superinterfaces in the file format
//      documented above, including the prefix when the list is empty.
// For an array whose ultimate component type is such a hidden class, use the
// hidden class's key above followed by "@array:" and the array rank in decimal.
//
// Starting from unsigned 32-bit zero, update hash = 31 * hash + byte for each
// byte, with overflow modulo 2^32. Mask the result with markWord::hash_mask and
// replace zero with one. Superclass and interface names are used as-is, without
// removing suffixes from them.
//
// The result is deterministic for the same input and hash mask, not a unique
// class identifier or a general guarantee that arbitrary generated classes
// match across JVMs. Distinct classes may receive the same hash.
static intptr_t stable_generated_class_identity_hash_code(Klass* klass) {
  if (!JVMCIUseStableGeneratedClassIdentityHashCodes) {
    return 0;
  }
  int dimension = 0;
  if (klass->is_objArray_klass()) {
    ObjArrayKlass* array_klass = ObjArrayKlass::cast(klass);
    dimension = array_klass->dimension();
    klass = array_klass->bottom_klass();
  }
  if (!klass->is_instance_klass()) {
    return 0;
  }
  InstanceKlass* instance_klass = InstanceKlass::cast(klass);
  if (!instance_klass->is_hidden()) {
    return 0;
  }
  const char* class_name = klass->name()->as_C_string();
  const char* unstable_suffix = strstr(class_name, "+0x");
  if (unstable_suffix == nullptr) {
    unstable_suffix = strstr(class_name, "/0x");
  }
  if (unstable_suffix == nullptr) {
    return 0;
  }
  size_t stable_name_length = unstable_suffix - class_name;
  while (stable_name_length > 0 && class_name[stable_name_length - 1] >= '0' && class_name[stable_name_length - 1] <= '9') {
    stable_name_length--;
  }
  if (stable_name_length > 0 && class_name[stable_name_length - 1] == '$') {
    stable_name_length--;
  }
  stringStream key;
  key.write(class_name, stable_name_length);
  if (instance_klass->super() != nullptr) {
    key.print_raw("@super:");
    key.print_raw(instance_klass->super()->name()->as_C_string());
  }
  append_interfaces(instance_klass, &key);
  if (dimension != 0) {
    key.print("@array:%d", dimension);
  }
  JVMCIClassIdentityHashCodeKey generated_key = { "@generated", key.as_string() };
  intptr_t hash_code = jvmci_class_identity_hash_code_key_hash(generated_key) & markWord::hash_mask;
  return hash_code == 0 ? 1 : hash_code;
}

static intptr_t class_identity_hash_code(Klass* klass) {
  if (!JVMCIUseStableGeneratedClassIdentityHashCodes && class_identity_hash_codes == nullptr) {
    return 0;
  }
  ResourceMark rm;
  intptr_t hash_code = stable_generated_class_identity_hash_code(klass);
  if (hash_code != 0) {
    return hash_code;
  }
  if (class_identity_hash_codes == nullptr) {
    return 0;
  }
  const char* loader_name = klass->class_loader_data()->loader_name();
  const char* class_name = klass->name()->as_C_string();
  hash_code = find_class_identity_hash_code(loader_name, class_name);
  if (hash_code != 0) {
    return hash_code;
  }
  int dimension = 0;
  Klass* component_klass = klass;
  if (klass->is_objArray_klass()) {
    ObjArrayKlass* array_klass = ObjArrayKlass::cast(klass);
    dimension = array_klass->dimension();
    component_klass = array_klass->bottom_klass();
  }
  if (component_klass->is_instance_klass()) {
    InstanceKlass* instance_klass = InstanceKlass::cast(component_klass);
    bool is_proxy_subclass = instance_klass->super() != nullptr && instance_klass->super()->name()->equals("java/lang/reflect/Proxy");
    if (instance_klass->local_interfaces()->length() != 0 || is_proxy_subclass) {
      stringStream interface_key;
      append_interfaces(instance_klass, &interface_key);
      if (dimension != 0) {
        interface_key.print("@array:%d", dimension);
      }
      return find_class_identity_hash_code(loader_name, interface_key.as_string());
    }
  }
  return 0;
}

void JVMCIGlobals::initialize_class_identity_hash_code(oop mirror, const char* primitive_name) {
  intptr_t requested_hash_code = primitive_name == nullptr ? class_identity_hash_code(java_lang_Class::as_Klass(mirror))
      : find_class_identity_hash_code(BOOTSTRAP_LOADER_NAME, primitive_name);
  if (requested_hash_code != 0) {
    intptr_t actual_hash_code = ObjectSynchronizer::FastHashCode(Thread::current(), mirror, &requested_hash_code);
    guarantee(actual_hash_code == requested_hash_code, "Class mirror identity hash code must be installed before publication");
  }
}

// Return true if jvmci flags are consistent.
bool JVMCIGlobals::check_jvmci_flags_are_consistent() {

#ifndef PRODUCT
#define APPLY_JVMCI_FLAGS(params3, params4) \
  JVMCI_FLAGS(params4, params3, params4, params3, IGNORE_RANGE, IGNORE_CONSTRAINT)
#define JVMCI_DECLARE_CHECK4(type, name, value, ...) bool name##checked = false;
#define JVMCI_DECLARE_CHECK3(type, name, ...)        bool name##checked = false;
#define JVMCI_FLAG_CHECKED(name)                          name##checked = true;
  APPLY_JVMCI_FLAGS(JVMCI_DECLARE_CHECK3, JVMCI_DECLARE_CHECK4)
#else
#define JVMCI_FLAG_CHECKED(name)
#endif

  // Checks that a given flag is not set if a given guard flag is false.
#define CHECK_NOT_SET(FLAG, GUARD)                     \
  JVMCI_FLAG_CHECKED(FLAG)                             \
  if (!GUARD && !FLAG_IS_DEFAULT(FLAG)) {              \
    jio_fprintf(defaultStream::error_stream(),         \
        "Improperly specified VM option '%s': '%s' must be enabled\n", #FLAG, #GUARD); \
    return false;                                      \
  }

  if (EnableJVMCIProduct) {
    if (FLAG_IS_DEFAULT(EnableJVMCI)) {
      FLAG_SET_DEFAULT(EnableJVMCI, true);
    }
    if (EnableJVMCI && FLAG_IS_DEFAULT(UseJVMCICompiler)) {
      FLAG_SET_DEFAULT(UseJVMCICompiler, true);
    }
  }

  JVMCI_FLAG_CHECKED(UseJVMCICompiler)
  JVMCI_FLAG_CHECKED(EnableJVMCI)
  JVMCI_FLAG_CHECKED(EnableJVMCIProduct)
  JVMCI_FLAG_CHECKED(UseGraalJIT)

  CHECK_NOT_SET(BootstrapJVMCI,               UseJVMCICompiler)
  CHECK_NOT_SET(PrintBootstrap,               UseJVMCICompiler)
  CHECK_NOT_SET(JVMCIThreads,                 UseJVMCICompiler)
  CHECK_NOT_SET(JVMCIHostThreads,             UseJVMCICompiler)
  CHECK_NOT_SET(LibJVMCICompilerThreadHidden, UseJVMCICompiler)

  if (UseJVMCICompiler) {
    if (!FLAG_IS_DEFAULT(EnableJVMCI) && !EnableJVMCI) {
      jio_fprintf(defaultStream::error_stream(),
          "Improperly specified VM option UseJVMCICompiler: EnableJVMCI cannot be disabled\n");
      return false;
    }
    FLAG_SET_DEFAULT(EnableJVMCI, true);
    FLAG_SET_ERGO_IF_DEFAULT(EagerJVMCI, true);
  }

  if (EnableJVMCI) {
    if (FLAG_IS_DEFAULT(UseJVMCINativeLibrary) && !UseJVMCINativeLibrary) {
      if (JVMCI::shared_library_exists()) {
        // If a JVMCI native library is present,
        // we enable UseJVMCINativeLibrary by default.
        FLAG_SET_DEFAULT(UseJVMCINativeLibrary, true);
      }
    }
  }

  if (UseJVMCICompiler) {
    if (BootstrapJVMCI && UseJVMCINativeLibrary) {
      jio_fprintf(defaultStream::error_stream(), "-XX:+BootstrapJVMCI is not compatible with -XX:+UseJVMCINativeLibrary\n");
      return false;
    }
    if (BootstrapJVMCI && (TieredStopAtLevel < CompLevel_full_optimization)) {
      jio_fprintf(defaultStream::error_stream(),
          "-XX:+BootstrapJVMCI is not compatible with -XX:TieredStopAtLevel=%zd\n", TieredStopAtLevel);
      return false;
    }
  }

  if (!EnableJVMCI) {
    // Switch off eager JVMCI initialization if JVMCI is disabled.
    // Don't throw error if EagerJVMCI is set to allow testing.
    if (EagerJVMCI) {
      FLAG_SET_DEFAULT(EagerJVMCI, false);
    }
  }
  JVMCI_FLAG_CHECKED(EagerJVMCI)

  CHECK_NOT_SET(JVMCIEventLogLevel,                            EnableJVMCI)
  CHECK_NOT_SET(JVMCITraceLevel,                               EnableJVMCI)
  CHECK_NOT_SET(JVMCICounterSize,                              EnableJVMCI)
  CHECK_NOT_SET(JVMCICountersExcludeCompiler,                  EnableJVMCI)
  CHECK_NOT_SET(JVMCINMethodSizeLimit,                         EnableJVMCI)
  CHECK_NOT_SET(JVMCIPrintProperties,                          EnableJVMCI)
  CHECK_NOT_SET(JVMCIThreadsPerNativeLibraryRuntime,           EnableJVMCI)
  CHECK_NOT_SET(JVMCICompilerIdleDelay,                        EnableJVMCI)
  CHECK_NOT_SET(UseJVMCINativeLibrary,                         EnableJVMCI)
  CHECK_NOT_SET(JVMCINativeLibraryThreadFraction,              EnableJVMCI)
  CHECK_NOT_SET(JVMCILibPath,                                  EnableJVMCI)
  CHECK_NOT_SET(JVMCINativeLibraryErrorFile,                   EnableJVMCI)
  CHECK_NOT_SET(JVMCILibDumpJNIConfig,                         EnableJVMCI)
  CHECK_NOT_SET(JVMCIClassIdentityHashCodeFile,                EnableJVMCI)
  CHECK_NOT_SET(JVMCIUseStableGeneratedClassIdentityHashCodes, EnableJVMCI)

#ifndef COMPILER2
  JVMCI_FLAG_CHECKED(EnableVectorAggressiveReboxing)
  JVMCI_FLAG_CHECKED(EnableVectorReboxing)
  JVMCI_FLAG_CHECKED(EnableVectorSupport)
  JVMCI_FLAG_CHECKED(MaxVectorSize)
  JVMCI_FLAG_CHECKED(ReduceInitialCardMarks)
  JVMCI_FLAG_CHECKED(UseMultiplyToLenIntrinsic)
  JVMCI_FLAG_CHECKED(UseSquareToLenIntrinsic)
  JVMCI_FLAG_CHECKED(UseMulAddIntrinsic)
  JVMCI_FLAG_CHECKED(UseMontgomeryMultiplyIntrinsic)
  JVMCI_FLAG_CHECKED(UseMontgomerySquareIntrinsic)
#endif // !COMPILER2
       //
  JVMCI_FLAG_CHECKED(UseVectorStubs)

#ifndef PRODUCT
#define JVMCI_CHECK4(type, name, value, ...) assert(name##checked, #name " flag not checked");
#define JVMCI_CHECK3(type, name, ...)        assert(name##checked, #name " flag not checked");
  // Ensures that all JVMCI flags are checked by this method.
  APPLY_JVMCI_FLAGS(JVMCI_CHECK3, JVMCI_CHECK4)
#undef APPLY_JVMCI_FLAGS
#undef JVMCI_DECLARE_CHECK3
#undef JVMCI_DECLARE_CHECK4
#undef JVMCI_CHECK3
#undef JVMCI_CHECK4
#undef JVMCI_FLAG_CHECKED
#endif // PRODUCT
#undef CHECK_NOT_SET

  if (JVMCILibDumpJNIConfig != nullptr) {
    _jni_config_file = new(mtJVMCI) fileStream(JVMCILibDumpJNIConfig);
    if (_jni_config_file == nullptr || !_jni_config_file->is_open()) {
      jio_fprintf(defaultStream::error_stream(),
          "Could not open file for dumping JVMCI shared library JNI config: %s\n", JVMCILibDumpJNIConfig);
      return false;
    }
  }

  if (!initialize_class_identity_hash_codes()) {
    return false;
  }

  return true;
}

// Convert JVMCI flags from experimental to product
bool JVMCIGlobals::enable_jvmci_product_mode(JVMFlagOrigin origin, bool use_graal_jit) {
  const char *JVMCIFlags[] = {
    "EnableJVMCI",
    "EnableJVMCIProduct",
    "UseJVMCICompiler",
    "JVMCIThreadsPerNativeLibraryRuntime",
    "JVMCICompilerIdleDelay",
    "JVMCIPrintProperties",
    "EagerJVMCI",
    "JVMCIThreads",
    "JVMCICounterSize",
    "JVMCICountersExcludeCompiler",
    "JVMCINMethodSizeLimit",
    "JVMCIEventLogLevel",
    "JVMCITraceLevel",
    "JVMCILibPath",
    "JVMCILibDumpJNIConfig",
    "JVMCIClassIdentityHashCodeFile",
    "JVMCIUseStableGeneratedClassIdentityHashCodes",
    "UseJVMCINativeLibrary",
    "JVMCINativeLibraryThreadFraction",
    "JVMCINativeLibraryErrorFile",
    "LibJVMCICompilerThreadHidden",
    nullptr
  };

  for (int i = 0; JVMCIFlags[i] != nullptr; i++) {
    JVMFlag *jvmciFlag = (JVMFlag *)JVMFlag::find_declared_flag(JVMCIFlags[i]);
    if (jvmciFlag == nullptr) {
      return false;
    }
    jvmciFlag->clear_experimental();
    jvmciFlag->set_product();
  }

  bool value = true;
  JVMFlag *jvmciEnableFlag = JVMFlag::find_flag("EnableJVMCIProduct");
  if (JVMFlagAccess::set_bool(jvmciEnableFlag, &value, origin) != JVMFlag::SUCCESS) {
    return false;
  }
  if (use_graal_jit) {
    JVMFlag *useGraalJITFlag = JVMFlag::find_flag("UseGraalJIT");
    if (JVMFlagAccess::set_bool(useGraalJITFlag, &value, origin) != JVMFlag::SUCCESS) {
      return false;
    }
  }

  // Effect of EnableJVMCIProduct on changing defaults of EnableJVMCI
  // and UseJVMCICompiler is deferred to check_jvmci_flags_are_consistent
  // so that setting these flags explicitly (e.g. on the command line)
  // takes precedence.

  return true;
}

bool JVMCIGlobals::gc_supports_jvmci() {
  return UseSerialGC || UseParallelGC || UseG1GC || UseZGC || UseShenandoahGC || UseEpsilonGC;
}

void JVMCIGlobals::check_jvmci_supported_gc() {
  if (EnableJVMCI) {
    // Check if selected GC is supported by JVMCI and Java compiler
    if (!gc_supports_jvmci()) {
      fatal("JVMCI does not support the selected GC");
    }
  }
}
