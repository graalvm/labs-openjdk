/*
 * Copyright (c) 2025, 2026, Oracle and/or its affiliates. All rights reserved.
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
 * @requires vm.jvmci
 * @library ../../../../../
 * @modules jdk.internal.vm.ci/jdk.vm.ci.meta
 *          java.base/java.lang:open
 *          java.base/java.lang.reflect:open
 *          jdk.internal.vm.ci/jdk.vm.ci.meta.annotation
 *          jdk.internal.vm.ci/jdk.vm.ci.hotspot
 *          jdk.internal.vm.ci/jdk.vm.ci.runtime
 *          jdk.internal.vm.ci/jdk.vm.ci.common
 *          java.base/jdk.internal.reflect
 *          java.base/jdk.internal.misc
 *          java.base/jdk.internal.vm
 *          java.base/sun.reflect.annotation
 * @run junit/othervm -XX:+UnlockExperimentalVMOptions -XX:+EnableJVMCI -XX:-UseJVMCICompiler jdk.vm.ci.runtime.test.TestResolvedJavaRecordComponent
 */

package jdk.vm.ci.runtime.test;

import jdk.vm.ci.meta.JavaType;
import jdk.vm.ci.meta.ResolvedJavaMethod;
import jdk.vm.ci.meta.ResolvedJavaRecordComponent;
import jdk.vm.ci.meta.ResolvedJavaType;
import org.junit.Test;

import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.RecordComponent;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.List;
import java.util.Map;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

/**
 * Tests for {@link ResolvedJavaRecordComponent}.
 */
public class TestResolvedJavaRecordComponent extends FieldUniverse {

    @Test
    public void equalsTest() {
        for (ResolvedJavaRecordComponent f : recordComponents.values()) {
            for (ResolvedJavaRecordComponent that : recordComponents.values()) {
                boolean expect = f == that;
                boolean actual = f.equals(that);
                assertEquals(expect, actual);
            }
        }
    }

    @Test
    public void getDeclaringRecordTest() {
        for (Map.Entry<RecordComponent, ResolvedJavaRecordComponent> e : recordComponents.entrySet()) {
            ResolvedJavaRecordComponent rc = e.getValue();
            ResolvedJavaType actual = rc.getDeclaringRecord();
            ResolvedJavaType expect = metaAccess.lookupJavaType(e.getKey().getDeclaringRecord());
            assertEquals(rc.toString(), expect, actual);
        }
    }

    @Test
    public void getNameTest() {
        for (Map.Entry<RecordComponent, ResolvedJavaRecordComponent> e : recordComponents.entrySet()) {
            ResolvedJavaRecordComponent rc = e.getValue();
            String actual = rc.getName();
            String expect = e.getKey().getName();
            assertEquals(rc.toString(), expect, actual);
        }
    }

    @Test
    public void getAccessorTest() {
        for (Map.Entry<RecordComponent, ResolvedJavaRecordComponent> e : recordComponents.entrySet()) {
            ResolvedJavaRecordComponent rc = e.getValue();
            Method expect = e.getKey().getAccessor();
            ResolvedJavaMethod actual = rc.getAccessor();
            assertEquals(rc.toString(), expect.getName(), actual.getName());
        }
    }

    @Test
    public void getTypeTest() {
        for (Map.Entry<RecordComponent, ResolvedJavaRecordComponent> e : recordComponents.entrySet()) {
            ResolvedJavaRecordComponent rc = e.getValue();
            JavaType actual = rc.getType();
            JavaType expect = metaAccess.lookupJavaType(e.getKey().getType()).resolve(rc.getDeclaringRecord());
            assertEquals(expect, actual);
        }
    }

    /**
     * Verifies that {@link ResolvedJavaRecordComponent#getType()} refreshes an unresolved type after
     * another operation loads it, without triggering class loading itself.
     */
    @Test
    public void getTypeRefreshesInitiallyUnresolvedTypeTest() throws ClassNotFoundException {
        IsolatedClassLoader loader = new IsolatedClassLoader();
        Class<?> recordClass = loader.loadClass(RECORD_CLASS_NAME);
        List<? extends ResolvedJavaRecordComponent> components = metaAccess.lookupJavaType(recordClass).getRecordComponents();
        assertEquals("record component count", 1, components.size());
        ResolvedJavaRecordComponent component = components.get(0);

        assertFalse("JVMCI metadata lookup loaded the component type", loader.isLoaded(COMPONENT_CLASS_NAME));
        assertFalse("component type was unexpectedly resolved", component.getType() instanceof ResolvedJavaType);
        assertFalse("component type was unexpectedly resolved on retry", component.getType() instanceof ResolvedJavaType);
        assertFalse("getType loaded the component type", loader.isLoaded(COMPONENT_CLASS_NAME));

        RecordComponent reflectionComponent = recordClass.getRecordComponents()[0];
        assertTrue("reflection did not load the component type", loader.isLoaded(COMPONENT_CLASS_NAME));

        JavaType resolvedType = component.getType();
        assertTrue("component type was not refreshed", resolvedType instanceof ResolvedJavaType);
        assertEquals("refreshed component type", metaAccess.lookupJavaType(reflectionComponent.getType()), resolvedType);
        assertEquals("reflection component lookup", component, metaAccess.lookupJavaRecordComponent(reflectionComponent));
    }

    @Test
    public void getTypeAnnotationInfoTest() {
        for (RecordComponent rc : recordComponents.keySet()) {
            ResolvedJavaRecordComponent jrc = metaAccess.lookupJavaRecordComponent(rc);
            byte[] rawAnnotations = getFieldValue(recordComponentTypeAnnotations, rc);
            TestResolvedJavaType.checkRawAnnotations(jrc, "getTypeAnnotationInfo", rawAnnotations, jrc.getTypeAnnotationInfo());
        }
    }

    @Test
    public void getDeclaredAnnotationInfoTest() {
        for (RecordComponent rc : recordComponents.keySet()) {
            ResolvedJavaRecordComponent jrc = metaAccess.lookupJavaRecordComponent(rc);
            byte[] rawAnnotations = getFieldValue(recordComponentAnnotations, rc);
            TestResolvedJavaType.checkRawAnnotations(jrc, "getDeclaredAnnotationInfo", rawAnnotations, jrc.getDeclaredAnnotationInfo(null));
        }
    }

    private static final String TEST_PACKAGE = TestResolvedJavaRecordComponent.class.getPackageName();
    private static final String RECORD_CLASS_NAME = TEST_PACKAGE + ".RecordWithInitiallyUnresolvedComponent";
    private static final String COMPONENT_CLASS_NAME = TEST_PACKAGE + ".InitiallyUnresolvedComponent";

    /** Loads test fixtures from the test output directory in an isolated namespace. */
    private static final class IsolatedClassLoader extends URLClassLoader {

        /** Creates a loader without a parent so the fixture bypasses the application class loader. */
        IsolatedClassLoader() {
            super(new URL[]{TestResolvedJavaRecordComponent.class.getProtectionDomain().getCodeSource().getLocation()}, null);
        }

        boolean isLoaded(String name) {
            return findLoadedClass(name) != null;
        }
    }

    private static final Field recordComponentAnnotations = lookupField(RecordComponent.class, "annotations");
    private static final Field recordComponentTypeAnnotations = lookupField(RecordComponent.class, "typeAnnotations");

}

/** Record fixture whose component type is loaded after JVMCI creates its metadata. */
record RecordWithInitiallyUnresolvedComponent(InitiallyUnresolvedComponent component) {
}

/** Component type loaded separately from its declaring record. */
final class InitiallyUnresolvedComponent {
}
