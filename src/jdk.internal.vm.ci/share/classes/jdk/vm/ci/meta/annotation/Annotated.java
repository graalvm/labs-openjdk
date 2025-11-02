/*
 * Copyright (c) 2023, 2025, Oracle and/or its affiliates. All rights reserved.
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
package jdk.vm.ci.meta.annotation;

import java.lang.reflect.AnnotatedElement;
import java.util.function.Function;

/**
 * Represents a program element such as a method, constructor, field or class for which annotations
 * may be directly present. This API is analogous to {@link java.lang.reflect.AnnotatedElement}
 * except that it only supports {@linkplain AnnotatedElement#getDeclaredAnnotations() declared annotations}.
 */
public interface Annotated {

    /**
     * Gets the class file info for the annotations directly present on this element
     * (which will be {@code null} if no such info exists), applies the provided
     * parser function to it and returns the result.
     * <p>
     * The result of the last call to this method is cached if {@code parser != null}.
     * That is, if an {@linkplain Object#equals(Object) equivalent} parser object is
     * used again without any intervening calls with a different parser, the same
     * result object will be returned.
     * <p>
     * Note: Since lambda objects implement {@link Object#equals(Object)} with identity,
     * if {@code parser} is a lambda object, it should be a singleton in a final
     * static field. There is no guarantee about the identity of the result of a
     * lambda expression (JLS {@jls 15.27.4}).
     *
     * @param <T>    the type of the result produced by the parser function
     * @param parser a function that takes the declared annotation information and
     *               produces a result of type T. If {@code null}, then the
     *               AnnotationsInfo value that would have been parsed is returned and
     *               the cache is not updated.
     * @return the result of applying the parser function to the declared annotation
     * information, potentially retrieved from cache if the identical parser object
     * was previously used
     */
    <T> T getDeclaredAnnotationInfo(Function<AnnotationsInfo, T> parser);

    /**
     * Gets the class file info for the type annotations associated with this element
     * or {@code null} if no such info exists.
     */
    AnnotationsInfo getTypeAnnotationInfo();
}
