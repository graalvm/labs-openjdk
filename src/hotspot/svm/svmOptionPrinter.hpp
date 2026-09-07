/*
 * Copyright (c) 2024, 2026, Oracle and/or its affiliates. All rights reserved.
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

#ifndef SVM_OPTION_PRINTER
#define SVM_OPTION_PRINTER
#ifndef PRODUCT

#include "runtime/globals_shared.hpp"

/*
 * This file provides some infrastructure for printing all hosted/runtime options that are relevant for SVM. This file is
 * explicitly not included in product builds.
 *
 * By default, this infrastructure prints the option values as they are specified in the C++ source code. This can be changed,
 * so that the runtime values are printed instead, see the comment below.
 */


namespace svm_gc {

const int DIAGNOSTIC = 1 << 0;
const int MANAGEABLE = 1 << 1;
const int EXPERIMENTAL = 1 << 2;
const int INTERNAL = 1 << 3;

char OPTION_VALUE_BUFFER[255];

class OptionPrinter {
public:
  void print_hosted_option(const char *type, const char *name, const char* doc) {
   tty->print_cr("@Option(help = \"%s\", type = OptionType.%s)//", doc, get_option_type(0));
   tty->print_cr("protected static final HostedOptionKey<%s> %s = new G1HostedOptionKey<>(%s);", type, name, OPTION_VALUE_BUFFER);
   tty->cr();
  }

  void print_hosted_option(const char *type, const char *name, int extra_flags, const char* doc) {
   tty->print_cr("@Option(help = \"%s\", type = OptionType.%s)//", doc, get_option_type(extra_flags));
   tty->print_cr("protected static final HostedOptionKey<%s> %s = new G1HostedOptionKey<>(%s);", type, name, OPTION_VALUE_BUFFER);
   tty->cr();
  }

  void print_runtime_option(const char *type, const char *name, const char* doc) {
   tty->print_cr("@Option(help = \"%s\", type = OptionType.%s)//", doc, get_option_type(0));
   tty->print_cr("protected static final RuntimeOptionKey<%s> %s = new RuntimeOptionKey<>(%s, IsolateCreationOnly);", type, name, OPTION_VALUE_BUFFER);
   tty->cr();
  }

  void print_runtime_option(const char *type, const char *name, int extra_flags, const char* doc) {
   tty->print_cr("@Option(help = \"%s\", type = OptionType.%s)//", doc, get_option_type(extra_flags));
   tty->print_cr("protected static final RuntimeOptionKey<%s> %s = new RuntimeOptionKey<>(%s, IsolateCreationOnly);", type, name, OPTION_VALUE_BUFFER);
   tty->cr();
  }

private:
  const char* get_option_type(int extra_flags) {
    if ((extra_flags & INTERNAL) != 0) {
      return "Internal";
    } else if ((extra_flags & EXPERIMENTAL) != 0) {
      return "Experimental";
    } else {
      return "Expert";
    }
  }
};

OptionPrinter option_printer;

#define GET_JAVA_OPTION_TYPE(t)

#define JAVA_OPTION_TYPE_bool     "Boolean"
#define JAVA_OPTION_TYPE_int      "Integer"
#define JAVA_OPTION_TYPE_uint     "Integer"
#define JAVA_OPTION_TYPE_intx     "Long"
#define JAVA_OPTION_TYPE_uintx    "Long"
#define JAVA_OPTION_TYPE_uint64_t "Long"
#define JAVA_OPTION_TYPE_size_t   "Long"
#define JAVA_OPTION_TYPE_double   "Double"
#define JAVA_OPTION_TYPE(TYPE)    JAVA_OPTION_TYPE_ ## TYPE

#define OPTION_VALUE_TO_STRING_bool(VALUE)     sprintf(OPTION_VALUE_BUFFER, "%s", (VALUE ? "true" : "false"))
#define OPTION_VALUE_TO_STRING_int(VALUE)      sprintf(OPTION_VALUE_BUFFER, "%d", VALUE)
#define OPTION_VALUE_TO_STRING_uint(VALUE)     sprintf(OPTION_VALUE_BUFFER, "%u", VALUE)
#define OPTION_VALUE_TO_STRING_intx(VALUE)     sprintf(OPTION_VALUE_BUFFER, "%ldL", VALUE)
#define OPTION_VALUE_TO_STRING_uintx(VALUE)    sprintf(OPTION_VALUE_BUFFER, "%luL", VALUE)
#define OPTION_VALUE_TO_STRING_uint64_t(VALUE) sprintf(OPTION_VALUE_BUFFER, "%luL", VALUE)
#define OPTION_VALUE_TO_STRING_size_t(VALUE)   sprintf(OPTION_VALUE_BUFFER, "%luL", VALUE)
#define OPTION_VALUE_TO_STRING_double(VALUE)   sprintf(OPTION_VALUE_BUFFER, "%f", VALUE)
#define OPTION_VALUE_TO_STRING(TYPE, VALUE)    OPTION_VALUE_TO_STRING_ ## TYPE(VALUE)

/* To print the runtime values, change the argument of OPTION_VALUE_TO_STRING(...) from 'value' to 'name' below. */
#define PRINT_NI_HOSTED_FLAG(    type, name, value, ...) OPTION_VALUE_TO_STRING(type, value); option_printer.print_hosted_option(JAVA_OPTION_TYPE(type), STR(name), __VA_ARGS__);
#define PRINT_NI_HOSTED_PD_FLAG( type, name,        ...) OPTION_VALUE_TO_STRING(type, name);  option_printer.print_hosted_option(JAVA_OPTION_TYPE(type), STR(name), __VA_ARGS__);
#define PRINT_NI_RUNTIME_FLAG(   type, name, value, ...) OPTION_VALUE_TO_STRING(type, value); option_printer.print_runtime_option(JAVA_OPTION_TYPE(type), STR(name), __VA_ARGS__);
#define PRINT_NI_RUNTIME_PD_FLAG(type, name,        ...) OPTION_VALUE_TO_STRING(type, name);  option_printer.print_runtime_option(JAVA_OPTION_TYPE(type), STR(name), __VA_ARGS__);

#define PRINT_NI_FLAGS                   \
  ALL_FLAGS(PRINT_NI_HOSTED_FLAG,        \
            PRINT_NI_HOSTED_PD_FLAG,     \
            PRINT_NI_RUNTIME_FLAG,       \
            PRINT_NI_RUNTIME_PD_FLAG,    \
            IGNORE_FLAG,                 \
            IGNORE_FLAG,                 \
            IGNORE_FLAG,                 \
            IGNORE_FLAG,                 \
            IGNORE_RANGE,                \
            IGNORE_CONSTRAINT)


} // namespace svm_gc

#endif // !PRODUCT
#endif // SVM_OPTION_PRINTER
