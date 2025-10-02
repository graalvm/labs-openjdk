#
# Copyright (c) 2019, 2025, Oracle and/or its affiliates. All rights reserved.
# DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
#
# This code is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License version 2 only, as
# published by the Free Software Foundation.
#
# This code is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# version 2 for more details (a copy is included in the LICENSE file that
# accompanied this code).
#
# You should have received a copy of the GNU General Public License version
# 2 along with this work; if not, write to the Free Software Foundation,
# Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
#
# Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
# or visit www.oracle.com if you need additional information or have any
# questions.
#

.PHONY: all clean_all clean_cr clean_ur build_cr build_ur \
	clean_product_cr_so clean_product_cr_a clean_notproduct_cr_so clean_notproduct_cr_a clean_fastdebug_cr_so clean_fastdebug_cr_a clean_debug_cr_so clean_debug_cr_a \
	clean_product_ur_so clean_product_ur_a clean_notproduct_ur_so clean_notproduct_ur_a clean_fastdebug_ur_so clean_fastdebug_ur_a clean_debug_ur_so clean_debug_ur_a \
	build_product_cr_so build_product_cr_a build_notproduct_cr_so build_notproduct_cr_a build_fastdebug_cr_so build_fastdebug_cr_a build_debug_cr_so build_debug_cr_a \
	build_product_ur_so build_product_ur_a build_notproduct_ur_so build_notproduct_ur_a build_fastdebug_ur_so build_fastdebug_ur_a build_debug_ur_so build_debug_ur_a \

uname_m := $(shell uname -m)
ifeq ($(uname_m),x86_64)
    ARCH_SUFFIX := amd64
else ifeq ($(uname_m),aarch64)
    ARCH_SUFFIX := aarch64
else
    $(error Unexpected architecture $(uname_m))
endif

MAKEFILE_PATH := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
SUB_MAKEFILE := $(MAKEFILE_PATH)SubMakefile

all: clean_all build_all

clean_all: clean_cr clean_ur

clean_cr: clean_product_cr_so clean_product_cr_a \
	clean_notproduct_cr_so clean_notproduct_cr_a \
	clean_fastdebug_cr_so clean_fastdebug_cr_a \
	clean_debug_cr_so clean_debug_cr_a

clean_ur: clean_product_ur_so clean_product_ur_a \
	clean_notproduct_ur_so clean_notproduct_ur_a \
	clean_fastdebug_ur_so clean_fastdebug_ur_a \
	clean_debug_ur_so clean_debug_ur_a

clean_product_cr_so:
	$(MAKE) -f $(SUB_MAKEFILE) clean DEBUG_LEVEL=product TARGET_FILE_FORMAT=so COMPRESSED_REFERENCES=true

clean_product_cr_a:
	$(MAKE) -f $(SUB_MAKEFILE) clean DEBUG_LEVEL=product TARGET_FILE_FORMAT=a COMPRESSED_REFERENCES=true

clean_notproduct_cr_so:
	$(MAKE) -f $(SUB_MAKEFILE) clean DEBUG_LEVEL=notproduct TARGET_FILE_FORMAT=so COMPRESSED_REFERENCES=true

clean_notproduct_cr_a:
	$(MAKE) -f $(SUB_MAKEFILE) clean DEBUG_LEVEL=notproduct TARGET_FILE_FORMAT=a COMPRESSED_REFERENCES=true

clean_fastdebug_cr_so:
	$(MAKE) -f $(SUB_MAKEFILE) clean DEBUG_LEVEL=fastdebug TARGET_FILE_FORMAT=so COMPRESSED_REFERENCES=true

clean_fastdebug_cr_a:
	$(MAKE) -f $(SUB_MAKEFILE) clean DEBUG_LEVEL=fastdebug TARGET_FILE_FORMAT=a COMPRESSED_REFERENCES=true

clean_debug_cr_so:
	$(MAKE) -f $(SUB_MAKEFILE) clean DEBUG_LEVEL=debug TARGET_FILE_FORMAT=so COMPRESSED_REFERENCES=true

clean_debug_cr_a:
	$(MAKE) -f $(SUB_MAKEFILE) clean DEBUG_LEVEL=debug TARGET_FILE_FORMAT=a COMPRESSED_REFERENCES=true

clean_product_ur_so:
	$(MAKE) -f $(SUB_MAKEFILE) clean DEBUG_LEVEL=product TARGET_FILE_FORMAT=so COMPRESSED_REFERENCES=false

clean_product_ur_a:
	$(MAKE) -f $(SUB_MAKEFILE) clean DEBUG_LEVEL=product TARGET_FILE_FORMAT=a COMPRESSED_REFERENCES=false

clean_notproduct_ur_so:
	$(MAKE) -f $(SUB_MAKEFILE) clean DEBUG_LEVEL=notproduct TARGET_FILE_FORMAT=so COMPRESSED_REFERENCES=false

clean_notproduct_ur_a:
	$(MAKE) -f $(SUB_MAKEFILE) clean DEBUG_LEVEL=notproduct TARGET_FILE_FORMAT=a COMPRESSED_REFERENCES=false

clean_fastdebug_ur_so:
	$(MAKE) -f $(SUB_MAKEFILE) clean DEBUG_LEVEL=fastdebug TARGET_FILE_FORMAT=so COMPRESSED_REFERENCES=false

clean_fastdebug_ur_a:
	$(MAKE) -f $(SUB_MAKEFILE) clean DEBUG_LEVEL=fastdebug TARGET_FILE_FORMAT=a COMPRESSED_REFERENCES=false

clean_debug_ur_so:
	$(MAKE) -f $(SUB_MAKEFILE) clean DEBUG_LEVEL=debug TARGET_FILE_FORMAT=so COMPRESSED_REFERENCES=false

clean_debug_ur_a:
	$(MAKE) -f $(SUB_MAKEFILE) clean DEBUG_LEVEL=debug TARGET_FILE_FORMAT=a COMPRESSED_REFERENCES=false

build_all: build_cr build_ur

build_all_linux_amd64: build_cr build_ur

build_all_linux_aarch64: build_gcc_cr build_gcc_ur

build_gcc_cr: build_product_cr_so build_product_cr_a \
	build_notproduct_cr_so build_notproduct_cr_a \
	build_fastdebug_cr_so build_fastdebug_cr_a \
	build_debug_cr_so build_debug_cr_a

build_gcc_ur: build_product_ur_so build_product_ur_a \
	build_notproduct_ur_so build_notproduct_ur_a \
	build_fastdebug_ur_so build_fastdebug_ur_a \
	build_debug_ur_so build_debug_ur_a

build_cr: build_product_cr_so build_product_cr_a \
	build_notproduct_cr_so build_notproduct_cr_a \
	build_fastdebug_cr_so build_fastdebug_cr_a \
	build_debug_cr_so build_debug_cr_a

build_ur: build_product_ur_so build_product_ur_a \
	build_notproduct_ur_so build_notproduct_ur_a \
	build_fastdebug_ur_so build_fastdebug_ur_a \
	build_debug_ur_so build_debug_ur_a

build_product_cr_so:
	$(MAKE) -f $(SUB_MAKEFILE) build DEBUG_LEVEL=product TARGET_FILE_FORMAT=so COMPRESSED_REFERENCES=true

build_product_cr_a:
	$(MAKE) -f $(SUB_MAKEFILE) build DEBUG_LEVEL=product TARGET_FILE_FORMAT=a COMPRESSED_REFERENCES=true

build_notproduct_cr_so:
	$(MAKE) -f $(SUB_MAKEFILE) build DEBUG_LEVEL=notproduct TARGET_FILE_FORMAT=so COMPRESSED_REFERENCES=true

build_notproduct_cr_a:
	$(MAKE) -f $(SUB_MAKEFILE) build DEBUG_LEVEL=notproduct TARGET_FILE_FORMAT=a COMPRESSED_REFERENCES=true

build_fastdebug_cr_so:
	$(MAKE) -f $(SUB_MAKEFILE) build DEBUG_LEVEL=fastdebug TARGET_FILE_FORMAT=so COMPRESSED_REFERENCES=true

build_fastdebug_cr_a:
	$(MAKE) -f $(SUB_MAKEFILE) build DEBUG_LEVEL=fastdebug TARGET_FILE_FORMAT=a COMPRESSED_REFERENCES=true

build_debug_cr_so:
	$(MAKE) -f $(SUB_MAKEFILE) build DEBUG_LEVEL=debug TARGET_FILE_FORMAT=so COMPRESSED_REFERENCES=true

build_debug_cr_a:
	$(MAKE) -f $(SUB_MAKEFILE) build DEBUG_LEVEL=debug TARGET_FILE_FORMAT=a COMPRESSED_REFERENCES=true

build_product_ur_so:
	$(MAKE) -f $(SUB_MAKEFILE) build DEBUG_LEVEL=product TARGET_FILE_FORMAT=so COMPRESSED_REFERENCES=false

build_product_ur_a:
	$(MAKE) -f $(SUB_MAKEFILE) build DEBUG_LEVEL=product TARGET_FILE_FORMAT=a COMPRESSED_REFERENCES=false

build_notproduct_ur_so:
	$(MAKE) -f $(SUB_MAKEFILE) build DEBUG_LEVEL=notproduct TARGET_FILE_FORMAT=so COMPRESSED_REFERENCES=false

build_notproduct_ur_a:
	$(MAKE) -f $(SUB_MAKEFILE) build DEBUG_LEVEL=notproduct TARGET_FILE_FORMAT=a COMPRESSED_REFERENCES=false

build_fastdebug_ur_so:
	$(MAKE) -f $(SUB_MAKEFILE) build DEBUG_LEVEL=fastdebug TARGET_FILE_FORMAT=so COMPRESSED_REFERENCES=false

build_fastdebug_ur_a:
	$(MAKE) -f $(SUB_MAKEFILE) build DEBUG_LEVEL=fastdebug TARGET_FILE_FORMAT=a COMPRESSED_REFERENCES=false

build_debug_ur_so:
	$(MAKE) -f $(SUB_MAKEFILE) build DEBUG_LEVEL=debug TARGET_FILE_FORMAT=so COMPRESSED_REFERENCES=false

build_debug_ur_a:
	$(MAKE) -f $(SUB_MAKEFILE) build DEBUG_LEVEL=debug TARGET_FILE_FORMAT=a COMPRESSED_REFERENCES=false
