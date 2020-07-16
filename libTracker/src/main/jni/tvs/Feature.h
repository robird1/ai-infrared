//
//  Feature.h
//  TVStrackeriOS - Android port with multi threading support
//  
//  Copyright 2014 TrueVisionSolutions Pty. Ltd.
//  All rights reserved
//  www.truevisionsolutions.net
//

#ifndef __TVStrackerAndroid__Feature__
#define __TVStrackerAndroid__Feature__

#include <memory>

#include <opencv2/core/core.hpp>

#include "utils/thread_pool.h"
namespace tvs {
struct FeatParameters {
  cv::Size size;
  int bins, smoothing, extSize;
  bool sign, gamma;
};
int saveFeatParameters(const char* fname, const FeatParameters &feat);
int loadFeatParameters(const char* fname, FeatParameters &feat);
int writeFeatParameters(FILE* f, const FeatParameters &feat);
int readFeatParameters(FILE* f, FeatParameters &feat);

class Feature{
public:
  Feature();
  Feature(Feature const&) = delete;
  Feature& operator=(Feature const&) = delete;
  Feature(Feature&&) = default;
  Feature& operator=(Feature&&) = default;
  ~Feature();

 // do not change the parameters between calls to setup() and extract()
  void setParameters(const cv::Size size, const bool gamma, const bool sign,
                     const int bins, const int smooth,
                     const int extractionSize) {
    _params.size = size;
    _params.gamma = gamma;
    _params.sign = sign;
    _params.bins = bins;
    _params.smoothing = smooth;
    _params.extSize = extractionSize;
  }
    
  void setup(const cv::Mat &I);
  
  void extract(cv::Mat_<float> &f, const cv::Mat_<float> &s,
               const cv::Mat_<uchar> &mask = cv::Mat());
  
protected:
  void multiThreadBoxFilter(cv::Mat &src, cv::Mat &dst, int size,
                            bool normalize);

  FeatParameters _params;
  cv::Size _srcImageSz;
  int _srcImageRotation;
  
  cv::Mat _graySrc;
  cv::Mat _tensor;
  cv::Mat _image, _tsmooth, _Gx, _Gy, _Ax, _Ay, _fcrop;
  cv::Mat_<float> _Ix, _Iy, _Im, _Ia;

  cv::Mat _tensor2C;
  cv::Mat _binIdx0, _binIdx1;
  
  cv::Mat _mem;

  //pool_t *_pool;
  std::unique_ptr<ThreadPool> _tpool;
  const int kThreadPoolSize = 8;
};
}  // namespace tvs



#endif /* defined(__TVStrackeriOS__fastHoG__) */
