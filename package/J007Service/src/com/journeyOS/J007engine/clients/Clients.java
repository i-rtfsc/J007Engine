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

import android.os.Handler;
import android.os.RemoteException;

import com.journeyOS.J007engine.IJ007EngineListener;
import com.journeyOS.J007engine.utils.SmartLog;

import java.io.FileDescriptor;
import java.io.PrintWriter;


/**
 * @author solo
 */
public class Clients extends ClientsHelper {
    private static final String TAG = Clients.class.getSimpleName();

    public Clients(Handler handler) {
        super(handler);
    }

    public boolean addListener(IJ007EngineListener listener, long factors, int callingPid) {
        if (addRemoteListener(listener, factors, callingPid)) {
            SmartLog.i(TAG, "addListener " + listener
                    + " for " + callingPid + " " + Long.toHexString(factors));
            return true;
        } else {
            return false;
        }
    }

    public void removeListener(IJ007EngineListener listener) {
        try {
            removeRemoteListener(listener);
            SmartLog.i(TAG, "removeListener " + listener);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public synchronized void dispatchFactorEvent(final long factors, final String state, final String packageName) {
        Operation operation = new Operation() {
            @Override
            public void execute(IJ007EngineListener listener, int pid) throws RemoteException {
                try {
                    SmartLog.i(TAG, "dispatch to " + pid + " ("
                            + state
                            + packageName
                            + ")");
                    listener.onSceneChanged(factors, state, packageName);
                } catch (Throwable e) {
                    SmartLog.w(TAG, "Exception in dispatch factor event with appInfo!!!");
                }
            }
        };
        foreach(operation, factors);
    }

    public void dump(FileDescriptor fd, PrintWriter pw) {
        pw.println(super.toString());
    }

    private interface Operation extends ListenerOperation<IJ007EngineListener> {
    }
}

