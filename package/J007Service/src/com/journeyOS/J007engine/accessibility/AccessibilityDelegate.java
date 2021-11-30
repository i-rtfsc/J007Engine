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

import java.util.Set;

public interface AccessibilityDelegate {
    Set<Integer> ALL_EVENT_TYPES = null;

    boolean onAccessibilityEvent(AccessibilityService service, AccessibilityEvent event);

    /**
     * 返回调用onAccessibilityEvent时的EventType的集合。如果需要对所有EventType都有效，返回null。
     */
    Set<Integer> getEventTypes();
}
