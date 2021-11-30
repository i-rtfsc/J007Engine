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

package com.journeyOS.J007engine.utils;

import android.os.Build;
import android.text.TextUtils;
import android.util.Log;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

/**
 * @author solo
 * smart log
 */
public class SmartLog {
    private static final String TAG = "J007Service";

    public static boolean isDebug() {
        return "eng".equals(Build.TYPE)
                || "userdebug".equals(Build.TYPE)
                || Log.isLoggable("J007Engine", android.util.Log.DEBUG);
    }

    private static String getMethodName() {
        final StackTraceElement stackTraceElement = Thread.currentThread().getStackTrace()[4];
        return stackTraceElement.getMethodName() + "()";
    }

    //e > w > i > d > v
    public static void e(String tag, String message) {
        Log.e(replaceTag(tag), getMethodName() + " " + message);
    }

    public static void w(String tag, String message) {
        Log.w(replaceTag(tag), getMethodName() + " " + message);
    }

    public static void i(String tag, String message) {
        Log.i(replaceTag(tag), getMethodName() + " " + message);
    }

    public static void d(String tag, String message) {
        if (isDebug()) {
            Log.d(replaceTag(tag), getMethodName() + " " + message);
        } else {
            Log.d(replaceTag(tag), message);
        }
    }

    public static void v(String tag, String message) {
        if (isDebug()) {
            Log.v(replaceTag(tag), getMethodName() + " " + message);
        } else {
            Log.v(replaceTag(tag), message);
        }
    }

    public static String replaceTag(String src) {
        if (TextUtils.isEmpty(TAG)) {
            return src;
        } else {
            return TAG + "-" + src;
        }
    }
}
