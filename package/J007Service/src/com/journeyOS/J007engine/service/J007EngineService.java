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

package com.journeyOS.J007engine.service;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.ServiceManager;
import android.os.SystemClock;
import android.util.Log;

import androidx.annotation.Nullable;

import com.journeyOS.J007engine.accessibility.AccessibilityManager;
import com.journeyOS.J007engine.help.DumpManager;
import com.journeyOS.J007engine.utils.Constant;
import com.journeyOS.J007engine.utils.SmartLog;

import java.io.FileDescriptor;
import java.io.PrintWriter;


/**
 * @author solo
 * J007 java service
 */
public class J007EngineService extends Service {
    private static final String TAG = J007EngineService.class.getSimpleName();
    private IBinder mIBinder;

    @Override
    public void onCreate() {
        super.onCreate();
        mIBinder = new J007EngineServiceImpl();
        onSystemReady();
        boolean isServiceEnabled = AccessibilityManager.getDefault().isServiceEnabled();
        SmartLog.d(TAG, "is accessibility service enabled = [" + isServiceEnabled + "]");
        if (!isServiceEnabled) {
            AccessibilityManager.getDefault().autoEnableAccessibilityService();
        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
    }

    @Override
    protected void dump(FileDescriptor fd, PrintWriter writer, String[] args) {
        //super.dump(fd, writer, args);
        DumpManager.dump(fd, writer, args);
    }

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return mIBinder;
    }

    private void onSystemReady() {
        ServiceManager.addService(Constant.J007_ENGINE_SERVICE_NAME, mIBinder);
        SmartLog.w(TAG, "J007 engine service boot up");

        do {
            mIBinder = ServiceManager.checkService(Constant.J007_ENGINE_SERVICE_NAME);
            if (mIBinder != null) {
                break;
            }
            SmartLog.w(TAG, "J007 engine service binder can't be added to system, try again after 5s...");
            SystemClock.sleep(5000);
        } while (mIBinder == null);
    }
}
