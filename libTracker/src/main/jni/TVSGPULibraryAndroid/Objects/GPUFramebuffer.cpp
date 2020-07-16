/**
   GPUFrameBuffer: Framebuffer classes
   
   Copyright 2014 TrueVisionSolution Pty. Ltd.
   All rights reserved

   Classes to manage fbo's using textures as targets
*/
#include "GPUFramebuffer.h"

#include <vector>
#include <string>
#include <sstream>
#include <dlfcn.h>
#include <algorithm>
#include <assert.h>

#include <android/log.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "GPUBasicDefinitions.h"

#ifdef TVS_INTERNAL_DEBUGGING
const char kLogTag[] = "GPUFramebuffer";
#define printInfo(...) __android_log_print(ANDROID_LOG_INFO, kLogTag, __VA_ARGS__)
#else
const char kLogTag[] = "UlsGPULibrary";
#define printInfo(...) 
#endif

#define printErr(...) __android_log_print(ANDROID_LOG_ERROR, kLogTag, __VA_ARGS__)

//*****************************************************************************/
//*****************************************************************************/
//*****************************************************************************/

namespace tvs {
namespace gpgpu {
GPUFramebufferGraphicBuffer::GPUFramebufferGraphicBuffer() {
  _targetTexture = nullptr;
  _width = _height = 0;
}

GPUFramebufferGraphicBuffer::~GPUFramebufferGraphicBuffer() {
  if (!_init) return;
  
  glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
  glDeleteFramebuffers(1, &_fbo);
  
  if (_useDepthBuffer) {
    glDeleteRenderbuffers(1, &_depthBuffer);
  }
  CHECK_GL_ERROR();
}

void GPUFramebufferGraphicBuffer::init(GPUTextureGraphicBuffer *texture,
                                       bool useDepthBuffer) {
  _width = _height = 0;
  _useDepthBuffer = useDepthBuffer;
  glGenFramebuffers(1, &_fbo);
  this->bind();
  if (texture) {
    this->setTargetTexture(texture);
  }
  _init = true;
}

void GPUFramebufferGraphicBuffer::label(const char *label) {
  // if (sFBSupport.deviceSupportsLabel()) {
  //   int length = std::min(static_cast<int>(strlen(label)), 32);
  //   sFBSupport.labelObjectEXT(GL_FRAMEBUFFER, _fbo, length, label);
  //   CHECK_GL_ERROR();
  // }
}

void GPUFramebufferGraphicBuffer::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
  CHECK_GL_ERROR();
}

void GPUFramebufferGraphicBuffer::unbind() {
  this->discardDepthBuffer();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  CHECK_GL_ERROR();
}

int GPUFramebufferGraphicBuffer::width() {
  return _width;
}

int GPUFramebufferGraphicBuffer::height() {
  return _height;
}

void GPUFramebufferGraphicBuffer::setTargetTexture(
    GPUTextureGraphicBuffer *texture) {
  assert(texture);
  const int w = this->width();
  const int h = this->height();

  _targetTexture = texture;
  _targetTexture->bind(0);
  GLint idx = _targetTexture->textureID();
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         idx, 0);
  CHECK_GL_ERROR();

  _width = _targetTexture->width();
  _height = _targetTexture->height();
  if (_useDepthBuffer) {
    if (w != _targetTexture->width() || h != _targetTexture->height()) {
      if (w == 0) { // first time after a call to init
        glGenRenderbuffers(1, &_depthBuffer);
        CHECK_GL_ERROR();
      }
      glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);
      GLenum depth = GL_DEPTH_COMPONENT16;
      // if (sFBSupport.deviceSupportsDepth24OES())
      //   depth = GL_DEPTH_COMPONENT24_OES;
      glRenderbufferStorage(GL_RENDERBUFFER, depth, _targetTexture->width(),
                            _targetTexture->height());
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                GL_RENDERBUFFER, _depthBuffer);
      CHECK_GL_ERROR();
    }
  }

  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    printErr("Error, framebuffer object not complete: %x", status);
  }

  CHECK_GL_ERROR();
}

void GPUFramebufferGraphicBuffer::synchGBContents() {
  glFinish();
}

void GPUFramebufferGraphicBuffer::discardDepthBuffer() {
  // if (sFBSupport.deviceSupportsDiscard() && _useDepthBuffer) {
  //   GLenum discards[] = {GL_DEPTH_ATTACHMENT};
  //   this->bind();
  //   sFBSupport.discardFramebufferEXT(GL_FRAMEBUFFER, 1, discards);
  // }
  // CHECK_GL_ERROR();
}
}  // namespace gpgpu
}  // namespace tvs
