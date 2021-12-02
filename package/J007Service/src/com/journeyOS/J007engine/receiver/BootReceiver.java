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

package com.journeyOS.J007engine.receiver;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.IBinder;
import android.os.ServiceManager;

import com.journeyOS.J007engine.utils.Constant;
import com.journeyOS.J007engine.utils.SmartLog;


/**
 * @author solo
 * boot completed broadcast receiver
 */
public class BootReceiver extends BroadcastReceiver {
    private static final String TAG = BootReceiver.class.getSimpleName();

    @Override
    public void onReceive(Context context, Intent intent) {
        SmartLog.d(TAG, "BootReceiver received!");
        if (Intent.ACTION_BOOT_COMPLETED.equals(intent.getAction())) {
            IBinder binder = ServiceManager.getService(Constant.J007_ENGINE_SERVICE_NAME);
            if (binder == null) {
                Intent intentService = new Intent();
                intentService.setPackage(Constant.J007_ENGINE_PACKAGE);
                intentService.setAction(Constant.J007_ENGINE_SERVICE_AIDL);
                context.startService(intentService);
            }
        }
    }
}
