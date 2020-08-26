//
//  FaceTracker.h
//  TVStrackerAndroid
//
//  Copyright 2014 TrueVisionSolutions Pty. Ltd.
//  All rights reserved
//  www.truevisionsolutions.net
//

#ifndef __TVS_FACE_TRACKER_H__
#define __TVS_FACE_TRACKER_H__

#include <string>
#include <vector>

#include <android/log.h>
#include <android/asset_manager.h>

#include <opencv2/core/core.hpp>

#include "Feature.h"
#include "FeatureComp.h"
#include "ColorConverter.h"
#include "linear_shape_regressor.h"
#include "linear_shape_classifier.h"
#include "face_pose_predictor.h"
#include "optical_flow_tracker_efficient.h"
#include "utils/linear_model.h"
#include "utils/android_assets.h"

#include "GrayInputConversion.h"

//==============================================================================

namespace tvs {
class FaceTracker {
 public:
  FaceTracker();
  FaceTracker(FaceTracker const&) = delete;
  FaceTracker& operator=(FaceTracker const&) = delete;
  FaceTracker(FaceTracker&&) = default;
  FaceTracker& operator=(FaceTracker&&) = default;

  int loadModels(AAssetManager* mgr, const std::string& cacheDir, const std::string& sPackageName);
  int loadModels(const std::string& folder, const std::string& sPackageName);
  /* texture - index of the OpenGL texture
   * sz - size of the source texture.
   */
  bool initialiseWithSrcTexture(GLint texture, const cv::Size& textureSize);
  bool isTracking() { return _tracking; }
  bool resetWithFaceRect(const cv::Rect& rect, const int rotation);

    bool resetWith5Points(std::vector<cv::Point2f> five_points);

  // inline void setShape(const cv::Mat_<float>& shape);
  void setThreshold(const float threshold) { _health_thresh = threshold; }

  bool update(const bool predictPupils, const bool highPrecision,
              const bool smooth);

  //Use this transformation for warp/crop in the GPU
  cv::Mat_<float> computeTransformation();
  //Use this method for warp/crop using cv::warpAffine
  cv::Mat_<float> computeTransformationWA();
  //this call doesn't use the GPU at all. The matrix M is the same as
  //returned by computeTransformation(). If GrayInputConversion is
  //NULL, no smoothing is used
  bool update(const cv::Mat&crop, const cv::Mat_<float>& M,
              const bool predictPupils, const bool highPrecision,
              GrayInputConversion *smoothData);
  
  const cv::Size getFeatureSize() {return _feature_params.size;}
  const unsigned int getPointCount() {
    return _center_refShape.rows / 2;
  }

  const cv::Mat_<float> getShape() {
    // Debug only.
//    __android_log_print(ANDROID_LOG_DEBUG, "jjj", "shape mat row %d col %d", _shape.rows, _shape.cols);
//    __android_log_print(ANDROID_LOG_DEBUG, "jjj", "shape pnt #1 %f, %f", *(_shape.ptr<float>(0)), *(_shape.ptr<float>(1)));
    _shape.copyTo(_shapeRotated);

	if (POSE_ROLL_ROTATE_DEGREE >= 0.1 || POSE_ROLL_ROTATE_DEGREE <= -0.1) {
      float xSum = 0, ySum = 0, xMean, yMean;
      int numPnt = _shapeRotated.rows/2;
      for (int i = 0; i < numPnt; i++) {
	    xSum += *(_shapeRotated.ptr<float>(i*2));
	    ySum += *(_shapeRotated.ptr<float>(i*2 + 1));
      }

      xMean = xSum / numPnt;
      yMean = ySum / numPnt;

      double poseRotation = POSE_ROLL_ROTATE_DEGREE * M_PI / 180.0;
	  float sinVal = sin(poseRotation),
	        cosVal = cos(poseRotation);
      for (int i = 0; i < numPnt; i++) {
	    float xShifted = *(_shapeRotated.ptr<float>(i*2)) - xMean;
	    float yShifted = *(_shapeRotated.ptr<float>(i*2 + 1)) - yMean;
	    float xRotated = cosVal * xShifted - sinVal * yShifted;
	    float yRotated = sinVal * xShifted + cosVal * yShifted;
	    *(_shapeRotated.ptr<float>(i*2)) = xRotated + xMean;
	    *(_shapeRotated.ptr<float>(i*2 + 1)) = yRotated + yMean;
      }
	}

	return _shapeRotated;
  }

  const cv::Mat_<float> getShape3D() { return _shape3D; }
  const cv::Mat_<float> getPupilLocation() { return _pupils; }
  const cv::Mat_<float> getConfidence() { return _confidence; }
  const cv::Mat_<float> getPose() {
	_pose.copyTo(_poseRollRotated);

	if (POSE_ROLL_ROTATE_DEGREE >= 0.1 || POSE_ROLL_ROTATE_DEGREE <= -0.1) {
      float* pRollAngle = _poseRollRotated.ptr<float>(2);
      *pRollAngle += POSE_ROLL_ROTATE_DEGREE * M_PI / 180.0;
	}

	return _poseRollRotated;
  }
  float getPoseQuality() { return 0.7f; }
  const cv::Mat_<float> getGaze() { return _gaze; }

  // Modified.
  void setFaceRegionBrightness(float faceRegionBrightness) { _faceRegionBrightness = faceRegionBrightness; }
  float getFaceRegionBrightness() { return _faceRegionBrightness; }

    void release();

 private:
  void calcGaze(const cv::Mat_<float> &s3D,	const cv::Mat_<float> &pose,
                const cv::Point2f leye, const cv::Point2f reye,
                cv::Point3f &ldir, cv::Point3f &rdir);
  void calcShape3D(const cv::Mat_<float> &s2D, cv::Mat_<float> &s3D,
                   cv::Mat_<float> &pose);
  cv::Mat_<float> calcCrop(const cv::Point2f p, const float scale,
                           const float theta);

  int loadTrackerFromFile(const std::string& fn, const std::string& sPackageName);
  int load3DPointsFromFile(const std::string& fn);

  GLint _srcTexture;
  tvs::Feature _feature;
  FeatureComp _featComp;
  tvs::FeatParameters _feature_params;
  ColorConverter _colorConv;
  
//  tvs::LinearShapeClassifier _center_classifier, _left_classifier,
//    _right_classifier;

#ifdef USE_SHARE_MEMORY
  static tvs::LinearShapeClassifier _center_classifier, _left_classifier, _right_classifier;
  static std::vector<tvs::LinearShapeRegressor> _center_predictor;
  static std::vector<tvs::LinearShapeRegressor> _left_predictor;
  static std::vector<tvs::LinearShapeRegressor> _right_predictor;
  static std::vector<tvs::LinearShapeRegressor> _pupil_predictor;
#else

  tvs::LinearShapeClassifier _center_classifier, _left_classifier, _right_classifier;
  //static tvs::LinearShapeClassifier _center_classifier;
  tvs::LinearShapeClassifier _left_classifier, _right_classifier;

  std::vector<tvs::LinearShapeRegressor> _center_predictor;
  std::vector<tvs::LinearShapeRegressor> _left_predictor;
  std::vector<tvs::LinearShapeRegressor> _right_predictor;
  std::vector<tvs::LinearShapeRegressor> _pupil_predictor;
#endif

#ifdef USE_SHARE_MEMORY
  static tvs::LinearModel _center_smodel, _left_smodel, _right_smodel;
  static tvs::LinearModel _smodel3d;
  static tvs::LinearModel _smodel3d_nojaw;
#else
  tvs::LinearModel _center_smodel, _left_smodel, _right_smodel;
  tvs::LinearModel _smodel3d;
  tvs::LinearModel _smodel3d_nojaw;
#endif

  tvs::FacePosePredictor _pose_predictor;
  tvs::OpticalFlowTrackerEfficient _smoother;

  // eye points indices.
  cv::Mat_<int> _right_eye_idx, _left_eye_idx;
  cv::Mat_<int> _nojaw_idx;
  
  // same as _regressors.at(0)._rpts, but centered at (0,0)
  const float POSE_ROLL_ROTATE_DEGREE = 0.0f;
  cv::Mat_<float> _center_refShape, _left_refShape, _right_refShape; 
  cv::Mat_<float> _shape, _shape3D, _shapeRotated, _shape3DRotated;
  cv::Mat_<float> _pupils;
  cv::Mat_<float> _pose, _poseRollRotated;
  cv::Mat_<float> _confidence;
//  cv::Mat_<float> _shape3d;
  cv::Mat_<float> _gaze; //6x1 [left eye gaze (x,y,z); right eye gaze (x,y,z)]

  float _yawAngle = 0.0f;
  bool _tracking = false;
  bool _dataSetup = false;
  float _health_thresh = 0.3f;

  cv::Size _srcTextureSize;
  bool _has_feat, _has_smodel3d;

  bool _oglInit = false;

  cv::Mat_<uchar> crop_;

  // Modified.
  std::string _cacheDir;
  float _faceRegionBrightness;



    // five points for pose
    cv::Mat s3D_5_points =
            (cv::Mat_<float>(5, 3) << -8.62339205, -10.72872357, -0.016413,
                    9.01201214, -10.72574278, -0.45656117, -0.01742033, -1.22387422,
                    -12.18645271, -6.99745019, 8.33293748, -2.69594813, 7.21358316,
                    8.31900264, -3.04771769);
    // 66 points for pose
    cv::Mat s3D_66_points =
            (cv::Mat_<float>(66, 3) << -20.185564, -7.145763, 21.307899, -19.967024,
                    -1.891076, 20.842501, -19.412371, 3.353981, 20.484600, -18.421980,
                    8.406944, 18.386900, -16.644670, 13.013322, 13.463300, -13.678328,
                    16.944630, 7.994040, -9.767842, 20.091175, 3.294330, -5.201912,
                    22.356962, -0.880153, -0.022917, 22.960785, -4.535720, 5.128310,
                    22.376883, -0.807894, 9.647635, 20.127127, 3.417690, 13.508148,
                    16.994251, 8.157040, 16.418850, 13.072994, 13.654400, 18.150314,
                    8.472739, 18.593700, 19.132236, 3.423655, 20.706301, 19.701797,
                    -1.818560, 21.074800, 19.933809, -7.072487, 21.538500, -15.872388,
                    -15.205700, 0.166523, -13.469811, -17.185856, -1.569210, -10.458370,
                    -17.982645, -2.959790, -7.353028, -17.843142, -4.296330, -4.374037,
                    -17.052441, -5.522260, 4.502909, -17.032055, -5.435420, 7.468646,
                    -17.812145, -4.178590, 10.556608, -17.940634, -2.809350, 13.546882,
                    -17.133162, -1.387640, 15.919419, -15.144661, 0.371491, 0.052194,
                    -11.634121, -4.954500, 0.059594, -8.613216, -7.760350, 0.069934,
                    -5.615549, -10.663300, 0.081177, -2.621786, -13.407500, -3.403901,
                    0.653349, -6.470770, -1.731869, 1.170404, -7.269570, 0.042811, 1.359366,
                    -8.018390, 1.813334, 1.180606, -7.218480, 3.476809, 0.669430, -6.401750,
                    -11.784244, -10.636571, -0.905324, -9.830465, -11.820810, -1.106490,
                    -7.498511, -11.834358, -1.227900, -5.584687, -10.662346, -0.977391,
                    -7.636899, -10.093248, -1.101560, -9.797220, -10.020590, -1.090110,
                    5.631073, -10.638862, -0.887733, 7.551945, -11.804009, -1.117790,
                    9.881868, -11.781710, -0.968074, 11.828261, -10.590059, -0.742362,
                    9.841214, -9.981352, -0.948989, 7.682000, -10.062106, -0.987214,
                    -7.268157, 8.330194, -2.746850, -5.227660, 6.480238, -4.893050,
                    -2.719716, 5.236776, -6.547080, 0.033887, 5.372379, -8.227710, 2.777040,
                    5.249964, -6.486810, 5.259501, 6.502118, -4.805470, 7.267647, 8.359178,
                    -2.638840, 5.407092, 10.329034, -3.932860, 2.919783, 11.570313,
                    -5.282930, 0.014817, 11.857227, -6.679820, -2.896385, 11.556911,
                    -5.343360, -5.397594, 10.306769, -4.021610, -2.937066, 7.380555,
                    -5.203950, 0.028976, 7.236275, -7.418240, 2.970087, 7.394091, -5.144570,
                    3.015483, 8.872896, -4.495520, 0.020359, 9.133659, -6.425230, -2.992473,
                    8.859679, -4.552100);

    // five points
    cv::Mat S_five;
    cv::Mat_<float> five_avg3D;

    // 66 points
    cv::Mat S_ss;
    cv::Mat_<float> S_ss_avg3D;
};
}  // namespace tvs
#endif //  __TVS_FACE_TRACKER_H__
