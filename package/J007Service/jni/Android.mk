LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := J007_engine_jni

LOCAL_SRC_FILES := \
    $(wildcard  $(LOCAL_PATH)/*.cpp)
    #$(subst $(LOCAL_PATH)/,,$(wildcard $(LOCAL_PATH)/*.cpp))

#LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog -landroid

# LOCAL_C_INCLUDES += \
#     $(TOP)/frameworks/av/include/media/ \
#     $(TOP)/frameworks/base/core/jni/include/

LOCAL_SHARED_LIBRARIES := \
                libbinder \
                libcutils \
                libutils \
                liblog \
                libandroid \
                libnativehelper \
                com.journeyOS.J007engine.hidl@1.0 \
                libhidlbase

LOCAL_CFLAGS += -Wall -Werror -Wno-unused-parameter -Wno-switch -Wno-unused-function -Wno-unused-variable


include $(BUILD_SHARED_LIBRARY)