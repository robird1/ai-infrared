/**
   AndGraphicBuffer: Android Graphic Buffer.

   A class to manager Android Graphic Buffers. Graphic Buffers are a
   non-public API used to share data between the CPU and GPU. A bit
   like iOS CoreVideo PixelBuffers, but with much, much less
   functionality and convenience.

   Some bits of this file are taken from the Android sources.

   Copyright 2014 TrueVisionSolution Pty. Ltd.
   All rights reserved
*/
#include "AndGraphicBuffer.h"

#include <dlfcn.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include <string.h>
#include <sstream>

#include <android/log.h>

#include "GPUBasicDefinitions.h"

#ifdef TVS_INTERNAL_DEBUGGING
const char kLogTag[] = "AndGraphicBuffer";
#define printInfo(...) {__android_log_print(ANDROID_LOG_INFO, kLogTag, \
                                            __VA_ARGS__);}
#else
const char kLogTag[] = "UlsGPULibrary";
#define printInfo(...)
#endif
#define printErr(...) {__android_log_print(ANDROID_LOG_ERROR, kLogTag, \
                                           __VA_ARGS__);}

// From the Android ui source code
enum {
  /* buffer is never read in software */
  GRALLOC_USAGE_SW_READ_NEVER         = 0x00000000,
  /* buffer is rarely read in software */
  GRALLOC_USAGE_SW_READ_RARELY        = 0x00000002,
  /* buffer is often read in software */
  GRALLOC_USAGE_SW_READ_OFTEN         = 0x00000003,
  /* mask for the software read values */
  GRALLOC_USAGE_SW_READ_MASK          = 0x0000000F,

  /* buffer is never written in software */
  GRALLOC_USAGE_SW_WRITE_NEVER        = 0x00000000,
  /* buffer is never written in software */
  GRALLOC_USAGE_SW_WRITE_RARELY       = 0x00000020,
  /* buffer is never written in software */
  GRALLOC_USAGE_SW_WRITE_OFTEN        = 0x00000030,
  /* mask for the software write values */
  GRALLOC_USAGE_SW_WRITE_MASK         = 0x000000F0,

  /* buffer will be used as an OpenGL ES texture */
  GRALLOC_USAGE_HW_TEXTURE            = 0x00000100,
  /* buffer will be used as an OpenGL ES render target */
  GRALLOC_USAGE_HW_RENDER             = 0x00000200,
  /* buffer will be used by the 2D hardware blitter */
  GRALLOC_USAGE_HW_2D                 = 0x00000400,
  /* buffer will be used by the HWComposer HAL module */
  GRALLOC_USAGE_HW_COMPOSER           = 0x00000800,
  /* buffer will be used with the framebuffer device */
  GRALLOC_USAGE_HW_FB                 = 0x00001000,
  /* buffer will be used with the HW video encoder */
  GRALLOC_USAGE_HW_VIDEO_ENCODER      = 0x00010000,
  /* mask for the software usage bit-mask */
  GRALLOC_USAGE_HW_MASK               = 0x00011F00,
};

enum {
  HAL_PIXEL_FORMAT_RGBA_8888          = 1,
  HAL_PIXEL_FORMAT_RGBX_8888          = 2,
  HAL_PIXEL_FORMAT_RGB_888            = 3,
  HAL_PIXEL_FORMAT_RGB_565            = 4,
  HAL_PIXEL_FORMAT_BGRA_8888          = 5,
  HAL_PIXEL_FORMAT_RGBA_5551          = 6,
  HAL_PIXEL_FORMAT_RGBA_4444          = 7,
  HAL_PIXEL_FORMAT_A8                 = 8,
};


//this definition taken from window.h internal to android
namespace {
#define ANDROID_NATIVE_MAKE_CONSTANT(a,b,c,d) \
    (((unsigned)(a)<<24)|((unsigned)(b)<<16)|((unsigned)(c)<<8)|(unsigned)(d))

#define ANDROID_NATIVE_WINDOW_MAGIC \
    ANDROID_NATIVE_MAKE_CONSTANT('_','w','n','d')

#define ANDROID_NATIVE_BUFFER_MAGIC \
    ANDROID_NATIVE_MAKE_CONSTANT('_','b','f','r')

typedef struct android_native_base_t
{
  /* a magic value defined by the actual EGL native type */
  int magic;

  /* the sizeof() of the actual EGL native type */
  int version;

  void* reserved[4];

  /* reference-counting interface */
  void (*incRef)(struct android_native_base_t* base);
  void (*decRef)(struct android_native_base_t* base);
} android_native_base_t;

typedef struct ANativeWindowBuffer
{
#ifdef __cplusplus
  ANativeWindowBuffer() {
    common.magic = ANDROID_NATIVE_BUFFER_MAGIC;
    common.version = sizeof(ANativeWindowBuffer);
    memset(common.reserved, 0, sizeof(common.reserved));
  }

  // Implement the methods that sp<ANativeWindowBuffer> expects so that it
  // can be used to automatically refcount ANativeWindowBuffer's.
  void incStrong(const void* id) const {
    common.incRef(const_cast<android_native_base_t*>(&common));
  }
  void decStrong(const void* id) const {
    common.decRef(const_cast<android_native_base_t*>(&common));
  }
#endif

  struct android_native_base_t common;

  int width;
  int height;
  int stride;
  int format;
  int usage;

  void* reserved[2];

  void* handle;

  void* reserved_proc[8];
} ANativeWindowBuffer_t;
}

// size to allocate the Gralloc Buffer
const long kAndGraphicBufferSize = 2048;


//*****************************************************************************/
//*****************************************************************************/
//*****************************************************************************/

/*
 * A class to get the pointers to the internal functions in the ui dll
 */
class AndroidGBFunctions {
 public:
  AndroidGBFunctions() {
    fGBConstructor = nullptr;
    fGBDestructor = nullptr;
    fGBLock = nullptr;
    fGBUnlock = nullptr;
    fGBGetNativeBuffer = nullptr;

    fEGLCreateImageKHR = nullptr;
    fEGLDestroyImageKHR = nullptr;
    fGLEGLImageTargetTexture2DOES = nullptr;
   _init = false;
   _hasEGLImageSupport = false;

//   initGBFunctions();
  }

  typedef void (*pfnGBCtor)(void*, uint32_t w, uint32_t h, uint32_t format,
                            uint32_t usage);
  pfnGBCtor fGBConstructor;

  typedef void (*pfnGBDtor)(void*);
  pfnGBDtor fGBDestructor;

  typedef int (*pfnGBLock)(void*, uint32_t usage, unsigned char **addr);
  pfnGBLock fGBLock;

  typedef int (*pfnGBUnlock)(void*);
  pfnGBUnlock fGBUnlock;

  typedef void* (*pfnGBGetNativeBuffer)(void*);
  pfnGBGetNativeBuffer fGBGetNativeBuffer;

  PFNGLEGLIMAGETARGETTEXTURE2DOESPROC fGLEGLImageTargetTexture2DOES;
  PFNEGLCREATEIMAGEKHRPROC fEGLCreateImageKHR;
  PFNEGLDESTROYIMAGEKHRPROC fEGLDestroyImageKHR;

  bool hasEGLImageSupport() {
    if (_init) return true;

    parseExtensions();
    _init = initEGLFunctions();
    return _init;
  }

 private:
  void parseExtensions() {
    const char * extensions = (const char *) glGetString(GL_EXTENSIONS);
    // printInfo("GL extensions: %s", extensions);

    std::string item;
    std::string str (extensions);
    std::stringstream iss (str);

    while (iss >> item) {
      if (item == "GL_OES_EGL_image") {
        _hasEGLImageSupport = true;
        break;
      }
    }
    if (!_hasEGLImageSupport) {
      printErr("Device doesn't support GL_OES_EGL_image! - This is required");
    }
  }

  bool initEGLFunctions() {
    if (!_hasEGLImageSupport) return false;

    //EGL function pointers. We have to check the extensions because a
    //non-NULL value does not guarantee support.
    fEGLCreateImageKHR = (PFNEGLCREATEIMAGEKHRPROC) eglGetProcAddress(
        "eglCreateImageKHR");
    fEGLDestroyImageKHR = (PFNEGLDESTROYIMAGEKHRPROC) eglGetProcAddress(
        "eglDestroyImageKHR");

    fGLEGLImageTargetTexture2DOES = (PFNGLEGLIMAGETARGETTEXTURE2DOESPROC)
        eglGetProcAddress("glEGLImageTargetTexture2DOES");

    if (!fEGLCreateImageKHR || !fEGLDestroyImageKHR
        || !fGLEGLImageTargetTexture2DOES) {
      printErr("EGL ");
      return false;
    }
    return true;
  }

  void initGBFunctions() {
    void *handle = dlopen("libui.so", RTLD_LAZY);
    if (!handle) {
      printErr("libui.so can't be loaded - NULL handle");
      return;
    }

    //The mangled function names
    fGBConstructor = (pfnGBCtor)dlsym(
        handle, "_ZN7android13GraphicBufferC1Ejjij");
    fGBDestructor = (pfnGBDtor)dlsym(
        handle, "_ZN7android13GraphicBufferD1Ev");
    fGBLock = (pfnGBLock)dlsym(
        handle, "_ZN7android13GraphicBuffer4lockEjPPv");
    fGBUnlock = (pfnGBUnlock)dlsym(
        handle, "_ZN7android13GraphicBuffer6unlockEv");
    fGBGetNativeBuffer = (pfnGBGetNativeBuffer)dlsym(
        handle, "_ZNK7android13GraphicBuffer15getNativeBufferEv");

    if (!fGBConstructor || !fGBDestructor || !fGBLock || !fGBUnlock
        || !fGBGetNativeBuffer) {
      printErr("Failed to lookup some GraphicBuffer functions");
     return;
    }
    return;
  }

  bool _hasEGLImageSupport;
  bool _init;
} sGBFunctions;

//*****************************************************************************/
//*****************************************************************************/
//*****************************************************************************/

namespace tvs {
namespace gpgpu {
AndGraphicBuffer::AndGraphicBuffer() {
  _agbPtr = nullptr;
  _eglImage = nullptr;
}

AndGraphicBuffer:: ~AndGraphicBuffer() {
  _eglImage = nullptr; //There's a comment on Mozilla sources about
                       //random crashes when releasing the EGLImage.
  if (_agbPtr) {
    sGBFunctions.fGBDestructor(_agbPtr);

    free(_agbPtr);
    _agbPtr = nullptr;
  }
}

void AndGraphicBuffer::init(int width, int height, int usageFlag,
                         GLenum imageFormat) {
  _width = width;
  _height = height;
  _usageFlags = usageFlag;
  _imageFormat = imageFormat;

  allocateGB();
}

GLenum AndGraphicBuffer::getImageFormat() {
  return _imageFormat;
}

int AndGraphicBuffer::getWidth() {
  return _width;
}

int AndGraphicBuffer::getHeight() {
  return _height;
}

int AndGraphicBuffer::getBytesPerRow() {
  return _bytesPerRow;
}

bool AndGraphicBuffer::lockBuffer(bool readOnly, unsigned char **ptr) {
  int usage = _usageFlags;
  if (readOnly) usage &= ~(kGBUsageSWWriteOften | kGBUsageSWWriteRarely
                           | GRALLOC_USAGE_HW_MASK);
  int status = sGBFunctions.fGBLock(_agbPtr, usage, ptr);
  if (status) printErr("Error locking graphic buffer, status: %d", status);
  return (status == 0);
}

bool AndGraphicBuffer::unlockBuffer() {
  int status = sGBFunctions.fGBUnlock(_agbPtr);
  if (status) printErr("Error unlocking graphic buffer, status: %d", status);
  return (status == 0);
}

bool AndGraphicBuffer::bindToCurrentTexture() {
  if (!sGBFunctions.hasEGLImageSupport()) return false;

  //create it here - the Graphic Buffer can be created out of an EGL
  //context, but the EGL image can't.
  if (!_eglImage) {
    if (!createEGLImageKHR()) {
      printErr("Create EGLImage failed: %p", _eglImage);
      return false;
    }
  }

  sGBFunctions.fGLEGLImageTargetTexture2DOES(GL_TEXTURE_2D, _eglImage);
  CHECK_GL_ERROR();
  return true;
}

bool AndGraphicBuffer::allocateGB() {
  if (!_agbPtr) {
    _agbPtr = malloc(kAndGraphicBufferSize);
    sGBFunctions.fGBConstructor(_agbPtr, _width, _height,
                                getHALPixelFormat(_imageFormat),
                                _usageFlags);

    ANativeWindowBuffer_t *nwb =
        (ANativeWindowBuffer_t*) sGBFunctions.fGBGetNativeBuffer(_agbPtr);
    _bytesPerRow = nwb->stride * (_imageFormat == GL_RGBA ? 4 : 3);
  }

  return true;
}

bool AndGraphicBuffer::createEGLImageKHR() {
  if (!sGBFunctions.hasEGLImageSupport()) return false;

  EGLint attribs[] = { EGL_IMAGE_PRESERVED_KHR, EGL_TRUE,
                       EGL_NONE, EGL_NONE };
  void *natBuff = sGBFunctions.fGBGetNativeBuffer(_agbPtr);

  _eglImage = sGBFunctions.fEGLCreateImageKHR(
      eglGetDisplay(EGL_DEFAULT_DISPLAY), EGL_NO_CONTEXT,
      EGL_NATIVE_BUFFER_ANDROID, (EGLClientBuffer)natBuff,
      attribs);

  if (!_eglImage) CHECK_EGL_ERROR();

  return _eglImage != nullptr;
}

int AndGraphicBuffer::getHALPixelFormat(GLenum format) {
  switch(format) {
    case GL_RGBA:
      return HAL_PIXEL_FORMAT_RGBA_8888;
    default:
      printErr("Invalid or unsupported format: %x", format);
      assert(0);
      return 0;
  }
}

}  // namespace gpgpu
}  // namespace tvs
