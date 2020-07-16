/**
   tvsTracker: TrueVisionSolutions Face Tracker Interface
   Copyright 2014 TrueVisionSolutions Pty. Ltd.
   All rights reserved
   www.truevisionsolutions.net
*/
#include "geometry.h"

#include <stdio.h>

using cv::Mat_;
using cv::Mat;

namespace {
//=============================================================================
Mat_<float> inv2x2(const Mat_<float> &A, const float tol = 1e-8f) {
  if (A.size() != cv::Size(2, 2)) { return Mat(); }
  float a = A(0, 0), b = A(0, 1), c = A(1, 0), d = A(1, 1);
  double det = a * d - b * c; 
  if (det < tol) { a += tol; d += tol; det = a * d - b * c; }
  return (Mat_<float>(2, 2) << static_cast<float>(d / det),
          static_cast<float>(-b / det), static_cast<float>(-c / det),
          static_cast<float>(a / det));
}
//=============================================================================
}
namespace tvs {
namespace geo {
//=============================================================================
cv::Rect calcBoundingBox(const Mat_<float> &pts) {
  int n = pts.rows/2;
  Mat_<float> s = pts.clone();
  Mat_<float> xy = s.reshape(1, n);
  Mat_<float> x = xy.col(0), y = xy.col(1);
  double xmin, xmax; minMaxLoc(x, &xmin, &xmax);
  double ymin, ymax; minMaxLoc(y, &ymin, &ymax);
  return cv::Rect(static_cast<int>(xmin), static_cast<int>(ymin),
                  static_cast<int>(xmax - xmin), static_cast<int>(ymax - ymin));
}

//=============================================================================
Mat_<float> alignPoints2d(const Mat_<float> &src, const Mat_<float> &dst) {
  if (src.size() != dst.size()) {
    fprintf(stderr, "Incompatible point sets to align\n");
    return Mat();
  }
  int n = src.rows / 2;
  Mat_<float> A(2 * n, 4);
  const float *p = src.ptr<float>(0);
  float *a = A.ptr<float>(0);
  for(int i = 0; i < n; i++){
    float x = *p++,y = *p++;
    *a++ = x; *a++ = -y; *a++ = 1; *a++ = 0;
    *a++ = y; *a++ =  x; *a++ = 0; *a++ = 1;
  }
  Mat_<float> H = A.t() * A;
  Mat_<float> g = A.t() * dst;
  Mat_<float> u;
  cv::solve(H, g, u, cv::DECOMP_CHOLESKY);
  return (Mat_<float>(2,3) << u(0), -u(1), u(2), u(1), u(0), u(3));
}
//=============================================================================
Mat_<float> invertTransform2d(const Mat_<float> &M) {
  if (M.size() != cv::Size(3, 2)) { return Mat(); }
  Mat_<float> R = M(cv::Rect(0, 0, 2, 2));
  Mat_<float> A = inv2x2(R);
  Mat_<float> t = -A * M.col(2);
  return (Mat_<float>(2,3) << A(0, 0), A(0, 1), t(0), A(1, 0), A(1, 1), t(1));
}
//=============================================================================
Mat_<float> transform2d(const Mat_<float> &src, const Mat_<float> &M) {
#ifdef DEBUG
  assert(M.size() == cv::Size(3, 2));
#endif
  Mat_<float> pts = src.reshape(0,(src.rows*src.cols)/2);
  Mat_<float> dst = pts * (M.colRange(0,2)).t();
  dst.col(0) += M(0,2);
  dst.col(1) += M(1,2);
  return dst.reshape(0,dst.rows * dst.cols);
}
//=============================================================================
void transform2d_inplace(Mat_<float> &src, const Mat_<float> &M) {
#ifdef DEBUG
  assert(M.size() == cv::Size(3, 2));
#endif
  Mat_<float> pts = src.reshape(0, (src.rows * src.cols)/2);
  pts = pts * M.colRange(0,2).t();
  pts.col(0) += M(0,2);
  pts.col(1) += M(1,2);
  return;
}
//=============================================================================
}  // namespace geometry
}  // namespace tvs
