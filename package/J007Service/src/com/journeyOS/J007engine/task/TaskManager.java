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

package com.journeyOS.J007engine.task;

import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.os.Message;
import android.os.Process;
import android.util.Singleton;

import com.journeyOS.J007engine.utils.SmartLog;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

public class TaskManager {
    private static final String TAG = TaskManager.class.getSimpleName();

    public static final String HANDLER_CLIENT_SESSION = "ClientSession";

    private final Executor mDiskIO;

    private final Executor mNetworkIO;

    private final Executor mMainThread;

    private Map<String, Handler> mHandlerMap = new ConcurrentHashMap<>();

    private TaskManager(Executor diskIO, Executor networkIO, Executor mainThread) {
        this.mDiskIO = diskIO;
        this.mNetworkIO = networkIO;
        this.mMainThread = mainThread;
    }

    public TaskManager() {
        this(Executors.newSingleThreadExecutor(), Executors.newFixedThreadPool(3),
                new MainThreadExecutor());
    }

    private static final Singleton<TaskManager> gDefault = new Singleton<TaskManager>() {
        @Override
        protected TaskManager create() {
            return new TaskManager();
        }
    };

    public static TaskManager getDefault() {
        return gDefault.get();
    }

    public Executor diskIO() {
        return mDiskIO;
    }

    public Executor networkIO() {
        return mNetworkIO;
    }

    public Executor mainThread() {
        return mMainThread;
    }

    public Handler mainHandle() {
        return new Handler(Looper.getMainLooper());
    }

    public Handler getHandle(String handlerName) {
        if ((mHandlerMap.size() != 0) && mHandlerMap.containsKey(handlerName)) {
            return mHandlerMap.get(handlerName);
        }

        HandlerThread handlerThread = new HandlerThread(handlerName, Process.THREAD_PRIORITY_BACKGROUND);
        handlerThread.start();
        SafeDispatchHandler handler = new SafeDispatchHandler(handlerThread.getLooper());
        mHandlerMap.put(handlerName, handler);
        return handler;
    }

    public void setOnMessageListener(Handler handler, final OnMessageListener listener) {
        if (handler == null || listener == null) {
            SmartLog.w(TAG, "handler or listener was NULL!");
            return;
        }

        if (handler instanceof SafeDispatchHandler) {
            SafeDispatchHandler safeHandler = (SafeDispatchHandler) handler;
            safeHandler.setOnHandleMessageListener(new SafeDispatchHandler.OnHandleMessageListener() {
                @Override
                public void handleMessage(Message msg) {
                    listener.handleMessage(msg);
                }
            });
        } else {
            SmartLog.w(TAG, "this handler wasn't SafeDispatchHandler");
        }

    }

    private static class MainThreadExecutor implements Executor {
        private Handler mainThreadHandler = new Handler(Looper.getMainLooper());

        @Override
        public void execute(Runnable command) {
            mainThreadHandler.post(command);
        }
    }

    private static class SafeDispatchHandler extends Handler {
        private static final String TAG = SafeDispatchHandler.class.getCanonicalName();

        public SafeDispatchHandler(Looper looper) {
            super(looper);
        }

        public SafeDispatchHandler(Looper looper, Callback callback) {
            super(looper, callback);
        }

        public SafeDispatchHandler() {
            super();
        }

        public SafeDispatchHandler(Callback callback) {
            super(callback);
        }

        @Override
        public void dispatchMessage(Message msg) {
            try {
                super.dispatchMessage(msg);
            } catch (Exception e) {
                SmartLog.d(TAG, "dispatchMessage Exception " + msg + " , " + e);
            } catch (Error error) {
                SmartLog.d(TAG, "dispatchMessage error " + msg + " , " + error);
            }
        }

        @Override
        public void handleMessage(Message msg) {
            try {
                super.handleMessage(msg);
                if (mListener != null) {
                    mListener.handleMessage(msg);
                }
            } catch (Exception e) {
                SmartLog.d(TAG, "handleMessage Exception " + msg + " , " + e);
            } catch (Error error) {
                SmartLog.d(TAG, "handleMessage error " + msg + " , " + error);
            }
        }

        private static OnHandleMessageListener mListener;

        protected void setOnHandleMessageListener(OnHandleMessageListener listener) {
            mListener = listener;
        }

        protected interface OnHandleMessageListener {
            void handleMessage(Message msg);
        }
    }

    public interface OnMessageListener {
        void handleMessage(Message msg);
    }
}

