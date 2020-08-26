//
//  FacePosePredictor.h
//  TVStrackeriOS
//
//  Copyright 2014 TrueVisionSolutions Pty. Ltd.
//  All rights reserved
//  www.truevisionsolutions.net
//

#ifndef __TVStrackeriOS__FacePosePredictor__
#define __TVStrackeriOS__FacePosePredictor__

#include <opencv2/core/core.hpp>

namespace tvs {
class FacePosePredictor{
public:
  FacePosePredictor();
  
  int setShape(const cv::Mat_<float> &s3D);
  
  cv::Mat_<float> calcPose(const cv::Mat_<float> &s2D);
  cv::Mat_<float> calcPose(const cv::Mat_<float> &s2D,
                           const cv::Mat_<float> &s3D);
private:
  void init();

  cv::Mat_<float> xyz;
  cv::Mat_<float> _avg3D;
  cv::Mat_<float> _StS_St;
  bool _init2D = false;
};
}  // namespace tvs
#endif /* defined(__TVStrackeriOS__FacePosePredictor__) */
