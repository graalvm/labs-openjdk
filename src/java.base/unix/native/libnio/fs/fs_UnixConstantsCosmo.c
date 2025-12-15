/*
 * Copyright (c) Elide Technologies.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Elide designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Oracle in the LICENSE file for building OpenJDK.
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
 */
#if defined(__COSMOPOLITAN__)
#include "jni.h"
#include "jni_util.h"
#include "jlong.h"

#include "nio.h"

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include "sun_nio_fs_UnixFileSystem.h"

static void throwUnixException(JNIEnv* env, int errnum) {
    jobject x = JNU_NewObjectByName(env, "sun/nio/fs/UnixException",
        "(I)V", errnum);
    if (x != NULL) {
        (*env)->Throw(env, x);
    }
}

struct MagicNumberWithString {
    char *str;
    int value;
};

#define COSMO_MAGIC_KVPAIR(x) {.str = #x , .value = x}
static struct MagicNumberWithString magicMap[] = {
    COSMO_MAGIC_KVPAIR(AT_FDCWD),
    COSMO_MAGIC_KVPAIR(AT_SYMLINK_NOFOLLOW),
    COSMO_MAGIC_KVPAIR(AT_REMOVEDIR)
};
#undef COSMO_MAGIC_KVPAIR
static int magicMapSize = 3;

JNIEXPORT jint JNICALL
Java_sun_nio_fs_UnixConstants_cosmoMagic0
    (JNIEnv* env, jclass this, jstring name)
{
    const char *cname = JNU_GetStringPlatformChars(env, name, NULL);
    jint res = 0;
    if (!cname) {
        return res;
    }

    for (int i = 0; i<magicMapSize; ++i) {
        if(0 == strcmp(magicMap[i].str, cname)) {
            res = magicMap[i].value;
        }
    }
    JNU_ReleaseStringPlatformChars(env, name, cname);
    return res;
}
#endif
