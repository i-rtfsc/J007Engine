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

#define CPU_SET_DEFAULT_APP             "default_app"
#define CPU_SET_SPECIAL_APP             "special_app"
#define CPU_SET_TOP_APP                 "top_app"
#define CPU_SET_DEFAULT_GAME            "default_game"
#define CPU_SET_GAME_8                  "default_game_8"
#define CPU_SET_GAME_7                  "default_game_7"
#define CPU_SET_GAME_4                  "default_game_4"
#define CPU_SET_GAME_RECORDING          "game_recording"
#define CPU_SET_BENCHMARK               "benchmark"

#include <string>
#include <map>

#include "policy_agent.h"

using namespace std;


//struct CpuConfig {
//    string name;
//    string description;
//    map <string, string> config;
//    vector <CpuConfig> cc;
//};

class CpuPolicyAgent : public PolicyAgent {
public:
    CpuPolicyAgent();

    virtual ~CpuPolicyAgent();

    bool onAppSwitch(int app, string status, string packageName) override;

protected:
    bool loadConfig() override;
    string convertApp(int app) override;

private:
    map <string, map<string, string>> mCpuConfig;
    //CpuConfig mCpuConfig;
};


#endif //_CPU_POLICY_AGENT_H
