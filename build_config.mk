LOCAL_J007_BUILD_CONFIG_OUT := ${TOP}/vendor/journeyOS/proprietary/J007Engine/package/J007Service/build/
LOCAL_J007_GIT_PATH := ${TOP}/vendor/journeyOS/proprietary/J007Engine/
LOCAL_J007_CMD := python $(TOP)/vendor/journeyOS/proprietary/J007Engine/build_script/build_tools.py \
                --out $(LOCAL_J007_BUILD_CONFIG_OUT) \
                --local $(LOCAL_J007_GIT_PATH)

$(warning $(LOCAL_J007_CMD))
_result_:=$(shell $(LOCAL_J007_CMD))
# $(foreach _,$(_result_),$(warning $(_)))