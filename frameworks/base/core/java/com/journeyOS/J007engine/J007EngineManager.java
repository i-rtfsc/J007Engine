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

package com.journeyOS.J007engine;

import android.content.Context;
import android.content.Intent;
import android.os.Handler;
import android.os.IBinder;
import android.os.Process;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.text.format.DateUtils;
import android.util.Log;
import android.util.Singleton;

import com.android.internal.os.BackgroundThread;


public class J007EngineManager {
    public static final String TAG = J007EngineManager.class.getSimpleName();
    public static final String J007_ENGINE_DEBUG = "J007Engine";

    public static final String J007_ENGINE_SERVICE_NAME = "J007engine";
    public static final String J007_ENGINE_PACKAGE = "com.journeyOS.J007engine";
    public static final String J007_ENGINE_SERVICE_AIDL = "com.journeyOS.J007engine.action.J007EngineService";

    public static final long SCENE_FACTOR_APP = 1 << 1;
    public static final long SCENE_FACTOR_LCD = 1 << 2;
    public static final long SCENE_FACTOR_BRIGHTNESS = 1 << 3;
    public static final long SCENE_FACTOR_NET = 1 << 4;
    public static final long SCENE_FACTOR_HEADSET = 1 << 5;
    public static final long SCENE_FACTOR_BATTERY = 1 << 6;

    //app type
    public static final long SCENE_FACTOR_APP_DEFAULT = 1 << 1;
    public static final long SCENE_FACTOR_APP_ALBUM = 1 << 2;
    public static final long SCENE_FACTOR_APP_BROWSER = 1 << 3;
    public static final long SCENE_FACTOR_APP_GAME = 1 << 4;
    public static final long SCENE_FACTOR_APP_IM = 1 << 5;
    public static final long SCENE_FACTOR_APP_MUSIC = 1 << 6;
    public static final long SCENE_FACTOR_APP_NEWS = 1 << 7;
    public static final long SCENE_FACTOR_APP_READER = 1 << 8;
    public static final long SCENE_FACTOR_APP_VIDEO = 1 << 9;

    private static OnSceneChangedListener mSceneListener;
    private static long mFactorId;

    private static IJ007EngineListener mCallback = new IJ007EngineListener.Stub() {

        @Override
        public void onSceneChanged(long factorId, String status, String packageName) throws RemoteException {
            notifySceneChanged(factorId, status, packageName);
        }
    };

    private static IJ007EngineManagerSingleton sJ007EngineManagerSingleton;
    private static IJ007EngineInterface sAsInterface;

    public static void initService(Context context) {
        if (context != null) {
            int uid = Process.getUidForName(context.getPackageName());
            if (uid < 1000) {
                IBinder binder = ServiceManager.getService(J007_ENGINE_SERVICE_NAME);
                if (binder == null) {
                    Intent intentService = new Intent();
                    intentService.setPackage(J007_ENGINE_PACKAGE);
                    intentService.setAction(J007_ENGINE_SERVICE_AIDL);
                    context.startService(intentService);
                }
            }
        }
    }

    public static IJ007EngineInterface getService(Context context) {
        return getService();
    }

    private static IJ007EngineInterface connect() {
        IBinder binder = ServiceManager.getService(J007_ENGINE_SERVICE_NAME);
        if (binder != null) {
            sAsInterface = IJ007EngineInterface.Stub.asInterface(binder);
            try {
                //sAsInterface.registerCallBack(mCallback);
                binder.linkToDeath(new IBinder.DeathRecipient() {
                    @Override
                    public void binderDied() {
                        if (isDebug()) {
                            Log.w(TAG, "J007 engine died; reconnecting");
                        }
                        try {
                            if (sAsInterface != null && mCallback != null) {
                                sAsInterface.unregisterCallback(mCallback);
                            }
                        } catch (RemoteException e) {
                            e.printStackTrace();
                        }
                        sJ007EngineManagerSingleton = null;
                        sAsInterface = null;
                        connect();
                    }
                }, 0);
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        } else {
            sJ007EngineManagerSingleton = null;
            sAsInterface = null;
            if (isDebug()) {
                Log.w(TAG, "J007 engine not found; trying again");
            }
            BackgroundThread.getHandler().postDelayed(() -> {
                connect();
            }, DateUtils.SECOND_IN_MILLIS * 5);
        }

        return sAsInterface;
    }

    private static IJ007EngineInterface getService() {
        if (sJ007EngineManagerSingleton == null) {
            sJ007EngineManagerSingleton = new IJ007EngineManagerSingleton();
        }
        return sJ007EngineManagerSingleton.get();
    }

    /**
     * showing engine
     *
     * @param direction 1=left
     *                  2=right
     *                  3=up
     * @return true: if the called was successful
     */
    public static boolean showingEngine(int direction) {
        return showEngineDelayed(direction, 0);
    }

    /**
     * pending show engine
     *
     * @param direction   1=left
     *                    2=right
     *                    3=up
     * @param delayMillis delay millisecond
     * @return true: if the called was successful
     */
    public static boolean showEngineDelayed(int direction, long delayMillis) {
        IJ007EngineInterface engineInterface = getService();
        boolean success = (engineInterface != null);
        if (isDebug()) {
            Log.d(TAG, "J007 engine service is running = [" + success + "]" +
                    " show engine delayed, direction = [" + direction + "]," +
                    " delay millis = [" + delayMillis + "]");
        }

        if (success) {
            try {
                engineInterface.showEngineDelayed(direction, delayMillis);
            } catch (RemoteException e) {
                success = false;
                e.printStackTrace();
            }
        }
        return success;
    }

    /**
     * hiding engine
     *
     * @param isAnimator true: with animator
     *                   false: without animator
     * @return true: if the called was successful
     */
    public static boolean hidingEngine(boolean isAnimator) {
        return hideEngineDelayed(isAnimator, 0);
    }

    /**
     * pending hide engine
     *
     * @param isAnimator  true: with animator
     *                    false: without animator
     * @param delayMillis delay millisecond
     * @return true: if the called was successful
     */
    public static boolean hideEngineDelayed(boolean isAnimator, long delayMillis) {
        IJ007EngineInterface engineInterface = getService();
        boolean success = (engineInterface != null);
        if (isDebug()) {
            Log.d(TAG, "J007 engine service is running = [" + success + "]" +
                    " hiding engine delayed, is run animator = [" + isAnimator + "]," +
                    " delay millis = [" + delayMillis + "]");
        }

        if (success) {
            try {
                engineInterface.hideEngineDelayed(isAnimator, delayMillis);
            } catch (RemoteException e) {
                success = false;
                e.printStackTrace();
            }
        }

        return success;
    }

    /**
     * Retrieve a String value from the J007 engine cache manager.
     *
     * @param feature which action
     * @return Retrieve a String value
     */
    public static String getConfig(int feature) {
        IJ007EngineInterface engineInterface = getService();
        boolean success = (engineInterface != null);
        if (isDebug()) {
            Log.d(TAG, "J00engine service is running = [" + success + "]" +
                    " get J007 engine config, feature = [" + feature + "]");
        }

        String value = "";
        if (success) {
            try {
                value = engineInterface.getConfig(feature);
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        }

        return value;
    }

    /**
     * Retrieve a String value from the engine cache manager.
     *
     * @param feature which action
     * @return Retrieve a String value
     */
    public static boolean setConfig(int feature, String json) {
        IJ007EngineInterface engineInterface = getService();
        boolean success = (engineInterface != null);
        if (isDebug()) {
            Log.d(TAG, "J007 engine service is running = [" + success + "]" +
                    " set J007 engine config, feature = [" + feature + "]" +
                    " json = [" + json + "]");
        }

        if (success) {
            try {
                engineInterface.setConfig(feature, json);
            } catch (RemoteException e) {
                success = false;
                e.printStackTrace();
            }
        }

        return success;
    }


    /**
     * adb shell setprop log.tag.J007Engine D
     */
    private static boolean isDebug() {
        return Log.isLoggable(J007_ENGINE_DEBUG, Log.DEBUG);
    }

    protected static void notifySceneChanged(long factorId, String status, String packageName) {
        mSceneListener.onSceneChanged(factorId, status, packageName);
    }

    /**
     * Registers a callback to be invoked when a change happens to a scene changed.
     *
     * @param listener The callback that will run.
     */
    public static void registerOnSceneChangedListener(long factorId, OnSceneChangedListener listener) {
        if (listener == null) {
            throw new IllegalArgumentException("listener should not be null");
        }

        mFactorId = factorId;
        mSceneListener = listener;
        IJ007EngineInterface engineInterface = getService();
        boolean success = (engineInterface != null);
        if (isDebug()) {
            Log.d(TAG, "J007 engine service is running = [" + success + "]" +
                    " register scene changed factorId = [" + factorId + "]");
        }
        
        if(success) {
            try {
                engineInterface.registerCallBack(factorId, mCallback);
            } catch (RemoteException e) {
                success = false;
                e.printStackTrace();
            }
        }
    }

    /**
     * Unregisters a previous callback.
     *
     * @param listener The callback that should be unregistered.
     * @see #registerOnSceneChangedListener
     */
    public static void unregisterOnSceneChangedListener(OnSceneChangedListener listener) {
        if (listener == null) {
            throw new IllegalArgumentException("listener should not be null");
        }

        IJ007EngineInterface engineInterface = getService();
        boolean success = (engineInterface != null);
        if (isDebug()) {
            Log.d(TAG, "J007 engine service is running = [" + success + "]" +
                    " unregister scene changed factorId");
        }

        if(success) {
            try {
                engineInterface.unregisterCallback(mCallback);
            } catch (RemoteException e) {
                success = false;
                e.printStackTrace();
            }
        }
    }

    public interface OnSceneChangedListener {
        void onSceneChanged(long factorId, String status, String packageName);
    }


    private static class IJ007EngineManagerSingleton extends Singleton<IJ007EngineInterface> {
        @Override
        protected IJ007EngineInterface create() {
            return connect();
        }
    }
}