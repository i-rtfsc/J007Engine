LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_RESOURCE_DIR := \
    $(LOCAL_PATH)/res

LOCAL_SRC_FILES := \
	$(call all-java-files-under, src)

LOCAL_STATIC_ANDROID_LIBRARIES := \
    androidx.appcompat_appcompat

LOCAL_JAVA_LIBRARIES := framework

LOCAL_PACKAGE_NAME := J007ServiceTest
LOCAL_CERTIFICATE := platform
LOCAL_USE_AAPT2 := true
LOCAL_PRIVATE_PLATFORM_APIS := true
#LOCAL_MODULE_TAGS := tests

include $(BUILD_PACKAGE)

#################################gen framework.jar#################################
include $(CLEAR_VARS)
LOCAL_MODULE := J007ServiceFrameworkStudioTest
LOCAL_MODULE_CLASS := FAKE
LOCAL_MODULE_SUFFIX := -timestamp
j007_engine_test_system_deps := $(call java-lib-deps,framework)
j007_engine_test_system_libs_path := $(abspath $(LOCAL_PATH))/system_libs

include $(BUILD_SYSTEM)/base_rules.mk

$(LOCAL_BUILT_MODULE): $(j007_engine_test_system_deps)
	$(hide) mkdir -p $(j007_engine_test_system_libs_path)
	$(hide) rm -rf $(j007_engine_test_system_libs_path)/*.jar
	$(hide) cp $(j007_engine_test_system_deps) $(j007_engine_test_system_libs_path)/framework.jar
	$(hide) echo "Fake: $@"
	$(hide) mkdir -p $(dir $@)
	$(hide) touch $@
#################################gen framework.jar#################################