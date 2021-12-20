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

import com.journeyOS.J007engine.IJ007EngineListener;


interface IJ007EngineInterface {
    void registerCallBack(long factors, IJ007EngineListener cb);
    void unregisterCallback(IJ007EngineListener cb);    
    void showEngineDelayed(int direction, long delayMillis);   
    void hideEngineDelayed(boolean isAnimator, long delayMillis);
    void setConfig(int feature, String json);
    String getConfig(int feature);
}