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


#ifndef _CPU_POLICY_AGENT_H
#define _CPU_POLICY_AGENT_H

#define CPU_SET_APP_DEFAULT             "default"
#define CPU_SET_APP_ALBUM               "album"
#define CPU_SET_APP_GAME                "game"
#define CPU_SET_APP_BENCHMARK            "benchmark"
#define CPU_SET_APP_VIDEO               "video"
#define CPU_SET_APP_IM                  "im"
#define CPU_SET_APP_NEWS                "news"
#define CPU_SET_APP_LAUNCHER            "launcher"

#include <string>
#include <map>

#include "policy_agent.h"
#include "../global_scene.h"

using namespace std;

class CpuPolicyAgent : public PolicyAgent {
public:
    CpuPolicyAgent();

    virtual ~CpuPolicyAgent();

    bool onAppSwitch(App app, string status, string packageName) override;

protected:
    bool loadConfig() override;

private:
    void initMap();

    map <string, map<string, string>> mCpuConfig;
    map <string, string> mAppType;
};


#endif //_CPU_POLICY_AGENT_H
