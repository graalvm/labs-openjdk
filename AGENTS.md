# Agent Instructions

This LabsJDK CE branch builds HotSpot's G1 GC as a standalone Native Image library.
The code is based on a reduced JDK source tree. Its exact base revision is recorded in
[BASE_REVISION](BASE_REVISION). Keep diffs against that revision as small as possible.
Read [README.md](README.md) for the repository layout, build commands, and limitations.

## Always-On Rules

- Keep JDK include blocks unchanged and in the same order.
- Preserve existing blank lines at the end of C and C++ source and header files.
- Preserve original public copyright headers, including third-party notices.
- Open the `svm_gc` namespace after includes or include guards, never around includes.
- Avoid local lambdas for simple one-off predicates.
  Prefer existing methods, ordinary control flow, or a named helper.
- Prefer narrow `#ifdef SVM` or `#ifndef SVM` guards over moved or rewritten JDK code
  or duplicated methods. Guard only the unavailable pieces when SVM follows the JDK flow.
- Prefer `#ifdef` or `#ifndef` guards when ignoring whole lines, and use `SVM_ONLY(...)` or `NOT_SVM(...)` for parts of a line.
- Exclude unused HotSpot-only code rather than adding placeholder APIs to compile it.
- Files under `src/hotspot/mocks` are generated, so do not edit them manually.
  Regenerate them when coverage changes. When importing JDK functionality, copy sources from `BASE_REVISION`
  and delete the corresponding mocks. Keep the import and SVM adaptations in separate commits.
- HotSpot globals are not automatically runtime options in this repository.
  Only globals declared as `ni_hosted` or `ni_runtime` are materialized in the SVM option table and parsed.
  Other globals, such as `UseNUMA`, fold to compile-time constants in the C++ code.

## Updating to a New JDK Version

- Fetch and merge the latest `origin/jdk25` into the existing issue branch.
  Do not rebase the branch or recreate its history.
- Preserve the reduced source layout, Native Image adaptations, public headers, and build configuration.
- Update `BASE_REVISION` to the full hash of the merged JDK revision.

## Validation

- Run the relevant `mx build` target. Use `SVM_GC_TARGETS=build_all mx build` for all library variants on the current platform.
- After JDK updates, build and run a small Native Image program with G1 to check startup.
