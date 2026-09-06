/*
 * Copyright (c) 2026, Oracle and/or its affiliates. All rights reserved.
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

/*
 * @test
 * @summary Restore Class identity hashes for proxies, arrays and named loaders
 * @requires vm.flagless
 * @requires vm.jvmci
 * @library /test/lib
 * @run driver TestClassIdentityHashCodes
 */

import java.io.DataOutputStream;
import java.io.InputStream;
import java.lang.reflect.Array;
import java.lang.reflect.Proxy;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

import jdk.test.lib.process.ProcessTools;

public class TestClassIdentityHashCodes {
    public interface Marker {}

    private static class Loader extends ClassLoader {
        Loader(String name) {
            super(name, TestClassIdentityHashCodes.class.getClassLoader());
        }

        Class<?> defineMarker() throws Exception {
            String name = Marker.class.getName();
            try (InputStream in = TestClassIdentityHashCodes.class.getResourceAsStream("/" + name.replace('.', '/') + ".class")) {
                byte[] bytes = in.readAllBytes();
                return defineClass(name, bytes, 0, bytes.length);
            }
        }
    }

    @SuppressWarnings("deprecation")
    private static List<Class<?>> classes(boolean perturb) throws Exception {
        Loader first = new Loader("first");
        Loader second = new Loader("second");
        Loader spaced = new Loader(" ");
        Loader unnamed = new Loader(null);
        Class<?> firstMarker = first.defineMarker();
        Class<?> secondMarker = second.defineMarker();
        Class<?> spacedMarker = spaced.defineMarker();
        Class<?> unnamedMarker = unnamed.defineMarker();
        if (perturb) {
            // Change both proxy numbering and the order of dynamic module creation.
            Proxy.getProxyClass(second, Comparable.class);
            Proxy.getProxyClass(null, Cloneable.class);
        }
        List<Class<?>> result = new ArrayList<>(List.of(Object.class, String.class, int.class, void.class, String[].class, int[][].class,
                        firstMarker, secondMarker, spacedMarker, unnamedMarker));
        List<Class<?>> proxies = List.of(
                        Proxy.getProxyClass(null),
                        Proxy.getProxyClass(first),
                        Proxy.getProxyClass(first, firstMarker),
                        Proxy.getProxyClass(second, secondMarker),
                        Proxy.getProxyClass(spaced, spacedMarker),
                        Proxy.getProxyClass(unnamed, unnamedMarker),
                        Proxy.getProxyClass(first, Runnable.class),
                        Proxy.getProxyClass(second, Runnable.class));
        for (Class<?> proxy : proxies) {
            result.add(proxy);
            result.add(Array.newInstance(proxy, 0).getClass());
            result.add(Array.newInstance(proxy, 0, 0).getClass());
        }
        return result;
    }

    private static String loaderName(Class<?> clazz) {
        ClassLoader loader = clazz.getClassLoader();
        if (loader == null) {
            return "bootstrap";
        }
        String name = loader.getName();
        return name == null || name.isEmpty() ? loader.getClass().getName() : name;
    }

    private static String classKey(Class<?> clazz) {
        Class<?> component = clazz;
        int rank = 0;
        while (component.isArray()) {
            component = component.getComponentType();
            rank++;
        }
        if (!Proxy.isProxyClass(component)) {
            return clazz.getName().replace('.', '/');
        }
        String key = "@interfaces:" + Arrays.stream(component.getInterfaces()).map(c -> c.getName().replace('.', '/')).collect(Collectors.joining(";"));
        return rank == 0 ? key : key + "@array:" + rank;
    }

    public static void main(String[] args) throws Exception {
        if (args.length == 0) {
            Path file = Path.of("class-identity-hash-codes.bin").toAbsolutePath();
            run("write", file);
            run("read", file);
            return;
        }
        boolean write = args[0].equals("write");
        List<Class<?>> classes = classes(!write);
        if (write) {
            try (var out = new DataOutputStream(Files.newOutputStream(Path.of(args[1])))) {
                out.writeInt(0x4A434948);
                out.writeInt(classes.size());
                for (int i = 0; i < classes.size(); i++) {
                    Class<?> clazz = classes.get(i);
                    out.writeInt(10001 + i);
                    out.writeUTF(loaderName(clazz));
                    out.writeUTF(classKey(clazz));
                }
            }
        } else {
            for (int i = 0; i < classes.size(); i++) {
                Class<?> clazz = classes.get(i);
                int actual = System.identityHashCode(clazz);
                int expected = 10001 + i;
                if (actual != expected) {
                    throw new AssertionError(clazz + " in loader " + loaderName(clazz) + ": expected " + expected + ", got " + actual);
                }
            }
        }
    }

    private static void run(String mode, Path file) throws Exception {
        List<String> command = new ArrayList<>(List.of("-Xshare:off", "-XX:+UnlockExperimentalVMOptions", "-XX:+EnableJVMCI",
                        "-XX:-UseJVMCICompiler", "-XX:+JVMCIUseStableGeneratedClassIdentityHashCodes"));
        if (mode.equals("read")) {
            command.add("-XX:JVMCIClassIdentityHashCodeFile=" + file);
        }
        command.addAll(List.of("-cp", System.getProperty("java.class.path"), TestClassIdentityHashCodes.class.getName(), mode, file.toString()));
        ProcessTools.executeProcess(ProcessTools.createLimitedTestJavaProcessBuilder(command)).shouldHaveExitValue(0);
    }
}
