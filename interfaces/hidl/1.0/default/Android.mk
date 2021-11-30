LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE_RELATIVE_PATH  := hw

LOCAL_PROPRIETARY_MODULE    := true

LOCAL_MODULE                := com.journeyOS.J007engine.hidl@1.0-service

LOCAL_INIT_RC               := com.journeyOS.J007engine.hidl@1.0-service.rc

LOCAL_VINTF_FRAGMENTS  := com.journeyOS.J007engine.hidl@1.0-service.xml

LOCAL_SRC_FILES := $(call all-cpp-files-under, src) \
                   $(call all-c-files-under, src)

LOCAL_SHARED_LIBRARIES := \
    liblog \
    libcutils \
    libdl \
    libbase \
    libutils \
    libhardware \
    libui \
    libbinder \
    libhidlbase \
    libhidltransport \
    com.journeyOS.J007engine.hidl@1.0

LOCAL_C_INCLUDES += \
    external/skia/include \
    frameworks/native/libs/nativewindow/include \
    frameworks/native/libs/arect/include \
    frameworks/native/libs/nativebase/include \
    system/libhidl/transport/token/1.0/utils/include \
    system/core/libcutils/include/cutils

include $(BUILD_EXECUTABLE)
