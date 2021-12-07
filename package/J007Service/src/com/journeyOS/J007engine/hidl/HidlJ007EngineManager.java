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

package com.journeyOS.J007engine.hidl;

import android.os.RemoteException;
import android.util.Log;
import android.util.Singleton;

import com.journeyOS.J007engine.hidl.V1_0.IJ007Engine;
import com.journeyOS.J007engine.hidl.V1_0.IJ007EngineCallback;
import com.journeyOS.J007engine.hidl.V1_0.J007EngineResponse;
import com.journeyOS.J007engine.hidl.V1_0.TCode;
import com.journeyOS.J007engine.utils.SmartLog;


/**
 * @author solo
 */
public class HidlJ007EngineManager extends IJ007EngineCallback.Stub {
    private static final String TAG = HidlJ007EngineManager.class.getSimpleName();

    public static final int SET_XXX = TCode.SET_XXX;
    public static final int GET_XXX = TCode.GET_XXX;
    public static final int SET_YYY = TCode.SET_YYY;
    public static final int GET_YYY = TCode.GET_YYY;

    private static final Singleton<HidlJ007EngineManager> gDefault = new Singleton<HidlJ007EngineManager>() {
        @Override
        protected HidlJ007EngineManager create() {
            return new HidlJ007EngineManager();
        }
    };

    private IJ007Engine mService;
    private boolean hasRegister = false;

    private HidlJ007EngineManager() {
        register();
    }

    public static HidlJ007EngineManager getService() {
        return gDefault.get();
    }


    @Override
    public void onResponse(J007EngineResponse j007EngineResponse) throws RemoteException {
        int code = j007EngineResponse.code;
        int result = j007EngineResponse.result;
        int status = j007EngineResponse.status;
        String messages = j007EngineResponse.messages;
        SmartLog.d(TAG, " code = [" + code + "], status = [" + status + "]," +
                " result = [" + result + "], messages = [" + messages + "]");
    }

    private IJ007Engine getServiceImpl(boolean force) {
        try {
            if (force) {
                mService = IJ007Engine.getService();
            } else {
                if (mService == null) {
                    mService = IJ007Engine.getService();
                }
            }
        } catch (RemoteException e) {
            e.printStackTrace();
        }
        if (SmartLog.isDebug()) {
            SmartLog.d(TAG, " force = [" + force + "], service = [" + mService + "]");
        }
        if (mService != null) {
            try {
                mService.linkToDeath(new DeathRecipient() {
                    @Override
                    public void serviceDied(long l) {
                        mService = null;
                        hasRegister = false;
                    }
                }, 0);
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        }
        return mService;
    }

    public void register() {
        if (hasRegister) {
            return;
        }

        IJ007Engine service = getServiceImpl(false);
        if (service != null) {
            try {
                service.registerCallback(this);
                hasRegister = true;
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        }
    }

    public void unregister() {
        if (!hasRegister) {
            return;
        }

        IJ007Engine service = getServiceImpl(false);
        if (service != null) {
            try {
                service.unregisterCallback(this);
                hasRegister = false;
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        }
    }

    public void notifySceneChanged(long factors, String state, String packageName) {
        if (SmartLog.isDebug()) {
            SmartLog.d(TAG, " factors = [" + factors + "], state = [" + state + "], packageName = [" + packageName + "]");
        }
        IJ007Engine service = getServiceImpl(false);
        if (service != null) {
            try {
                //if J007Engine death, try to register if needed
                register();
                service.notifySceneChanged((int) factors, state, packageName);
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        }
    }

    public void setConfig(int code, String config) {
        IJ007Engine service = getServiceImpl(false);
        if (service != null) {
            try {
                service.setConfig(code, config);
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        }
    }

    public String getConfig(int code) {
        String config = null;
        IJ007Engine service = getServiceImpl(false);
        if (service != null) {
            try {
                config = service.getConfig(code);
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        }
        return config;
    }

}
