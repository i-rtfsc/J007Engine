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

#ifndef _J007ENGINE_H
#define _J007ENGINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <list>

#include <hardware/hardware.h>
#include <hidl/HidlTransportSupport.h>
#include <hidl/MQDescriptor.h>
#include <cutils/properties.h>
#include <com/journeyOS/J007engine/hidl/1.0/IJ007Engine.h>
#include <com/journeyOS/J007engine/hidl/1.0/types.h>

#include "policy/policy_agent.h"

using ::com::journeyOS::J007engine::hidl::V1_0::IJ007Engine;
using ::com::journeyOS::J007engine::hidl::V1_0::IJ007EngineCallback;
using ::com::journeyOS::J007engine::hidl::V1_0::Status;
using ::com::journeyOS::J007engine::hidl::V1_0::TCode;
using ::com::journeyOS::J007engine::hidl::V1_0::J007EngineResponse;
using ::android::hardware::hidl_death_recipient;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::interfacesEqual;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hidl::base::V1_0::IBase;
using ::android::sp;
using ::android::wp;
using namespace std;


class J007Engine : public IJ007Engine, public hidl_death_recipient {
public:
    J007Engine();

    ~J007Engine();

    static J007Engine *getInstance();

    void addAgents(string tag, PolicyAgent* agent);

    Return<void> registerCallback(const sp <IJ007EngineCallback> &callback) override;

    Return<void> unregisterCallback(const sp <IJ007EngineCallback> &callback) override;

    Return<bool>
    notifySceneChanged(const int32_t factors, const hidl_string &status, const hidl_string &packageName) override;

    Return<bool> setConfig(const TCode code, const hidl_string &val) override;

    Return<void> getConfig(const TCode code, IJ007Engine::getConfig_cb _hidl_cb) override;

    Return<void> read(const hidl_string &file, IJ007Engine::read_cb _hidl_cb) override;

    Return<bool> write(const hidl_string &file, const hidl_string &val) override;

    Return<void> readProperty(const hidl_string &key, const hidl_string &defaultVaule,
                              IJ007Engine::readProperty_cb _hidl_cb) override;

    Return<bool> writeProperty(const hidl_string &key, const hidl_string &val) override;

    virtual void serviceDied(uint64_t cookie, const wp <IBase> & /* who */) override;

    Return<void>
    getPackageName(const int32_t pid, IJ007Engine::getPackageName_cb _hidl_cb) override;

private:
    void initAgent();

    bool notifyCpuAgentAppSwitch();

    bool unregisterCallbackInternal(const sp <IBase> &cb);

    void onResponse(TCode code, string messages);

    static J007Engine *sInstance;
    //call back lists
    vector <sp<IJ007EngineCallback>> mCallbacks;
    recursive_mutex mCallbacksLock;

    string mConfigs = "";

    map<string, PolicyAgent*> mAgentMap;
};


#endif //_J007ENGINE_H
