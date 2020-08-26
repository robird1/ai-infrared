/**
   GPUTexture: Texture class with a AndGraphicBuffer backend
   Copyright 2014 TrueVisionSolution Pty. Ltd.
   All rights reserved
*/
#include "GPUTexture.h"

#include <iostream>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#ifdef TVS_INTERNAL_DEBUGGING
const char kLogTag[] = "GPUTexture";
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
GPUTextureGraphicBuffer::GPUTextureGraphicBuffer() {
  _width = _height = 0;
  _init = false;
  _gBuffPtr = nullptr;
}

GPUTextureGraphicBuffer::~GPUTextureGraphicBuffer() {
  glDeleteTextures(1, &_textID);
}

void GPUTextureGraphicBuffer::init() {
  glGenTextures(1, &_textID);
  this->bind(0);
	
  CHECK_GL_ERROR();
}

void GPUTextureGraphicBuffer::destroy() {
  glDeleteTextures(1, &_textID);
  CHECK_GL_ERROR();
}

bool GPUTextureGraphicBuffer::bind(int textUnit) {
  if (!_gBuffPtr) return false;
  glActiveTexture(textUnit+GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _textID);
  CHECK_GL_ERROR();

  _gBuffPtr->bindToCurrentTexture();
  CHECK_GL_ERROR();
  return true;
}

void GPUTextureGraphicBuffer::setFilteringParameters(GLenum interpMAG,
                                                     GLenum interpMIN) {
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpMAG);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpMIN);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  CHECK_GL_ERROR();
}

void GPUTextureGraphicBuffer::updateTexture(AndGraphicBuffer *gb) {
  _gBuffPtr = gb;
  if (_gBuffPtr) {
    _width = _gBuffPtr->getWidth();
    _height = _gBuffPtr->getHeight();
  } else {
    _width = _height = 0;
  }
}

void GPUTextureGraphicBuffer::releaseAGB() {
  this->updateTexture(nullptr);
}
}  // namespace gpgpu
}  // namespace tvs
