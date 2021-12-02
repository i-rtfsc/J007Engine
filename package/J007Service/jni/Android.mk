LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# include $(LOCAL_PATH)/../../../version.mk
# B_VERSION_CODE := $(build_version_code)
# B_VERSION_NAME := "$(build_version_name)"
# LOCAL_CFLAGS += -DB_VERSION_CODE
# LOCAL_CFLAGS += -DB_VERSION_NAME

LOCAL_MODULE := J007_engine_jni

LOCAL_SRC_FILES := \
    $(subst $(LOCAL_PATH)/,,$(wildcard $(LOCAL_PATH)/*.cpp))

LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog -landroid

LOCAL_CFLAGS += -Wall -Werror -Wno-unused-parameter -Wno-switch -Wno-unused-function -Wno-unused-variable

include $(BUILD_SHARED_LIBRARY)