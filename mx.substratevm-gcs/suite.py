#
# Copyright (c) 2026, Oracle and/or its affiliates. All rights reserved.
# DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
#
# This code is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License version 2 only, as
# published by the Free Software Foundation.  Oracle designates this
# particular file as subject to the "Classpath" exception as provided
# by Oracle in the LICENSE file that accompanied this code.
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

suite = {
    "mxversion": "7.82.2",
    "name": "substratevm-gcs",

    "defaultLicense": "GPLv2-CPE",

    "capture_suite_commit_info": True,

    "libraries" : {
        "MUSL_GCC_TOOLCHAIN" : {
            "packedResource": True,
            "urls" : ["https://lafo.ssw.uni-linz.ac.at/pub/toolchain-gcc-musl/toolchain-gcc-10.3.0-zlib-1.2.13-musl-1.2.5.2-linux-amd64.tar.gz"],
            "digest" : "sha512:1740d965ce08e581ef05b46748b562e05054bc4c267bfa1149ad6d80c305efa7368a2fd3f8a9b5d5ba95c86a011e7b494aa2b6fd8bc1ec50c89f5fd91b24a9c4",
        },
    },

    "projects": {
        "hotspot": {
            "class": "HotspotNativeProject",
            "subDir": "src",
            "results": [
                "<staticlib:g1gc-cr>",
                "<staticlib:g1gc-ur>",
            ],
            "multitarget": {
                "libc": ["default", "glibc", "musl"],
            },
            "os_arch": {
                "linux": {
                    "amd64": {
                        "ignore": False,
                        "always_build_targets": ['linux-amd64-glibc', 'linux-amd64-musl'],
                    },
                    "aarch64": {
                        "ignore": False,
                    },
                    "<others>": {
                        "ignore": "Only AMD64 and AArch64 are supported at the moment",
                    },
                },
                "darwin": {
                    "aarch64": {
                        "ignore": False,
                    },
                    "<others>": {
                        "ignore": "Only AArch64 is supported at the moment",
                    },
                },
                "windows": {
                    "amd64": {
                        "ignore": False,
                    },
                    "<others>": {
                        "ignore": "Only AMD64 is supported at the moment",
                    },
                },
                "<others>": {
                    "<others>": {
                        "ignore": "Only Linux/Darwin/Windows are supported at the moment",
                    },
                },
            },
        },
    },

    "distributions": {
        "SVM_G1GC": {
            "native": True,
            "platformDependent": True,
            "layout": {
                "<os>-<arch>/include/": [
                    "file:src/hotspot/svm/exports/g1GCStructs.h",
                ]
            },
            "os_arch": {
                "linux": {
                    "amd64": {
                        "layout": {
                            "<os>-<arch>/<staticlib:g1gc-cr>": {
                                "source_type": "dependency",
                                "dependency": "hotspot",
                                "path": "<os>-<arch>/glibc/<staticlib:g1gc-cr>",
                                "optional": True,
                            },
                            "<os>-<arch>/<staticlib:g1gc-ur>": {
                                "source_type": "dependency",
                                "dependency": "hotspot",
                                "path": "<os>-<arch>/glibc/<staticlib:g1gc-ur>",
                                "optional": True,
                            },
                            "<os>-<arch>/<staticlib:g1gc-musl-cr>": {
                                "source_type": "dependency",
                                "dependency": "hotspot",
                                "path": "<os>-<arch>/musl/<staticlib:g1gc-cr>",
                                "optional": True,
                            },
                            "<os>-<arch>/<staticlib:g1gc-musl-ur>": {
                                "source_type": "dependency",
                                "dependency": "hotspot",
                                "path": "<os>-<arch>/musl/<staticlib:g1gc-ur>",
                                "optional": True,
                            },
                            "<os>-<arch>/<staticlib:g1gc-musl-swcfi-cr>": {
                                "source_type": "dependency",
                                "dependency": "hotspot",
                                "path": "<os>-<arch>/musl-swcfi/<staticlib:g1gc-cr>",
                                "optional": True,
                            },
                            "<os>-<arch>/<staticlib:g1gc-musl-swcfi-ur>": {
                                "source_type": "dependency",
                                "dependency": "hotspot",
                                "path": "<os>-<arch>/musl-swcfi/<staticlib:g1gc-ur>",
                                "optional": True,
                            },
                        },
                    },
                    "aarch64": {
                        "layout": {
                            "<os>-<arch>/<staticlib:g1gc-cr>": {
                                "source_type": "dependency",
                                "dependency": "hotspot",
                                "path": "<os>-<arch>/glibc/<staticlib:g1gc-cr>",
                                "optional": True,
                            },
                            "<os>-<arch>/<staticlib:g1gc-ur>": {
                                "source_type": "dependency",
                                "dependency": "hotspot",
                                "path": "<os>-<arch>/glibc/<staticlib:g1gc-ur>",
                                "optional": True,
                            },
                        },
                    },
                },
                "darwin": {
                    "aarch64": {
                        "layout": {
                            "<os>-<arch>/<staticlib:g1gc-cr>": {
                                "source_type": "dependency",
                                "dependency": "hotspot",
                                "path": "<os>-<arch>/default/<staticlib:g1gc-cr>",
                                "optional": True,
                            },
                            "<os>-<arch>/<staticlib:g1gc-ur>": {
                                "source_type": "dependency",
                                "dependency": "hotspot",
                                "path": "<os>-<arch>/default/<staticlib:g1gc-ur>",
                                "optional": True,
                            },
                        },
                    },
                },
                "windows": {
                    "amd64": {
                        "layout": {
                            "<os>-<arch>/<staticlib:g1gc-cr>": {
                                "source_type": "dependency",
                                "dependency": "hotspot",
                                "path": "<os>-<arch>/default/<staticlib:g1gc-cr>",
                                "optional": True,
                            },
                            "<os>-<arch>/<staticlib:g1gc-ur>": {
                                "source_type": "dependency",
                                "dependency": "hotspot",
                                "path": "<os>-<arch>/default/<staticlib:g1gc-ur>",
                                "optional": True,
                            },
                        },
                    },
                },
            },
            "description": "SubstrateVM G1 GC components",
            "maven": False
        },

        "SVM_G1GC_GRAALVM_SUPPORT": {
            "native": True,
            "platformDependent": True,
            "description": "SubstrateVM G1 GC libraries for inclusion in GraalVM distributions",
            "layout": {
                "clibraries/": ["extracted-dependency:substratevm-gcs:SVM_G1GC"],
                "builder/clibraries/": ["extracted-dependency:substratevm-gcs:SVM_G1GC"],
            },
        },
    },
}
