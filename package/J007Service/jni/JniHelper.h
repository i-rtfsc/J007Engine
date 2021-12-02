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

#ifndef _JNI_HELPER_H
#define _JNI_HELPER_H

static const char *native_library_class_path_name = "com/journeyOS/J007engine/jni/JniJ007Engine";

jstring jni_native_library_get_version_name(JNIEnv *env, jclass thiz);

jint jni_native_library_get_version_code(JNIEnv *env, jclass thiz);

static JNINativeMethod native_library_methods[] = {
        {"buildVersionName", "()Ljava/lang/String;", (void *) jni_native_library_get_version_name},
        {"buildVersionCode", "()I",                  (void *) jni_native_library_get_version_code},
};

#endif //_JNI_HELPER_H
