/*
 * Copyright (c) 2015, 2025, Oracle and/or its affiliates. All rights reserved.
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
 */

/**
 * @test
 * @summary Tests that the initializer for CompilerToVM::methods is sorted (case-insensitive).
 * @run main compiler.jvmci.compilerToVM.CheckCompilerToVMMethodTableIsSorted
 */

package compiler.jvmci.compilerToVM;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;

public class CheckCompilerToVMMethodTableIsSorted {

    public static final String METHOD_TABLE_DECL_OPEN = "JNINativeMethod CompilerToVM::methods[] = {";
    public static final String METHOD_TABLE_DECL_CLOSE = "};";

    /**
     * Gets the absolute path to {@code <jdk>/src/hotspot} by searching up the file system starting
     * at {@code dir}.
     */
    private static Path getHotSpotSrcDir(Path dir) {
        while (dir != null) {
            Path path = dir.resolve("src").resolve("hotspot");
            if (Files.exists(path)) {
                return path;
            }
            dir = dir.getParent();
        }
        throw new RuntimeException("Could not locate the src/hotspot directory by searching up from " + dir);
    }

    public static void main(String[] args) throws IOException {
        Path testSrcDir = null;
        Path jvmciCompilerToVM;
        boolean check;
        if (args.length == 0) {
            check = true;
            testSrcDir = Paths.get(System.getProperty("test.src"));
            Path root = getHotSpotSrcDir(testSrcDir);
            jvmciCompilerToVM = root.resolve("share/jvmci/jvmciCompilerToVM.cpp");
        } else {
            check = false;
            jvmciCompilerToVM = Path.of(args[0]);
        }
        List<String> lines = Files.readAllLines(jvmciCompilerToVM);
        int open = lines.indexOf(METHOD_TABLE_DECL_OPEN);
        if (open == -1) {
            throw new RuntimeException("Could not find \"" + METHOD_TABLE_DECL_OPEN + "\" in " + jvmciCompilerToVM);
        }
        int close = lines.subList(open, lines.size()).indexOf(METHOD_TABLE_DECL_CLOSE);
        if (close == -1) {
            throw new RuntimeException("Could not find \"" + METHOD_TABLE_DECL_CLOSE + "\" in " + jvmciCompilerToVM + " after line " + open);
        }
        close = open + close;
        List<String> table = lines.subList(open + 1, close);
        List<String> sortedTable = table.stream().sorted(Comparator.comparing(String::toLowerCase)).toList();
        String unsortedMsg = null;
        for (int i = 0; i != table.size(); i++) {
            String expect = sortedTable.get(i);
            String actual = table.get(i);
            if (!expect.equals(actual)) {
                unsortedMsg = String.format("CompilerToVM::methods table is out of (case-insensitive) order%n%s:%d:%n  expect: %s%n  actual: %s",
                        jvmciCompilerToVM, i + open + 2, expect, actual);
                if (check) {
                    String errorMsg = String.format("""
                            %s

                            This should be fixable by running:

                                java %s.java %s
                            """,
                            unsortedMsg,
                            testSrcDir.resolve(CheckCompilerToVMMethodTableIsSorted.class.getSimpleName()),
                            jvmciCompilerToVM);
                    throw new RuntimeException(errorMsg);
                }
                break;
            }
        }
        if (unsortedMsg != null) {
            // Update file
            System.out.println(unsortedMsg);
            List<String> newLines = new ArrayList<>(lines.subList(0, open + 1));
            newLines.addAll(sortedTable);
            newLines.addAll(lines.subList(close, lines.size()));
            Files.writeString(jvmciCompilerToVM, String.join("\n", newLines) + "\n");
            System.out.printf("CompilerToVM::methods table in %s is now sorted%n", jvmciCompilerToVM);
        } else {
            System.out.printf("CompilerToVM::methods table in %s is already sorted%n", jvmciCompilerToVM);
        }
    }
}
