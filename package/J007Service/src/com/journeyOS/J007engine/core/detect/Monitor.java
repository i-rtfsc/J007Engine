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

package com.journeyOS.J007engine.core.detect;

import com.journeyOS.J007engine.J007EngineManager;

/**
 * @author solo
 */
public abstract class Monitor {
    public abstract void onStart();

    public abstract void onStop();

    /**
     * 前台变化为何种类型APP场景
     * 1 << 1
     */
    public static final long SCENE_FACTOR_APP = J007EngineManager.SCENE_FACTOR_APP;

    /**
     * 屏幕亮灭场景
     * 1 << 2
     */
    public static final long SCENE_FACTOR_LCD = J007EngineManager.SCENE_FACTOR_LCD;
    /**
     * 屏幕亮度变化
     * 1 << 3
     */
    public static final long SCENE_FACTOR_BRIGHTNESS = J007EngineManager.SCENE_FACTOR_BRIGHTNESS;
    /**
     * 网络变化场景
     * 1 << 4
     */
    public static final long SCENE_FACTOR_NET = J007EngineManager.SCENE_FACTOR_NET;
    /**
     * 耳机插拔场景
     * 1 << 5
     */
    public static final long SCENE_FACTOR_HEADSET = J007EngineManager.SCENE_FACTOR_HEADSET;
    /**
     * 电池电量、温度等变化场景
     * 1 << 6
     */
    public static final long SCENE_FACTOR_BATTERY = J007EngineManager.SCENE_FACTOR_BATTERY;

    public static final String APP_DEFAULT = J007EngineManager.APP_DEFAULT;
}
