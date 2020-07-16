//
//  FacePosePredictor.cpp
//  TVStrackeriOS - Android port
//
//  Copyright 2014 TrueVisionSolutions Pty. Ltd.
//  All rights reserved
//  www.truevisionsolutions.net
//

#include "face_pose_predictor.h"

#include <android/log.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#ifdef TVS_INTERNAL_DEBUGGING
const char kLogTag[] = "UlsFPS";
#else
const char kLogTag[] = "UlsFaceTracker";
#endif

#define printErr(...) {__android_log_print(ANDROID_LOG_ERROR, kLogTag, __VA_ARGS__);}

using cv::Mat_;
using cv::Mat;
using cv::SVD;
//=============================================================================
namespace tvs {
FacePosePredictor::FacePosePredictor() {
  _init2D = false;
  xyz = (Mat_<float>(198, 1) << -20.6547, -20.0519, -19.1146, -17.7568, -15.6469, -12.4006, -8.27017, -3.55056, 1.65875, 6.75338, 11.0958, 14.7165, 17.3322, 18.7221, 19.3316, 19.5157, 19.3623, -16.9434, -14.6923, -11.7473, -8.64008, -5.61118, 3.24342, 6.14406, 9.21432, 12.2557, 14.7675, -0.800008, -0.571382, -0.341526, -0.111056, -3.34691, -1.6415, 0.142254, 1.89493, 3.5165, -12.5316, -10.6698, -8.3451, -6.35058, -8.3556, -10.5048, 4.83678, 6.66716, 8.99246, 11.0209, 9.08377, 6.92444, -6.63855, -4.73902, -2.32888, 0.427259, 3.15408, 5.72158, 7.86034, 6.14905, 3.75933, 0.883179, -2.0422, -4.62825, -2.38864, 0.55887, 3.50364, 3.65722, 0.689237, -2.33557, -5.64822, -0.423649, 4.7667, 9.73356, 14.1974, 17.9009, 20.7526, 22.6779, 22.9008, 21.9412, 19.3665, 15.9593, 11.8354, 7.12069, 2.01325, -3.2566, -8.51341, -14.0024, -16.1532, -17.1684, -17.2567, -16.6863, -17.3161, -18.3113, -18.6656, -18.0793, -16.2699, -11.6067, -8.59445, -5.60559, -2.62069, 0.90089, 1.2941, 1.35258, 1.04463, 0.412997, -9.74495, -11.0691, -11.2534, -10.2247, -9.50683, -9.27615, -11.0227, -12.3254, -12.4738, -11.4279, -10.6753, -10.5977, 8.84013, 6.8457, 5.4219, 5.35547, 5.03248, 6.09946, 7.80446, 9.90529, 11.3254, 11.8243, 11.738, 10.6744, 7.57584, 7.21472, 7.15671, 8.62822, 9.10764, 9.05505, 21.3079, 20.8425, 20.4846, 18.3869, 13.4633, 7.99404, 3.29433, -0.880153, -4.53572, -0.807894, 3.41769, 8.15704, 13.6544, 18.5937, 20.7063, 21.0748, 21.5385, 0.166523, -1.56921, -2.95979, -4.29633, -5.52226, -5.43542, -4.17859, -2.80935, -1.38764, 0.371491, -4.9545, -7.76035, -10.6633, -13.4075, -6.47077, -7.26957, -8.01839, -7.21848, -6.40175, -0.905324, -1.10649, -1.2279, -0.977391, -1.10156, -1.09011, -0.887733, -1.11779, -0.968074, -0.742362, -0.948989, -0.987214, -2.74685, -4.89305, -6.54708, -8.22771, -6.48681, -4.80547, -2.63884, -3.93286, -5.28293, -6.67982, -5.34336, -4.02161, -5.20395, -7.41824, -5.14457, -4.49552, -6.42523, -4.5521);
}

int FacePosePredictor::setShape(const cv::Mat_<float> &s3D) {
  if (s3D.cols == 1) {
    int n = s3D.rows / 3;
    xyz.create(3 * n, 1);
    for (int i = 0; i < n; i++) {
      xyz(i) = s3D(3 * i);
      xyz(i + n) = s3D(3 * i + 1);
      xyz(i + n * 2) = s3D(3 * i + 2);
    }
  } else {
    printErr("Unsupported shape vector in setShape\n"); return -1;
  }
  _init2D = false;
  return 0;
}

Mat_<float> FacePosePredictor::calcPose(const Mat_<float> &s2D) {
  int n = s2D.rows / 2; //double t2[2], t3[3];
  if (!_init2D) init();
  Mat_<float> X = (s2D.reshape(1,n)).clone();

  Mat_<float> avg2d;
  cv::reduce(X, avg2d, 0, CV_REDUCE_AVG);
  X.col(0) -= avg2d(0);
  X.col(1) -= avg2d(1);
  Mat_<float> M = _StS_St * X;
  Mat_<float> MtM = M.t() * M; SVD svd(MtM, SVD::MODIFY_A);
  svd.w.at<float>(0,0) = 1.0f/sqrtf(svd.w.at<float>(0,0));
  svd.w.at<float>(1,0) = 1.0f/sqrtf(svd.w.at<float>(1,0));
  Mat_<float> T(3, 3);
  T(cv::Rect(0,0,3,2)) = svd.u * cv::Mat::diag(svd.w) * svd.vt * M.t();
  float scale = 0.5f * sum(T(cv::Rect(0,0,3,2)).mul(M.t()))[0];
  {
    T(2,0) = T(0,1) * T(1,2) - T(0,2) * T(1,1);
    T(2,1) = T(0,2) * T(1,0) - T(0,0) * T(1,2);
    T(2,2) = T(0,0) * T(1,1) - T(0,1) * T(1,0);
  }
  Mat_<float> r; Rodrigues(T, r); T *= scale;
  float x = avg2d(0) - (T(0,0)*_avg3D(0) + T(0,1)*_avg3D(1) + T(0,2)*_avg3D(2));
  float y = avg2d(1) - (T(1,0)*_avg3D(0) + T(1,1)*_avg3D(1) + T(1,2)*_avg3D(2));

  return (Mat_<float>(6, 1) << r(0), r(1), r(2), x, y, scale);
}

Mat_<float> FacePosePredictor::calcPose(const Mat_<float> &s2D,
                                        const Mat_<float> &s3D) {
  int n = s2D.rows / 2;

  Mat_<float> X = (s2D.reshape(1,n)).clone();
  Mat_<float> avg2d;
  cv::reduce(X, avg2d, 0, CV_REDUCE_AVG);
  X.col(0) -= avg2d(0);
  X.col(1) -= avg2d(1);

  cv::Mat_<float> avg3D;
  cv::Mat_<float> S = s3D.reshape(1,3).t();  //3D shape is x,x,...,y,y,...,z,z??
  cv::reduce(S, avg3D, 0, CV_REDUCE_AVG);
  S.col(0) -= avg3D(0);
  S.col(1) -= avg3D(1);
  S.col(2) -= avg3D(2);

  Mat_<float> M = ((S.t() * S).inv(cv::DECOMP_CHOLESKY)) * S.t() * X;
  Mat_<float> MtM = M.t() * M; SVD svd(MtM, SVD::MODIFY_A);
  svd.w.at<float>(0,0) = 1.0f/sqrtf(svd.w.at<float>(0,0));
  svd.w.at<float>(1,0) = 1.0f/sqrtf(svd.w.at<float>(1,0));
  Mat_<float> T(3, 3);
  T(cv::Rect(0,0,3,2)) = svd.u * cv::Mat::diag(svd.w) * svd.vt * M.t();
  float scale = 0.5 * sum(T(cv::Rect(0,0,3,2)).mul(M.t()))[0];
  {
    T(2,0) = T(0,1) * T(1,2) - T(0,2) * T(1,1);
    T(2,1) = T(0,2) * T(1,0) - T(0,0) * T(1,2);
    T(2,2) = T(0,0) * T(1,1) - T(0,1) * T(1,0);
  }
  Mat_<float> r; Rodrigues(T, r); T *= scale;
  float x = avg2d(0) - (T(0,0)*avg3D(0) + T(0,1)*avg3D(1) + T(0,2)*avg3D(2));
  float y = avg2d(1) - (T(1,0)*avg3D(0) + T(1,1)*avg3D(1) + T(1,2)*avg3D(2));

  return (Mat_<float>(6, 1) << r(0), r(1), r(2), x, y, scale);
}
  
void FacePosePredictor::init() {
  cv::Mat_<float> S = xyz.reshape(1,3).t();
  
  cv::reduce(S, _avg3D, 0, CV_REDUCE_AVG);
  S.col(0) -= _avg3D(0);
  S.col(1) -= _avg3D(1);
  S.col(2) -= _avg3D(2);
  xyz = S;
  
  _StS_St = ((S.t() * S).inv(cv::DECOMP_CHOLESKY)) * S.t();
  _init2D = true;
}

}  // namespace tvs
