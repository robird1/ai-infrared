/******************************************************************************
 *     Copyright (c) 2017-2018 ULSee Corporation.
 *     All Rights Reserved.
 ******************************************************************************/
#ifndef __LOG_H__
#define __LOG_H__

#ifdef __cplusplus
extern "C" {
#endif
#ifdef _DEBUG_
#undef  LOG_TAG
#define LOG_TAG "ULS"
#ifdef __ANDROID__
//#include <utils/Log.h>
#include <android/log.h>
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__);
#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__);
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__);
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__);
#else  /* !_ANDROID_ */
#include <stdio.h>
#define LOGI(...) \
     do { \
         printf(__VA_ARGS__); } \
     while (0)

#define LOGW(...) \
     do { \
         printf(__VA_ARGS__); } \
     while (0)

#define LOGE(...) \
     do { \
         printf(__VA_ARGS__); } \
     while (0)

#define LOGD(...) \
     do { \
         printf(__VA_ARGS__); } \
     while (0)
#endif /* !_ANDROID_ */
#else  /* !_DEBUG */
#define LOGI(...)
#define LOGW(...)
#define LOGE(...)
#define LOGD(...)
#endif

#ifdef __cplusplus
}
#endif

#endif /* __LOG_H__ */

/* EOF */

