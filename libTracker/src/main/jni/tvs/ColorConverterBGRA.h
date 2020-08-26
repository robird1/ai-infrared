//
//  ColorConverter.h
//  TVStrackeriOS
//
//  Created by Jesus Nuevo Chiquero TVS on 26/07/14.
//  Copyright (c) 2014 TrueVisionSolutions Pty Ltd. All rights reserved.
//  Android port (c) 2014 TrueVisionSolutions Pty Ltd.

#ifndef __TVStrackerAndroid__ColorConverterBGRA__
#define __TVStrackerAndroid__ColorConverterBGRA__

#include <memory>

#include <opencv2/core/core.hpp>
#include "TVSGPULibraryAndroid/TVSGPULibrary.h"

class ColorConverterBGRA {
public:
  // targetSize - size of the gray scale destination.
  // rotation - rotation to be applied to the input texture, in degrees.
  // Valid values are 0, 90, 180 and 270
  void initialise(const cv::Size targetSize, const int rotation = 0);

  // srcTexture - OpenGL texture index to use as source. The texture
  // is bound with GL_TEXTURE_EXTERNAL_OES.
  void startConverting(const GLint srcTexture);
  void finishConversion();

  // A call to getResult must be followed by a call to releaseResult
  // when done using the cv::Mat
  const cv::Mat& getResult();
  void releaseResult();
  
private:
  tvs::gpgpu::GPURGBA2BGRA _colorConv;
  std::unique_ptr<tvs::gpgpu::GPUVertexData> _vao;
  tvs::gpgpu::GPUFramebufferGraphicBuffer _fbo;
  tvs::gpgpu::GPUTextureGraphicBuffer _packedGray;
  tvs::gpgpu::AndGraphicBuffer _grayGB;

  bool _oglInit = false;
  cv::Mat _gbWrapper;
  bool _resultAvailable = false;
  tvs::gpgpu::GBMatWrapping _mapping;
};

#endif /* defined(__TVStrackerAndroid__ColorConverter__) */
