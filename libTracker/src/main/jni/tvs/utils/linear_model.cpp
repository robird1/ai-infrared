/**
   tvsTracker: TrueVisionSolutions Face Tracker Interface
   Copyright 2014 TrueVisionSolutions Pty. Ltd.
   All rights reserved
   www.truevisionsolutions.net
*/
#include "tvs/utils/linear_model.h"

#include <opencv2/core/core.hpp>

using cv::Mat_;
using cv::Mat;
using cv::Size;

namespace tvs {
//=============================================================================
Mat_<float> LinearModel::project(const Mat_<float> &x) const {
  if (x.size() != Size(1,this->ndims())) { return Mat(); }
  return _evec.t()*(x-_mean);
}
//=============================================================================
Mat_<float> LinearModel::reconstruct(const Mat_<float> &p) const {
  if (p.size() != Size(1,this->nmodes())) { return Mat(); }
  return _mean + _evec*p;
}
//=============================================================================
int LinearModel::clamp(Mat_<float> &p,const float v) const {
  if (p.size() != Size(1,_evec.cols)) { return -1; }
  for (int i = 0; i < p.rows; i++) {
    if (fabs(p(i)) > v*_eval(i)) {
      if (p(i) > 0)p(i) = v*_eval(i);
      else p(i) = -v*_eval(i);
    }
  }
  return 0;
}
//=============================================================================
int LinearModel::save(const char *fname) {
  FILE *f = fopen(fname,"wb");
  if (f == NULL) { return -1; }
  this->write(f); fclose(f);
  return 0;
}
//=============================================================================
int LinearModel::load(const char *fname) {
  FILE *f = fopen(fname,"rb");
  if (f == NULL) { return -1; }
  int ret = this->read(f);
  fclose(f);
  return ret;
}
//=============================================================================
int LinearModel::write(FILE* f) {
  if (!_mean.isContinuous() || !_evec.isContinuous() ||
      !_eval.isContinuous()) { return -1; }
  int n = this->ndims(), m = this->nmodes();
  if (fwrite(&n, sizeof(int), 1, f) != 1) { return -1; }
  if (fwrite(&m, sizeof(int), 1, f) != 1) { return -1; }
  if ((int)fwrite(_mean.ptr<float>(0),sizeof(float), n, f) != n) { return -1; }
  if ((int)fwrite(_evec.ptr<float>(0),sizeof(float), n*m,f)!=n*m) { return -1; }
  if ((int)fwrite(_eval.ptr<float>(0),sizeof(float), m,f) != m) { return -1; }
  return 0;
}
//=============================================================================
int LinearModel::read(FILE* f) {
  int n; if (fread(&n, sizeof(int), 1, f) != 1) { return -1; }
  int m; if (fread(&m, sizeof(int), 1, f) != 1) { return -1; }

  #ifdef ULS_USE_NEON
  cv::Mat_<float16_t > tmp_mean, tmp_evec, tmp_eval;
  tmp_mean.create(n,1);
  tmp_evec.create(n, m);
  tmp_eval.create(m,1);
  if ((int)fread(tmp_mean.ptr<float16_t>(0),sizeof(float16_t), n, f) != n) { return -1; }
  if ((int)fread(tmp_evec.ptr<float16_t>(0),sizeof(float16_t), n*m,f)!=n*m) { return -1; }
  if ((int)fread(tmp_eval.ptr<float16_t>(0),sizeof(float16_t), m, f) != m) { return -1; }

  _mean.create(n,1);
  for(int i = 0; i< n; i++) {
     _mean(i) = tmp_mean(i);
  }


  _evec.create(n,m);
  for(int i =0; i< n*m; i++) {
     _evec(i) = tmp_evec(i);
  }

  _eval.create(m, 1);
  for(int i = 0; i< m; i++){
     _eval(i) = tmp_eval(i);
  }

#else
  _mean.create(n,1); _evec.create(n,m); _eval.create(m,1);
  if ((int)fread(_mean.ptr<float>(0),sizeof(float), n, f) != n) { return -1; }
  if ((int)fread(_evec.ptr<float>(0),sizeof(float), n*m,f)!=n*m) { return -1; }
  if ((int)fread(_eval.ptr<float>(0),sizeof(float), m, f) != m) { return -1; }

#endif

  return 0;
}
//=============================================================================
}  // namespace tvs
