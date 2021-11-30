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

import android.util.ArrayMap;
import android.util.Singleton;

import com.journeyOS.J007engine.clients.ClientSession;
import com.journeyOS.J007engine.hidl.HidlJ007EngineManager;
import com.journeyOS.J007engine.utils.JsonUtils;
import com.journeyOS.J007engine.utils.SmartLog;

import java.util.Map;


public class NotifyManager {
    private static final String TAG = NotifyManager.class.getSimpleName();
    private static final Singleton<NotifyManager> gDefault = new Singleton<NotifyManager>() {
        @Override
        protected NotifyManager create() {
            return new NotifyManager();
        }
    };
    private static String sPackageName = "null";
    private static SceneState sState;
    private Object mLock = new Object();
    private Map<Long, NotifyListener> mMapListener = new ArrayMap<>();

    private NotifyManager() {
        sState = new SceneState();
        sState.battery = new SceneState.Battery();
    }

    public static NotifyManager getDefault() {
        return gDefault.get();
    }

    public int onFactorChanged(long factors, SceneState state) {
        String data = JsonUtils.toJson(state);
        SmartLog.d(TAG, " factors = [" + factors + "], state = [" + state + "], packageName = [" + sPackageName + "]");
        HidlJ007EngineManager.getService().notifySceneChanged(factors, data, sPackageName);
        synchronized (mLock) {
            try {
                ClientSession clientSession = ClientSession.getDefault();
                if (clientSession != null) {
                    clientSession.dispatchFactorEvent(factors, data, sPackageName);
                    if (mMapListener != null) {
                        NotifyListener listener = mMapListener.get(factors);
                        if (listener != null) {
                            listener.onFactorChanged(factors, state);
                        }
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return 0;
    }

    public int onFactorChanged(long factors, String packageName) {
        String data = JsonUtils.toJson(getCurrentState());
        SmartLog.d(TAG, " factors = [" + factors + "], state = [" + data + "], packageName = [" + packageName + "]");
        HidlJ007EngineManager.getService().notifySceneChanged(factors, data, packageName);
        synchronized (mLock) {
            try {
                ClientSession clientSession = ClientSession.getDefault();
                if (clientSession != null) {
                    clientSession.dispatchFactorEvent(factors, data, packageName);
                    if (mMapListener != null) {
                        NotifyListener listener = mMapListener.get(factors);
                        if (listener != null) {
                            listener.onFactorChanged(factors, getCurrentState());
                        }
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return 0;
    }

    public SceneState getCurrentState() {
        return sState;
    }

    public String getPackageName() {
        return sPackageName;
    }

    public void setPackageName(String packageName) {
        this.sPackageName = packageName;
    }

    public void setOnNotifyListener(long factoryId, NotifyListener listener) {
        mMapListener.put(factoryId, listener);
    }

    public interface NotifyListener {
        void onFactorChanged(long factoryId, SceneState data);
    }
}
