/*
 * Copyright (c) 2025, Oracle and/or its affiliates. All rights reserved.
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

import java.util.function.Function;

/**
 * A partial implementation of the {@link Annotated} interface, providing
 * the caching mechanism for parsed declared annotation information.
 * <p>
 * This class maintains a single-entry cache for the result of parsing
 * the declared annotation information, allowing for efficient reuse of
 * the parsed data when the same parser function is applied multiple times.
 * <p>
 * Subclasses must provide an implementation for the {@link #getRawDeclaredAnnotationInfo}
 * method, which returns the raw annotation information.
 */
public abstract class AbstractAnnotated implements Annotated {

    /**
     * Record used for entry in {@link #parsedDeclaredAnnotationsCache}.
     */
    private record ParsedDeclaredAnnotationsCacheEntry<R>(Function<AnnotationsInfo, R> parser, R result) {
    }

    /**
     * Single-entry cache for the result of parsing {@link #getDeclaredAnnotationInfo}.
     */
    private volatile ParsedDeclaredAnnotationsCacheEntry<?> parsedDeclaredAnnotationsCache;

    @SuppressWarnings("unchecked")
    @Override
    public <T> T getDeclaredAnnotationInfo(Function<AnnotationsInfo, T> parser) {
        if (parser == null) {
            return (T) getRawDeclaredAnnotationInfo();
        }
        ParsedDeclaredAnnotationsCacheEntry<?> cache = parsedDeclaredAnnotationsCache;
        if (cache == null || !parser.equals(cache.parser)) {
            AnnotationsInfo info = getRawDeclaredAnnotationInfo();
            cache = new ParsedDeclaredAnnotationsCacheEntry<>(parser, parser.apply(info));
            parsedDeclaredAnnotationsCache = cache;
        }
        return (T) cache.result;
    }

    /**
     * Gets the unparsed class file info for the annotations directly present on this element.
     */
    public abstract AnnotationsInfo getRawDeclaredAnnotationInfo();
}
