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

#ifndef _JNIHELPER_H
#define _JNIHELPER_H

static const char *native_library_class_path_name = "com/journeyOS/J007engine/jni/JniJ007Engine";

jstring jni_native_library_read(JNIEnv *env, jclass thiz, jstring jFileName, jint size);

jint jni_native_library_writer(JNIEnv *env, jclass thiz, jstring jFileName, jstring jContent, jint size);

jstring jni_native_library_get_fps(JNIEnv *env, jclass thiz);

jstring jni_native_library_get_package_name(JNIEnv *env, jclass thiz, jint pid);

jint jni_native_library_get_brightness(JNIEnv *env, jclass thiz);

static JNINativeMethod native_library_methods[] = {
        {"nativeFileRead",       "(Ljava/lang/String;I)Ljava/lang/String;",  (void *) jni_native_library_read},
        {"nativeFileWrite",      "(Ljava/lang/String;Ljava/lang/String;I)I", (void *) jni_native_library_writer},
        {"nativeGetFps",         "()Ljava/lang/String;",                     (void *) jni_native_library_get_fps},
        {"nativeGetPackageName", "(I)Ljava/lang/String;",                    (void *) jni_native_library_get_package_name},
        {"nativeGetBrightness",  "()I",                                      (void *) jni_native_library_get_brightness},
};

#endif //_JNIHELPER_H
