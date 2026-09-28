# Substrate VM G1 Garbage Collector Sources

This branch is based on the LabsJDK CE tag `jvmci-25.2-b20` and contains the
Substrate VM integration of the G1 garbage collector.

## Maintenance Model

Most work in this repository is updating the reduced OpenJDK code base to a newer JDK version.
These updates are expensive because the downstream G1 sources must stay aligned with a large amount of upstream OpenJDK code.
For that reason, keep the diff against OpenJDK as small as possible.

## Limitations

- The supported platforms are Linux/AMD64, Linux/AArch64, Darwin/AArch64, and Windows/AMD64.
- Exactly one isolate is supported.
  It is not possible to spawn multiple isolates.
- Images with disabled multithreading (`-H:-MultiThreaded`) are not supported.
- Images without a dedicated reference handler thread (`-H:-UseReferenceHandlerThread`) are not supported.
- Images without a dedicated VM operation thread (`-H:-UseDedicatedVMOperationThread`) are not supported.

## Building

Use `mx build` to build this repository.
This creates include files and static libraries that Native Image uses when building an image with G1 GC (`-H:+UseG1GC`).
The static library used by native-image depends on whether compressed references are enabled:

- `libg1gc-cr.a` or `g1gc-cr.lib`: used if references are compressed
- `libg1gc-ur.a` or `g1gc-ur.lib`: used if references are uncompressed

## Repository Structure

The repository structure mostly matches OpenJDK.
The main differences are:

- Files that are fully Substrate VM-specific are in `src/hotspot/svm`.
- SVM-specific changes in shared files are guarded with `#ifdef SVM` or `#ifndef SVM`.
- An mx/Ninja native build replaces the OpenJDK build system.

## Development and Debugging

### Building a Debug Version

By default, `mx build` builds only the optimized static libraries.
These libraries do not contain debug information.
For G1 development or debugging, build G1 as a shared library with debug information.
The native image can then link that shared library dynamically, so C++ changes only require rebuilding G1, not the native image.

The G1 GC can be built with one of the following debug levels:

- `debug`: easy to debug, but not optimized and slow. Runs assertions and self-verification code.
- `fastdebug`: partly optimized and much faster than `debug`. Also runs assertions and self-verification code.
- `product`: fully optimized, without debug information. This is the version shipped with GraalVM.

To build a custom G1 variant, set the environment variable `SVM_GC_TARGETS`.
Generated Ninja targets use this pattern: `build_[debug|fastdebug|product]_[ur|cr]_[a|so]`, e.g.:

```shell
SVM_GC_TARGETS='build_debug_cr_so build_debug_ur_so' mx build
```

Custom builds archive only the selected static libraries. Shared libraries remain local build
outputs and are not added to the mx distributions.

The shortcut `build_all` builds both shared and static libraries for all debug levels.

After building a debug version of G1, point `-H:CLibraryPath=...` to the directory
that contains the G1 library and set
`-H:G1DebugLevel=[debug|fastdebug|product]` to match that library.

## Debugging

Debug builds already enable many assertions and self-verification checks.
The following runtime options are also useful:

- `-XX:+PrintGC`: prints some information about garbage collections
- `-XX:+VerboseGC`: prints detailed information about garbage collections
- `-H:+VerifyHeap`: verifies the heap before and after a garbage collection.
  This can be very helpful, but is also rather slow.

Helpful debugging hints:

- `gc_create(...)` in `svmToGC.cpp` is the main entry point.
  It runs when the native image initializes the garbage collector during startup.
- A breakpoint in `VMError::report_and_die(...)` is often useful.
  Most fatal errors and assertion failures on the C++ side end there.

## Major Differences to the OpenJDK Sources

Our G1 version is built from a subset of the OpenJDK sources (230k committed lines, of which 172k are actually reachable).
This includes the G1 sources, common GC infrastructure, and other OpenJDK code that G1 needs, such as OS support, threading support, and common data structures.

The list below shows the main differences from OpenJDK.
In general, we keep the diff against OpenJDK small because large diffs make JDK updates harder.

- During startup and shutdown, our G1 version runs only a small part of the code that OpenJDK usually runs.
- The address space for the image heap and the Java heap is reserved on the Native Image side.
- In our G1 version, we use additional region types to model the image heap: "closed image heap", "open image heap", and matching humongous region types for large objects.
  - Image heap regions are always alive.
  - Image heap regions are never marked.
  - Image heap regions are never compacted.
  - Image heap regions are never scrubbed.
  - Image heap regions have an empty remembered set and state "untracked" (because image heap objects are always alive).
  - Image heap regions are not part of any region set.
  - Image heap regions are always fully parsable, i.e., `pb` (parsable-bottom) is always the same as `hr->bottom()`.
  - Closed image heap regions are neither visited nor modified by the GC.
  - Some image heap regions don't need write-barriers, so they don't have a block offset table.
    - Closed image heap regions.
    - Humongous open image heap regions that contain primitive arrays.
      This assumes that primitive arrays do not contain references.
  - Open image heap regions are scanned for references (similar to the old generation).
    So, we need to set `TARS` (top-at-rebuild-start) accordingly.
- Our G1 version supports pinning any Java object.
  OpenJDK only supports pinning primitive arrays.
  Regions with pinned objects are never compacted.
  Pinned objects are not evacuated and are handled like evacuation failures.
  Primitive arrays in pinned regions are never overwritten by G1, even if they are unreachable.
  For Java code, it is therefore not necessary to keep a strong reference to primitive arrays that are pinned.
  This also means that primitive arrays must never have references to other objects, not even to a Java monitor.
  - G1 uses a thread-local cache for object pinning, so note that the number of pinned objects is only consistent at a safepoint.
- All `oop` and `Klass` related C++ classes are implemented differently because they must access data in a way that works for Native Image.
  There are also a few conceptual differences:
  - In Native Image, we always use a heap base.
  - If compressed references are used, then the mark word always has 32-bit.
  - Header-size computation is different because Native Image can configure the header size.
- Argument parsing and options (i.e., globals) are handled completely differently in our G1 version as it is necessary to distinguish between hosted and runtime options.
  OpenJDK options that are not relevant for Native Image become constants when the C++ sources are compiled.
  Only options declared with `ni_hosted` or `ni_runtime` are materialized in the SVM option table and can be parsed.
- Stack handling is different because G1 must call into Native Image to get stack information.
- Thread handling is different because Native Image owns all Java threads.
  G1 may only spawn internal worker threads (i.e., non-Java threads).
- OpenJDK uses `dlsym()` to support libc versions where some Linux and POSIX functions are unavailable.
  This does not work when the G1 library is linked into a fully statically linked Native Image (`--static`) because `dlsym()` always returns null in that case.
  So, we modify the sources in a way so that relevant libc functions are referenced directly.
  The Native Image link step resolves these references from `libc.a` for a fully static image or from `libc.so` for dynamically linked musl.
- The handling and lifecycle of JIT compiled code (`nmethods`) is completely different.
- VM operations and safepoints are implemented differently.
  This is mainly needed because the VM operation thread is a normal Java thread in Native Image.
- Performance data support is different because Native Image manages all performance-data memory and data structures.
- Our G1 version does not have any metaspace or class (un)loading support.
- There are many locking differences, e.g.:
  - Only a subset of the OpenJDK locks is used.
  - Far more locks must be lockable by Java threads.
    One of the reasons for this is that the VM operation thread is a Java thread in Native Image.
- We removed support for throwing and handling Java exceptions.
  For example, if the Java heap runs out of memory, OpenJDK would normally throw an `OutOfMemoryError`.
  In our G1 version, Native Image throws the `OutOfMemoryError` instead.
- Native Image supports hybrid objects.
  These are variable-sized objects with both instance fields and primitive array elements.
  OpenJDK does not have this concept.
  Because of that, methods like `oop::is_array()`, `oop::is_objArray()`, `oop::is_typeArray()`, `Klass::is_array_klass()`, `Klass::is_objArray_klass()`, and `Klass::is_typeArray_klass()` can be dangerous because they return false for hybrid objects.
- We added SW-CFI support.
  As of now, only the assembly code needs adjustments:
  - Using the SW-CFI pattern in front of indirect calls/jumps (no occurrences as of now) and `jmp __x86_return_thunk` instead of `ret`.
  - Using `endbr64` in indirectly called functions (no occurrences as of now, look for function address taking in `sed 's/DECLARE_FUNC(\(.*\)):/-e\1[^(]/;t;d' src/hotspot/os_cpu/linux_x86/linux_x86_64.S | xargs git grep`).

## License

This code is licensed under the GNU General Public License version 2. Some
files are subject to the Classpath exception, as stated in their headers.
