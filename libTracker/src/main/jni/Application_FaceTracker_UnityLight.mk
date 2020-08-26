APP_STL := gnustl_static
APP_OPTIM := release #debug
#APP_ABI := arm64-v8a
APP_ABI := armeabi-v7a arm64-v8a x86 x86_64
APP_PLATFORM := android-19
NDK_TOOLCHAIN_VERSION := 4.9
APP_CFLAGS := -O3 -DNDEBUG -funsafe-math-optimizations -Wall
APP_CFLAGS += -D_EIGEN
APP_CPPFLAGS := -frtti -fexceptions -Wall -std=c++11 -fvisibility=hidden -DPLATFORM_ID=1073
APP_CXXFLAGS := -D__STDC_CONSTANT_MACROS
