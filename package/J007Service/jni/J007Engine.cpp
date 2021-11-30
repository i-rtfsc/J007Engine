/*
 * Copyright (c) 2020 game dock developers
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

#include <jni.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "JniLog.h"
#include "FileHelper.h"
#include "Utils.h"

jstring jni_native_library_read(JNIEnv *env, jclass thiz, jstring jFileName, jint size) {
    const char *nFileName = env->GetStringUTFChars(jFileName, 0);
    char buf[size];

    FileHelper *fileHelper = new FileHelper(nFileName);
    int result = fileHelper->readFile(buf, size);
    if (DEBUG) {
        LOGI("read file %s\n", buf);
    }
    env->ReleaseStringUTFChars(jFileName, nFileName);
    if (result == -1) {
        return env->NewStringUTF("file not exists");
    } else {
        return env->NewStringUTF(buf);
    }
}

jint jni_native_library_writer(JNIEnv *env, jclass thiz, jstring jFileName, jstring jContent,
                               jint size) {
    const char *nFileName = env->GetStringUTFChars(jFileName, 0);
    const char *nContent = env->GetStringUTFChars(jContent, 0);

    FileHelper *fileHelper = new FileHelper(nFileName);
    int result = fileHelper->writeFile(nContent, size);

    env->ReleaseStringUTFChars(jFileName, nFileName);
    env->ReleaseStringUTFChars(jContent, nContent);
    return result;
}

jstring jni_native_library_get_fps(JNIEnv *env, jclass thiz) {
    char fps[5];
    if (!Utils::read_process_str("cat /sys/class/drm/sde-crtc-0/measured_fps | awk '{print $2}'",
                                 fps)) {
        if (DEBUG) {
            LOGI("get fps %s\n", fps);
        }
        return env->NewStringUTF(fps);
    }

    return env->NewStringUTF("");
}

jstring jni_native_library_get_package_name(JNIEnv *env, jclass thiz, jint pid) {
    char path[DEFAULT_PATH_SIZE];
    sprintf(path, "ps -A | grep %d | awk '{print $9}'", pid);
    if (DEBUG) {
        LOGI("path %s\n", path);
    }
    char package_name[DEFAULT_PATH_SIZE];
    if (!Utils::read_process_str(path, package_name)) {
        if (DEBUG) {
            LOGI("get package name %s\n", package_name);
        }
        return env->NewStringUTF(package_name);
    }

    return env->NewStringUTF("");
}

jint jni_native_library_get_brightness(JNIEnv *env, jclass thiz) {
    int brightness;
    Utils::read_file_int("/sys/class/backlight/panel0-backlight/brightness", &brightness);
    if (DEBUG) {
        LOGI("get brightness %d\n", brightness);
    }
    return brightness;
}