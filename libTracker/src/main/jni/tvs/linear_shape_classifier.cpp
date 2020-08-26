/**
   Linear shape classifier -  Android port
   Copyright 2014 TrueVisionSolutions Pty. Ltd.
   All rights reserved
   www.truevisionsolutions.net
*/

#include "linear_shape_classifier.h"

#include <Eigen/Core>
typedef Eigen::Map< Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic,Eigen::RowMajorBit> > cv2eiMapU;
typedef Eigen::Map < Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajorBit>, Eigen::AlignedBit > cv2eiMapF;

using cv::Mat_;
using cv::Mat;

namespace tvs {
//=============================================================================
int LinearShapeClassifier::save(const char* fname) {
  FILE *f = fopen(fname,"wb");
  if (f == NULL) { return -1; }
  this->write(f); fclose(f);
  return 0;
}
//=============================================================================
int LinearShapeClassifier::load(const char* fname) {
  FILE *f = fopen(fname,"rb");
  if (f == NULL) { return -1; }
  int ret = this->read(f);
  fclose(f);
  return ret;
}
//=============================================================================
int LinearShapeClassifier::write(FILE *f) {
  if(fwrite(&_rpts.rows,sizeof(int),1,f) != 1)return -1;
  if((int)fwrite(_rpts.ptr<float>(0),sizeof(float),_rpts.rows,f)
     != _rpts.rows)return -1;
  if(tvs::io::writeMat(_evec,f) < 0)return -1;
  if(tvs::io::writeMat(_gain,f) < 0)return -1;
  if(tvs::io::writeMat(_bias,f) < 0)return -1;
  return 0;
}

int LinearShapeClassifier::read(FILE *f) {
  int n; if(fread(&n,sizeof(int),1,f) != 1)return -1;

  #ifdef ULS_USE_NEON
  cv::Mat_<float16_t > tmp_rpts;
  tmp_rpts.create(n,1);
  if((int)fread(tmp_rpts.ptr<float16_t >(0), sizeof(float16_t), n,f) != n) {return -1;}

  cv::Mat_<float16_t > tmp_evec, tmp_gain, tmp_bias;
  if (io::readMat(tmp_evec, f) < 0) { return -1; }
  if (io::readMat(tmp_gain, f) < 0) { return -1; }
  if (io::readMat(tmp_bias, f) < 0) { return -1; }

  int size_evec = tmp_evec.rows * tmp_evec.cols;
  int size_gain = tmp_gain.rows * tmp_gain.cols;
  int size_bias = tmp_bias.rows * tmp_bias.cols;

  _rpts.create(n, 1);
  for(int i =0; i< n;i++) {
    _rpts(i) = tmp_rpts(i);
  }

  //evec
  _evec.create(tmp_evec.rows, tmp_evec.cols);
  for(int i =0; i< size_evec; i++) {
     _evec(i) = tmp_evec(i);
  }

  //gain
    _gain.create(tmp_gain.rows, tmp_gain.cols);
  for(int i =0; i< size_gain; i++) {
     _gain(i) = tmp_gain(i);
  }

  //bias
  _bias.create(tmp_bias.rows, tmp_bias.cols);
  for(int i =0; i< size_bias; i++) {
     _bias(i) = tmp_bias(i);
  }

#else
  _rpts.create(n,1);
  if((int)fread(_rpts.ptr<float>(0),sizeof(float),n,f)!= n)return -1;
  if(tvs::io::readMat(_evec,f) < 0)return -1;
  if(tvs::io::readMat(_gain,f) < 0)return -1;
  if(tvs::io::readMat(_bias,f) < 0)return -1;
#endif
  return 0;
}

cv::Mat_<float> LinearShapeClassifier::predict(const cv::Mat_<float> &vec,
                                               const cv::Mat_<float> &pts) {
  Mat_<float> s(pts.rows/2, 1);
  if (_gain.cols == _vec.rows) {
    //printf("global not supported yet!");
    exit(0);
  } else { //per-point regressors
    int n = pts.rows / 2; int ndims = vec.rows / n;
    Mat_<float> p(n, 1);
    for (int i = 0; i < n; i++) {
      Eigen::Map<const Eigen::VectorXf,Eigen::Aligned>
          ge(_gain.ptr<float>(i), ndims),
          ve(vec.ptr<float>(i*ndims), ndims);
      p(i) = ge.dot(ve);
    }
    cv2eiMapF se(s.ptr<float>(0), s.rows, s.cols);
    cv2eiMapF pe(p.ptr<float>(0), p.rows, p.cols);
    cv2eiMapF ee(_evec.ptr<float>(0), _evec.rows, _evec.cols);
    cv2eiMapF be(_bias.ptr<float>(0), _bias.rows, _bias.cols);
    se = ee * pe + be;
  }
  return s;
}
}  // namespace tvs
