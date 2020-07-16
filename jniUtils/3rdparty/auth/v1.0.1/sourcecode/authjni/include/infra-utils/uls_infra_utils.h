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

typedef void*               ULSHandler;

typedef enum {
   ULS_NO_AUTH = 0,
   ULS_AUTH_ERROR = 1,
   ULS_AUTH_SUCCESS = 2
} ULSAuthRetEnum;

typedef struct {
	float x;
	float y;
	float width;
	float height;
}uls_rect;

typedef struct {
	float x;
	float y;
}uls_point;

typedef struct {
    unsigned char* 	buffer;
    int 			size;
}uls_img;

//image format, rotation
typedef enum _ImageFormatEnum {
    ULS_IMG_GRAY                = 0,
    ULS_IMG_YCbCr420P           = 1,    /*Y, Cb and Cr are planar*/
    ULS_IMG_YCrCb420P_YV12      = 2,    /*Y, Cr and Cb are planar, YV12*/
    ULS_IMG_YCbCr411P           = 3,
    ULS_IMG_YCbCr422P           = 4,
    ULS_IMG_YCbCr444P           = 5,
    ULS_IMG_YCbCr422I           = 6,    /*Y, Cb and Cr are interleaved in the order of Cb Y Cr Y*/
    ULS_IMG_YCbYCr422I          = 7,    /*Y, Cb and Cr are interleaved in the order of Y Cb Y Cr*/
    ULS_IMG_YCbCr420SP          = 8,    /*Y is planar, Cb and Cr is interleaved, NV12*/
    ULS_IMG_YCrCb420SP          = 9,    /*Y is planar, Cr and Cb is interleaved, NV21*/
    ULS_IMG_BGR888              = 10,
    ULS_IMG_BGRA8888            = 11,
    ULS_IMG_RGBA8888            = 12,
    ULS_IMG_ABGR8888            = 13,
    ULS_IMG_ARGB8888            = 14
} ULSImageFormatEnum;

typedef enum _ImageROTATE {
    ULS_IMG_ORIENTATION_ROTATE_0   = 0,
    ULS_IMG_ORIENTATION_ROTATE_90  = 1,
    ULS_IMG_ORIENTATION_ROTATE_180 = 2,
    ULS_IMG_ORIENTATION_ROTATE_270 = 3
} ULSImageRotateEnum;

#ifdef __cplusplus
}
#endif

#endif //_ULS_INFRA_UTILS_H_
