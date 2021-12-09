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


#include "cpu_policy_agent.h"
#include "../log.h"
#include "../utils.h"
#include "../json/json_object.h"
#include "../factors.h"

#define LOG_TAG        "J007Engine-CpuPolicyAgent"


CpuPolicyAgent::CpuPolicyAgent() {
    loadConfig();
}

CpuPolicyAgent::~CpuPolicyAgent() {
}

bool CpuPolicyAgent::onAppSwitch(int app, string status, string packageName) {
    LOGI("app switch, app = %d , status = %s , packageName = %s\n", app, status.c_str(),
         packageName.c_str());

    map <string, string> configs = mCpuConfig[convertApp(app)];
    for (auto &&config : configs) {
        LOGD("cpu = %s , value = %s\n", config.first.c_str(), config.second.c_str());
        //TODO
    }

    return true;
}

bool CpuPolicyAgent::loadConfig() {
    string configs = Utils::readFile(CPU_POLICY_AGENT_FILE);
    JsonObject oJson(configs);
    LOGD("cpuset size = %d ", oJson["cpuset"].GetArraySize());
    for (int i = 0; i < oJson["cpuset"].GetArraySize(); ++i) {
        string name;
        map <string, string> config;

        oJson["cpuset"][i].Get("name", name);
        LOGD("name = %s, configs = %d ", name.c_str(), oJson["cpuset"][i]["config"].GetArraySize());
        for (int j = 0; j < oJson["cpuset"][i]["config"].GetArraySize(); ++j) {
            string cpu = oJson["cpuset"][i]["config"][j]("cpu");
            string value = oJson["cpuset"][i]["config"][j]("value");
            //LOGD("cpu = %s , value = %s\n", cpu.c_str(), value.c_str());
            config.insert({cpu, value});
        }
        mCpuConfig.insert({name, config});
    }

    return true;
}


string CpuPolicyAgent::convertApp(int app) {
    string appStr = CPU_SET_DEFAULT_APP;
    switch (app) {
        case SCENE_FACTOR_APP_DEFAULT:
            appStr = CPU_SET_DEFAULT_APP;
            break;
        case SCENE_FACTOR_APP_ALBUM:
            //TODO
            appStr = CPU_SET_SPECIAL_APP;
            break;
        case SCENE_FACTOR_APP_BROWSER:
            //TODO
            appStr = CPU_SET_DEFAULT_APP;
            break;
        case SCENE_FACTOR_APP_GAME:
            appStr = CPU_SET_DEFAULT_GAME;
            break;
        case SCENE_FACTOR_APP_IM:
            //TODO
            appStr = CPU_SET_TOP_APP;
            break;
        case SCENE_FACTOR_APP_MUSIC:
            //TODO
            appStr = CPU_SET_DEFAULT_APP;
            break;
        case SCENE_FACTOR_APP_NEWS:
            //TODO
            appStr = CPU_SET_SPECIAL_APP;
            break;
        case SCENE_FACTOR_APP_READER:
            //TODO
            appStr = CPU_SET_SPECIAL_APP;
            break;
        case SCENE_FACTOR_APP_VIDEO:
            //TODO
            appStr = CPU_SET_GAME_RECORDING;
            break;
    }
    return appStr;
}