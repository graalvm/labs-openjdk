# This file is rendered into JSON with:
#
#    sjsonnet .ci/ci.jsonnet
#
# See: https://github.com/databricks/sjsonnet/releases

# JDK tests to run
local run_test_spec = 'test/hotspot/jtreg/compiler/jvmci test/jdk/tools/jlink/plugins/SaveJlinkArgfilesPluginTest.java';

local common = import 'common.libsonnet';

{
    overlay: '3bc18600dc70191a267a14f17e817fd071a2d277',
    specVersion: '3',

    local prebuild_artifact_url = 'jdk_tlda.archive.artifacts',
    Prebuild(major_java_version, release, defs):: common.LinuxAMD64(defs, major_java_version) + common.AMD64 + common.DefaultBootJDK + common.Packages + common.mxDependencies {
        name: 'prebuild-labsjdk-%s-ee-%s-linux-amd64' % [major_java_version, release.name],
        targets: ['dependency'],
        components+: ['build-labsjdk-snapshots-%s' % release.name],
        logs: [
            '*.log',
            '*/build/*/configure-support/sign-dummy/sign-dummy.signlog',
            'snapshot-labsjdk-versions.json'
        ],
        deploysArtifacts: true,
        python_version: '3',
        diskspace_required: '10G',
        setup: [
            ['set-export', 'BUILD_MAIN', '$PWD'],
            ['set-export', 'JIB_DATA_DIR', '${BUILD_MAIN}/../jib'],
            ['set-export', 'JIB_SERVER', defs.jib_server],
            ['set-export', 'JIB_SERVER_MIRRORS', defs.jib_server_mirrors],
        ],
        timelimit: '45:00',
        notify_emails: [defs.build_failure_notify],
        publishArtifacts: [
            {
                name: 'jdk-%s-%s-%s_tlda' % [release.name, major_java_version, release.build],
                dir: '.',
                patterns: [prebuild_artifact_url],
            },
        ],

        run+:
            # Clone JDK sources
            common.clone_labsjdk_builder(defs) +
            common.clone(defs, 'labsjdk-ee', release, 'labsjdk-ee', is_closed=true) +
            common.clone(defs, 'labsjdk-ce', release, 'labsjdk-ee/open') +
            [
                ['set-export', 'JAVA_VERSION', ['python', '-c', 'import sys; sys.path.insert(0, "${LABSJDK_BUILDER_DIR}"); import util; print(util.get_java_version(util.get_version_numbers_file("./labsjdk-ee/open")))']],
                ['set-export', 'JAVA_VERSION_BUILD', ['python', '-c', 'import sys; sys.path.insert(0, "${LABSJDK_BUILDER_DIR}"); import util; print(util.get_jdk_build_num("./labsjdk-ee/open", "${JAVA_VERSION}",show_output=False))']],
                [ 'python3', '-u', '${LABSJDK_BUILDER_DIR}/prebuild_labsjdk.py',
                    '--uploader=${ARTIFACT_UPLOADER_SCRIPT}',
                    '--notify-email', defs.upload_failure_notify,
                    '--artifact-repo-key=${ARTIFACT_REPO_KEY_LOCATION}',
                    '--patches=patches.tar.gz',
                    '--fingerprint=' + 'prebuild.fingerprint',
                    '--graalvm-version=%s' % release.name,
                    '--labsjdk-versions=snapshot-labsjdk-versions.json',
                    '--check-exists',
                    release.build,
                    'labsjdk-ee',
                    'jdk_tlda',
                ],
                ['cat', prebuild_artifact_url],
            ],
    },


    # Creates a builder object. This builds all variants (i.e. jdk, llvm and static libs).
    #
    # number major_java_version: major Java version (e.g. 25)
    # string edition: "ce" or "ee"
    # object platform: details of a platform including the packages, docker image or devkit supplying the dependencies
    Build(major_java_version, edition, release, platform, static_libs, llvm, defs):: platform + common.mxDependencies + common.Packages {
        local env = platform.environment,
        local os = platform.os,
        local arch = platform.arch,
        local exe_suffix = if os == 'windows' then '.exe' else '',
        local sep = if os == 'windows' then '\\' else '/',

        local path(p) = if os == 'windows' then std.strReplace(p, '/', '\\') else p,
        local exe(p) = if os == 'windows' then p + '.exe' else p,
        local jdk_src_dir = if edition == 'ee' then 'jdk_tlda' else 'labsjdk-' + edition,
        local type = if static_libs then 'static-libs' else if llvm then 'llvm' else 'snapshot',
        local is_windows(os) = os == 'windows',

        name: '%s-labsjdk-%s-%s-%s-%s-%s' % [type, major_java_version, edition, release.name, os, arch],
        targets: ['ondemand'],
        logs: [
            '*.log',
            '*/build/*/configure-support/sign-dummy/sign-dummy.signlog',
        ],
        diskspace_required: '10G',
        deploysArtifacts: true,
        python_version: '3',
        components+: ['build-labsjdk-snapshots-%s' % release.name],
        environment+: {
            ARTIFACT_REPO_URL: defs.artifacts_service_url,
            MX_PYTHON_VERSION: '3',
            MX_PYTHON: 'python3',
            },
        setup: common.clone_labsjdk_builder(defs, is_windows(os)) + [
            ['set-export', 'BUILD_MAIN', '$PWD'],
            ['set-export', 'JIB_DATA_DIR', path('${BUILD_MAIN}/../jib')],
            ['set-export', 'JIB_SERVER', defs.jib_server],
            ['set-export', 'JIB_SERVER_MIRRORS', defs.jib_server_mirrors],
        ] + (if edition == 'ce' then
             common.clone(defs, 'labsjdk-ce', release, 'labsjdk-ce', is_windows(os)) + [
                ['python3', '${LABSJDK_BUILDER_DIR}/snapshot_id.py', '-o', 'labsjdk-ce/snapshot_id.txt', 'labsjdk-ce'],
                ['set-export', 'JAVA_VERSION', ['python', '-c', 'import sys; sys.path.insert(0, "${LABSJDK_BUILDER_DIR}"); import util; print(util.get_java_version(util.get_version_numbers_file("./labsjdk-ce")))']],
                ['set-export', 'JAVA_VERSION_BUILD', ['python', '-c', 'import sys; sys.path.insert(0, "${LABSJDK_BUILDER_DIR}"); import util; print(util.get_jdk_build_num("./labsjdk-ce", "${JAVA_VERSION}",show_output=False))']],
                ['set-export', 'LLVM_BUNDLE_EDITION', ''],
                ['set-export', 'LLVM_BUNDLE_DIRECTORY_PREFIX', '-community'],

            ]
        else
        common.clone(defs, 'labsjdk-ce', release, 'labsjdk-ee-open', is_windows(os)) + [
            ['set-export', 'JAVA_VERSION', ['python', '-c', 'import sys; sys.path.insert(0, "${LABSJDK_BUILDER_DIR}"); import util; print(util.get_java_version(util.get_version_numbers_file("./labsjdk-ee-open")))']],
            ['set-export', 'JAVA_VERSION_BUILD', ['python', '-c', 'import sys; sys.path.insert(0, "${LABSJDK_BUILDER_DIR}"); import util; print(util.get_jdk_build_num("./labsjdk-ee-open", "${JAVA_VERSION}",show_output=False))']],
            ['set-export', 'LLVM_BUNDLE_EDITION', '-svmee'],
            ['set-export', 'LLVM_BUNDLE_DIRECTORY_PREFIX', ''],

            # Download TLDA source archive from prebuild step for EE labsjdk
            ['set-export', 'TLDA_URL', ['cat', prebuild_artifact_url]],
            ['python3', '-c', 'from urllib.request import urlretrieve;urlretrieve( "$TLDA_URL", "jdk_tlda.tar.gz")'],

            # Extract the TLDA sources. The -m options avoids issues with the
            # clock skew between the machine producing the archive and the
            # machine running this builder. This prevents issues when running
            # make such as:
            #
            #   Error: The configuration is not up to date for 'linux-aarch64'.
            #
            ['tar', 'xzfm', 'jdk_tlda.tar.gz'],

        ]) +
        (if llvm && os == 'darwin' && arch == 'aarch64' then [
            ['curl', '-L', defs.graalvm_llvm_standalone_macos_aarch64_url, '|', 'tar', 'xz'],
            ['set-export', 'TOOLCHAIN_GRAALVM', '$PWD/llvm${LLVM_BUNDLE_DIRECTORY_PREFIX}-24.1.0-macos-aarch64'],
            ['set-export', 'LLVM_TOOLCHAIN_LLI', '${TOOLCHAIN_GRAALVM}/bin/lli'],
        ] else []) +
        (if llvm && os == 'linux' && arch == 'amd64' then [

            ['curl', '-L', defs.graalvm_base_linux_url, '|', 'tar', 'xz'],
            ['set-export', 'TOOLCHAIN_GRAALVM', '$PWD/graalvm-ee-java11-22.1.0-dev'],
            [platform.java_home('$TOOLCHAIN_GRAALVM') + '/bin/gu', 'install', '-u', defs.graalvm_llvm_toolchain_installable_url],
            ['set-export', 'LLVM_TOOLCHAIN_LLI', platform.java_home('$TOOLCHAIN_GRAALVM') + '/bin/lli'],

        ] else []),

        timelimit: if llvm && os == 'darwin' then '4:30:00' else '3:30:00',
        notify_emails: [defs.build_failure_notify],

        local required_static_libs_platforms = common.get(common.get(platform, 'required_static_libs'), edition, []),
        local release_qualifier = release.name,

        local publishStaticLibsArtifacts = if static_libs then [{
            name: 'static-libs-' + edition + '-' + major_java_version + release_qualifier + '-' + os + '-' + arch,
            dir: '.',
            patterns: ['__ci_artifact_bookmarks'],
        }] else [],

        local requireStaticLibsArtifacts = [{
            name: 'static-libs-' + edition + '-' + major_java_version + release_qualifier + '-' + name,
            dir: '.',
        } for name in required_static_libs_platforms],

        local requireTLDAArtifacts = if edition == 'ee' then [
            {
                name: 'jdk-%s-%s-%s_tlda' % [release.name, major_java_version, release.build],
                dir: '.',
            },
        ] else [],

        publishArtifacts: publishStaticLibsArtifacts,
        requireArtifacts: requireStaticLibsArtifacts + requireTLDAArtifacts,
        local build(level, check_exists, major_java_version, build_number) = [ 'python3', '-u', '${LABSJDK_BUILDER_DIR}/build_labsjdk.py',
            '--bundles=' + if static_libs
                then 'static-libs'
                else if llvm then 'product,test'
                else 'product,static-libs,test',
            '--log=' + if os == 'windows' then 'info' else 'warn',
            '--java-major-version=' + major_java_version,
            '--java-build-number=' + build_number,
            '--jvmci-version=' + release.build,
            '--boot-jdk=${BOOT_JDK}',
            '--patches=' + jdk_src_dir + '/patches.tar.gz',
            '--clean-after-build',
            '--jdk-debug-level=' + level,
            '--java-home-link-target=' + level + '-java-home',
            '--fingerprint=' + level + '.fingerprint',
            '--uploader=${ARTIFACT_UPLOADER_SCRIPT}',
            '--graalvm-version=%s' % release.name,
            '--notify-email', defs.upload_failure_notify,
            '--artifact-repo-key=${ARTIFACT_REPO_KEY_LOCATION}',
            '--snapshot-id=' + path(jdk_src_dir + '/snapshot_id.txt'),
            jdk_src_dir,
        ] +
        (if edition == 'ee' then [
            '--jdk-build-num=' + path(jdk_src_dir + '/build_num.txt'),
        ] else []) +
        (if llvm then [
            '--sulong-toolchain=$LLVM_TOOLCHAIN_LLI',
            '--no-tests',  # do not need test bundles for llvm-labsjdk
        ] else [
            '--ci-artifact-bookmarks=__ci_artifact_bookmarks',
            '--test=test/hotspot/jtreg/compiler/jvmci',
        ]) +
        (if check_exists then ['--check-exists'] else []) +
        (if platform.arch == 'riscv64' then [
            '--target-platform=linux-riscv64',
            '--sysroot=$RISCV_HOME/fedora28_riscv_root',
        ] else []),

        run+: [
            build('release', true, '${JAVA_VERSION}', '${JAVA_VERSION_BUILD}'),
            build('fastdebug', false, '${JAVA_VERSION}', '${JAVA_VERSION_BUILD}'),
        ],
    },

    local produce_builds(major_java_version, releases, static_libs, llvm, defs) = [
        self.Build(major_java_version, edition, release, platform, static_libs, llvm, defs)
        for release in releases
        for platform in (
            if static_libs then static_libs_platforms(major_java_version, release, defs)
            else if llvm then llvm_platforms(defs, major_java_version, release)
            else jdk_platforms(major_java_version, release, defs)
        )
        for edition in platform.editions
    ],

    # Platform configs for builders that only produce static libs.
    local static_libs_platforms(major_java_version, release, defs) = [
        # linux-amd64-musl static libs are only built for CE
        common.LinuxDockerAMD64Musl(defs) + common.MuslBootJDK + { editions: ['ce', 'ee'] },
    ],

    # Platform configs for builders that produce llvm bitcode libs.
    local llvm_platforms(defs, major_java_version, release) = [
        common.LinuxAMD64(defs, major_java_version) + common.AMD64 + common.DefaultBootJDK + {
            editions: ['ce', 'ee'],
        },
        common.DarwinAArch64 + common.DefaultBootJDK + {
            editions: ['ce', 'ee'],
        },
    ],

    # Platform configs for JDK image builders.
    local jdk_platforms(major_java_version, release, defs) = [
        # linux-amd64 has CE and EE builds and the CE builds include the linux-amd64-musl static libs
        common.LinuxAMD64(defs, major_java_version) + common.AMD64 + common.DefaultBootJDK +
        { editions: ['ce', 'ee'] } +
        { required_static_libs:
            {ce: ['linux-amd64_musl'],
             ee: ['linux-amd64_musl'],
            },
        },

        common.LinuxAArch64(defs, major_java_version) + common.AArch64 + common.JTReg + common.DefaultBootJDK +
        { editions: ['ce', 'ee'] },

        # darwin-aarch64 has CE and EE builds on 11 and 16+
        common.DarwinAArch64
        { editions: ['ce', 'ee'] },

        common.WindowsCygwin + common.AMD64 + common.DefaultBootJDK +
        { editions: ['ce', 'ee'] },

    ] + static_libs_platforms(major_java_version, release, defs),  # For building musl boot JDKs

    SourceBuild(java_version, release, defs):: common.LinuxAMD64(defs, java_version) + common.AMD64 + common.OCI + common.DefaultBootJDK + common.Packages + common.mxDependencies + {
        local jvmci_build_number = release.build,
        local repo_name = 'labsjdk-ee',
        local jdk_conf_name = 'linux-x86_64-server-release',
        local src_bundle_suffixes = ['', '-security'],

        copy_source_bundle(suffix=''):: ['cp', '${JDK_SOURCE}/' + repo_name + '/build/' + jdk_conf_name + '/source-bundles/jdk-${JAVA_VERSION}+${JAVA_VERSION_BUILD}_src' + suffix + '.zip', '../results/'],
        unzip_source_bundle(suffix=''):: ['unzip', '-q', '../results/jdk-${JAVA_VERSION}+${JAVA_VERSION_BUILD}_src' + suffix + '.zip'],

        docker: {
          image: defs.linux_docker_image_amd64,
        },
        targets+: ['ondemand'],
        python_version: '3',
        deploysArtifacts: true,
        components+: ['build-labsjdk-snapshots-%s' % release.name],
        name: 'source-labsjdk-%s-ee-%s-%s-%s' % [java_version, release.name, self.os, self.arch],
        setup+: common.clone_labsjdk_builder(defs) + [
            ['set-export', 'BUILD_MAIN', '.'],
            ['mkdir', '../results'],
            ['mkdir', '../jvmci'],
            ['set-export', 'JDK_SOURCE', ['realpath', '..']],
        ],
        run+:
        common.clone(defs, repo_name, release, '$JDK_SOURCE/' + repo_name, is_closed=true) +
        [
            ['git', '-C', '$JDK_SOURCE/' + repo_name, 'clone', defs.labsjdk_ce_url, 'open'],
            ['git', '-C', '$JDK_SOURCE/' + repo_name + '/open', 'checkout', '${MAIN_REVISION}'],
            ['set-export', 'SNAPSHOT_ID', ['python3', '${LABSJDK_BUILDER_DIR}/snapshot_id.py', '$JDK_SOURCE/' + repo_name + '/open', '$JDK_SOURCE/' + repo_name]],
            ['set-export', 'JAVA_VERSION', ['python', '-c', 'import sys; sys.path.insert(0, "${LABSJDK_BUILDER_DIR}"); import util ;print(util.get_java_version(util.get_version_numbers_file("$JDK_SOURCE/labsjdk-ee/open")))']],
            ['set-export', 'JAVA_VERSION_BUILD', ['python', '-c', 'import sys; sys.path.insert(0, "${LABSJDK_BUILDER_DIR}"); import util; print(util.get_jdk_build_num("$JDK_SOURCE/labsjdk-ee/open", "${JAVA_VERSION}",show_output=False))']],
            ['cd', '$JDK_SOURCE/' + repo_name],
            [ 'sh', 'configure',
                '--with-devkit=$DEVKIT_ROOT',
                '--with-boot-jdk=$BOOT_JDK',
                '--with-jvm-features=compiler1,compiler2,jvmci',
                '--enable-jvm-feature-shenandoahgc',
                '--with-version-build=${JAVA_VERSION_BUILD}',
                '--with-version-pre=',
                '--with-version-opt=LTS-jvmci-' + release.name + '-' + release.build,
            ],
            ['make', 'source'],
        ] +
        [self.copy_source_bundle(suffix) for suffix in src_bundle_suffixes] +
        [
            ['zip', '-ur', '../results/jdk-${JAVA_VERSION}+${JAVA_VERSION_BUILD}_src.zip', '$BUILD_MAIN/closed/make/conf/*'],
            ['tar', 'czf', '../' + 'labsjdk-ee-${JAVA_VERSION}+${JAVA_VERSION_BUILD}-jvmci-%s-%s-${SNAPSHOT_ID}-src.tar.gz' % [release.name, release.build], '../results'],
            ['cd', '$JDK_SOURCE/jvmci'],
        ] +
        [self.unzip_source_bundle(suffix) for suffix in src_bundle_suffixes] +
        [
            [ 'sh', 'configure',
                '--with-version-pre=',
                '--with-version-opt=jvmci-' + release.build,
                '--with-devkit=$DEVKIT_ROOT',
                '--with-boot-jdk=$BOOT_JDK',
                '--with-version-build=${JAVA_VERSION_BUILD}',
                '--with-zlib=system',
                '--enable-dtrace=no',
                '--disable-warnings-as-errors',
            ],
            ['make', 'images', 'static-libs-bundles'],
            [
                'artifact_uploader.py',
                '../' + 'labsjdk-ee-${JAVA_VERSION}+${JAVA_VERSION_BUILD}-jvmci-%s-%s-${SNAPSHOT_ID}-src.tar.gz' % [release.name, release.build],
                'labsjdk/' + 'labsjdk-ee-${JAVA_VERSION}+${JAVA_VERSION_BUILD}-jvmci-%s-%s-${SNAPSHOT_ID}-src.tar.gz' % [release.name, release.build],
                'labsjdk',
                '--version', '${JAVA_VERSION}+${JAVA_VERSION_BUILD}-jvmci-%s-%s-${SNAPSHOT_ID}' % [release.name, release.build],
                '--jdk', java_version + '',
                '--revision', ['git', '-C', '$JDK_SOURCE/' + repo_name + '/open', 'rev-parse', 'HEAD'],
                '--edition', 'ee',
                '--artifact-type', 'source',
                '--lifecycle', 'snapshot',
                '--lifecycle', 'snapshot',
            ],
        ],
    },

    local setupJDKSources(conf) = {
        run+: [
            # To reduce load, the CI system does not fetch all tags so it must
            # be done explicitly as `build_labsjdk.py` relies on it.
            ['git', 'fetch', '-q', '--tags'],
        ] + (if conf.os == 'windows' then [
            # Need to fix line endings on Windows to satisfy cygwin
            # https://stackoverflow.com/a/26408129
            ['set-export', 'JDK_SRC_DIR', '${PWD}\\..\\jdk'],
            ['git', 'clone', '--quiet', '--config', 'core.autocrlf=input', '-c', 'gc.auto=0', '.', '${JDK_SRC_DIR}'],
        ] else [
            ['set-export', 'JDK_SRC_DIR', '${PWD}'],
        ]) + [
            ['set-export', 'JDK_SUITE_DIR', '${JDK_SRC_DIR}'],
        ],
    },

    # Only used by gate jobs to test that building labsjdk works.
    # It omits builds of static libs and llvm and also does no uploading.
    BuildValidation(defs, conf, is_musl_build, graalvm_version):: conf + setupJDKSources(conf) + (if is_musl_build then common.MuslBootJDK else common.DefaultBootJDK) + {
        name: 'gate-build-jdk' + conf.name,
        timelimit: '3:00:00',  # Windows is the long pole
        diskspace_required: '10G',
        logs: ['*.log'],
        targets: ['gate'],
        deploysArtifacts: true,

        local skip_fastdebug = conf.name == '-windows-cygwin-amd64',

        local build_labsjdk(jdk_debug_level, java_home_env_var) = [
            ['set-export', java_home_env_var, conf.path('${PWD}/../%s-java-home' % jdk_debug_level)],
            ['python3', '-u', conf.path('${LABSJDK_BUILDER_DIR}/build_labsjdk.py'),
                '--boot-jdk=${BOOT_JDK}',
                '--clean-after-build',
                '--uploader=disabled',
                '--jdk-debug-level=' + jdk_debug_level,
                '--jvmci-version=b01',
                '--graalvm-version=%s' % graalvm_version,
                '--test=' + run_test_spec,
                '--java-home-link-target=${%s}' % java_home_env_var,
            ] + (if is_musl_build then ['--bundles=static-libs'] else [])
            + ['${JDK_SRC_DIR}'],
        ],

        run+: (if !is_musl_build then [
            # Checks that each devkit mentioned in this file corresponds to a devkit in make/conf/jib-profiles.js
            ['python3', '-u', conf.path('${PWD}/.ci/check_devkit_versions.py')],
        ] else []) + common.clone_labsjdk_builder(defs, conf.os == 'windows') + [
            # This restricts cygwin to be on the PATH only while using jib.
            # It must not be on the PATH when building Graal.
            ['set-export', 'OLD_PATH', '${PATH}'],
            ['set-export', 'PATH', '${JIB_PATH}'],
            ['set-export', 'JIB_SERVER', defs.jib_server],
            ['set-export', 'JIB_SERVER_MIRRORS', defs.jib_server_mirrors],
            ['set-export', 'JIB_DATA_DIR', conf.path('${PWD}/../jib')],
        ] +
        build_labsjdk('release', 'JAVA_HOME') +
        (if skip_fastdebug then [] else build_labsjdk('fastdebug', 'JAVA_HOME_FASTDEBUG')),
    },

    local build_confs(defs) = [
        common.LinuxAMD64Graal(defs, true),
        common.LinuxAArch64Graal(defs, true),
        common.DarwinAArch64,
        common.Windows + common.AMD64,
    ],

    local amd64_musl_confs(defs) = [
        common.LinuxDockerAMD64Musl(defs),
    ],

    DefineBuilds(defs)::
        [self.BuildValidation(defs, conf, is_musl_build=false, graalvm_version=25.1) for conf in build_confs(defs)] +
        [self.BuildValidation(defs, conf, is_musl_build=true, graalvm_version=25.1) for conf in amd64_musl_confs(defs)],

    CreateBuilds(major_java_version, releases, defs)::
        [self.Prebuild(major_java_version, release, defs) for release in releases] +
        [self.SourceBuild(major_java_version, release, defs) for release in releases]

        + produce_builds(major_java_version, releases, false, false, defs)
        + produce_builds(major_java_version, releases, true, false, defs)
        + produce_builds(major_java_version, releases, false, true, defs),

    # These are configuration values that must be supplied externally (e.g. in an overlay)
    local defs = {
        # Linux Docker image based on Alpine 3.15 (based on musl-1.2.2).
        linux_docker_image_amd64_musl: 'artifacts.acme.com/docker/images/buildslave:gcc-10.3.1-musl-1.2.2',

        # Oracle Linux 8 Docker image for AMD64 architecture.
        linux_docker_image_amd64: 'artifacts.acme.com/docker/images/buildslave:ol8',

        # Primary JIB (JDK Integrated Builder) server URL.
        jib_server: 'https://build.dependencies.acme.com/artifactory',

        # Mirrors JIB (Java Image Builder) server URL.
        jib_server_mirrors: 'https://build.dependencies2.acme.com/artifactory https://build.dependencies3.acme.com/artifactory',

        # Internal artifacts service used to store the labsjdk binaries.
        artifacts_service_url: 'https://build.dependencies3.acme.com/artifactory',

        # GraalVM base image for the Linux AMD64 platform.
        graalvm_base_linux_url: 'https://acme.com/graalvm-bundle-release-generic-local/graalvm-java11-linux-amd64-22.1.0.tar.gz',

        # GraalVM LLVM toolchain installable for the Linux AMD64 platform.
        graalvm_llvm_toolchain_installable_url: 'https://acme.com/graalvm-bundle-release-generic-local/content/24.1.0/llvm-native-standalone-svm-java23-linux-amd64-24.1.0-signed.tar.gz',

        # GraalVM LLVM standalone toolchain tarball for macOS AArch64 (Apple Silicon).
        graalvm_llvm_standalone_macos_aarch64_url: 'https://acme.com/graalvm-bundle-release-generic-local/content/24.1.0/llvm-native-standalone-svm-java23-darwin-aarch64-24.1.0-signed.tar.gz',

        # Recipient(s) to notify in case of upload failures.
        upload_failure_notify: 'reviewer1@oracle.com,reviewer2@oracle.com',

        # List of recipient to notify in case of build failures.
        build_failure_notify: 'reviewer1@oracle.com,reviewer2@oracle.com',

        # List of default reviewers for automated Graal pull requests.
        graal_prs_default_reviewers: 'reviewer1@oracle.com,reviewer2@oracle.com',

        # Graal Enterprise repository.
        graal_enterprise_url: 'ssh://git@bitbucket.acme.com:8999/graal/graal-enterprise.git.',

        # Graal repository
        graal_url: 'ssh://git@bitbucket.acme.com:8999/graal/graal.git.',

        # GraalOS repository.
        graalos_url: 'ssh://git@bitbucket.acme.com:8999/graal/graalos.git.',

        # Buildbot CI repository.
        buildbot_ci_url: 'ssh://git@bitbucket.acme.com:8999/graal/buildbot-ci.git.',

        # labsjdk-builder repository.
        labsjdk_builder_url: 'ssh://git@bitbucket.acme.com:8999/graal/labsjdk-builder.git.',

        # labsjdk community edition repository.
        labsjdk_ce_url: 'ssh://git@bitbucket.acme.com:8999/graal/labsjdk-ce.git',

        # ol-automation user mail.
        ol_automation_user: 'ol@oracle.com',

        # Base URL for Bitbucket repositories
        bitbucket_base_url: 'ssh://git@bitbucket.acme.com:8999/graal/',

        # Proxy server URL.
        proxy_url: 'http://proxy.acme.com:8090',
    },
    builds: self.DefineBuilds(defs) + self.CreateBuilds(25, common.jvmci_releases, defs),
}
