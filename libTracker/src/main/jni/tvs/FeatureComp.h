//
//  FeatureComp.h
//  Created by Jesus Nuevo Chiquero TVS on 25/07/14.
//  Copyright (c) 2014 TrueVisionSolutions Pty Ltd. All rights reserved.
//  Android port (c) 2014 TrueVisionSolutions Pty Ltd.

#ifndef __TVStrackerAndroid_FeatureComp__
#define __TVStrackerAndroid_FeatureComp__

#include <memory>

#include <opencv2/core/core.hpp>
#include "TVSGPULibraryAndroid/TVSGPULibrary.h"

class FeatureComp {
public:
  FeatureComp(int targetCount = 1);
  void setSizes(const cv::Size srcTextureSize,
                const cv::Size targetSize);
  void initialiseGPU();

  const cv::Mat& setup(const int srcTexture, const cv::Mat_<float>& M);

  //extractGray is a trick to avoid doing the extraction of all
  //feature regions together in the same thread. If 'false', the
  //returned matrix is a 4 channel matrix with all channels being the
  //same.
  const cv::Mat& setupMulti(const int srcTexture,
                            const std::vector<cv::Mat_<float>>& M,
                            bool extractGray = true);

  
  
  cv::Mat_<float> convertCVSimTransfToOpenGL(const cv::Mat_<float> &M) const;

private:
  
  std::unique_ptr<tvs::gpgpu::GPUVertexData>  _vaoWarp;
  tvs::gpgpu::GPUFramebufferGraphicBuffer _fboWarp;
  tvs::gpgpu::AndGraphicBuffer _warpGB;
  tvs::gpgpu::GPUTextureGraphicBuffer _textureGB;
  // tvs::gpgpu::GPUBGRA2PackedGray _bgra2Gray;
  tvs::gpgpu::GPUBGRA2Gray _bgra2gggg;

  cv::Mat _graySrc;

  int _targetCount;
  
  cv::Size _regionSz;
  cv::Size _srcImageSz;
  bool _oglInit = false;
};
#endif /* defined(__TVStrackerAndroid_FeatureComp__) */
