/**
   GPUFrameBuffer: Framebuffer classes
   Copyright 2014 TrueVisionSolution Pty. Ltd.
   All rights reserved
*/
#ifndef __GPU_FRAMEBUFFER_H__
#define __GPU_FRAMEBUFFER_H__

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "GPUTexture.h"


namespace tvs {
namespace gpgpu {
class GPUFramebuffer {
 public:
  virtual ~GPUFramebuffer() {}
  virtual int width() = 0;
  virtual int height() = 0;

  virtual void bind() = 0;
  virtual void unbind() = 0;

  virtual void label(const char *label) = 0;
};

/**
 * A framebuffer object using a texture backed by a GraphicBuffer as target
 */
class GPUFramebufferGraphicBuffer : public GPUFramebuffer {
 public:
  GPUFramebufferGraphicBuffer();
  ~GPUFramebufferGraphicBuffer();

  // call this function only once per object, the old FBO is not
  // released automatically. If the EGL context is destroyed, call
  // this function in the new EGL context to initialise.
  void init(GPUTextureGraphicBuffer *texture, bool useDepthBuffer);
  // this call may do nothing depending on the device support
  void label(const char *label);

  void bind();
  void unbind();

  int width();
  int height();
  
  void setTargetTexture(GPUTextureGraphicBuffer *texture);
  // make sure all rendering has finished - this is a just a glFinish().
  // call this before locking the GB for SW rendering.
  void synchGBContents();

 private:
  // this call may do nothing depending on the device support
  void discardDepthBuffer();

  bool _init;
  GLuint _fbo;
  GLuint _depthBuffer;
  int _width, _height;

  bool _useDepthBuffer;

  GPUTextureGraphicBuffer *_targetTexture;
};
}  // namespace gpgpu
}  // namespace tvs

#endif // __GPU_FRAMEBUFFER_H_
