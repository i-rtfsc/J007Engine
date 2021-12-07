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

#ifndef _GLOBAL_SCENE_H
#define _GLOBAL_SCENE_H

#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

struct SourceScene {
    int factors;
    string status;
    string packageName;
};

struct Battery {
    int level;
    int pluggedIn;
    int status;
    int health;
    int temperature;
};

class GlobalScene {
public:
    GlobalScene();

    ~GlobalScene();

    static GlobalScene *getInstance();

    void updateScene(int32_t factors, string status, string packageName);

    SourceScene getSourceScene();

    int getApp();

    Battery getBattery();

    long getBrightness();


private:
    static GlobalScene *sInstance;

    void initConfig();

    SourceScene mSourceScene;
    int mApp;
    Battery mBattery;
    long mBrightness;
};


#endif //_GLOBAL_SCENE_H
