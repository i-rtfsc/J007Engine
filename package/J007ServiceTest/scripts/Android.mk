LOCAL_PATH:= $(call my-dir)
PARENT_DIR_PATH := $(wildcard ..)

#################################gen framework.jar#################################
include $(CLEAR_VARS)
LOCAL_MODULE := J007ServiceFrameworkStudioTest
LOCAL_MODULE_CLASS := FAKE
LOCAL_MODULE_SUFFIX := -timestamp
j007_engine_system_deps := $(call java-lib-deps,framework)
j007_engine_system_libs_path := $(abspath $(PARENT_DIR_PATH))/system_libs

include $(BUILD_SYSTEM)/base_rules.mk

$(LOCAL_BUILT_MODULE): $(j007_engine_system_deps)
	$(hide) mkdir -p $(j007_engine_system_libs_path)
	$(hide) rm -rf $(j007_engine_system_libs_path)/framework.jar
	$(hide) cp $(j007_engine_system_deps) $(j007_engine_system_libs_path)/framework.jar
	$(hide) echo "Fake: $@"
	$(hide) mkdir -p $(dir $@)
	$(hide) touch $@
#################################gen framework.jar#################################
