//
//  FeatureComp.cpp
//  Created by Jesus Nuevo Chiquero TVS on 25/07/14.
//  Copyright (c) 2014 TrueVisionSolutions Pty Ltd. All rights reserved.
//  Android port (c) 2014 TrueVisionSolutions Pty Ltd.

#include "FeatureComp.h"

#include <android/log.h>

#ifdef TVS_INTERNAL_DEBUGGING
const char kLogTag[] = "FeatureComp";
#define printInfo(...) {__android_log_print(ANDROID_LOG_INFO, kLogTag, \
                                            __VA_ARGS__);}
#else
const char kLogTag[] = "UlsFaceTracker";
#define printInfo(...)
#endif
#define printErr(...) {__android_log_print(ANDROID_LOG_ERROR, kLogTag, \
                                           __VA_ARGS__);}
//==============================================================================
FeatureComp::FeatureComp(int targetcount) {
  _regionSz.width = 0;
  _targetCount = targetcount;
}
//==============================================================================
void FeatureComp::setSizes(const cv::Size srcTextureSize,
                           const cv::Size targetSize) {
  _srcImageSz = srcTextureSize;
  _regionSz = targetSize;
}
//==============================================================================
const cv::Mat& FeatureComp::setup(const int srcTexture,
                                  const cv::Mat_<float>& M) {
  if (!_oglInit) initialiseGPU();

  _textureGB.updateTexture(&_warpGB);
  _fboWarp.bind();
  _fboWarp.setTargetTexture(&_textureGB);
  glViewport(0, 0, _fboWarp.width(), _fboWarp.height());
  glClear(GL_COLOR_BUFFER_BIT);
  CHECK_GL_ERROR();

  glActiveTexture(GL_TEXTURE0 + 2);
  glBindTexture(GL_TEXTURE_EXTERNAL_OES, srcTexture);
  glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  CHECK_GL_ERROR();
  
  _vaoWarp->bind();
  CHECK_GL_ERROR();

  _bgra2gggg.use();
  
  {
    cv::Mat_<float> corners = (cv::Mat_<float>(4,2) << 0.0f, 0.0f,
                               _regionSz.width, 0.0f,
                               _regionSz.width, _regionSz.height,
                               0.0f, _regionSz.height);
    corners = corners * M.colRange(0, 2).t();
    corners.col(0) += M(0,2);
    corners.col(1) += M(1,2);

    // cv::Mat r = _srcImageSz.height - corners.col(0).clone();
    // cv::Mat ry = _srcImageSz.width - corners.col(1);
    // ry.copyTo(corners.col(0));
    // r.copyTo(corners.col(1));

    corners.col(0) /= _srcImageSz.width;
    corners.col(1) /= _srcImageSz.height;

    _vaoWarp->updateTextureCoordinates(corners.ptr<float>(),
                                       8, 0, true);
  }
  CHECK_GL_ERROR();
  _bgra2gggg.draw(6);
  
  _fboWarp.unbind();
  _fboWarp.synchGBContents();
  _vaoWarp->unbind();
  glFlush();
  
  CHECK_GL_ERROR();
  cv::Mat mat;
  tvs::gpgpu::GBMatWrapping gbWrap;
  gbWrap.wrap(&_warpGB, mat, true);

  _graySrc.create(_regionSz, CV_8U);
  cv::Mat out[] = {_graySrc};
  cv::Mat in[] = {mat.rowRange(0, _regionSz.height)};
  int fromTo [] = {0, 0};
  cv::mixChannels(in, 1, out, 1, fromTo, 1);

  gbWrap.unwrap(mat);

  return _graySrc;
}
//==============================================================================
const cv::Mat& FeatureComp::setupMulti(const int srcTexture,
                                       const std::vector<cv::Mat_<float>>& M,
                                       const bool extractChannel) {
  if (!_oglInit) initialiseGPU();
  if (M.size() == 0) {
    printErr("Empty transformation matrices vector! Result undefined");
    return _graySrc;
  }
  
  _textureGB.updateTexture(&_warpGB);
  _fboWarp.bind();
  _fboWarp.setTargetTexture(&_textureGB);
  glViewport(0, 0, _fboWarp.width(), _fboWarp.height());
  glClear(GL_COLOR_BUFFER_BIT);
  CHECK_GL_ERROR();

  glActiveTexture(GL_TEXTURE0 + 2);
  glBindTexture(GL_TEXTURE_EXTERNAL_OES, srcTexture);
  glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  CHECK_GL_ERROR();
  
  _vaoWarp->bind();
  CHECK_GL_ERROR();

  _bgra2gggg.use();

  int count = std::min(static_cast<int>(M.size()), _targetCount);

  cv::Mat_<float> all_corners(4 * count, 2);
  for (int k = 0; k < count; k++) {
    cv::Mat_<float> corners = (cv::Mat_<float>(4,2) << 0.0f, 0.0f,
                               _regionSz.width, 0.0f,
                               _regionSz.width, _regionSz.height,
                               0.0f, _regionSz.height);
    corners = corners * M.at(k).colRange(0, 2).t();
    corners.col(0) += M.at(k)(0,2);
    corners.col(1) += M.at(k)(1,2);

    corners.copyTo(all_corners.rowRange(4 * k, 4 * (k+1)));
    // cv::Mat r = _srcImageSz.height - corners.col(0).clone();
    // cv::Mat ry = _srcImageSz.width - corners.col(1);
    // ry.copyTo(corners.col(0));
    // r.copyTo(corners.col(1));
  }
  all_corners.col(0) /= _srcImageSz.width;
  all_corners.col(1) /= _srcImageSz.height;
  
  _vaoWarp->updateTextureCoordinates(all_corners.ptr<float>(),
                                     8 * count, 0, true);
  
  
  CHECK_GL_ERROR();
  _bgra2gggg.draw(6 * count);
  
  _fboWarp.unbind();
  _fboWarp.synchGBContents();
  _vaoWarp->unbind();
  glFlush();
  
  CHECK_GL_ERROR();
  cv::Mat mat;
  tvs::gpgpu::GBMatWrapping gbWrap;
  gbWrap.wrap(&_warpGB, mat, true);

  if (extractChannel) {
    _graySrc.create(cv::Size(_regionSz.width, _regionSz.height * count), CV_8U);
    cv::Mat out[] = {_graySrc};
    cv::Mat in[] = {mat.rowRange(0, _regionSz.height * count)};
    int fromTo [] = {0, 0};
    cv::mixChannels(in, 1, out, 1, fromTo, 1);
    
    gbWrap.unwrap(mat);

    return _graySrc;
  } else {
    printErr("no channel extraction is not supported yet! Result undefined");
    return _graySrc;
  }
}
//==============================================================================
cv::Mat_<float> FeatureComp::convertCVSimTransfToOpenGL(
    const cv::Mat_<float> & M) const {
  cv::Mat_<float> A (2,3);
  M.colRange(0,2).copyTo(A.colRange(0, 2));
  float ct = M(0,0);
  float st = M(1,0);
  float tx = M(0,2) - ct/2.0f + st/2.0f + 0.5f;
  float ty = M(1,2) - ct/2.0f - st/2.0f + 0.5f;
  A(0,2) = tx;
  A(1,2) = ty;
  return A;
}
//==============================================================================
void FeatureComp::initialiseGPU() {
  _oglInit = false;
  if (_srcImageSz == cv::Size()) {
    printErr("Source image size must be set before 1st call");
    abort();
  }
  printInfo("Initialising featureComp");
  const int w = _regionSz.width;
  const int h = _regionSz.height * _targetCount;
  const int flag = tvs::gpgpu::AndGraphicBuffer::kGBUsageSWReadOften
      | tvs::gpgpu::AndGraphicBuffer::kGBUsageSWWriteNever
      | tvs::gpgpu::AndGraphicBuffer::kGBUsageHWTexture
      | tvs::gpgpu::AndGraphicBuffer::kGBUsageHWRender;
  _warpGB.init(w, h, flag,  GL_RGBA);
  _textureGB.init();
  
  CHECK_GL_ERROR();

  printInfo("Initialising graphicbuffers done");
  _fboWarp.init(nullptr, false);
 
  _bgra2gggg.init(true);
  _bgra2gggg.label("bgra2gggg");
  _bgra2gggg.updateTextureUnit(2);

  // _bgra2gggg.use();
  // glm::mat4 rotmat = glm::mat4();
  // /* if (_srcImgRotation == 0) {
  //    } else */ if (_srcImageRotation == 90) {
  //   rotmat = glm::eulerAngleZ(M_PI_2);     
  // } else if (_srcImageRotation == 180) {
  //   rotmat = glm::eulerAngleZ(M_PI);
  // } else if (_srcImageRotation == 270) {
  //   rotmat = glm::eulerAngleZ(M_PI * 1.5f);
  // } else {
  //   printErr("Bad rotation value for src image");
  //   return;
  // }
  // _bgra2gggg.updateProjectionMatrix(rotmat);
  // CHECK_GL_ERROR();

  printInfo("Initialising featcomp programs done");

  //if not reset, the VAO is created in the new EGLcontext, and the
  //old one is released after that => trouble if they have the same
  //object id.
  _vaoWarp.reset(nullptr);
  printInfo("Releasing old VAO done");

  if (_targetCount == 1) {
    _vaoWarp = std::unique_ptr<tvs::gpgpu::GPUVertexData>(
        tvs::gpgpu::createBackgroundQuad());
  } else {
    _vaoWarp = std::unique_ptr<tvs::gpgpu::GPUVertexData>(
        new tvs::gpgpu::GPUVertexData(tvs::gpgpu::GPUVertexData::VD_TEXTCOORDS));
    _vaoWarp->setElementIndicesCount(6 * _targetCount);
    _vaoWarp->setVertexCount(12 * _targetCount);
    float ystep = 2.0f / _targetCount;
    cv::Mat_<float> vertices (4 * _targetCount, 3);
    cv::Mat_<float> texCoords ( 4 * _targetCount, 2);
    texCoords = 0.0f; //these don't matter since they're updated on each call
    cv::Mat_<short> indices (2 * _targetCount, 3);
    
    vertices.col(2) = 1.0f;
    const cv::Mat_<short> ind = (cv::Mat_<short> (2,3) <<
                           2,1,0,
                           0,3,2);
    for (int i = 0; i < _targetCount; i++) {
      cv::Mat_<float> vert = vertices.rowRange(4 * i, 4 * (i+1));
      vert(0, 0) = vert(3, 0) = -1.0f;
      vert(1, 0) = vert(2, 0) = 1.0f;
      vert(0, 1) = vert(1, 1) = -1.0f + ystep * i;
      vert(2, 1) = vert(3, 1) = -1.0f + ystep * (i+1);

      indices.rowRange(2 * i, 2 * (i+1)) = ind + 4 * i;
    }
    _vaoWarp->updateElementIndices(indices.ptr<GLushort>(),
                                   6 * _targetCount, 0);
    _vaoWarp->updateVertexPosition(vertices.ptr<float>(),
                                   12 * _targetCount, 0, false);
    _vaoWarp->updateTextureCoordinates(texCoords.ptr<float>(),
                                       8 * _targetCount, 0, true);
  }
  CHECK_GL_ERROR();

  printInfo("Initialising VAO done");

  _fboWarp.unbind();
  _vaoWarp->unbind();
  CHECK_GL_ERROR();

  _oglInit = true;
}
//==============================================================================
