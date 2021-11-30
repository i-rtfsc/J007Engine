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

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.BatteryManager;
import android.util.Singleton;

import com.journeyOS.J007engine.core.J007Core;
import com.journeyOS.J007engine.core.NotifyManager;
import com.journeyOS.J007engine.core.SceneState;
import com.journeyOS.J007engine.utils.SmartLog;

/**
 * @author solo
 */
public class BatteryMonitor extends Monitor {
    private static final String TAG = BatteryMonitor.class.getSimpleName();
    private static final Singleton<BatteryMonitor> gDefault = new Singleton<BatteryMonitor>() {
        @Override
        protected BatteryMonitor create() {
            return new BatteryMonitor();
        }
    };
    private Context mContext;

    private BatteryMonitor() {
        mContext = J007Core.getCore().getContext();
    }

    public static BatteryMonitor getDefault() {
        return gDefault.get();
    }

    @Override
    public void onStart() {
        SmartLog.i(TAG, "start battery monitor");
        IntentFilter filter = new IntentFilter();
        filter.addAction(Intent.ACTION_BATTERY_CHANGED);
        mContext.registerReceiver(new BatteryBroadcastReceiver(), filter);
    }

    @Override
    public void onStop() {
    }

    private class BatteryBroadcastReceiver extends BroadcastReceiver {

        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (Intent.ACTION_BATTERY_CHANGED.equals(action)) {
                int level = (int) (100f
                        * intent.getIntExtra(BatteryManager.EXTRA_LEVEL, 0)
                        / intent.getIntExtra(BatteryManager.EXTRA_SCALE, 100));
                int pluggedIn = intent.getIntExtra(BatteryManager.EXTRA_PLUGGED, 0);

                int status = intent.getIntExtra(BatteryManager.EXTRA_STATUS,
                        BatteryManager.BATTERY_STATUS_UNKNOWN);

                int health = intent.getIntExtra(BatteryManager.EXTRA_HEALTH, 0);
                int temperature = intent.getIntExtra(BatteryManager.EXTRA_TEMPERATURE, 0);

                SceneState sceneState = NotifyManager.getDefault().getCurrentState();
                SceneState.Battery current = sceneState.battery;
                current.level = level;
                current.pluggedIn = pluggedIn;
                current.status = status;
                current.health = health;
                current.temperature = temperature;
                sceneState.battery = current;
                NotifyManager.getDefault().onFactorChanged(Monitor.SCENE_FACTOR_BATTERY, sceneState);
            }
        }
    }

}
