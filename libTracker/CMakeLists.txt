cmake_minimum_required(VERSION 3.4.1)

#key验证
link_directories("${CMAKE_CURRENT_LIST_DIR}/../jniUtils/3rdparty/auth/v2.0.1/libs/${ANDROID_ABI}")
include_directories("${CMAKE_CURRENT_LIST_DIR}/../jniUtils/3rdparty/auth/v2.0.1/include")

#opencv
link_directories("${CMAKE_CURRENT_LIST_DIR}/../jniUtils/3rdparty/opencv/v3.4.0/lib/${ANDROID_ABI}")
include_directories("${CMAKE_CURRENT_LIST_DIR}/../jniUtils/3rdparty/opencv/v3.4.0/include")

#ncnn
include_directories("${CMAKE_CURRENT_LIST_DIR}/../jniUtils/3rdparty/ln/20190704/include")
link_directories("${CMAKE_CURRENT_LIST_DIR}/../jniUtils/3rdparty/ln/20190704/lib/${ANDROID_ABI}")

#共用
include_directories("${CMAKE_CURRENT_LIST_DIR}/../jniUtils/")

#detector
include_directories("${CMAKE_CURRENT_LIST_DIR}/../jniUtils/detector/v1.0.3/include")
#include_directories("${CMAKE_CURRENT_LIST_DIR}/src/main/jni/dlib_unused")
#include_directories("${CMAKE_CURRENT_LIST_DIR}/src/main/jni/AcfFaceDetector")
#include_directories("${CMAKE_CURRENT_LIST_DIR}/src/main/jni/NPDFdt")

include_directories("${CMAKE_CURRENT_LIST_DIR}/src/main/jni/")
include_directories("${CMAKE_CURRENT_LIST_DIR}/src/main/jni/TVSGPULibraryAndroid")

#compile flags should not set -O3， it will influence the tracker effect
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -fvisibility=hidden -fomit-frame-pointer -fstrict-aliasing -ffunction-sections -fdata-sections -ffast-math")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fvisibility=hidden -fvisibility-inlines-hidden -fomit-frame-pointer -fstrict-aliasing -ffunction-sections -fdata-sections -ffast-math")
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -Wl,--gc-sections")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -fopenmp")

#ULS_USE_NEON indicates using float16_t Mat definitions
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DULS_USE_NEON")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_SHARE_MEMORY")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti -fexceptions -Wall -std=c++11 -DPLATFORM_ID=561")

file(GLOB native_srcs
        "${CMAKE_CURRENT_LIST_DIR}/src/main/jni/TVSGPULibraryAndroid/*.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/main/jni/TVSGPULibraryAndroid/Objects/*.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/main/jni/TVSGPULibraryAndroid/Programs/*.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/main/jni/TVSGPULibraryAndroid/Utils/*.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/main/jni/tvs/*.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/main/jni/tvs/utils/*.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/main/jni/uls-dense-points/*.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/main/jni/*.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/../jniUtils/*.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/../jniUtils/jniWrapper/*.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/../jniUtils/detector/v1.0.3/src/*.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/../jniUtils/blurEvaluation/*.cpp"
         )


add_library(
            ulsTracker_native
            SHARED
            ${native_srcs}
            )



find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

set(OpenCV_LIBS
        opencv_objdetect
        opencv_video
        opencv_videoio
        opencv_videostab
        opencv_core
        opencv_dnn
        opencv_calib3d
        opencv_features2d
        opencv_flann
        opencv_highgui
        opencv_imgcodecs
        opencv_imgproc
        opencv_ml
        opencv_photo
        opencv_shape
        opencv_stitching
        opencv_superres
        cpufeatures
        IlmImf
        libjasper
        libjpeg
        libpng
        libtiff
        libwebp
        tbb
        tegra_hal
        )

target_link_libraries(
                    ulsTracker_native
                    ln
                    log
                    android
                    m
                    EGL
                    GLESv2
                    z
                    jnigraphics
                    opencv_java3
                    #{OpenCV_LIBS}
                    ULSeeAuth
                    )