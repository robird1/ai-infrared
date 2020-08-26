LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := opencv-core-a
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/libopencv_core.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := opencv-calib3d-a
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/libopencv_calib3d.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := opencv-hal-a
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/libopencv_hal.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := opencv-highgui-a
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/libopencv_highgui.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := opencv-imgcodecs-a
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/libopencv_imgcodecs.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := tiff-a
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/liblibtiff.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libpng-a
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/liblibpng.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := jpeg-a
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/liblibjpeg.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := IlmImf-a
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/libIlmImf.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := jasper-a
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/liblibjasper.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := webp-a
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/liblibwebp.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := opencv-imgproc-a
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/libopencv_imgproc.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := opencv-video-a
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/libopencv_video.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := tbb-a
LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/libtbb.a
include $(PREBUILT_STATIC_LIBRARY)

#include $(CLEAR_VARS)
#LOCAL_MODULE := opencv-objdetect-a
#LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/libopencv_objdetect.a
#include $(PREBUILT_STATIC_LIBRARY)

#include $(CLEAR_VARS)
#LOCAL_MODULE := acf-a
#LOCAL_SRC_FILES := libs/$(TARGET_ARCH_ABI)/libacf_lib.a
#include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := auth-so
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../jniUtils/3rdparty/auth/v2.0.1/libs/$(TARGET_ARCH_ABI)/libULSeeAuth.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := n-so
LOCAL_SRC_FILES := ../../../../jniUtils/3rdparty/n/20190704/lib/$(TARGET_ARCH_ABI)/libn.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
OPENCV_LIB_TYPE:=STATIC
OPENCV_CAMERA_MODULES:=off
#include d:/AndroidDev/android-OpenCV-3.0.0-sdk/sdk/native/jni/OpenCV.mk
include $(LOCAL_PATH)/../../../../jniUtils/3rdparty/opencv/v3.0.0/OpenCV-android-sdk/sdk/native/jni/OpenCV.mk
LOCAL_SRC_FILES  := \
	TVSGPULibraryAndroid/Objects/GPUFramebuffer.cpp \
	TVSGPULibraryAndroid/Objects/GPUVertexData.cpp \
	TVSGPULibraryAndroid/Objects/AndGraphicBuffer.cpp \
	TVSGPULibraryAndroid/Objects/GPUTexture.cpp \
	TVSGPULibraryAndroid/Programs/GPUWarp.cpp \
	TVSGPULibraryAndroid/Utils/GBMatWrapping.cpp \
	TVSGPULibraryAndroid/Utils/ShaderCompilation.cpp \
	TVSGPULibraryAndroid/Utils/utilities.cpp \
	tvs/utils/geometry.cpp \
	tvs/utils/io.cpp \
	tvs/utils/linear_model.cpp \
	tvs/utils/thread_pool.cpp \
	tvs/utils/android_assets.cpp \
	tvs/face_tracker.cpp \
	tvs/face_pose_predictor.cpp \
	tvs/linear_shape_regressor.cpp \
	tvs/linear_shape_classifier.cpp \
	tvs/optical_flow_tracker_efficient.cpp \
	tvs/Feature.cpp	\
	tvs/FeatureComp.cpp \
	tvs/ColorConverter.cpp \
	tvs/ColorConverterBGRA.cpp \
	${LOCAL_PATH}/../../../../jniUtils/detector/v1.0.3/src/DetectMgr.cpp \
	${LOCAL_PATH}/../../../../jniUtils/detector/v1.0.3/src/detector.cpp \
	${LOCAL_PATH}/../../../../jniUtils/detector/v1.0.3/src/util.cpp \
	${LOCAL_PATH}/../../../../jniUtils/detector/v1.0.3/src/stich.cpp \
	${LOCAL_PATH}/../../../../jniUtils/ModelLoader.cpp \
	${LOCAL_PATH}/../../../../jniUtils/jniWrapper/JniUtils.cpp \
	${LOCAL_PATH}/../../../../jniUtils/jniWrapper/JniString.cpp \
	${LOCAL_PATH}/../../../../jniUtils/jniWrapper/JniBitmap.cpp \
	${LOCAL_PATH}/../../../../jniUtils/jniWrapper/JniFloatArray2D.cpp \
	${LOCAL_PATH}/../../../../jniUtils/jniWrapper/JniFloatArray.cpp \
	${LOCAL_PATH}/../../../../jniUtils/ProcessUtils.cpp \
    ${LOCAL_PATH}/../../../../jniUtils/blurEvaluation/blur_evaluation.cpp \
    TVSMultiTrackerEngine.cpp \
    uls-dense-points/dense_points.cpp \
    tvstrackerlib.cpp

    #facelib unused
    #facelib/entropy_decoder/entropy_decoder_kernel_2.cpp \
    #facelib/base64/base64_kernel_1.cpp \
    #facelib/threads/threads_kernel_shared.cpp \
    #facelib/threads/threads_kernel_1.cpp \
    #facelib/threads/threads_kernel_2.cpp \

    #single tracker unused
    #TVSTrackerEngine.cpp \

#this folder contains eigen and GLM
LOCAL_C_INCLUDES += /Users/jesus/Documents/TVS/AndroidDevelopment/android_includes/ \
	${LOCAL_PATH}/TVSGPULibraryAndroid/ \
	$(LOCAL_PATH)/../../../../jniUtils/3rdparty/auth/v2.0.1/include \
	${LOCAL_PATH}/../../../../jniUtils/3rdparty/n/20190704/include/ \
	${LOCAL_PATH}/../../../../jniUtils/detector/v1.0.3/include \
	${LOCAL_PATH}/../../../../jniUtils \

LOCAL_C_FLAGS  += -ffunction-sections -fdata-sections -fvisibility=hidden
#-mfloat-abi=hard
#LOCAL_CPPFLAGS += -ffunction-sections -fdata-sections -fvisibility=hidden -DUSE_SHARE_MEMORY
LOCAL_CPPFLAGS += -ffunction-sections -fdata-sections -fvisibility=hidden -fpermissive -DUSE_SHARE_MEMORY -DULS_USE_NEON
#-mfloat-abi=hard
#LOCAL_CFLAGS := -D__cpusplus -g -mfloat-abi=softfp -mfpu=neon -march=armv7-a -mtune=cortex-a8
LOCAL_LDLIBS    += -llog -landroid -lm -lEGL -lGLESv2 -lz -ljnigraphics
LOCAL_LDFLAGS 	:= -Wl,--gc-sections  -Wl,--discard-all -fopenmp
LOCAL_SHARED_LIBRARIES +=   auth-so \
                            n-so

LOCAL_MODULE    := ulsTracker_native
LOCAL_STATIC_LIBRARIES := \
        opencv-video-a \
        opencv-calib3d-a \
        opencv-highgui-a \
        opencv-imgproc-a \
        opencv-core-a \
        opencv-hal-a \
        opencv-imgcodecs-a \
        tiff-a \
        libpng-a \
        jpeg-a \
        IlmImf-a \
        jasper-a \
        webp-a \
        tbb-a

        #acf-a \
        #opencv-objdetect-a \

LOCAL_CFLAGS += -DHAVE_NEON=1
			# use NEON
#LOCAL_CFLAGS += -D__ARM_NEON
LOCAL_ARM_MODE := arm
LOCAL_ARM_NEON := true

#LOCAL_ARM_MODE := arm
#APP_UNIFIED_HEADERS := true
# LOCAL_ARM_NEON := true

# Added to suppress error "format not a string literal and no format arguments".
LOCAL_DISABLE_FORMAT_STRING_CHECKS := true

include $(BUILD_SHARED_LIBRARY)
