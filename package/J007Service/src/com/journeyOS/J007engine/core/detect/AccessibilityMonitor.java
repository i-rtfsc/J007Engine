/*
 * Copyright (c) 2018 anqi.huang@outlook.com
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

import android.content.Context;
import android.util.Singleton;

import com.journeyOS.J007engine.accessibility.AccessibilityInfoObserver;
import com.journeyOS.J007engine.accessibility.AccessibilityService;
import com.journeyOS.J007engine.accessibility.ActivityListener;
import com.journeyOS.J007engine.core.J007Core;
import com.journeyOS.J007engine.core.NotifyManager;
import com.journeyOS.J007engine.database.LocalDataSourceImpl;
import com.journeyOS.J007engine.database.app.App;
import com.journeyOS.J007engine.task.TaskManager;
import com.journeyOS.J007engine.utils.SmartLog;


public class AccessibilityMonitor extends Monitor implements ActivityListener {
    private static final String TAG = AccessibilityMonitor.class.getSimpleName();

    private final AccessibilityInfoObserver mAccessibilityInfoObserver;

    private static String sPackageName = null;

    private static final Singleton<AccessibilityMonitor> gDefault = new Singleton<AccessibilityMonitor>() {
        @Override
        protected AccessibilityMonitor create() {
            return new AccessibilityMonitor();
        }
    };
    private Context mContext;

    private AccessibilityMonitor() {
        mContext = J007Core.getCore().getContext();
        mAccessibilityInfoObserver = new AccessibilityInfoObserver(mContext);
    }

    public static AccessibilityMonitor getDefault() {
        return gDefault.get();
    }

    @Override
    public void onStart() {
        addAccessibilityServiceDelegates();
    }

    @Override
    public void onStop() {
    }

    private void addAccessibilityServiceDelegates() {
        AccessibilityService.addDelegate(100, mAccessibilityInfoObserver);
        mAccessibilityInfoObserver.addListener(this);
    }

    @Override
    public void activityResumed(String packageName, String activity) {
        if (SmartLog.isDebug()) {
            SmartLog.d(TAG, "on activity lister, packageName = [" + packageName + "], activity = [" + activity + "]");
        }

        NotifyManager.getDefault().setPackageName(packageName);
        if (!packageName.equals(sPackageName)) {
            SmartLog.d(TAG, "packageName = [" + packageName + "]");
            if (packageName == null) {
                return;
            }

            TaskManager.getDefault().diskIO().execute(new Runnable() {
                @Override
                public void run() {
                    App app = LocalDataSourceImpl.getDefault().getApp(packageName);
                    if (app == null) {
                        app = new App();
                        app.packageName = packageName;
                        app.type = Monitor.APP_DEFAULT;
                    }
                    NotifyManager.getDefault().onFactorChanged(Monitor.SCENE_FACTOR_APP, app);
                }
            });
        }
        sPackageName = packageName;
    }

}
