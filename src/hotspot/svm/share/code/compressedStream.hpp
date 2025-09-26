/*
 * Copyright (c) 1997, 2019, Oracle and/or its affiliates. All rights reserved.
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
 *
 */

#ifndef SHARE_CODE_COMPRESSEDSTREAM_HPP
#define SHARE_CODE_COMPRESSEDSTREAM_HPP

#include "memory/allocation.hpp"

// NOTE (chaeubl): a modified version of Pack200 that supports jlong values and encodes them in up to 11 bytes.

namespace svm_gc {

class CompressedStream : public ResourceObj {
  friend class VMStructs;
 protected:
  u_char* _buffer;
  jlong _position;

  enum {
    // Constants for UNSIGNED5 coding of Pack200
    lg_H = 6, H = 1<<lg_H,    // number of high codes (64)
    L = (1<<BitsPerByte)-H,   // number of low codes (192)
    MAX_i = 11
  };

  // these inlines are defined only in compressedStream.cpp
  static inline jlong decode_sign(jlong value);  // for Pack200 SIGNED5

 public:
  CompressedStream(u_char* buffer, jlong position = 0) {
    _buffer   = buffer;
    _position = position;
  }

  u_char* buffer() const               { return _buffer; }

  // Positioning
  jlong position() const             { return _position; }
  void set_position(jlong position)  { _position = position; }
};


class CompressedReadStream : public CompressedStream {
 private:
  inline u_char read()                 { return _buffer[_position++]; }

  // This encoding, called UNSIGNED5, is taken from J2SE Pack200.
  // It assumes that most values have lots of leading zeroes.
  // Very small values, in the range [0..191], code in one byte.
  // Any 32-bit value (including negatives) can be coded, in
  // up to five bytes.  The grammar is:
  //    low_byte  = [0..191]
  //    high_byte = [192..255]
  //    any_byte  = low_byte | high_byte
  //    coding = low_byte
  //           | high_byte low_byte
  //           | high_byte high_byte low_byte
  //           | high_byte high_byte high_byte low_byte
  //           | high_byte high_byte high_byte high_byte any_byte
  // Each high_byte contributes six bits of payload.
  // The encoding is one-to-one (except for integer overflow)
  // and easy to parse and unparse.

  jlong read_int_mb(jlong b0) {
    jlong pos = position() - 1;
    u_char* buf = buffer() + pos;
    assert(buf[0] == b0 && b0 >= L, "correctly called");
    jlong    sum = b0;
    // must collect more bytes:  b[1]...b[4]
    int lg_H_i = lg_H;
    for (int i = 1; ; ) {
      jlong b_i = buf[i++];
      sum += b_i << lg_H_i;
      if (b_i < L || i == MAX_i) {
        set_position(pos+i);
        return sum;
      }
      lg_H_i += lg_H;
    }
    ShouldNotReachHere();
    return 0;
  }

 public:
  CompressedReadStream(u_char* buffer, jlong position = 0)
  : CompressedStream(buffer, position) {}

  jboolean read_bool()                 { return (jboolean) read();      }
  jbyte    read_byte()                 { return (jbyte   ) read();      }
  jchar    read_char()                 { return (jchar   ) read_int();  }
  jshort   read_short()                { return (jshort  ) read_signed_int(); }
  jlong    read_int()                  { jlong   b0 = read();
                                         if (b0 < L)  return b0;
                                         else         return read_int_mb(b0);
                                       }
  jlong    read_signed_int();
};


} // namespace svm_gc

#endif // SHARE_CODE_COMPRESSEDSTREAM_HPP
