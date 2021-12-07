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

import android.os.RemoteException;

import com.journeyOS.J007engine.IJ007EngineListener;
import com.journeyOS.J007engine.IJ007EngineInterface;
import com.journeyOS.J007engine.clients.ClientSession;
import com.journeyOS.J007engine.hidl.HidlJ007EngineManager;
import com.journeyOS.J007engine.utils.SmartLog;

/**
 * @author solo
 * J007 java service impl
 */
public class J007EngineServiceImpl extends IJ007EngineInterface.Stub {
    private static final String TAG = J007EngineServiceImpl.class.getSimpleName();

    public J007EngineServiceImpl() {
    }

    @Override
    public void registerCallBack(long factors, IJ007EngineListener listener) throws RemoteException {
        if (listener == null || factors == 0) {
            SmartLog.i(TAG, "listener or factors is null");
        }

        try {
            ClientSession.getDefault().insertToCategory(factors, listener);
        } catch (Exception e) {
            e.printStackTrace();
            SmartLog.e(TAG, "Exception in register listener : " + factors + ", " + listener);
        }
    }

    @Override
    public void unregisterCallback(IJ007EngineListener listener) throws RemoteException {
        if (listener == null) {
            SmartLog.i(TAG, "listener is null");
        }
        try {
            ClientSession.getDefault().removeFromCategory(listener);
        } catch (Exception e) {
            e.printStackTrace();
            SmartLog.w(TAG, "Exception in unregisterLister : " + listener);
        }
    }

    @Override
    public void showEngineDelayed(int direction, long delayMillis) throws RemoteException {
        SmartLog.d(TAG, "showing engine Delayed, direction = [" + direction + "], delayMillis = [" + delayMillis + "]");
    }

    @Override
    public void hideEngineDelayed(boolean isAnimator, long delayMillis) throws RemoteException {
        SmartLog.d(TAG, "hiding engine Delayed, isAnimator = [" + isAnimator + "], delayMillis = [" + delayMillis + "]");
    }

    @Override
    public void setConfig(int feature, String json) throws RemoteException {
        SmartLog.d(TAG, "set J007 engine, feature = [" + feature + "], config = [" + json + "]");
        HidlJ007EngineManager.getService().setConfig(feature, json);
    }

    @Override
    public String getConfig(int feature) throws RemoteException {
        SmartLog.d(TAG, "get J007 engine, feature = [" + feature + "]");
        return HidlJ007EngineManager.getService().getConfig(feature);
        //return null;
    }

}
