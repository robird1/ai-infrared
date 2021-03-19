cmake_minimum_required(VERSION 3.4.1)

#cryptopp
link_directories("${CMAKE_CURRENT_LIST_DIR}/src/main/cpp/cryptopp/v8.2/lib/${ANDROID_ABI}")
include_directories("${CMAKE_CURRENT_LIST_DIR}/src/main/cpp")

#zstd
include_directories("${CMAKE_CURRENT_LIST_DIR}/src/main/cpp/zstd")
include_directories("${CMAKE_CURRENT_LIST_DIR}/src/main/cpp/zstd/decompress")
include_directories("${CMAKE_CURRENT_LIST_DIR}/src/main/cpp/zstd/compress")
include_directories("${CMAKE_CURRENT_LIST_DIR}/src/main/cpp/zstd/common")
include_directories("${CMAKE_CURRENT_LIST_DIR}/src/main/cpp/zstd/dictBuilder")
include_directories("${CMAKE_CURRENT_LIST_DIR}/src/main/cpp/zstd/deprecated")
include_directories("${CMAKE_CURRENT_LIST_DIR}/src/main/cpp/zstd/legacy")

include_directories("${CMAKE_CURRENT_LIST_DIR}/src/main/cpp/facecode_sh")
include_directories("${CMAKE_CURRENT_LIST_DIR}/src/main/cpp")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -DSHOWLOG")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp -static-openmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11  -fopenmp -static-openmp")
file(GLOB native_srcs
        "src/main/cpp/zstd/decompress/*.c"
        "src/main/cpp/zstd/compress/*.c"
        "src/main/cpp/zstd/common/*.c"
        "src/main/cpp/zstd/dictBuilder/*.c"
        "src/main/cpp/zstd/deprecated/*.c"
        "src/main/cpp/zstd/legacy/*.c"
        "src/main/cpp/facecode_sh/pem/*.cpp"
        "src/main/cpp/facecode_sh/QRCodeGen/*.cpp"
        "src/main/cpp/facecode_sh/FloatCompressor/*.cpp"
        "src/main/cpp/native-lib.cpp"
        "src/main/cpp/facecode_sh/*.cpp"
        )

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        facecode

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
            ${native_srcs} )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        facecode
        cryptopp_shared

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )