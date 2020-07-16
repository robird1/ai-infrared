/**
   tvsTracker: TrueVisionSolutions Face Tracker Interface
   Copyright 2014 TrueVisionSolutions Pty. Ltd.
   All rights reserved.
   www.truevisionsolutions.net
*/

#ifndef __LINEAR_MODEL_H__
#define __LINEAR_MODEL_H__

#include <stdio.h>
#include <opencv2/core/core.hpp>

namespace tvs {
//=============================================================================
class LinearModel{
 public:

#ifdef USE_SHARE_MEMORY
  //  LinearModel(){;}
  LinearModel() {
    _mean = (cv::Mat_<float>(1, 1) << 0.0);
    _evec = (cv::Mat_<float>(1, 1) << 0.0);
    _eval = (cv::Mat_<float>(1, 1) << 0.0);
  }
#else
  LinearModel(){;}
#endif

  LinearModel(const char* fname) { this->load(fname); }

  int save(const char *fname);
  int load(const char *fname);
  int write(FILE* f);
  int read(FILE* f);

  inline int ndims() const { return _mean.rows; }
  inline int nmodes() const { return _evec.cols; }
  
  cv::Mat_<float> project(const cv::Mat_<float> &x) const;
  cv::Mat_<float> reconstruct(const cv::Mat_<float> &p) const;
  int clamp(cv::Mat_<float> &p,const float v = 3.0f) const;

  cv::Mat_<float> _mean;
  cv::Mat_<float> _evec;
  cv::Mat_<float> _eval;
};
//=============================================================================
}  // namespace tvs

#endif  // __LINEAR_MODEL_H__
