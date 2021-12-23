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


package com.journeyOS.J007engine.database;

public class DBConfigs {
    //database name
    public static final String DB_NAME = "J007.db";
    //database version
    public static final int DB_VERSION = 1;

    public static class App {
        //table
        public static final String TABLE_NAME = "app";
        //column
        public static final String PACKAGE_NAME = "packageName";
        //column
        public static final String TYPE = "type";
        //column
        public static final String MODE = "mode";
        //column
        public static final String FPS = "fps";
        //column
        public static final String CPU = "cpu";
        //column
        public static final String MEMC = "memc";
        //column
        public static final String BL = "bl";
    }

    public static class Settings {
        public static final String APP_INIT = "app_init";
        public static final boolean APP_INIT_DEFAULT = false;
    }

}
