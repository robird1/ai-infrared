LOCAL_PATH:= $(call my-dir)

$(warning ../include/curl/lib/$(TARGET_ARCH_ABI)/libcurl.a)
include $(CLEAR_VARS)
LOCAL_MODULE := CURL
LOCAL_SRC_FILES := ../include/curl/lib/$(TARGET_ARCH_ABI)/libcurl.a
TARGET_PRELINK_MODULES := false
include $(PREBUILT_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_CPP_EXTENSION := .cpp
LOCAL_CPPFLAGS += -std=c++11 -fsigned-char -DSHOWLOG -DUSE_CURL

BLACKLIST_SRC_FILES := \
  /temp/test.cpp

FACEAPI_SRC_FILES := \
  $(wildcard $(LOCAL_PATH)/../src/*.cpp)\
  $(wildcard $(LOCAL_PATH)/../src/data/*.cpp) \
  $(wildcard $(LOCAL_PATH)/../src/utils/*.cpp)

  $(info 0:$(FACEAPI_SRC_FILES))

LOCAL_SRC_FILES := \
  $(filter-out $(BLACKLIST_SRC_FILES),$(subst $(LOCAL_PATH)/,,$(FACEAPI_SRC_FILES)))


LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/_include
LOCAL_C_INCLUDES :=$(LOCAL_PATH)/../_include \
        $(LOCAL_PATH)/../_include/uls_json \
		$(LOCAL_PATH)/../include \
		$(LOCAL_PATH)/../src \
		$(LOCAL_PATH)/../include/curl/include

LOCAL_MODULE := ULSeeAuth
LOCAL_MODULE_TAGS := optional

#动态库
LOCAL_LDLIBS := $(LOCAL_PATH)/../include/curl/lib/$(TARGET_ARCH_ABI)/libcurl.a -lz -llog
include $(BUILD_SHARED_LIBRARY)

#静态库
#LOCAL_STATIC_LIBRARIES += CURL
#include $(BUILD_STATIC_LIBRARY)
