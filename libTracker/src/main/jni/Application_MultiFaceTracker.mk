APP_STL := gnustl_static
APP_OPTIM := release #debug
APP_ABI := arm64-v8a armeabi-v7a
#APP_ABI :=
#arm64-v8a x86 x86_64
APP_PLATFORM := android-19
#NDK_TOOLCHAIN_VERSION := 4.9
#NDK_TOOLCHAIN_VERSION=clang3.2
APP_CFLAGS := -Ofast -DNDEBUG -funsafe-math-optimizations -Wall
APP_CFLAGS += -D_EIGEN
APP_CPPFLAGS := -frtti -fexceptions -Wall -std=c++11 -fvisibility=hidden -DPLATFORM_ID=561 #-DSHOWLOG
APP_CXXFLAGS := -D__STDC_CONSTANT_MACROS
#APP_CXXFLAGS += -DNO_AUTH
