cmake_minimum_required(VERSION 3.4.1)

#key验证
link_directories("${CMAKE_CURRENT_LIST_DIR}/../jniUtils/3rdparty/auth/v2.0.1/libs/${ANDROID_ABI}")
include_directories("${CMAKE_CURRENT_LIST_DIR}/../jniUtils/3rdparty/auth/v2.0.1/include")

#opencv
link_directories("${CMAKE_CURRENT_LIST_DIR}/../jniUtils/3rdparty/opencv/v3.4.0/lib/${ANDROID_ABI}")
include_directories("${CMAKE_CURRENT_LIST_DIR}/../jniUtils/3rdparty/opencv/v3.4.0/include")

#ncnn
include_directories("${CMAKE_CURRENT_LIST_DIR}/../jniUtils/3rdparty/n/20190704/include")
link_directories("${CMAKE_CURRENT_LIST_DIR}/../jniUtils/3rdparty/n/20190704/lib/${ANDROID_ABI}")

#共用
include_directories("${CMAKE_CURRENT_LIST_DIR}/../jniUtils/")

#detector
include_directories("${CMAKE_CURRENT_LIST_DIR}/../jniUtils/detector/v1.0.3/include")

include_directories("${CMAKE_CURRENT_LIST_DIR}/src/main/cpp")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -DSHOWLOG")

file(GLOB native_srcs
    "src/main/cpp/extractor/*.cpp"
    "src/main/cpp/qrcode/*.cpp"
    "src/main/cpp/*.cpp"
    "../jniUtils/*.cpp"
    "../jniUtils/jniWrapper/*.cpp"
    "../jniUtils/detector/v1.0.3/src/*.cpp"
   )


add_library(
              faceverification
              SHARED
              ${native_srcs})


target_link_libraries(
                       faceverification
                       n
                        z
                        jnigraphics
                        opencv_java3
                        log
                        android
                        ULSeeAuth
                      )