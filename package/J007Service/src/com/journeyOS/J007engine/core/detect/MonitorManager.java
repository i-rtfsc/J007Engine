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

package com.journeyOS.J007engine.core.detect;

import android.util.Singleton;

import com.journeyOS.J007engine.utils.SmartLog;

import java.util.ArrayList;

/**
 * @author solo
 */
public class MonitorManager {
    private static final String TAG = MonitorManager.class.getSimpleName();

    private static final Singleton<MonitorManager> gDefault = new Singleton<MonitorManager>() {
        @Override
        protected MonitorManager create() {
            return new MonitorManager();
        }
    };
    private final ArrayList<Monitor> mMonitors = new ArrayList<Monitor>();

    private MonitorManager() {
    }

    public static MonitorManager getDefault() {
        return gDefault.get();
    }

    public void init() {
        addMonitor(AccessibilityMonitor.getDefault());
        addMonitor(BatteryMonitor.getDefault());
        addMonitor(BrightnessMonitor.getDefault());
        startMonitors();
    }

    private void addMonitor(Monitor monitor) {
        try {
            mMonitors.add(monitor);
        } catch (Throwable e) {
            e.printStackTrace();
            SmartLog.e(TAG, "Failed to add monitor " + monitor.getClass().getName());
        }
    }

    public void startMonitors() {
        final int size = mMonitors.size();
        for (int i = 0; i < size; i++) {
            final Monitor monitor = mMonitors.get(i);
            try {
                monitor.onStart();
            } catch (Exception e) {
                e.printStackTrace();
                SmartLog.e(TAG, "Failed to start monitor " + monitor.getClass().getName());
            }
        }
    }

    public void stopMonitors() {
        final int size = mMonitors.size();
        for (int i = 0; i < size; i++) {
            final Monitor monitor = mMonitors.get(i);
            try {
                monitor.onStop();
            } catch (Exception e) {
                e.printStackTrace();
                SmartLog.e(TAG, "Failed to stop monitor " + monitor.getClass().getName());
            }
        }
    }

}
