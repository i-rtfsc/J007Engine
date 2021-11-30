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

import android.content.ContentResolver;
import android.content.Context;
import android.database.ContentObserver;
import android.net.Uri;
import android.os.FileObserver;
import android.os.Handler;
import android.provider.Settings;
import android.util.Singleton;

import com.journeyOS.J007engine.core.J007Core;
import com.journeyOS.J007engine.core.NotifyManager;
import com.journeyOS.J007engine.core.SceneState;
import com.journeyOS.J007engine.utils.SmartLog;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;


/**
 * @author solo
 */
public class BrightnessMonitor extends Monitor {
    private static final String TAG = BrightnessMonitor.class.getSimpleName();

    private Context mContext;

    private static final Singleton<BrightnessMonitor> gDefault = new Singleton<BrightnessMonitor>() {
        @Override
        protected BrightnessMonitor create() {
            return new BrightnessMonitor();
        }
    };

    //private BrightnessFileObserver mBrightnessFileObserver;
    private BrightnessObserver mBrightnessObserver;

    private BrightnessMonitor() {
        mContext = J007Core.getCore().getContext();
        //mBrightnessFileObserver = new BrightnessFileObserver();
        mBrightnessObserver = new BrightnessObserver(null);
    }

    public static BrightnessMonitor getDefault() {
        return gDefault.get();
    }

    @Override
    public void onStart() {
        SmartLog.i(TAG, "start brightness monitor");
        //mBrightnessFileObserver.startWatching();
        mBrightnessObserver.register();
    }

    @Override
    public void onStop() {
    }

    private class BrightnessFileObserver extends FileObserver {
        private static final String BRIGHTNESS_FILE = "/sys/class/backlight/panel0-backlight/brightness";

        public BrightnessFileObserver() {
            super(BRIGHTNESS_FILE, FileObserver.MODIFY);
        }

        @Override
        public void onEvent(int event, String path) {
            long brightness = readBrightness();
            SmartLog.d(TAG, " event = [" + event + "], path = [" + path + "], brightness = [" + brightness + "]");
            SceneState sceneState = NotifyManager.getDefault().getCurrentState();
            sceneState.brightness = brightness;
            NotifyManager.getDefault().onFactorChanged(Monitor.SCENE_FACTOR_BRIGHTNESS, sceneState);
        }

        private long readBrightness() {
            BufferedReader reader = null;
            String line = null;
            try {
                reader = new BufferedReader(new FileReader(BRIGHTNESS_FILE));
                line = reader.readLine();
            } catch (Exception e) {
                e.printStackTrace();
                SmartLog.w(TAG, "Exception in read brightness: " + e.getMessage());
            } finally {
                if (reader != null) {
                    try {
                        reader.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }

            if (line == null) {
                return 0;
            } else {
                try {
                    return Long.valueOf(line);
                } catch (Throwable e) {
                }
            }
            return 0;
        }
    }

    private class BrightnessObserver extends ContentObserver {
        private final Uri BRIGHTNESS_URI =
                Settings.System.getUriFor(Settings.System.SCREEN_BRIGHTNESS);
//        private final Uri BRIGHTNESS_MODE_URI =
//                Settings.System.getUriFor(Settings.System.SCREEN_BRIGHTNESS_MODE);
//        private final Uri BRIGHTNESS_ADJ_URI =
//                Settings.System.getUriFor(Settings.System.SCREEN_AUTO_BRIGHTNESS_ADJ);

        public BrightnessObserver(Handler handler) {
            super(handler);
        }

        public void register() {
            Uri brightnessUri = Settings.System.getUriFor(Settings.System.SCREEN_BRIGHTNESS);
            mContext.getContentResolver().registerContentObserver(brightnessUri,
                    true, this);
        }

        public void unregister() {
            mContext.getContentResolver().unregisterContentObserver(this);
        }

        @Override
        public void onChange(boolean selfChange, Uri uri) {
            SmartLog.d(TAG, " selfChange = [" + selfChange + "], uri = [" + uri + "]");
            if (selfChange) {
                return;
            }
            if (BRIGHTNESS_URI.equals(uri)) {
                long brightness = getSystemBrightness(mContext);
                SmartLog.d(TAG, " brightness = [" + brightness + "]");
                SceneState sceneState = NotifyManager.getDefault().getCurrentState();
                sceneState.brightness = brightness;
                NotifyManager.getDefault().onFactorChanged(Monitor.SCENE_FACTOR_BRIGHTNESS, sceneState);
            }
        }

        private int getSystemBrightness(Context context) {
            int systemBrightness = 0;
            try {
                ContentResolver contentResolver = context.getContentResolver();
                systemBrightness = Settings.System.getInt(contentResolver, Settings.System.SCREEN_BRIGHTNESS);
            } catch (Settings.SettingNotFoundException e) {
                e.printStackTrace();
            }
            return systemBrightness;
        }
    }

}
