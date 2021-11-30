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

import com.google.gson.Gson;

import java.lang.reflect.Type;

/**
 * Json JsonUtils/Deserializer.
 */

public class JsonUtils {
    private static final Gson GSON = new Gson();

    JsonUtils() {
    }

    /**
     * Serialize an object to Json.
     *
     * @param object to toJson.
     */
    public static String toJson(Object object) {
        return GSON.toJson(object);
    }

    public static String toJson(Object object, Type type) {
        return GSON.toJson(object, type);
    }

    /**
     * Deserialize a json representation of an object.
     *
     * @param string A json string to fromJson.
     */
    public static <T> T fromJson(String string, Class<T> clazz) {
        return GSON.fromJson(string, clazz);
    }

    public static <T> T fromJson(String string, Type type) {
        return GSON.fromJson(string, type);
    }
}
