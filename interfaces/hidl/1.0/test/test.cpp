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

using ::android::sp;
using ::android::hardware::Return;
using ::android::hardware::Void;
// Generated HIDL files
using ::com::journeyOS::J007engine::hidl::V1_0::IJ007Engine;
using ::com::journeyOS::J007engine::hidl::V1_0::Status;
using ::com::journeyOS::J007engine::hidl::V1_0::TCode;
using ::com::journeyOS::J007engine::hidl::V1_0::J007EngineResponse;

int main() {
    sp<IJ007Engine> service = IJ007Engine::getService();
    if (service == nullptr) {
        printf("Failed to get J007Engine service\n");
        return -1;
    }

    //service->getConfig(TCode::GET_XXX);

    return 0;
}
