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

import com.journeyOS.J007engine.J007EngineManager;

/**
 * @author solo
 * constant
 */
public class Constant {
    /**
     * "J007engine";
     */
    public static final String J007_ENGINE_SERVICE_NAME = J007EngineManager.J007_ENGINE_SERVICE_NAME;

    /**
     * "com.journeyOS.J007engine";
     */
    public static final String J007_ENGINE_PACKAGE = J007EngineManager.J007_ENGINE_PACKAGE;

    /**
     * "com.journeyOS.J007engine.action.J007EngineService";
     */
    public static final String J007_ENGINE_SERVICE_AIDL = J007EngineManager.J007_ENGINE_SERVICE_AIDL;


    //SharedPreferences KEY
    public static final String DB_INIT = "J007DbInit";
    public static final String DB_BL_INIT = "J007DbBLInit";
}
