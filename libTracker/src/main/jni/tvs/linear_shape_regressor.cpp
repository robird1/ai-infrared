//
//  linear_shape_regressor.cpp
//  TVStrackeriOS - Android port
//
//  Created by Jesus Nuevo Chiquero TVS on 25/04/14.
//  Copyright (c) 2014 TrueVisionSolutions Pty Ltd. All rights reserved.
//

#include "linear_shape_regressor.h"
#include <android/log.h>
#include <Eigen/Core>

#include "utils/io.h"
#include "utils/geometry.h"

#ifdef TVS_INTERNAL_DEBUGGING
const char kLogTag[] = "UlsLSR";
#else
const char kLogTag[] = "UlsFaceTracker";
#endif

#define printErr(...) {__android_log_print(ANDROID_LOG_ERROR, kLogTag, __VA_ARGS__);}

typedef Eigen::Map< Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic,
                                  Eigen::RowMajorBit> > cv2eiMapU;
typedef Eigen::Map < Eigen::Matrix<float,Eigen::Dynamic,
                                   Eigen::Dynamic, Eigen::RowMajorBit>,
                     Eigen::AlignedBit > cv2eiMapF;
typedef Eigen::Map<const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic,
                                       Eigen::RowMajorBit>,
                   Eigen::AlignedBit> cv2ConsteiMapF;

using cv::Mat_;
using cv::Mat;
using std::vector;
namespace geo = tvs::geo;

namespace tvs {
//==============================================================================
int LinearShapeRegressor::save(const char* fname) {
  FILE *f = fopen(fname,"wb"); if (f == NULL) { return -1; }
  this->write(f); fclose(f); return 0;
}
//==============================================================================
int LinearShapeRegressor::load(const char* fname) {
  FILE *f = fopen(fname,"rb"); if (f == NULL) { return -1; }
  int ret = this->read(f); fclose(f); return ret;
}
//==============================================================================
int LinearShapeRegressor::write(FILE* f){
  if(fwrite(&_rpts.rows,sizeof(int),1,f) != 1)return -1;
  if((int)fwrite(_rpts.ptr<float>(0),sizeof(float),_rpts.rows,f)
     != _rpts.rows)return -1;
  if(tvs::io::writeMat(_evec,f) < 0)return -1;
  if(tvs::io::writeMat(_gain,f) < 0)return -1;
  if(tvs::io::writeMat(_bias,f) < 0)return -1;
  int val = 0; if((int)fwrite(&val,sizeof(int),1,f) != 1)return -1;
  return 0;
}

int LinearShapeRegressor::read(FILE* f){
  int n; if(fread(&n,sizeof(int),1,f) != 1)return -1;

  #ifdef ULS_USE_NEON
  cv::Mat_<float16_t > tmp_rpts;
  tmp_rpts.create(n,1);
  if((int)fread(tmp_rpts.ptr<float16_t >(0), sizeof(float16_t), n,f) != n) {return -1;}

   cv::Mat_<float16_t > tmp_evec, tmp_gain, tmp_bias;
   if (io::readMat(tmp_evec, f) < 0) { return -1; }
   if (io::readMat(tmp_gain, f) < 0) { return -1; }
   if (io::readMat(tmp_bias, f) < 0) { return -1; }
   int size_evec = tmp_evec.cols * tmp_evec.rows;
   int size_gain = tmp_gain.cols * tmp_gain.rows;
   int size_bias = tmp_bias.cols * tmp_bias.rows;

  _rpts.create(n ,1);
  for(int i = 0; i < n; i++) {
    _rpts(i) = tmp_rpts(i);
  }

  _evec.create(tmp_evec.rows, tmp_evec.cols);
  for(int i=0; i< size_evec; i++) {
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

  cv2eiMapF re(_rpts.ptr<float>(), _rpts.rows, _rpts.cols); _rptsE = re;
  cv2eiMapF be(_bias.ptr<float>(), _bias.rows, _bias.cols); _biasE = be;
  int val = 0; if (fread(&val, sizeof(int), 1, f) != 1) { return 0; }
  if (val == 0) { return 0;
  } else {
    return -1;
  }
}

cv::Mat_<float> LinearShapeRegressor::predict(Feature &H,
                                               const cv::Mat_<float> &pts) {
  H.extract(_vec, pts);
  cv::Mat_<float> M = geo::alignPoints2d(pts, _rpts);
  cv::Mat_<float> s = geo::transform2d(pts, M);
  cv::Mat_<float> Mi = geo::invertTransform2d(M);
  computeUpdate(s);
  geo::transform2d_inplace(s, Mi);
  return s;
}

void LinearShapeRegressor::computeUpdate(cv::Mat_<float> &pts) {
  if (_gain.cols == _vec.rows) {
    cv2eiMapF Ge(_gain.ptr<float>(), _gain.rows, _gain.cols);
    cv2eiMapF Ve(_vec.ptr<float>(), _vec.rows, _vec.cols);
    cv2eiMapF Ee(_evec.ptr<float>(), _evec.rows, _evec.cols);
    cv2eiMapF Be(_bias.ptr<float>(), _bias.rows, _bias.cols);
    cv2eiMapF Se(pts.ptr<float>(0), pts.rows, pts.cols);
    Se += Ee * (Be + Ge * Ve);
  } else { //per-point regressors
    int n = pts.rows / 2;
    int ndims = _vec.rows / n;
    if (_evec.cols == 2 * n) {
      Eigen::MatrixXf pe (2 * n, 1);
      for (int i = 0; i < n; i++) {
        cv2eiMapF ge(_gain.ptr<float>(2*i), 2, ndims);
        cv2eiMapF ve(_vec.ptr<float>(ndims*i), ndims, 1);
        pe.block(2*i, 0, 2, 1).noalias() = ge * ve;
      }
      cv2eiMapF se(pts.ptr<float>(0), pts.rows, pts.cols);
      cv2eiMapF ee(_evec.ptr<float>(0), _evec.rows, _evec.cols);
      cv2eiMapF be(_bias.ptr<float>(0), _bias.rows, _bias.cols);
      se += ee * pe + be;
    } else if (_evec.cols == 4 * n + 1) {

      if (_gain.rows == 2 * n) {
        Eigen::MatrixXf pe(4*n + 1, 1);
        pe(4*n) = 1.0f;
        for (int i = 0; i < n; i++) {
          cv2eiMapF ge(_gain.ptr<float>(2*i), 2, ndims);
          cv2eiMapF ve(_vec.ptr<float>(ndims*i), ndims, 1);
          pe.block(2*i, 0, 2, 1).noalias() = ge * ve;
        }

        cv2ConsteiMapF epts (pts.ptr<float>(), pts.rows, pts.cols);
        pe.block(0, 0, 2*n, 1) += _biasE;
        pe.block(2*n, 0, 2*n, 1).noalias() = epts - _rptsE;
        cv2eiMapF se(pts.ptr<float>(0), pts.rows, pts.cols);
        cv2eiMapF ee(_evec.ptr<float>(0), _evec.rows, _evec.cols);
        se += ee * pe;
      } else {
        printErr("Unsupported regression matrix\n"); exit(0);
      } //-----------------------------------------------------------
    }
  }
  return;
}
}  // namespace tvs
