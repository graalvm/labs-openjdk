#
# Copyright (c) 2019, 2026, Oracle and/or its affiliates. All rights reserved.
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

import os
import re
import shlex
import subprocess
import mx
import mx_gate
import mx_native
import mx_subst

from mx_gate import Task

suite = mx.suite('substratevm-gcs')

os_arch_couplet = '%s_%s' % (mx.get_os(), mx.get_arch())

REACHABILITY_ANALYSIS_ENV = 'SVM_GC_REACHABILITY_ANALYSIS'
PRODUCT_STATIC_TARGETS = ('build_product_ur_a', 'build_product_cr_a')
REACHABILITY_ANALYSIS_TARGETS = (
    'build_product_ur_a', 'build_product_cr_a',
    'build_product_ur_so', 'build_product_cr_so',
    'build_debug_ur_so', 'build_debug_cr_so',
)


def _reachability_analysis_enabled():
    value = mx.get_env(REACHABILITY_ANALYSIS_ENV, '').lower()
    if value not in ('', '0', 'false', '1', 'true'):
        mx.abort(f'{REACHABILITY_ANALYSIS_ENV} must be true or false')
    return value in ('1', 'true')


def mx_register_dynamic_suite_constituents(register_project, register_distribution):
    if os_arch_couplet == 'linux_amd64' and mx.suite("sdk", fatalIfMissing=False) is None:
        # graal/sdk is not available, use a replacement for sdk:MUSL_NINJA_TOOLCHAIN
        mx.log("Registering replacement MUSL_NINJA_TOOLCHAIN")
        deps = ["MUSL_GCC_TOOLCHAIN", "mx:GCC_NINJA_TOOLCHAIN"]
        layout = {
            "toolchain.ninja": {
                "source_type": "string",
                "value": (
                    "include <ninja-toolchain:GCC_NINJA_TOOLCHAIN>\n"
                    "CC=<path:MUSL_GCC_TOOLCHAIN>/musl-toolchain/bin/x86_64-linux-musl-gcc\n"
                    "CXX=<path:MUSL_GCC_TOOLCHAIN>/musl-toolchain/bin/x86_64-linux-musl-g++\n"
                    "AR=<path:MUSL_GCC_TOOLCHAIN>/musl-toolchain/bin/x86_64-linux-musl-ar\n"
                ),
            },
        }
        path = None
        platformDependent = True
        theLicense = None
        attrs = {
            "native_toolchain": {
                "kind": "ninja",
                "target": {
                    "libc": "musl",
                },
            },
        }
        register_distribution(mx.LayoutDirDistribution(suite, 'MUSL_NINJA_TOOLCHAIN', deps, layout, path, platformDependent, theLicense, **attrs))


def _svm_gc_component():
    if os_arch_couplet in ('linux_amd64', 'linux_aarch64', 'darwin_aarch64', 'windows_amd64'):
        if mx.suite("sdk", fatalIfMissing=False):
            import mx_sdk_vm
            return mx_sdk_vm.GraalVmJreComponent(
                suite=suite,
                name='SubstrateVM GC',
                short_name='svmgc',
                dir_name='svm',
                installable_id='native-image',
                license_files=[],
                third_party_license_files=[],
                support_distributions=['substratevm-gcs:SVM_G1GC_GRAALVM_SUPPORT'],
                installable=True,
            )
        else:
            mx.warn("Could not register the SubstrateVM GC component since /sdk was not imported")
    return None

svm_gc = _svm_gc_component()
if svm_gc is not None:
    import mx_sdk_vm
    mx_sdk_vm.register_graalvm_component(svm_gc)


class _TargetBuildConfig:
    DEBUG_LEVELS = ('product', 'notproduct', 'fastdebug', 'debug')
    TARGET_FILE_FORMATS = ('so', 'a')
    REFERENCE_MODES = (
        ('cr', ['-DSVM_COMPRESSED_REFERENCES']),
        ('ur', []),
    )

    def __init__(self, target):
        self.platform = self._platform(target)
        self.reachability_analysis = _reachability_analysis_enabled()
        if self.reachability_analysis and self.platform not in ('bsd', 'linux'):
            mx.abort(f'{REACHABILITY_ANALYSIS_ENV} is only supported on Linux and Darwin')
        self.arch_suffix = self._arch_suffix(target)
        self.arch_cpu = 'x86' if self.arch_suffix == 'amd64' else 'aarch64'
        self.os_cpu = f'{self.platform}_{self.arch_cpu}'
        self.defines = [
            '-DSVM', '-DINCLUDE_SUFFIX_OS=_' + self.platform, '-D_LP64=1', '-DINCLUDE_JVMCI=1', '-DINCLUDE_JFR=0', '-DINCLUDE_G1GC=1', '-DINCLUDE_JVMTI=0',
            '-DINCLUDE_SERVICES=0', '-DINCLUDE_MANAGEMENT=0', '-DINCLUDE_CDS=0', '-DINCLUDE_CMSGC=0', '-DINCLUDE_EPSILONGC=0', '-DINCLUDE_PARALLELGC=0',
            '-DINCLUDE_SERIALGC=0', '-DINCLUDE_SHENANDOAHGC=0', '-DINCLUDE_ZGC=0', '-DINCLUDE_NMT=0', '-DVM_LITTLE_ENDIAN', '-DIGNORE_CODE_RELATED_OOPS=1',
            '-DSUPPORTS_CLOCK_MONOTONIC', '-D__STDC_FORMAT_MACROS', '-D__STDC_LIMIT_MACROS', '-D__STDC_CONSTANT_MACROS', '-D_FILE_OFFSET_BITS=64', '-D_REENTRANT',
        ]
        self.debug_level_defines = {
            'product': ['-DPRODUCT', self._debug_level_define('product')],
            'notproduct': [self._debug_level_define('notproduct')],
            'fastdebug': ['-DASSERT', self._debug_level_define('fastdebug')],
            'debug': ['-DASSERT', self._debug_level_define('debug')],
        }

        if self.arch_suffix == 'amd64':
            self.defines += ['-DINCLUDE_SUFFIX_CPU=_x86', '-DAMD64']
        else:
            self.defines += ['-DINCLUDE_SUFFIX_CPU=_aarch64', '-DAARCH64']

        self.shared_library_linker_libs = []

        if self.platform == 'windows':
            self.defines += ['-DINCLUDE_SUFFIX_COMPILER=_visCPP', '-DTARGET_COMPILER_visCPP', '-D_WINDOWS', '-DNOMINMAX', '-DWIN32_LEAN_AND_MEAN']
            self.target_prefix = ''
            self.object_ext = '.obj'
            self.target_ext_by_format = {
                'so': '.dll',
                'a': '.lib',
            }
            self.compiler_flags_by_format = {
                'so': ['-DSVM_GC_BUILD_SHARED_LIBRARY'],
                'a': [],
            }
            self.excluded_platforms = ['bsd', 'linux', 'posix']
            self.platform_includes = ['mocks/os/windows/include']
            disabled_warnings = ['4624', '4244', '4291', '4146', '4127', '4722']
            self.compiler_flags = [
                '-std:c++14',
                '-GR-',
                '-EHs-c-',
                '-FS',
                '-MD',
                '-D_HAS_EXCEPTIONS=0',
            ] + [f'-wd{warning}' for warning in disabled_warnings]
            self.debug_level_compiler_flags = {
                'product': ['-O2'],
                'notproduct': ['-O2'],
                'fastdebug': ['-Zi', '-O2', '-Oy-'],
                'debug': ['-Zi', '-Od'],
            }
            self.shared_library_linker_flags = ['-dll']
            self.shared_library_linker_libs = [
                'advapi32.lib',
                'version.lib',
            ]
        else:
            self.defines += ['-DINCLUDE_SUFFIX_COMPILER=_gcc', '-DTARGET_COMPILER_gcc']
            self.target_prefix = 'lib'
            self.object_ext = '.o'
            self.target_ext_by_format = {
                'so': '.so',
                'a': '.a',
            }
            self.compiler_flags_by_format = {
                'so': ['-DSVM_GC_BUILD_SHARED_LIBRARY', '-fvisibility=hidden'],
                'a': [],
            }
            if self.reachability_analysis:
                self.compiler_flags_by_format['so'] += ['-ffunction-sections', '-fdata-sections']
            self.excluded_platforms = [p for p in ('bsd', 'linux', 'windows') if p != self.platform]
            self.platform_includes = ['os/posix', 'os/posix/include', 'mocks/os/posix']
            self.compiler_flags = ['-Wno-format-zero-length', '-fPIC', '-fno-rtti', '-fno-exceptions', '-fno-strict-aliasing', '-fno-omit-frame-pointer', '-fstack-protector', '-std=c++14']

            self.debug_level_compiler_flags = {
                'product': ['-O3'],
                'notproduct': ['-O3'],
                'fastdebug': ['-g', '-O3'],
                'debug': ['-g', '-O0'],
            }

            if self.platform == 'bsd':
                self.defines += ['-DDARWIN', '-D_ALLBSD_SOURCE', '-D_DARWIN_C_SOURCE', '-D_XOPEN_SOURCE', '-DMAC_OS_X_VERSION_MIN_REQUIRED=110000']
                self.compiler_flags += ['-mmacosx-version-min=11.00.00']
                self.shared_library_linker_flags = ['-dynamiclib', '-compatibility_version', '1.0.0', '-current_version', '1.0.0', '-mmacosx-version-min=11.00.00', '-Wl,-rpath,@loader_path/.', '-Wl,-rpath,@loader_path/..']
            else:
                self.defines += ['-DLINUX']
                if target.libc == 'musl':
                    self.defines += ['-DMUSL_LIBC']
                self.shared_library_linker_flags = ['-shared', '-Wl,-z,noexecstack']

    def target_file(self, debug_level, reference_mode, target_file_format):
        return f'{self.target_prefix}g1gc{self._debug_level_file_suffix(debug_level)}-{reference_mode}{self.target_ext_by_format[target_file_format]}'

    def shared_library_linker_flags_for(self, target_file):
        # Windows uses a distinct import library per DLL to prevent naming conflicts with the logic that builds the static libs
        if self.platform == 'windows':
            target_file_base, _ = os.path.splitext(target_file)
            return self.shared_library_linker_flags + [f'-implib:{target_file_base}.import.lib']

        flags = list(self.shared_library_linker_flags)
        if self.reachability_analysis:
            if self.platform == 'bsd':
                flags += ['-Wl,-dead_strip', f'-Wl,-map,{target_file}.reachability.map']
            else:
                flags += ['-Wl,--gc-sections', f'-Wl,-Map={target_file}.reachability.map']
        return flags

    def object_file(self, source_file, debug_level, reference_mode, target_file_format):
        base, _ = os.path.splitext(source_file)
        return f'{debug_level}-{reference_mode}{self.target_ext_by_format[target_file_format]}/{base}{self.object_ext}'

    def source_files(self, project_root, extension):
        result = []
        for root, _, files in os.walk(project_root):
            for filename in files:
                if not filename.endswith(extension):
                    continue

                source_file = os.path.relpath(os.path.join(root, filename), project_root).replace(os.sep, '/')
                if self._platform_excluded(source_file) or self._cpu_excluded(source_file):
                    continue

                result.append(source_file)
        return sorted(result)

    def include_dirs(self):
        return [
            '.', f'os/{self.platform}', *self.platform_includes, 'share', 'share/include', 'share/precompiled', 'share/utilities', 'mocks',
            f'mocks/os/{self.platform}', 'mocks/share', 'mocks/share/include', 'svm', 'svm/share', f'cpu/{self.arch_cpu}', f'os_cpu/{self.os_cpu}',
            f'mocks/cpu/{self.arch_cpu}', f'mocks/os_cpu/{self.os_cpu}',
        ]

    def compiler_flags_for(self, debug_level, target_file_format, reference_mode_defines):
        return (
            self.compiler_flags +
            self.debug_level_compiler_flags[debug_level] +
            self.debug_level_defines[debug_level] +
            self.compiler_flags_by_format[target_file_format] +
            self.defines + reference_mode_defines
        )

    def _platform_excluded(self, source_file):
        parts = source_file.split('/')
        filename = parts[-1]
        for excluded in self.excluded_platforms:
            if any(part == excluded or part.startswith(f'{excluded}_') for part in parts):
                return True

            if re.search(rf'_{re.escape(excluded)}(_.*)?\.(cpp|S)$', filename):
                return True

        return False

    def _cpu_excluded(self, source_file):
        filename = source_file.split('/')[-1]
        excluded_suffixes = ('_aarch64.cpp', '_aarch64.S') if self.arch_suffix == 'amd64' else ('_x86.cpp', '_x86_64.S')
        return filename.endswith(excluded_suffixes)

    @staticmethod
    def _platform(target):
        if target.os == 'darwin':
            return 'bsd'
        elif target.os in ('linux', 'windows'):
            return target.os

        mx.abort(f'Unsupported target OS: {target.os}')

    @staticmethod
    def _arch_suffix(target):
        if target.arch in ('amd64', 'aarch64'):
            return target.arch

        mx.abort(f'Unsupported target architecture: {target.arch}')

    @staticmethod
    def _debug_level_define(debug_level):
        return rf'-DDEBUG_LEVEL=\"{debug_level}\"'

    @staticmethod
    def _debug_level_file_suffix(debug_level):
        if debug_level == 'product':
            return ''
        elif debug_level == 'notproduct':
            return '-notproduct'
        elif debug_level == 'fastdebug':
            return '-fastdebug'
        elif debug_level == 'debug':
            return '-debug'

        mx.abort(f'Unsupported debug level: {debug_level}')


def _ninja_toolchain_variable(toolchain_file, variable):
    result = None
    with open(toolchain_file, encoding='utf-8') as fp:
        for line in fp:
            line = line.strip()
            include_match = re.fullmatch(r'include\s+(.+)', line)
            if include_match:
                include_file = include_match.group(1)
                if not os.path.isabs(include_file):
                    include_file = os.path.join(os.path.dirname(toolchain_file), include_file)
                included_result = _ninja_toolchain_variable(include_file, variable)
                if included_result is not None:
                    result = included_result
                continue

            variable_match = re.fullmatch(rf'{re.escape(variable)}\s*=\s*(.*)', line)
            if variable_match:
                result = variable_match.group(1)
    return result


def _toolchain_supports_compiler_flag(toolchain, flag):
    toolchain_file = os.path.join(toolchain.get_path(), 'toolchain.ninja')
    cxx = _ninja_toolchain_variable(toolchain_file, 'CXX')
    if cxx is None:
        mx.abort(f'Could not determine CXX from {toolchain_file}')

    try:
        subprocess.check_output(shlex.split(cxx) + [flag, '-c', os.devnull], stderr=subprocess.STDOUT)
        return True
    except subprocess.CalledProcessError:
        return False


class HotspotNativeProject(mx_native.NinjaProject):
    def __init__(self, suite, name, deps, workingSets, subDir, **kwargs):
        d = os.path.join(suite.dir, subDir, name)
        srcDirs = [d]
        kwargs.setdefault('max_jobs', mx.AbstractNativeBuildTask.default_parallelism)
        super(HotspotNativeProject, self).__init__(suite, name, subDir, srcDirs, deps, workingSets, d, **kwargs)

    toolchain_kind = 'ninja'

    def get_results(self):
        subst_engine = mx_subst.as_engine(mx_subst.results_substitutions)
        for r in self.results:
            yield subst_engine.substitute(r, dependency=self)

    def generate_manifest_for_task(self, task, output_dir, filename):
        with mx_native.NinjaManifestGenerator(self, output_dir, filename, toolchain=task.toolchain) as gen:
            gen.comment("Toolchain configuration")
            gen.include(os.path.join(task.toolchain.get_path(), 'toolchain.ninja'))
            config = _TargetBuildConfig(task.toolchain.spec.target)
            cpp_sources = config.source_files(self.dir, '.cpp')
            asm_sources = config.source_files(self.dir, '.S')

            if config.platform == 'linux' and _toolchain_supports_compiler_flag(task.toolchain, '-fno-lifetime-dse'):
                config.compiler_flags += ['-fno-lifetime-dse']

            gen.include_dirs(config.include_dirs())

            reference_mode_targets = []
            for reference_mode, reference_mode_defines in config.REFERENCE_MODES:
                all_targets = []
                for debug_level in config.DEBUG_LEVELS:
                    for target_file_format in config.TARGET_FILE_FORMATS:
                        cflags = config.compiler_flags_for(debug_level, target_file_format, reference_mode_defines)
                        object_files = []
                        for source_file in cpp_sources:
                            object_file = config.object_file(source_file, debug_level, reference_mode, target_file_format)
                            object_files += gen.n.build(object_file, 'cxx', f'$project/{source_file}', variables={'cflags': cflags})
                        for source_file in asm_sources:
                            object_file = config.object_file(source_file, debug_level, reference_mode, target_file_format)
                            object_files += gen.n.build(object_file, 'asm', f'$project/{source_file}', variables={'cflags': cflags})
                        target_file = config.target_file(debug_level, reference_mode, target_file_format)
                        if target_file_format == 'a':
                            gen.ar(target_file, object_files)
                        else:
                            gen.n.build(target_file, 'linkxx', object_files, variables={
                                'ldflags': config.shared_library_linker_flags_for(target_file),
                                'ldlibs': config.shared_library_linker_libs,
                            })
                        name = f'build_{debug_level}_{reference_mode}_{target_file_format}'
                        all_targets += gen.n.build(name, 'phony', target_file)
                reference_mode_targets += gen.n.build(f'build_{reference_mode}', 'phony', all_targets)
            gen.n.build('build_all', 'phony', reference_mode_targets)

            targets_str = mx.get_env('SVM_GC_TARGETS')
            if targets_str:
                targets = targets_str.split()
            else:
                targets = list(REACHABILITY_ANALYSIS_TARGETS if _reachability_analysis_enabled() else PRODUCT_STATIC_TARGETS)
            gen.n.default(targets)

    def _archivable_results(self, target_arch, use_relpath, single):
        assert not single
        archive_dir = os.path.join(self.out_dir, target_arch)

        targets = set(mx.get_env('SVM_GC_TARGETS', '').split())
        if not targets:
            for r in self.get_results():
                yield self._archivable_result(use_relpath, archive_dir, r)
            return

        build_all = 'build_all' in targets
        subst_engine = mx_subst.as_engine(mx_subst.results_substitutions)
        for debug_level in _TargetBuildConfig.DEBUG_LEVELS:
            for reference_mode, _ in _TargetBuildConfig.REFERENCE_MODES:
                if build_all or f'build_{reference_mode}' in targets or f'build_{debug_level}_{reference_mode}_a' in targets:
                    suffix = reference_mode if debug_level == 'product' else f'{debug_level}-{reference_mode}'
                    result = subst_engine.substitute(f'<staticlib:g1gc-{suffix}>', dependency=self)
                    yield self._archivable_result(use_relpath, archive_dir, result)

    def _build_task(self, target_arch, args, toolchain=None):
        return HotspotNativeBuildTask(args, self, target_arch, toolchain=toolchain)


class HotspotNativeBuildTask(mx_native.NinjaBuildTask):
    def newestOutput(self):
        if mx.get_env('SVM_GC_TARGETS'):
            return None
        manifest = os.path.join(self.out_dir, 'build.ninja')
        try:
            with open(manifest, encoding='utf-8') as fp:
                manifest_has_reachability_analysis = '.reachability.map' in fp.read()
        except FileNotFoundError:
            manifest_has_reachability_analysis = False
        if manifest_has_reachability_analysis != _reachability_analysis_enabled():
            return None
        return mx.TimeStampFile.newest(os.path.join(self.out_dir, r) for r in self.subject.get_results())


def gate_body(args, tasks):
    with Task('Build all SVM GC configurations', tasks, tags=['build_all']) as t:
        if t:
            os.environ['SVM_GC_TARGETS'] = 'build_all'
            mx.command_function('build')(['--no-daemon'])

mx_gate.add_gate_runner(suite, gate_body)
