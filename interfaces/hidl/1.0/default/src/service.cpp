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

#include <com/journeyOS/J007engine/hidl/1.0/IJ007Engine.h>
#include <hidl/LegacySupport.h>

#include "log.h"
#include "J007_engine.h"

// libhwbinder:
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

// Generated HIDL files
using ::com::journeyOS::J007engine::hidl::V1_0::IJ007Engine;

using ::android::status_t;
using ::android::OK;
using ::android::sp;

int main() {
    J007Engine *j007engine = J007Engine::getInstance();

    configureRpcThreadpool(1, true);
    const status_t status = j007engine->registerAsService();
    if (status == OK) {
        ALOGI("J007Engine HAL Ready.");
        joinRpcThreadpool();
    }

    delete j007engine;
    ALOGE("Cannot register J007Engine HAL service %d.", status);
    return 0;
}
