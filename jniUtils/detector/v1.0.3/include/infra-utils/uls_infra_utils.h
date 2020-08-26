/******************************************************************************
 * Copyright (c) 2017-2018 ULSee Corporation.
 * All Rights Reserved.
 * *****************************************************************************/

#ifndef _ULSEE_SDK_INCLUDE_INFRA_UTILS_H_
#define _ULSEE_SDK_INCLUDE_INFRA_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

//set visibility of APIs
#if defined _SKIP_DLL_DEFINITIONS_
#define DLL_PUBLIC
#define DLL_LOCAL
#else
#if defined _WIN32 || defined __CYGWIN__
#ifdef tvsTracker_EXPORTS
#ifdef __GNUC__
#define DLL_PUBLIC __attribute__ ((dllexport))
#else
#define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
#endif
#else
#ifdef __GNUC__
#define DLL_PUBLIC __attribute__ ((dllimport))
#else
#define DLL_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
#endif
#endif
#define DLL_LOCAL
#else
#if __GNUC__ >= 4
#define DLL_PUBLIC __attribute__ ((visibility ("default")))
#endif
#endif
#endif

//return values from APIs
#ifndef ULS_API
#define ULS_API
#endif

#ifndef ULS_API_RET_SUCCESS
#define ULS_API_RET_SUCCESS         (0)
#endif

#ifndef ULS_API_RET_NOT_INIT
#define ULS_API_RET_NOT_INIT       (-1)
#endif

#ifndef ULS_API_RET_PARAM_ERR
#define ULS_API_RET_PARAM_ERR      (-2)
#endif

#ifndef ULS_API_RET_DECODE_ERR
#define ULS_API_RET_DECODE_ERR     (-3)
#endif

#ifndef ULS_API_RET_INTERNAL_ERR
#define ULS_API_RET_INTERNAL_ERR   (-4)
#endif

#ifndef ULS_API_RET_INVALIDKEY
#define ULS_API_RET_INVALIDKEY     (-5)
#endif

#ifndef ULS_API_BADRESOURCE
#define ULS_API_BADRESOURCE        (-6)
#endif

#ifndef ULS_API_TIMEOUT
#define ULS_API_TIMEOUT            (-7)
#endif

#ifndef ULS_API_NOMEM
#define ULS_API_NOMEM              (-8)
#endif

typedef void *uls_handler;

typedef enum {
    ULS_NO_AUTH = 0,
    ULS_AUTH_ERROR = 1,
    ULS_AUTH_SUCCESS = 2
} uls_auth_ret_enum;

typedef struct {
    float x;
    float y;
    float width;
    float height;
} uls_rect;

typedef struct {
    float x;
    float y;
} uls_point;

typedef enum {
    ULS_IMG_ORIENTATION_ROTATE_0 = 0,
    ULS_IMG_ORIENTATION_ROTATE_90 = 1,
    ULS_IMG_ORIENTATION_ROTATE_180 = 2,
    ULS_IMG_ORIENTATION_ROTATE_270 = 3
} uls_image_rotate_enum;

#ifdef __cplusplus
}
#endif

#endif //_ULS_INFRA_UTILS_H_
