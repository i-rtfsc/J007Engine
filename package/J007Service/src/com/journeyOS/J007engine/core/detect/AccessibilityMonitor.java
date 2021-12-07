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
import com.journeyOS.J007engine.utils.SmartLog;


public class AccessibilityMonitor extends Monitor implements ActivityListener {
    private static final String TAG = AccessibilityMonitor.class.getSimpleName();

    public static final String ALBUM = "album";
    public static final String BROWSER = "browser";
    public static final String GAME = "game";
    public static final String IM = "im";
    public static final String MUSIC = "music";
    public static final String NEWS = "news";
    public static final String READER = "reader";
    public static final String VIDEO = "video";

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
            //TODO
            NotifyManager.getDefault().onFactorChanged(Monitor.SCENE_FACTOR_APP, Monitor.SCENE_FACTOR_APP_DEFAULT);
        }
        sPackageName = packageName;
    }

//    public long getAppType(String packageName) {
//        App app = DatabaseManager.getDefault().queryApp(packageName);
//        SmartLog.d(TAG, "app = [" + app.type + "]");
//        if (ALBUM.equals(app.type)) {
//            return Monitor.SCENE_FACTOR_APP_ALBUM;
//        } else if (BROWSER.equals(app.type)) {
//            return Monitor.SCENE_FACTOR_APP_BROWSER;
//        } else if (GAME.equals(app.type)) {
//            return Monitor.SCENE_FACTOR_APP_GAME;
//        } else if (IM.equals(app.type)) {
//            return Monitor.SCENE_FACTOR_APP_IM;
//        } else if (MUSIC.equals(app.type)) {
//            return Monitor.SCENE_FACTOR_APP_MUSIC;
//        } else if (NEWS.equals(app.type)) {
//            return Monitor.SCENE_FACTOR_APP_NEWS;
//        } else if (READER.equals(app.type)) {
//            return Monitor.SCENE_FACTOR_APP_READER;
//        } else if (VIDEO.equals(app.type)) {
//            return Monitor.SCENE_FACTOR_APP_VIDEO;
//        } else {
//            return Monitor.SCENE_FACTOR_APP_DEFAULT;
//        }
//    }

}
