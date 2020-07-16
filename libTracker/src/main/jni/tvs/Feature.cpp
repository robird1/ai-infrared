//
//  Feature.cpp
//  TVStrackeriOS - Android port
//
//  Copyright 2014 TrueVisionSolutions Pty. Ltd.
//  All rights reserved
//  www.truevisionsolutions.net
//


#include "Feature.h"

#include <math.h>

#include <android/log.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define GLM_FORCE_RADIANS // not required for GLM version >= 0.9.6
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <Eigen/Core>
#include <Eigen/Dense>
typedef Eigen::Map<
  Eigen::Matrix<unsigned char,Eigen::Dynamic,Eigen::Dynamic,
                Eigen::RowMajorBit>, Eigen::AlignedBit > cv2eiMapU;
typedef Eigen::Map<
  Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic,
                Eigen::RowMajorBit>, Eigen::AlignedBit > cv2eiMapI;
typedef Eigen::Map<
  Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic,
                Eigen::RowMajorBit>, Eigen::AlignedBit > cv2eiMapF;
typedef Eigen::Map<
  const Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic,
                      Eigen::RowMajorBit>, Eigen::AlignedBit> cv2eiConstMapF;
typedef Eigen::Map<
  Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic,
                Eigen::RowMajorBit>, Eigen::AlignedBit > cv2eiMapD;
typedef Eigen::Map<Eigen::VectorXf> cv2eiVecF;
typedef Eigen::Map<Eigen::Matrix<unsigned char, Eigen::Dynamic, 1> > cv2eiVecU;

typedef Eigen::Map<
  Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic, Eigen::RowMajorBit>,
  Eigen::AlignedBit, Eigen::OuterStride<> > cv2eiMapFS/*with stride*/;

using cv::Mat_;
using cv::Mat;
using std::atan2;

#ifdef TVS_INTERNAL_DEBUGGING
const char kLogTag[] = "Feature";
#define printDebug(...) {__android_log_print(ANDROID_LOG_DEBUG, kLogTag, __VA_ARGS__);}
#define printWarn(...) {__android_log_print(ANDROID_LOG_WARN, kLogTag, __VA_ARGS__);}
#define printInfo(...) {__android_log_print(ANDROID_LOG_INFO, kLogTag, __VA_ARGS__);}
#else
const char kLogTag[] = "UlsFaceTracker";
#define printDebug(...)
#define printWarn(...)
#define printInfo(...)
#endif

#define printErr(...) {__android_log_print(ANDROID_LOG_ERROR, kLogTag, __VA_ARGS__);}

//A box filter separated in horizontal and vertical filters
namespace {
struct FilterData {
  cv::Mat src; //src matrix
  cv::Mat dst; //destination
  int filterSize; //filter size
  //bool horizontal; //whether to use the horizontal filter
  float scaleFactor; // scale factor to multiply the result by
  unsigned int id; // thread id, for debugging
};

void horizontalFilter(FilterData *data) {
  //  struct FilterData *data = (FilterData*)data_;
  if (data->src.size() != data->dst.size()
      || data->src.channels() != data->dst.channels()) {
    printErr("Bad sizes or channel count\n");
    return;
  }
  
  const int w = data->src.cols, h = data->src.rows, c = data->src.channels();

  cv2eiMapFS S (data->src.ptr<float>(0), h, w * c,
                Eigen::OuterStride<>(data->src.step[0] / sizeof(float)));
  cv2eiMapFS D (data->dst.ptr<float>(0), h, w * c,
                Eigen::OuterStride<>(data->dst.step[0] / sizeof(float)));

  if (data->filterSize == 7) {
    //columns, across channels
    D.block(0, 3*c, h, (w-6) * c).noalias() = S.block(0, 0, h, (w-6)*c)
        + S.block(0, c, h, (w-6)*c) + S.block(0, 2*c, h, (w-6)*c)
        + S.block(0, 3*c, h, (w-6)*c) + S.block(0, 4*c, h, (w-6)*c)
        + S.block(0, 5*c, h, (w-6)*c) + S.block(0, 6*c, h, (w-6)*c);
    
    D.block(0, 0, h, c).noalias() = S.block(0, 0, h, c)
        + S.block(0,c, h, c)
        + S.block(0, 2*c, h, c)
        + S.block(0, 3*c, h, c);
    D.block(0, c, h, c).noalias() = S.block(0, 0, h, c) + S.block(0, c, h, c)
        + S.block(0, 2*c, h, c)
        + S.block(0, 3*c, h, c)
        + S.block(0, 4*c, h, c);

    D.block(0, 2*c, h, c).noalias() = S.block(0,0, h, c)
        + S.block(0, c, h, c)
        + S.block(0, 2*c, h, c)
        + S.block(0, 3*c, h, c)
        + S.block(0, 5*c, h, c);

    D.block(0, (w-1)*c, h, c).noalias() = S.block(0, (w-1)*c, h, c)
        + S.block(0,(w-2)*c, h, c)
        + S.block(0, (w-3)*c, h, c)
        + S.block(0, 4*c, h, c);
    D.block(0, (w-2)*c, h, c).noalias() = S.block(0, (w-1)*c, h, c)
        + S.block(0, (w-2)*c, h, c)
        + S.block(0, (w-3)*c, h, c)
        + S.block(0, (w-4)*c, h, c)
        + S.block(0, (w-5)*c, h, c);
    D.block(0, (w-3)*c, h, c).noalias() = S.block(0, (w-1)*c, h, c)
        + S.block(0, (w-2)*c, h, c)
        + S.block(0, (w-3)*c, h, c)
        + S.block(0, (w-4)*c, h, c)
        + S.block(0, (w-5)*c, h, c)
        + S.block(0, (w-6)*c, h, c);
  } else {
    D = S;//copy
    for (int i = 0; i < data->filterSize; i++) {
      int j = i - data->filterSize/2;
      if (j < 0) {
        D.block(0, -j * c, h, (w + j) * c) += S.block(0, 0, h, (w + j) * c);
      } else if (j > 0){
        D.block(0,  0, h, (w - j) * c) += S.block(0, j * c, h, (w - j) * c);
      }
    }    
  }
  if (data->scaleFactor != 1.0f) {
    D *=  data->scaleFactor;
  }
  return;
}
//=============================================================================
void verticalFilter(FilterData *data) {
  //  struct FilterData *data = (FilterData*)data_;
  if (data->src.size() != data->dst.size()
      || data->src.channels() != data->dst.channels()) {
    printErr("Bad sizes or channel count\n");
    return;
  }
  
  const int w = data->src.cols, h = data->src.rows, c = data->src.channels();
  cv2eiMapFS S (data->src.ptr<float>(0), h, w * c,
                Eigen::OuterStride<>(data->src.step[0] / sizeof(float)));
  cv2eiMapFS D (data->dst.ptr<float>(0), h, w * c,
                Eigen::OuterStride<>(data->dst.step[0] / sizeof(float)));
  
  //rows
  if (data->filterSize == 7) {
    const long columns = D.cols();
    D.block(3, 0, (h - 6), columns).noalias() = S.block(0, 0, h - 6, columns)
        + S.block(1, 0, h-6, columns) + S.block(2, 0, h-6, columns)
        + S.block(3, 0, h-6, columns) + S.block(4, 0, h-6, columns)
        + S.block(5, 0, h-6, columns) + S.block(6, 0, h-6, columns);

    D.row(0).noalias() = S.row(1) + S.row(2) +  S.row(0) + S.row(3);
    D.row(1).noalias() = S.row(0) + S.row(1) + S.row(2) + S.row(3) + S.row(4);
    D.row(2).noalias() = S.row(0) + S.block(1, 0, 5, columns).colwise().sum();
    
    D.row(h - 1).noalias() = S.row(h - 1) + S.row(h - 2) +  S.row(h - 3)
        + S.row(h - 4);
    D.row(h - 2).noalias() = S.row(h - 1) + S.row(h - 2) + S.row(h-3)
        + S.row(h-4) + S.row(h-5);
    D.row(h - 3).noalias() = S.row(h - 1) + S.block(h-6, 0, 5, columns)
        .colwise().sum();
  } else if (data->filterSize == 3) {
    S.block(0, c, h, (w-2)*c).noalias() = S.block(0, 0, h, (w-2)*c)
        + S.block(0, c, h, (w-2)*c) + S.block(0, 2*c, h, (w-2)*c);
    S.block(0,0,h,c).noalias() = S.block(0,0,h,c) + S.block(0,c,h,c);
    S.block(0, (w-1)*c, h, c) = S.block(0, (w-2)*c, h, c)
        + S.block(0, (w-1)*c, h, c);

    //rows
    const long columns = S.cols();
    D.block(1, 0, (h-2), columns).noalias() = S.block(0, 0, h-2, columns)
        + S.block(1, 0, h-2, columns) + S.block(2, 0, h-2, columns);
    D.block(1, 0, 1, columns).noalias() = S.block(0,0,1,columns)
        + S.block(1,0,1,columns);
    D.block(h-1, 0, 1, columns).noalias() = S.block(h-2, 0, 1, columns)
        + S.block(h-1, 0, 1, columns);
  } else {
    D = S; // copy
    for (int i = 0; i < data->filterSize; i++) {
      int j = i - data->filterSize/2;
      if (j < 0) {
        D.block(-j, 0, h + j, S.cols()) += S.block(0, 0, h + j, S.cols());
      } else if (j > 0) {
        D.block(0,  0, h - j, S.cols()) += S.block(j, 0, h - j, S.cols());
      }
    }
  }
  if (data->scaleFactor != 1.0f) {
    // printErr("Scaling %d - V %f", data->id, data->scaleFactor);
    D *= data->scaleFactor;
  }
  return;
}
//=============================================================================
/*void *separableFilter(void *data_) {
  struct FilterData *data = (FilterData*)data_;
  
  if (data->src.size() != data->dst.size()
      || data->src.channels() != data->dst.channels()) {
    fprintf(stderr, "Bad sizes or channel count\n");
    return NULL;
  }
  if (data->horizontal) horizontalFilter(data);
  else verticalFilter(data);
  return NULL;
  }*/
}  // namespace
//=============================================================================
//=============================================================================

namespace tvs {
//=============================================================================
int saveFeatParameters(const char* fname, const FeatParameters &feat) {
  FILE *f = fopen(fname,"wb");
  if (f == NULL) { return -1; }
  int ret = writeFeatParameters(f, feat);
  fclose(f); return ret;
}
//=============================================================================
int loadFeatParameters(const char* fname, FeatParameters &feat) {
  FILE *f = fopen(fname,"rb");
  if (f == NULL) { return -1; }
  int ret = readFeatParameters(f, feat);
  fclose(f); return ret;
}
//=============================================================================
int writeFeatParameters(FILE* f, const FeatParameters &feat) {
  if (fwrite(&feat.size, sizeof(cv::Size), 1, f) != 1) { return -1; }
  if (fwrite(&feat.bins, sizeof(int), 1, f) != 1) { return -1; }
  if (fwrite(&feat.smoothing, sizeof(int), 1, f) != 1) { return -1; }
  if (fwrite(&feat.extSize, sizeof(int), 1, f) != 1) { return -1; }
  int sign = static_cast<int>(feat.sign);
  int gamma = static_cast<int>(feat.gamma);
  if (fwrite(&sign, sizeof(int), 1, f) != 1) { return -1; }
  if (fwrite(&gamma, sizeof(int), 1, f) != 1) { return -1; }
  return 0;
}
//=============================================================================
int readFeatParameters(FILE* f, FeatParameters &feat) {
  if (fread(&feat.size, sizeof(cv::Size), 1, f) != 1) { return -1; }
  if (fread(&feat.bins, sizeof(int), 1, f) != 1) { return -1; }
  if (fread(&feat.smoothing, sizeof(int), 1, f) != 1) { return -1; }
  if (fread(&feat.extSize, sizeof(int), 1, f) != 1) { return -1; }
  int sign, gamma;
  if (fread(&sign, sizeof(int), 1, f) != 1) { return -1; }
  if (fread(&gamma, sizeof(int), 1, f) != 1) { return -1; }
  feat.sign = sign; feat.gamma = gamma;
  return 0;
}
//=============================================================================
Feature::Feature() {
  _tpool = std::unique_ptr<ThreadPool> (new ThreadPool(kThreadPoolSize));
}

Feature::~Feature() {
}

void Feature::setup(const cv::Mat &I) {
  if(I.channels() == 1){
    if(_image.size() != I.size())_image.create(I.size(),CV_32F);
    if(_Ix.size() != _image.size())_Ix.create(_image.size());
    if(_Iy.size() != _image.size())_Iy.create(_image.size());
    cv2eiMapU crop(I.data,I.rows,I.cols);
    cv2eiMapF im(_image.ptr<float>(0),_image.rows,_image.cols);
    cv2eiMapF Ix(_Ix.ptr<float>(0),_Ix.rows,_Ix.cols);
    cv2eiMapF Iy(_Iy.ptr<float>(0),_Iy.rows,_Iy.cols);
    im = crop.cast<float>(); if(_params.gamma)im = im.cwiseSqrt();
    Ix.block(0,1,Ix.rows(),Ix.cols()-2) =
    im.block(0,2,im.rows(),im.cols()-2) -
    im.block(0,0,im.rows(),im.cols()-2);
    Iy.block(1,0,Iy.rows()-2,Iy.cols()) =
    im.block(2,0,im.rows()-2,im.cols()) -
    im.block(0,0,im.rows()-2,im.cols());
    Ix.col(0) = im.col(1)-im.col(0);
    Iy.row(0) = im.row(1)-im.row(0);
    Ix.col(Ix.cols()-1) = im.col(im.cols()-1)-im.col(im.cols()-2);
    Iy.row(Iy.rows()-1) = im.row(im.rows()-1)-im.row(im.rows()-2);
  } else {
#ifdef DEBUG
    printErr("Only Eigen currently supported!");
#endif
    abort();
  }
  cv::cartToPolar(_Ix, _Iy, _Im, _Ia);

  cv::Size size = I.size();
  if((_tensor.size() != I.size()) || (_tensor.channels() != _params.bins)) {
    _tensor = Mat::zeros(size,CV_32FC(_params.bins));
  } else {
    _tensor = 0.0f;
  }
  float *tp = _Ia.ptr<float>(0),*vp = _Im.ptr<float>(0);
  float *Tp = _tensor.ptr<float>(0);
  int npix = size.width*size.height;
  if(!_params.sign){
    float t,z,v; int j1,j2,j3;
    for(int i = 0; i < npix; i++,Tp += _params.bins){
      t = *tp++; t = (t > CV_PI) ? t - CV_PI : t;
      z = *vp++; v = (t/CV_PI)*_params.bins;
      j1 = cv::min(int(v),_params.bins-1);
      j2 = j1+1;
      j3 = j2 % _params.bins;
      Tp[j1] = (j2-v)*z; Tp[j3] = (v-j1)*z;
    }
  }else{ float t,z,v; int j1,j2,j3;
    for(int i = 0; i < npix; i++,Tp += _params.bins){
      t = *tp++;
      z = *vp++;
      v = (0.5*t/CV_PI)*_params.bins;
      j1 = cv::min(int(v),_params.bins-1);
      j2 = j1+1;
      j3 = j2 % _params.bins;
      Tp[j1] = (j2-v)*z; Tp[j3] = (v-j1)*z;
    }
  }

  int cc = _params.smoothing; if(cc % 2 == 0)cc -= 1;
  // cv::boxFilter(_tensor, _tsmooth, -1, cv::Size(cc, cc), cv::Point(-1,-1),
  //               true);
  multiThreadBoxFilter(_tensor, _tsmooth, 7, true); 
  
  return;
}

//=============================================================================
void Feature::extract(cv::Mat_<float> &feature, const cv::Mat_<float> &s,
                        const cv::Mat_<uchar>& mask) {
  const int bins = _params.bins;
  const int width = _params.extSize/_params.smoothing;
  const int height = _params.extSize/_params.smoothing;
  const int n = s.rows/2;
  if (mask.empty()){
    if (feature.size() != cv::Size(1,width*height * bins*n))
      feature.create(width*height*bins*n,1);
  } else {
    int m = static_cast<int>(sum(mask)[0]);
    if (feature.size() != cv::Size(1,width*height * bins*m))
      feature.create(width*height*bins*m,1);
  }
  const int row_length = _tsmooth.cols;
  const int w = _tsmooth.cols-1, h = _tsmooth.rows-1;
  const float w2 = _params.extSize * 0.5f - _params.smoothing * 0.5f;
  const float h2 = _params.extSize * 0.5f - _params.smoothing * 0.5f;
  float *Tp = _tsmooth.ptr<float>(0);
  float *fp = feature.ptr<float>(0);
  for (int idx = 0; idx < n; idx++) {
    if (!mask.empty()) { if(!mask(idx)) { continue; } }
    float x0 = s(2 * idx);
    float y0 = s(2 * idx + 1);
    if ( (y0 < 0) || (y0 >= h) || (x0 < 0) || (x0 >= w) ) {
      int m = width * height * bins;
      for (int i = 0; i < m; i++) { *fp++ = 0.0f; }
      continue;
    }
    x0 -= w2; y0 -= h2;
    Mat_<float> fi (width * height * bins, 1, fp);
    int x1 = static_cast<int>(x0);
    int x2 = x1 + 1;
    int y1 = static_cast<int>(y0);
    int y2 = y1 + 1;
    const float wx1 = x2 - x0, wx2 = x0 - x1;
    const float wy1 = y2 - y0, wy2 = y0 - y1;
    const float v11 = wy1 * wx1, v12 = wy1 * wx2;
    const float v21 = wy2 * wx1, v22 = wy2 * wx2;
    for (int iy = 0; iy < height; iy++) {
      float y = y0 + static_cast<float>(iy * _params.smoothing);
      if ( (y < 0) || (y >= static_cast<float>(h)) ) {
        int m = width * bins;
        for (int i = 0; i < m; i++) { *fp++ = 0.0f; }
        continue;
      }
      for (int ix = 0; ix < width; ix++) {
        float x = x0 + static_cast<float>(ix * _params.smoothing);
        if ( (x < 0) || (x >= static_cast<float>(w)) ) {
          for (int i = 0; i < bins; i++) { *fp++ = 0.0f; }
          continue;
        }
        x1 = static_cast<int>(x);
        y1 = static_cast<int>(y);
        y2 = y1 + 1;

        cv2eiVecF dst (fp, bins);
        cv2eiVecF s11 (&Tp[(y1 * row_length + x1) * bins], bins);
        cv2eiVecF s12 (&Tp[(y1 * row_length + x1 + 1) * bins], bins);
        cv2eiVecF s21 (&Tp[(y2 * row_length + x1) * bins], bins);
        cv2eiVecF s22 (&Tp[(y2 * row_length + x1 + 1) * bins], bins);
        dst.noalias() = v11 * s11 + v12 * s12 + v21 * s21 + v22 * s22;

        fp += bins;
      }
    }
    cv2eiVecF efi (fi.ptr<float>(), fi.rows);
    efi /= sqrtf(efi.dot(efi)) + 1e-6f;
  }
  return;
}
//=============================================================================
void Feature::multiThreadBoxFilter(Mat &src, Mat &dst, int size,
                                   bool normalize) {
  std::vector<struct FilterData> data(kThreadPoolSize);
  dst.create(src.size(), src.type());
  _mem.create(src.size(), src.type());

  int step = src.rows/data.size();
  for (int i = 0; i < kThreadPoolSize; i++) {
    data.at(i).src = src.rowRange(i * step, (i+1) * step);
    data.at(i).dst = _mem.rowRange(i * step, (i+1) * step);
    data.at(i).filterSize = 7;
    //data.at(i).horizontal = true;
    data.at(i).scaleFactor = 1.0f;
    data.at(i).id = i;
    _tpool->enqueue(horizontalFilter, &(data.at(i)));
    //    pool_enqueue(_pool, &(data.at(i)), false);
  }
  //  pool_wait(_pool);
  _tpool->wait();

  step = src.cols/data.size();
  for (int i = 0; i < kThreadPoolSize; i++) {
    data.at(i).src = _mem.colRange( i * step, (i+1) * step);
    data.at(i).dst = dst.colRange( i * step, (i+1) * step);
    //data.at(i).horizontal = false;
    if (normalize) data.at(i).scaleFactor = 1.0f / (size * size);
    _tpool->enqueue(verticalFilter, &(data.at(i)));
    //    pool_enqueue(_pool, &(data.at(i)), false);
  }
  _tpool->wait();
  //  pool_wait(_pool);
}
//=============================================================================
}  // namespace tvs

