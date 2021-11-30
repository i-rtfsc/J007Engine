LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE_RELATIVE_PATH  := hw

LOCAL_PROPRIETARY_MODULE    := true

LOCAL_MODULE                := com.journeyOS.J007engine.hidl.test

LOCAL_SRC_FILES := \
	test.cpp

LOCAL_SHARED_LIBRARIES := \
    liblog \
    libbase \
    libutils \
    libhardware \
    libbinder \
    libhidlbase \
    libhidltransport \
    com.journeyOS.J007engine.hidl@1.0

include $(BUILD_EXECUTABLE)
