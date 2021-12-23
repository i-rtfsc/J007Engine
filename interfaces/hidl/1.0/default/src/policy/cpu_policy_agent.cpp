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
    initMap();
    loadConfig();
}

CpuPolicyAgent::~CpuPolicyAgent() {
}

bool CpuPolicyAgent::onAppSwitch(App app, string status, string packageName) {
    LOGI("app switch, packageName = %s , type = %s , status = %s\n", app.packageName.c_str(), app.type.c_str(),
         status.c_str());

    string cup_set = mAppType[app.type];
    LOGI("cup_set = %s \n", cup_set.c_str());
    map <string, string> configs = mCpuConfig[cup_set];
    for (auto &&config : configs) {
        LOGI("cpu = %s , value = %s\n", config.first.c_str(), config.second.c_str());
        //TODO
        //set cpu from config
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

void CpuPolicyAgent::initMap() {
    mAppType.insert({APP_DEFAULT, CPU_SET_APP_DEFAULT});

    mAppType.insert({APP_ALBUM, CPU_SET_APP_ALBUM});

    mAppType.insert({APP_GAME, CPU_SET_APP_GAME});

    mAppType.insert({APP_BENCHMARK, CPU_SET_APP_BENCHMARK});

    mAppType.insert({APP_VIDEO, CPU_SET_APP_VIDEO});
    mAppType.insert({APP_LIVE, CPU_SET_APP_VIDEO});

    mAppType.insert({APP_IM, CPU_SET_APP_IM});
    mAppType.insert({APP_MUSIC, CPU_SET_APP_IM});

    mAppType.insert({APP_NEWS, CPU_SET_APP_NEWS});
    mAppType.insert({APP_READER, CPU_SET_APP_NEWS});
    mAppType.insert({APP_BROWSER, CPU_SET_APP_NEWS});
    mAppType.insert({APP_WEIBO, CPU_SET_APP_NEWS});
    mAppType.insert({APP_SHOP, CPU_SET_APP_NEWS});

    mAppType.insert({APP_LAUNCHER, CPU_SET_APP_LAUNCHER});
    mAppType.insert({APP_NAVIGATION, CPU_SET_APP_LAUNCHER});
}
