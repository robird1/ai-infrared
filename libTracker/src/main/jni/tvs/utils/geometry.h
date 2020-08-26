/**
   tvsTracker: TrueVisionSolutions Face Tracker Interface
   Copyright 2014 TrueVisionSolutions Pty. Ltd.
   All rights reserved
   www.truevisionsolutions.net
*/
#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <opencv2/core/core.hpp>

namespace tvs {
namespace geo {
//=============================================================================
cv::Rect calcBoundingBox(const cv::Mat_<float> &pts2D);

cv::Mat_<float> alignPoints2d(const cv::Mat_<float> &src,
                              const cv::Mat_<float> &dst);

cv::Mat_<float> invertTransform2d(const cv::Mat_<float> &M);

cv::Mat_<float> transform2d(const cv::Mat_<float> &src,
                            const cv::Mat_<float> &M);

void transform2d_inplace(cv::Mat_<float> &src, const cv::Mat_<float> &M);
//=============================================================================
}  // namespace geo
}  // namespace tvs

#endif  // __GEOMETRY_H__
