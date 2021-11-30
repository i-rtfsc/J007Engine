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

package com.journeyOS.J007engine.core;

import android.app.Application;
import android.util.Singleton;

import com.journeyOS.J007engine.core.detect.MonitorManager;
import com.journeyOS.J007engine.utils.SmartLog;

/**
 * @author solo
 */
public class J007Core {
    private static final String TAG = J007Core.class.getSimpleName();
    private static final boolean DEBUG = true;
    private static final Singleton<J007Core> gDefault = new Singleton<J007Core>() {
        @Override
        protected J007Core create() {
            return new J007Core();
        }
    };
    private Application sContext = null;
    private boolean isRunning = false;

    private J007Core() {
    }

    public static J007Core getCore() {
        return gDefault.get();
    }

    public void running(Application context) {
        SmartLog.d(TAG, "isRunning = [" + isRunning + "]");
        if (!isRunning) {
            isRunning = true;
            this.sContext = context;
            MonitorManager.getDefault().init();
        }
    }

    public Application getContext() {
        return sContext;
    }
}
