/**
   AndGraphicBuffer: Android Graphic Buffer.

   A class to manager Android Graphic Buffers. Graphic Buffers are a
   non-public API used to share data between the CPU and GPU. A bit
   like iOS CoreVideo PixelBuffers, but with much, much less
   functionality and convenience.

   Copyright 2014 TrueVisionSolution Pty. Ltd.
   All rights reserved
*/
#ifndef __ANDROID_GRAPHIC_BUFFER_H__
#define __ANDROID_GRAPHIC_BUFFER_H__

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

namespace tvs {
namespace gpgpu {
class AndGraphicBuffer {
 public:
  // these constants are from Android's internal headers
  enum {kGBUsageSWReadNever = 0x000,
        kGBUsageSWReadRarely = 0x002,
        kGBUsageSWReadOften = 0x003,
        
        kGBUsageSWWriteNever = 0x000,
        kGBUsageSWWriteRarely = 0x020,
        kGBUsageSWWriteOften = 0x030,

        kGBUsageHWTexture = 0x100,
        kGBUsageHWRender = 0x200,
        kGBUsageHW2D = 0x400,    
  };

  AndGraphicBuffer();
  ~AndGraphicBuffer();

  /*
   * Initialise the graphic buffer. Usage flags are a combination of
   * the flags above. Image format must always be GL_RGBA.
   */
  void init(int width, int height, int usageFlag, GLenum imageFormat);
  // always returns GL_RGBA
  GLenum getImageFormat();
  
  int getWidth();
  int getHeight();
  int getBytesPerRow();

  // lock and get a pointer to the data
  bool lockBuffer(bool readOnly, unsigned char **ptr);
  bool unlockBuffer();

  // bind the Android Graphic buffer as an EGLImage 
  bool bindToCurrentTexture();
  
 private:
  bool allocateGB();
  bool createEGLImageKHR();

  int getHALPixelFormat(GLenum format);
  
  EGLImageKHR _eglImage;
  
  void *_agbPtr;
  int _width;
  int _height;
  int _bytesPerRow;
  int _usageFlags;
  GLenum _imageFormat;
};

}
}  // namespace tvs

#endif // __ANDROID_GRAPHIC_BUFFER_H__
