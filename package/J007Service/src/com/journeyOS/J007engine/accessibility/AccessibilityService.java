/*
 * Copyright (c) 2019 anqi.huang@outlook.com
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

package com.journeyOS.J007engine.accessibility;

import android.view.accessibility.AccessibilityEvent;
import android.view.accessibility.AccessibilityNodeInfo;

import com.journeyOS.J007engine.utils.SmartLog;

import java.util.HashSet;
import java.util.Map;
import java.util.Set;
import java.util.SortedMap;
import java.util.TreeMap;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

public class AccessibilityService extends android.accessibilityservice.AccessibilityService {
    private static final String TAG = AccessibilityService.class.getSimpleName();

    private static final SortedMap<Integer, AccessibilityDelegate> mDelegates = new TreeMap<>();
    private static final ReentrantLock LOCK = new ReentrantLock();
    private static final Condition ENABLED = LOCK.newCondition();
    private static AccessibilityService instance;
    private static boolean containsAllEventTypes = false;
    private static final Set<Integer> eventTypes = new HashSet<>();
    private AccessibilityNodeInfo mFastRootInActiveWindow;

    public static void addDelegate(int uniquePriority, AccessibilityDelegate delegate) {
        mDelegates.put(uniquePriority, delegate);
        Set<Integer> set = delegate.getEventTypes();
        if (set == null) {
            containsAllEventTypes = true;
        } else {
            eventTypes.addAll(set);
        }
    }

    public static AccessibilityService getInstance() {
        return instance;
    }

    @Override
    public void onAccessibilityEvent(final AccessibilityEvent event) {
        instance = this;
        if (!containsAllEventTypes && !eventTypes.contains(event.getEventType())) {
            return;
        }

        int type = event.getEventType();
        if (type == AccessibilityEvent.TYPE_WINDOW_STATE_CHANGED ||
                type == AccessibilityEvent.TYPE_VIEW_FOCUSED) {
            AccessibilityNodeInfo root = super.getRootInActiveWindow();
            if (root != null) {
                mFastRootInActiveWindow = root;
            }
        }

        for (Map.Entry<Integer, AccessibilityDelegate> entry : mDelegates.entrySet()) {
            AccessibilityDelegate delegate = entry.getValue();
            Set<Integer> types = delegate.getEventTypes();
            if (types != null && !delegate.getEventTypes().contains(event.getEventType())) {
                continue;
            }
            if (delegate.onAccessibilityEvent(AccessibilityService.this, event)) {
                break;
            }
        }
    }


    @Override
    public void onInterrupt() {

    }

    @Override
    public AccessibilityNodeInfo getRootInActiveWindow() {
        try {
            return super.getRootInActiveWindow();
        } catch (IllegalStateException e) {
            return null;
        }
    }

    @Override
    public void onDestroy() {
        instance = null;
        super.onDestroy();
    }


    @Override
    protected void onServiceConnected() {
        instance = this;
        super.onServiceConnected();
        LOCK.lock();
        ENABLED.signalAll();
        LOCK.unlock();
    }

    public AccessibilityNodeInfo fastRootInActiveWindow() {
        return mFastRootInActiveWindow;
    }


    public static boolean waitForEnabled(long timeOut) {
        if (instance != null) {
            return true;
        }
        LOCK.lock();
        try {
            if (instance != null) {
                return true;
            }
            if (timeOut == -1) {
                ENABLED.await();
                return true;
            }
            return ENABLED.await(timeOut, TimeUnit.MILLISECONDS);
        } catch (InterruptedException e) {
            e.printStackTrace();
            return false;
        } finally {
            LOCK.unlock();
        }
    }

}
