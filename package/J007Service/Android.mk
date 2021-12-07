LOCAL_PATH:= $(call my-dir)

#################################add prebuilt lib#################################
include $(CLEAR_VARS)
LOCAL_PREBUILT_STATIC_JAVA_LIBRARIES := \
    gson:libs/gson-2.8.6.jar
include $(BUILD_MULTI_PREBUILT)
#################################add prebuilt lib#################################

#################################add build info to BuildConfig#################################
# include $(LOCAL_PATH)/../../version.mk
# LOCAL_BUILDCONFIG_CLASS := src/com/journeyOS/J007engine/BuildConfig.java
# BC_OUT_DIR := $(LOCAL_PATH)
# BC_APPLICATION_ID := "com.journeyOS.J007engine"
# BC_VERSION_CODE := $(build_version_code)
# BC_VERSION_NAME := "$(build_version_name)"
# include $(LOCAL_PATH)/buildconfig.mk
#################################add build info to BuildConfig#################################

include $(CLEAR_VARS)
LOCAL_RESOURCE_DIR := \
    $(LOCAL_PATH)/res

LOCAL_SRC_FILES := \
		$(call all-java-files-under, src)

LOCAL_AIDL_INCLUDES := \
        aidl/com/journeyOS/J007engine/IJ007EngineInterface.aidl \
        aidl/com/journeyOS/J007engine/IJ007EngineCallBack.aidl

LOCAL_STATIC_ANDROID_LIBRARIES := \
    androidx.appcompat_appcompat

LOCAL_STATIC_JAVA_LIBRARIES := \
    gson \
    com.journeyOS.J007engine.hidl-V1.0-java

LOCAL_PACKAGE_NAME := J007Service
LOCAL_CERTIFICATE := platform
LOCAL_MODULE_TAGS := optional
LOCAL_JNI_SHARED_LIBRARIES := J007_engine_jni
LOCAL_PROGUARD_FLAG_FILES := proguard.flags
LOCAL_USE_AAPT2 := true
LOCAL_PRIVILEGED_MODULE := true
LOCAL_PRIVATE_PLATFORM_APIS := true
LOCAL_REQUIRED_MODULES := privapp_whitelist_com.journeyOS.J007engine.xml
include $(BUILD_PACKAGE)

#################################priv-app permissions#################################
include $(CLEAR_VARS)
LOCAL_MODULE := privapp_whitelist_com.journeyOS.J007engine.xml
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_ETC)/permissions
LOCAL_SRC_FILES := $(LOCAL_MODULE)
include $(BUILD_PREBUILT)
#################################priv-app permissions#################################

#################################gen framework.jar#################################
include $(CLEAR_VARS)
LOCAL_MODULE := J007ServiceFrameworkStudio
LOCAL_MODULE_CLASS := FAKE
LOCAL_MODULE_SUFFIX := -timestamp
j007_engine_system_deps := $(call java-lib-deps,framework)
j007_engine_system_libs_path := $(abspath $(LOCAL_PATH))/system_libs

include $(BUILD_SYSTEM)/base_rules.mk

$(LOCAL_BUILT_MODULE): $(j007_engine_system_deps)
	$(hide) mkdir -p $(j007_engine_system_libs_path)
	$(hide) rm -rf $(j007_engine_system_libs_path)/framework.jar
	$(hide) cp $(j007_engine_system_deps) $(j007_engine_system_libs_path)/framework.jar
	$(hide) echo "Fake: $@"
	$(hide) mkdir -p $(dir $@)
	$(hide) touch $@
#################################gen framework.jar#################################

#################################gen J007engine.jar#################################
include $(CLEAR_VARS)
LOCAL_MODULE := J007ServiceHidlStudio
LOCAL_MODULE_CLASS := FAKE
LOCAL_MODULE_SUFFIX := -timestamp
j007_engine_hidl_deps := $(call java-lib-deps,com.journeyOS.J007engine.hidl-V1.0-java)
j007_engine_hidl_libs_path := $(abspath $(LOCAL_PATH))/system_libs

include $(BUILD_SYSTEM)/base_rules.mk

$(LOCAL_BUILT_MODULE): $(j007_engine_hidl_deps)
	$(hide) mkdir -p $(j007_engine_hidl_libs_path)
	$(hide) rm -rf $(j007_engine_hidl_libs_path)/J007engine.jar
	$(hide) cp $(j007_engine_hidl_deps) $(j007_engine_hidl_libs_path)/J007engine.jar
	$(hide) echo "Fake: $@"
	$(hide) mkdir -p $(dir $@)
	$(hide) touch $@
#################################gen J007engine.jar#################################

include $(call all-makefiles-under,$(LOCAL_PATH))