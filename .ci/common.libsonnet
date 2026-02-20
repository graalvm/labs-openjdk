{
    # The JVMCI releases that can be built from this repo.
    jvmci_releases:: [
        self.JVMCIRelease(name='25.1', build='b16', jdk_version='25.0.2+10')
    ],

    # Specifies a JVMCI release.
    #
    # name:
    #    The release name (see GR-70870). The value is an arbitrary string.
    #    It may look like a version (e.g. "25.1") but can just as easily be
    #    something like "Feature".
    #    See: jdk.graal.compiler.hotspot.JVMCIVersionCheck.Version#releaseName
    #
    # build:
    #    The JVMCI build number string (e.g. "b03" or "b10")
    #    See: jdk.graal.compiler.hotspot.JVMCIVersionCheck.Version#jvmciBuild
    #
    # jdk_version:
    #    The JDK version info to include in artifact names (e.g. "25.0.1+8")
    #
    JVMCIRelease(name, build, jdk_version):: {
        name: name,
        build: build
    },

    # Version of the labsjdk-builder scripts to use
    labsjdk_builder_version:: '548e136cd2b100fb1b7c4143bfcc91f3b100ac42',

    # Clones a JDK repository
    # If `!is_closed`, then the cloned repo is checked out to branch ${MAIN_REVISION}.
    # If `is_closed`, then the cloned repo is checked out to branch jdk25.
    clone(defs, repo, release, dst_dir, is_windows=false, is_closed=false):: [
        ['git', 'clone', '--config', 'core.autocrlf=input', '--quiet', defs.bitbucket_base_url + repo + '.git', dst_dir],
    ] + (if is_closed then [
        ['git', '-C', dst_dir, 'checkout', 'jdk25'],
    ] else [
        ['git', '-C', dst_dir, 'checkout', '${MAIN_REVISION}'],
    ]),

    # Clones the labsjdk-builder repository to ${PWD}/labsjdk-builder
    clone_labsjdk_builder(defs, is_windows=false):: (if !is_windows then [
        ['set-export', 'LABSJDK_BUILDER_DIR', '${PWD}/../labsjdk-builder'],
    ] else [
        ['set-export', 'LABSJDK_BUILDER_DIR', '${PWD}\\..\\labsjdk-builder'],
    ]) + [
        ['git', 'clone', '--quiet', '--config', 'core.autocrlf=input', defs.labsjdk_builder_url, '${LABSJDK_BUILDER_DIR}'],
        ['git', '-C', '${LABSJDK_BUILDER_DIR}', 'checkout', self.labsjdk_builder_version],
    ],

    # Returns the value of the `name` field if it exists in `obj` otherwise `default`.
    get(obj, name, default=null)::
        if obj == null then default else
        if std.objectHas(obj, name) then obj[name] else default,

    # Returns true if `str` contains `needle` as a substring.
    contains(str, needle):: std.findSubstr(needle, str) != [],

    Packages:: {
        packages+: {
            'pip:requests': '2.25.1',
        },
    },

    mxDependencies:: {
        python_version: '3',
        packages+: {
            mx: 'HEAD',
      },
    },

    OSBase:: self.mxDependencies {
        path(unixpath):: unixpath,
        exe(unixpath):: unixpath,
        jdk_home(java_home):: self.path(java_home),
        java_home(jdk_home):: self.path(jdk_home),
        copydir(src, dst):: ['cp', '-r', src, dst],
        environment+: {
            JIB_PATH: '${PATH}',
            MAKE: 'make',
            ZLIB_BUNDLING: 'system',
            MX_PYTHON: 'python3.8',
        },
        packages+: {
            cmake: '==3.22.2',
        },
    },

    OCI:: {
        capabilities+: ['oci'],
    },

    Windows:: self.OSBase {
        path(unixpath):: std.strReplace(unixpath, '/', '\\'),
        exe(unixpath):: self.path(unixpath) + '.exe',
        # https://docs.microsoft.com/en-us/windows-server/administration/windows-commands/xcopy
        copydir(src, dst):: ['xcopy', self.path(src), self.path(dst), '/e', '/i', '/q'],

        downloads+: {
            CYGWIN: { name: 'cygwin', version: '3.0.7', platformspecific: true },
        },
        packages+: {
            # devkit_platform_revisions in make/conf/jib-profiles.js
            'devkit:VS2022-17.13.2+1': '==0',
        },
        capabilities+: ['windows'],
        name+: '-windows-cygwin',
        os:: 'windows',
        environment+: {
            JIB_PATH: '$CYGWIN\\bin;$PATH',
            ZLIB_BUNDLING: 'bundled',
            # Make `msbuild` and common *nix utilities in MKS_HOME available
            PATH: 'C:\\Windows\\Microsoft.NET\\Framework64\\v4.0.30319;$MKS_HOME;$PATH',
            # Tell `msbuild` to use the environment as is (i.e., the devkit)
            UseEnv: 'true',
        },
    },

    WindowsCygwin:: self.OSBase {
        downloads+: {
            CYGWIN: { name: 'cygwin', version: '3.0.7', platformspecific: true },
        },
        capabilities+: ['windows'],
        name+: '-windows-cygwin',
        os:: 'windows',
        environment+: {
            PATH: '$CYGWIN\\bin;$PATH',
        },
    },
    Linux:: self.OSBase {
        capabilities+: ['linux', '!e3'],
        name+: '-linux',
        os:: 'linux',
        environment+: {
            MAKE: 'make',
        },
    },

    LinuxDockerAMD64Musl(defs):: self.Linux + self.OCI + self.AMD64Musl {
        docker: {
            image: defs.linux_docker_image_amd64_musl,
            alpineDns: true,
        },
        # No packages are need for building the musl static libs
        packages: {},
    },

    DefaultBootJDK:: {
        downloads+: {
            BOOT_JDK: {
                name: 'jpg-jdk',
                version: '24',
                build_id: '36',
                release: true,
                platformspecific: true,
            },
        },
    },

    MuslBootJDK:: {
        downloads+: {
            BOOT_JDK: {
                name: 'labsjdk',
                version: 'ee-24+36-jvmci-b01-linux-amd64_musl',
                platformspecific: false,
            },
        },
        environment+: {
            LD_LIBRARY_PATH: '$BOOT_JDK/lib/server',
        },
    },

    JTReg:: {
        downloads+: {
            JT_HOME: {
                name: 'jtreg',
                version: '4.2',
            },
        },
    },

    # devkit_platform_revisions in make/conf/jib-profiles.js
    get_devkit_platform_revisions(major_java_version, platform):: {
        local devkit_platform_revisions = {
            '25': {
                linux_x64: 'gcc14.2.0-OL6.4+1.0',
                macosx_x64: 'Xcode14.3.1+1.0',
                windows_x64: 'VS2022-17.13.2+1.0',
                linux_aarch64: 'gcc14.2.0-OL7.6+1.0',
                linux_arm: 'gcc8.2.0-Fedora27+1.0',
                linux_ppc64le: 'gcc8.2.0-Fedora27+1.0',
                linux_s390x: 'gcc8.2.0-Fedora27+1.0',
            },
        },

        local value = devkit_platform_revisions[std.toString(major_java_version)][platform],
        local name = std.substr(value, 0, std.length(value) - 2),
        local version = std.substr(value, std.length(value) - 1, 1),
        packages: {
            ['devkit:' + name]: '==' + version,
        },
    },

    LinuxAMD64Graal(defs, for_jdk_build):: self.Linux + self.AMD64 {
        docker: {
            image: defs.linux_docker_image_amd64,
            mount_modules: true,  # needed for installing the devtoolset package below
        },
        packages+: if for_jdk_build then {
            # devkit_platform_revisions in make/conf/jib-profiles.js
            'devkit:gcc14.2.0-OL6.4+1': '==0',
        } else {
            # When building/testing GraalVM, do not use a devkit as it is known not to
            # work well when dynamically linking libstdc++.
            devtoolset: '==7',
        },
    },

    LinuxAMD64(defs, major_java_version):: self.Linux + self.get_devkit_platform_revisions(major_java_version, 'linux_x64') + self.OCI {
      docker: {
            image: defs.linux_docker_image_amd64,
      },
    },

    LinuxAArch64(defs, major_java_version):: self.Linux + self.get_devkit_platform_revisions(major_java_version, 'linux_aarch64') + self.OCI + self.AArch64 {
      docker: {
            image: defs.linux_docker_image_amd64,
            mount_modules: true,
      },
    },

    LinuxAArch64Graal(defs, for_jdk_build):: self.Linux + self.AArch64 {
        packages+: if for_jdk_build then {
            # devkit_platform_revisions in make/conf/jib-profiles.js
            'devkit:gcc14.2.0-OL7.6+1': '==0',
        } else {
            # See GR-26071 as well as comment in self.LinuxAMD64
            devtoolset: '==7',
        },
        docker: {
            image: defs.linux_docker_image_amd64,
            mount_modules: true,
        },
    },

    Darwin:: self.OSBase {
        jdk_home(java_home):: java_home + '/../..',
        java_home(jdk_home):: jdk_home + '/Contents/Home',
        packages+: {
            # No need to specify a "make" package as Mac OS X has make 3.81
            # available once Xcode has been installed.
        },
        os:: 'darwin',
        name+: '-darwin',
    },


    DarwinAArch64:: self.Darwin + self.AArch64 + {
        capabilities+: ['darwin_ventura'],
    },

    AMD64:: {
        capabilities+: ['amd64'],
        name+: '-amd64',
        arch:: 'amd64',
    },

    AMD64Musl:: self.AMD64 {
        name+: '-musl',
        arch+: '_musl',
    },

    AArch64:: {
        capabilities+: ['aarch64'],
        name+: '-aarch64',
        arch:: 'aarch64',
    },
}
