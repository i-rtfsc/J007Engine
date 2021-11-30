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

package com.journeyOS.J007engine.clients;

import android.os.Binder;
import android.os.Handler;
import android.os.Message;
import android.util.Singleton;

import com.journeyOS.J007engine.IJ007EngineListener;
import com.journeyOS.J007engine.task.TaskManager;

public class ClientSession {
    private static final int DELAYED_MILLIS = 50;
    private static final int MSG_OBJ = 1;
    private static final Singleton<ClientSession> gDefault = new Singleton<ClientSession>() {
        @Override
        protected ClientSession create() {
            return new ClientSession();
        }
    };
    private final Handler mHandler;
    private final Clients mClisnts;

    private ClientSession() {
        mHandler = TaskManager.getDefault().getHandle(TaskManager.HANDLER_CLIENT_SESSION);
        mClisnts = new Clients(mHandler);
        listener();
    }

    public static ClientSession getDefault() {
        return gDefault.get();
    }

    public void insertToCategory(long factorsFromClient, IJ007EngineListener listener) {
        int callingPid = Binder.getCallingPid();
        mClisnts.addListener(listener, factorsFromClient, callingPid);
    }

    public void removeFromCategory(IJ007EngineListener listener) {
        mClisnts.removeListener(listener);
    }

    public synchronized void dispatchFactorEvent(final long factors, final String state, final String packageName) {
        Message message = Message.obtain();
        message.what = MSG_OBJ;
        MessagesInfo info = new MessagesInfo(factors, state, packageName);
        message.obj = info;
        mHandler.sendMessageDelayed(message, DELAYED_MILLIS);
    }

    private void listener() {
        TaskManager.getDefault().setOnMessageListener(mHandler, new TaskManager.OnMessageListener() {
                    @Override
                    public void handleMessage(Message msg) {
                        switch (msg.what) {
                            case MSG_OBJ:
                                MessagesInfo obj = (MessagesInfo) msg.obj;
                                mClisnts.dispatchFactorEvent(obj.factoryId, obj.state, obj.packageName);
                                break;
                        }
                    }
                }
        );
    }

    private static class MessagesInfo {
        public long factoryId;
        public String state;
        public String packageName;

        private MessagesInfo(long factoryId, String state, String packageName) {
            this.factoryId = factoryId;
            this.state = state;
            this.packageName = packageName;
        }
    }
}
