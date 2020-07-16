# This is the Android makefile for libyuv for both platform and NDK.
LOCAL_PATH:= $(call my-dir)

$(warning ../include/curl/lib/$(TARGET_ARCH_ABI)/libcurl.a)
include $(CLEAR_VARS)
LOCAL_MODULE := CURL
LOCAL_SRC_FILES := ../include/curl/lib/$(TARGET_ARCH_ABI)/libcurl.a
TARGET_PRELINK_MODULES := false
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)
CFLAGS=-D__ANDROID_API__=$19
LOCAL_CPP_EXTENSION := .cpp
LOCAL_CPPFLAGS += -std=c++11 -fsigned-char -DSHOWLOG
LOCAL_SRC_FILES := \
    ../src/AUZMgr.cpp        \
    ../src/ServerMgr.cpp   \
    ../src/LicenseMgr.cpp      \
  
$(warning 123$(LOCAL_PATH))

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/_include
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../_include \
		$(LOCAL_PATH)/../include \
		$(LOCAL_PATH)/../include/curl/include

LOCAL_MODULE := ULSeeAuth
LOCAL_MODULE_TAGS := optional
#LOCAL_STATIC_LIBRARIES += CURL
#LOCAL_LDLIBS := $(LOCAL_PATH)/include/curl/lib/arm64-v8a/libcurl.a
#include $(BUILD_SHARED_LIBRARY)
include $(BUILD_STATIC_LIBRARY)
