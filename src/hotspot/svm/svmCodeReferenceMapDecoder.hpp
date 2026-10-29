/*
 * Copyright (c) 2020, 2026, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Oracle designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Oracle in the LICENSE file that accompanied this code.
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

#ifndef SVM_CODE_REFERENCE_MAP_DECODER_HPP
#define SVM_CODE_REFERENCE_MAP_DECODER_HPP

#include "memory/iterator.hpp"
#include "utilities/globalDefinitions.hpp"


namespace svm_gc {

class SVMCodeReferenceMapDecoder : public AllStatic {
 public:
  static void walk_offsets_from_pointer(u_char *base_address, u_char *encoded_reference_map, jlong reference_map_index, OopClosure *f);
};


} // namespace svm_gc

#endif // SVM_CODE_REFERENCE_MAP_DECODER_HPP
