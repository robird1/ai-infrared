//
//  ColorConverter.cpp
//  TVStrackeriOS
//
//  Created by Jesus Nuevo Chiquero TVS on 26/07/14.
//  Copyright (c) 2014 TrueVisionSolutions Pty Ltd. All rights reserved.
//  Android port (c) 2014 TrueVisionSolutions Pty Ltd.

#include <opencv2/imgproc.hpp>
#include "ColorConverterBGRA.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/euler_angles.hpp>
#ifdef TVS_INTERNAL_DEBUGGING
const char kLogTag[] = "Colorconverter";
#define printInfo(...) {__android_log_print(ANDROID_LOG_INFO, kLogTag, __VA_ARGS__);}
#else
const char kLogTag[] = "ULSFaceTracker";
#define printInfo(...)
#endif

void ColorConverterBGRA::initialise(const cv::Size targetSize, const int rotation) {
  // printInfo("Initialising color converter");
  
  _oglInit = false;
 _fbo.init(nullptr, false);
  const int flag = tvs::gpgpu::AndGraphicBuffer::kGBUsageSWReadOften
      | tvs::gpgpu::AndGraphicBuffer::kGBUsageSWWriteNever
      | tvs::gpgpu::AndGraphicBuffer::kGBUsageHWTexture
      | tvs::gpgpu::AndGraphicBuffer::kGBUsageHWRender;

//  _grayGB.init(targetSize.width / 4, targetSize.height, flag, GL_RGBA);
  _grayGB.init(targetSize.width, targetSize.height, flag, GL_RGBA);
  //_packedGray.reset(nullptr);
  _packedGray.init();
  
  _colorConv.init(true);
  _colorConv.label("bgra2gray");
//  _colorConv.updateSampleStep(1.0f/targetSize.width, 0.0f);
  _colorConv.updateTextureUnit(2);
  glm::mat4 rotmat = glm::mat4();
  if (rotation == 90) {
    rotmat = glm::eulerAngleZ(-M_PI_2);
//    _colorConv.updateSampleStep(0.0f, 1.0f/targetSize.height);
  } else if (rotation == 180) {
    rotmat = glm::eulerAngleZ(M_PI);
//    _colorConv.updateSampleStep(-1.0f/targetSize.width, 0.0f);
  } else if (rotation == 270) {
    rotmat = glm::eulerAngleZ(1.5f * M_PI);
//    _colorConv.updateSampleStep(0.0f, -1.0f/targetSize.height);
  }
  _colorConv.updateProjectionMatrix(rotmat);
  CHECK_GL_ERROR();
  
 //if not reset, the VAO is created in the new EGLcontext, and the old
 //one is released after that => trouble if they have the same object id..
  _vao.reset(nullptr);
  _vao = std::unique_ptr<tvs::gpgpu::GPUVertexData>(
      tvs::gpgpu::createBackgroundQuad());
  CHECK_GL_ERROR();

  _fbo.unbind();
  _vao->unbind();
  glFlush();
  CHECK_GL_ERROR();
  _oglInit = true;
}

void ColorConverterBGRA::startConverting(const GLint srcTexture) {
  _resultAvailable = false;
  // printInfo("ColorConverter::startConverting");
  _packedGray.updateTexture(&_grayGB);
  _fbo.bind();
  _fbo.setTargetTexture(&_packedGray);
  glViewport(0,0, _fbo.width(), _fbo.height());
  glClear(GL_COLOR_BUFFER_BIT);
  CHECK_GL_ERROR();

  glActiveTexture(GL_TEXTURE0 + 2);
  glBindTexture(GL_TEXTURE_EXTERNAL_OES, srcTexture);
  glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  CHECK_GL_ERROR();
  
  _vao->bind();
  CHECK_GL_ERROR();

  _colorConv.use();
  _colorConv.draw(_vao->getElementIndicesCount());

  glFlush();
  CHECK_GL_ERROR();
}

void ColorConverterBGRA::finishConversion() {
  _fbo.unbind();
  _fbo.synchGBContents();
  _vao->unbind();
  _resultAvailable = true;
  CHECK_GL_ERROR();
}

const cv::Mat& ColorConverterBGRA::getResult() {
  if (!_resultAvailable) return _gbWrapper;

  if (_mapping._agb == nullptr) {
    
    _mapping.wrap(&_grayGB, _gbWrapper, true);
//    _gbWrapper = _gbWrapper.reshape(1);
    cvtColor(_gbWrapper, _gbWrapper, CV_RGBA2RGB);
  }
  return _gbWrapper;
}

void ColorConverterBGRA::releaseResult() {
  _mapping.unwrap(_gbWrapper);
}

