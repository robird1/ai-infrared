//
//  regressors.h
//  TVStrackeriOS
//
//  Created by Jesus Nuevo Chiquero TVS on 25/04/14.
//  Copyright (c) 2014 TrueVisionSolutions Pty Ltd. All rights reserved.
//  Android port (c) 2014 TrueVisionSolutions Pty Ltd.

#ifndef __TVStrackeriOS__regressors__
#define __TVStrackeriOS__regressors__

#include <vector>
#include <Eigen/Core>

#include "Feature.h"
namespace tvs {
class LinearShapeRegressor{
public:

  int save(const char* fname);
  int load(const char* fname);
  int write(FILE* f);
  int read(FILE* f);
  
  cv::Mat_<float> predict(Feature& H, const cv::Mat_<float>& pts);
  const cv::Mat_<float> getLastFeature() {return _vec;}

  cv::Mat_<float> _rpts;
  cv::Mat_<float> _evec;
  cv::Mat_<float> _gain;
  cv::Mat_<float> _bias;

#ifdef USE_SHARE_MEMORY
  LinearShapeRegressor() {
    _rpts = (cv::Mat_<float>(1, 1) << 0.0);
    _evec = (cv::Mat_<float>(1, 1) << 0.0);
    _gain = (cv::Mat_<float>(1, 1) << 0.0);
    _bias = (cv::Mat_<float>(1, 1) << 0.0);
    _vec = (cv::Mat_<float>(1, 1) << 0.0);
    //_rptsE = Eigen::MatrixXf(1, 1);
    _rptsE = Eigen::MatrixXf::Zero(1,1);
    //_biasE = Eigen::MatrixXf(1, 1);
    _biasE = Eigen::MatrixXf::Zero(1,1);
  }
#endif

private:
  //uses the feature in _vec
  void computeUpdate(cv::Mat_<float>& pts);
  
  cv::Mat_<float> _vec;
  Eigen::MatrixXf _rptsE, _biasE;
};
}  // namespace tvs
#endif /* defined(__TVStrackeriOS__regressors__) */
