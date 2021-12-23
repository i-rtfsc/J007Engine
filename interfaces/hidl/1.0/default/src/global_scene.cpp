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

#include "global_scene.h"
#include "log.h"
#include "factors.h"
#include "json/json_object.h"

#define LOG_TAG        "J007Engine-GlobalScene"

GlobalScene *GlobalScene::sInstance = NULL;

GlobalScene::GlobalScene() {
    initConfig();
}

GlobalScene::~GlobalScene() {
}

GlobalScene *GlobalScene::getInstance() {
    if (sInstance == NULL) {
        sInstance = new GlobalScene();
    }

    return sInstance;
}

void GlobalScene::initConfig() {
    ALOGI("init global scene...");
    //TODO
}

void GlobalScene::updateScene(int32_t factors, string status, string packageName) {
    if (DEBUG) {
        LOGI("notify scene changed, factors = %d , status = %s , packageName = %s\n", factors, status.c_str(),
             packageName.c_str());
    }

    //update source scene
    mSourceScene.factors = factors;
    mSourceScene.status = status;
    mSourceScene.packageName = packageName;

    JsonObject oJson(status);
    switch (factors) {
        case SCENE_FACTOR_APP:
            oJson["app"].Get("packageName", mApp.packageName);
            oJson["app"].Get("type", mApp.type);
            oJson["app"].Get("mode", mApp.mode);
            oJson["app"].Get("fps", mApp.fps);
            oJson["app"].Get("cpu", mApp.cpu);
            oJson["app"].Get("memc", mApp.memc);
            break;
        case SCENE_FACTOR_LCD:
            //TODO
            break;
        case SCENE_FACTOR_BRIGHTNESS:
            //TODO
            ALOGI("brightness  = %d ", SCENE_FACTOR_BRIGHTNESS);
            //mBrightness =
            break;
        case SCENE_FACTOR_NET:
            //TODO
            break;
        case SCENE_FACTOR_HEADSET:
            //TODO
            break;
        case SCENE_FACTOR_BATTERY:
            //TODO

            oJson["battery"].Get("temperature", mBattery.temperature);
            LOGI("battery temperature  = %d ", mBattery.temperature);
            break;
    }
}

SourceScene GlobalScene::getSourceScene() {
    return mSourceScene;
}

App GlobalScene::getApp() {
    return mApp;
}

Battery GlobalScene::getBattery() {
    return mBattery;
}

long GlobalScene::getBrightness() {
    return mBrightness;
}