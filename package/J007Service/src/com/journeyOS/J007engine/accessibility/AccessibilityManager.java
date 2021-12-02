/*
 * Copyright (c) 2019 anqi.huang@outlook.com
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

package com.journeyOS.J007engine.accessibility;

import android.content.Context;
import android.content.Intent;
import android.provider.Settings;
import android.text.TextUtils;
import android.util.Singleton;

import com.journeyOS.J007engine.core.J007Core;
import com.journeyOS.J007engine.core.detect.MonitorManager;
import com.journeyOS.J007engine.utils.SmartLog;


public class AccessibilityManager {
    private static final String TAG = AccessibilityManager.class.getSimpleName();
    private static final String ACCESSIBILITY_SERVICES = "com.journeyOS.J007engine/com.journeyOS.J007engine.accessibility.AccessibilityService";

    private Context mContext;

    private AccessibilityManager() {
        mContext = J007Core.getCore().getContext();
    }

    private static final Singleton<AccessibilityManager> gDefault = new Singleton<AccessibilityManager>() {
        @Override
        protected AccessibilityManager create() {
            return new AccessibilityManager();
        }
    };

    public static AccessibilityManager getDefault() {
        return gDefault.get();
    }

    public void autoEnableAccessibilityService() {
        Context context = mContext;
        int enabled = 0;
        boolean found = false;
        try {
            enabled = Settings.Secure.getInt(context.getContentResolver(),
                    Settings.Secure.ACCESSIBILITY_ENABLED);
            SmartLog.d(TAG, "enabled = " + enabled);
        } catch (Settings.SettingNotFoundException e) {
            SmartLog.e(TAG, "accessibility to not found: " + e.getMessage());
        }

        TextUtils.SimpleStringSplitter sCS = new TextUtils.SimpleStringSplitter(':');
        if (enabled == 1) {
            String settingValue = Settings.Secure.getString(context.getContentResolver(),
                    Settings.Secure.ENABLED_ACCESSIBILITY_SERVICES);
            if (settingValue != null) {
                TextUtils.SimpleStringSplitter splitter = sCS;
                splitter.setString(settingValue);
                while (splitter.hasNext()) {
                    String accessabilityService = splitter.next();
                    if (accessabilityService.equalsIgnoreCase(ACCESSIBILITY_SERVICES)) {
                        SmartLog.d(TAG, "we've found the correct accessibility is switched on!");
                        found = true;
                    }
                }
            }
        } else {
            SmartLog.v(TAG, "accessibility is disabled");
            Settings.Secure.putInt(context.getContentResolver(), Settings.Secure.ACCESSIBILITY_ENABLED, 1);
        }

        if (!found) {
            String settingValue = Settings.Secure.getString(context.getContentResolver(),
                    Settings.Secure.ENABLED_ACCESSIBILITY_SERVICES);
            SmartLog.d(TAG, "enabled accessibility services = " + settingValue);
            if (settingValue != null) {
                Settings.Secure.putString(context.getContentResolver(),
                        Settings.Secure.ENABLED_ACCESSIBILITY_SERVICES,
                        ACCESSIBILITY_SERVICES + ":" + settingValue);
            } else {
                Settings.Secure.putString(context.getContentResolver(),
                        Settings.Secure.ENABLED_ACCESSIBILITY_SERVICES,
                        ACCESSIBILITY_SERVICES);
            }
        }
    }

    public void openSettingsAccessibilityService() {
        Context context = mContext;
        try {
            Intent intent = new Intent(Settings.ACTION_ACCESSIBILITY_SETTINGS);
            context.startActivity(intent);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public boolean isServiceEnabled() {
        Context context = mContext;
        int enabled = 0;
        boolean found = false;
        try {
            enabled = Settings.Secure.getInt(context.getContentResolver(),
                    Settings.Secure.ACCESSIBILITY_ENABLED);
            SmartLog.d(TAG, "enabled = " + enabled);
        } catch (Settings.SettingNotFoundException e) {
            SmartLog.e(TAG, "accessibility to not found: " + e.getMessage());
        }

        TextUtils.SimpleStringSplitter sCS = new TextUtils.SimpleStringSplitter(':');
        if (enabled == 1) {
            String settingValue = Settings.Secure.getString(context.getContentResolver(),
                    Settings.Secure.ENABLED_ACCESSIBILITY_SERVICES);
            if (settingValue != null) {
                TextUtils.SimpleStringSplitter splitter = sCS;
                splitter.setString(settingValue);
                while (splitter.hasNext()) {
                    String accessabilityService = splitter.next();
                    if (accessabilityService.equalsIgnoreCase(ACCESSIBILITY_SERVICES)) {
                        SmartLog.d(TAG, "we've found the correct accessibility is switched on!");
                        return true;
                    }
                }
            }
        } else {
            SmartLog.v(TAG, "accessibility is disabled");
        }

        return found;
    }

}
