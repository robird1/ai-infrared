//
//  linear_shape_classifier.h
//  TVStrackeriOS - Android port
//
//  Created by Jesus Nuevo Chiquero TVS on 25/04/14.
//  Copyright (c) 2014 TrueVision Solutions Pty Ltd. All rights reserved.
//

#ifndef __TVStrackeriOS__classifiers__
#define __TVStrackeriOS__classifiers__

#include <Eigen/Core>

#include "utils/io.h"
namespace tvs {
class LinearShapeClassifier{
public:
  //LinearShapeClassifier() {;};
  int save(const char* fname);
  int load(const char* fname);
  int write(FILE* f);
  int read(FILE* f);

  cv::Mat_<float> predict(const cv::Mat_<float>& vec,
                          const cv::Mat_<float> &pts);

  cv::Mat_<float> _rpts;
  cv::Mat_<float> _evec;
  cv::Mat_<float> _gain;
  cv::Mat_<float> _bias;

#ifdef USE_SHARE_MEMORY
  LinearShapeClassifier() {
    _rpts = (cv::Mat_<float>(1, 1) << 0.0);
    _evec = (cv::Mat_<float>(1, 1) << 0.0);
    _gain = (cv::Mat_<float>(1, 1) << 0.0);
    _bias = (cv::Mat_<float>(1, 1) << 0.0);
    _vec = (cv::Mat_<float>(1, 1) << 0.0);
  }
#endif

private:
  cv::Mat_<float> _vec;
};
}  // namespace tvs
#endif /* defined(__TVStrackeriOS__classifiers__) */
