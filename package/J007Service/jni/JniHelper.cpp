/*
 * Copyright (c) 2021 anqi.huang@outlook.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*android specific headers*/
#include <jni.h>
#include <android/log.h>
#include <stdlib.h>
#include <assert.h>

#include "JniLog.h"
#include "JniHelper.h"

#ifndef NELEM
#define NELEM(x) ((int)(sizeof(x) / sizeof((x)[0])))
#endif

int registerNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *gMethods,
                          int numMethods) {
    jclass clazz = env->FindClass(className);
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {

        return JNI_FALSE;
    }

    return JNI_TRUE;
}

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    jint result = -1;
    JNIEnv *env = NULL;

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        LOGE("ERROR: GetEnv failed\n");
        goto bail;
    }

    assert(env != NULL);

    if (registerNativeMethods(env, native_library_class_path_name,native_library_methods,
                               NELEM(native_library_methods)) < 0) {
        LOGE("ERROR: Exif native registration failed\n");
        goto bail;
    }
    result = JNI_VERSION_1_6;

    bail:
    return result;
}