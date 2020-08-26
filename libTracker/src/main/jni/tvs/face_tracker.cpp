//
//  Tracker.cpp
//  TVStrackerAndroid
//
//  Copyright 2014 TrueVisionSolutions Pty. Ltd.
//  All rights reserved
//  www.truevisionsolutions.net
//

#include "face_tracker.h"
#include <mutex>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <android/log.h>

#include "utils/geometry.h"

#include <android/log.h>

#ifdef TVS_INTERNAL_DEBUGGING
const char kLogTag[] = "UlsTracker";
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

namespace geo = tvs::geo;
using cv::Mat_;
using cv::Mat;


//==============================================================================
namespace {
cv::Mat_<float> centerShape(const cv::Mat_<float>& src) {
  cv::Mat_<float> mean, mx, mn;
  int n = src.rows/2;
  cv::reduce(src.reshape(0, n), mx, 0, CV_REDUCE_MAX);
  cv::reduce(src.reshape(0, n), mn, 0, CV_REDUCE_MIN);
  cv::Mat_<float> s = src.clone().reshape(0, n);
  s.col(0) -= mn(0) + (mx(0) - mn(0))/2.0f;
  s.col(1) -= mn(1) + (mx(1) - mn(1))/2.0f;
  return s.reshape(0,n*2);
}
}  // namespace
//==============================================================================
namespace tvs {

#ifdef USE_SHARE_MEMORY
    LinearShapeClassifier FaceTracker::_center_classifier = LinearShapeClassifier();
    LinearShapeClassifier FaceTracker::_left_classifier = LinearShapeClassifier();
    LinearShapeClassifier FaceTracker::_right_classifier = LinearShapeClassifier();

    std::vector<LinearShapeRegressor> FaceTracker::_center_predictor;
    std::vector<LinearShapeRegressor> FaceTracker::_left_predictor;
    std::vector<LinearShapeRegressor> FaceTracker::_right_predictor;
    std::vector<LinearShapeRegressor> FaceTracker::_pupil_predictor;

    LinearModel FaceTracker::_center_smodel = LinearModel();
    LinearModel FaceTracker::_left_smodel = LinearModel();
    LinearModel FaceTracker::_right_smodel = LinearModel();

    LinearModel FaceTracker::_smodel3d = LinearModel();
    LinearModel FaceTracker::_smodel3d_nojaw = LinearModel();

    std::mutex mtx;

    //    FeatParameters FaceTracker::_feature_params = FeatParameters::FeatParameters();
#endif

//LinearShapeClassifier FaceTracker::_center_classifier = LinearShapeClassifier::LinearShapeClassifier();
//LinearShapeClassifier FaceTracker::_center_classifier;


FaceTracker::FaceTracker() {

  //set default feature params (optionally overwritten in read function)
  _feature_params.size = cv::Size(128, 128);
  _feature_params.bins = 16;
  _feature_params.smoothing = 8;
  _feature_params.sign = true;
  _feature_params.gamma = true;
  _feature_params.extSize = 32;

  //set initial values for other things
  _health_thresh = 0.3f; // default=0.3
  _shape = Mat();
  _shapeRotated = Mat();
  _pose = Mat();
  _poseRollRotated = Mat();
  _confidence = Mat();
  _has_feat = false;
  _has_smodel3d = false;

  _feature.setParameters(_feature_params.size, _feature_params.gamma,
                         _feature_params.sign, _feature_params.bins,
                         _feature_params.smoothing, _feature_params.extSize);


  // new add
  S_five = s3D_5_points.clone();
  cv::reduce(S_five, five_avg3D, 0, CV_REDUCE_AVG);
  S_five.col(0) -= five_avg3D(0);
  S_five.col(1) -= five_avg3D(1);
  S_five.col(2) -= five_avg3D(2);

  S_ss = s3D_66_points.clone();
  cv::reduce(S_ss, S_ss_avg3D, 0, CV_REDUCE_AVG);
  S_ss.col(0) -= S_ss_avg3D(0);
  S_ss.col(1) -= S_ss_avg3D(1);
  S_ss.col(2) -= S_ss_avg3D(2);
}

int FaceTracker::loadModels(AAssetManager* mgr, const std::string& cacheDir, const std::string& sPackageName) {
  // Modified. Keep the cache directory for usage in TimeBomb.
  _cacheDir = cacheDir;

  const std::string tmpFN = cacheDir+"/tracker.tmp";
  if (!copyFileFromAsset(mgr, "ULSFaceTrackerAssets/ulsTracker.model", tmpFN))
    return -1;

  if (loadTrackerFromFile(tmpFN, sPackageName) < 0) {
    printErr("Error loadTrackerFromFile  path: %s sPackageName: %s ", tmpFN.c_str(), sPackageName.c_str());
    unlink(tmpFN.c_str());
    return -2;
  }
  if (!_has_smodel3d) {
    if (!copyFileFromAsset(mgr, "ULSFaceTrackerAssets/3d.pts", tmpFN))
      return -3;

    if (load3DPointsFromFile(tmpFN) < 0) {
      unlink(tmpFN.c_str());
      return -4;
    }
    unlink(tmpFN.c_str());
  }
  _dataSetup = true;

  return 0;
}

int FaceTracker::loadModels(const std::string& folder, const std::string& sPackageName) {
  if (loadTrackerFromFile(folder + "/ulsTracker.model", sPackageName) < 0) return -1;
  if (!_has_smodel3d) {
    if (load3DPointsFromFile(folder + "/3d.pts") < 0) return -2;
  }
  _dataSetup = true;
  return 0;
}
//==============================================================================
bool FaceTracker::initialiseWithSrcTexture(GLint texture, const cv::Size& sz) {
  if (!_dataSetup) return false;
  _srcTexture = texture;

  printInfo("feat comp initialisation");
  _featComp.setSizes(sz, _feature_params.size);
  _featComp.initialiseGPU();
  printInfo("feat comp initialisation done");

  printInfo("color conv initialisation");
  _colorConv.initialise(sz);
  printInfo("color conv initialisation done");

  _srcTextureSize = sz;

  _oglInit = true;
  return true;
}

//==============================================================================
bool FaceTracker::resetWithFaceRect(const cv::Rect& r, const int rotation) {
  printInfo("resetting\n");
  if (r.x > 0) {
    _center_refShape.copyTo(_shape);
    float rollAngle = 0.0f;
    if (rotation == 90) rollAngle = M_PI_2;
    else if (rotation == 180) rollAngle = M_PI;
    else if (rotation == 270) rollAngle = 1.5 * M_PI;
    cv::Mat_<float> Mt = (cv::Mat_<float>(2,2) <<
                          cosf(rollAngle), sinf(rollAngle),
                          -sinf(rollAngle), cosf(rollAngle));
    _shape = _shape.reshape(0, _shape.rows * _shape.cols/2) * Mt;
    _shape = _shape.reshape(0, _shape.rows * _shape.cols);
    const int n = _shape.rows * _shape.cols / 2;
    cv::Mat_<float> x,y, xy = _shape.reshape(0, n);
    x = xy.col(0);
    y = xy.col(1);

    double mn, mx;
    cv::minMaxLoc(x, &mn, &mx);
    x *= r.width / (mx - mn);
    cv::minMaxLoc(y, &mn, &mx);
    y *= r.height / (mx - mn);
    x += r.x + r.width / 2.0f - sum(x)[0] / n;
    y += r.y + r.height / 2.0f - sum(y)[0] / n;


    //estimate pose here
    _pose = _pose_predictor.calcPose(_shape);
    _yawAngle = 0.0f;
    _tracking = true;
    return true;
  }

  _tracking = false;
  return false;
}


bool FaceTracker::resetWith5Points(std::vector<cv::Point2f> five_points) {
  printInfo("resetWith5Points\n");
  if (five_points.size() > 0) {
    _center_refShape.copyTo(_shape);
    // float rollAngle = 0.0f;
    // if (rotation == 90) rollAngle = M_PI_2;
    // else if (rotation == 180) rollAngle = M_PI;
    // else if (rotation == 270) rollAngle = 1.5 * M_PI;
    // cv::Mat_<float> Mt = (cv::Mat_<float>(2,2) <<
    //                       cosf(rollAngle), sinf(rollAngle),
    //                       -sinf(rollAngle), cosf(rollAngle));
    // _shape = _shape.reshape(0, _shape.rows * _shape.cols/2) * Mt;
    // _shape = _shape.reshape(0, _shape.rows * _shape.cols);
    // const int n = _shape.rows * _shape.cols / 2;
    // cv::Mat_<float> x,y, xy = _shape.reshape(0, n);
    // x = xy.col(0);
    // y = xy.col(1);

    // double mn, mx;
    // cv::minMaxLoc(x, &mn, &mx);
    // x *= r.width / (mx - mn);
    // cv::minMaxLoc(y, &mn, &mx);
    // y *= r.height / (mx - mn);
    // x += r.x + r.width / 2.0f - sum(x)[0] / n;
    // y += r.y + r.height / 2.0f - sum(y)[0] / n;

    Mat s2D_5_gt(5, 2, CV_32FC1);
    for (int num = 0; num < five_points.size(); num++) {
      s2D_5_gt.at<float>(num, 0) = five_points[num].x;
      s2D_5_gt.at<float>(num, 1) = five_points[num].y;
    }

    Mat_<float> avg2d_5_gt;
    cv::reduce(s2D_5_gt, avg2d_5_gt, 0, CV_REDUCE_AVG);
    s2D_5_gt.col(0) -= avg2d_5_gt(0);
    s2D_5_gt.col(1) -= avg2d_5_gt(1);

    // calculate M between s2d_5_gt and five_avg3D
    Mat_<float> M = ((S_five.t() * S_five).inv(cv::DECOMP_CHOLESKY)) *
                    S_five.t() * s2D_5_gt;
    Mat_<float> s2D_66_estimate_shape = S_ss * M;

    // calcalate lmean and rmean
    cv::Point2f lmean(0, 0), rmean(0, 0);
    if (s2D_66_estimate_shape.rows != 66) {
      return false;
    }
    for (int j = 0; j < 6; j++) {
      const int i1 = 36 + j;
      lmean.x += s2D_66_estimate_shape.at<float>(i1, 0);
      lmean.y += s2D_66_estimate_shape.at<float>(i1, 1);
    }
    lmean *= 1.0f / 6.0f;
    if (s2D_66_estimate_shape.rows != 66) {
      return false;
    }
    for (int j = 0; j < 6; j++) {
      const int i1 = 42 + j;
      rmean.x += s2D_66_estimate_shape.at<float>(i1, 0);
      rmean.y += s2D_66_estimate_shape.at<float>(i1, 1);
    }
    rmean *= 1.0f / 6.0f;

    cv::Point2f leye = five_points[0], reye = five_points[1];

    float scale =
            static_cast<float>(cv::norm(reye - leye) / norm(rmean - lmean));
    _shape = scale * s2D_66_estimate_shape.reshape(0, 66 * 2).clone();
    cv::Point2f lscale = scale * lmean, rscale = scale * rmean;
    float tx = 0.5f * (leye.x + reye.x) - 0.5f * (lscale.x + rscale.x);
    float ty = 0.5f * (leye.y + reye.y) - 0.5f * (lscale.y + rscale.y);
    int n = _shape.rows / 2;
    for (int j = 0; j < n; j++) {
      _shape(2 * j) += tx;
      _shape(2 * j + 1) += ty;
    }

    // estimate pose here
    _pose = _pose_predictor.calcPose(_shape);
    _yawAngle = 0.0f;
    _tracking = true;
    return true;
  }

  _tracking = false;
  return false;
}

//==============================================================================
bool FaceTracker::update(const bool predictPupils, const bool highPrecision,
                         const bool smooth) {
  if (!_tracking || _shape.empty()) {
    if (!_tracking) printInfo("Not tracking");
    return false;
  }
  if (!_oglInit) {
    printErr("Error: tracker not initialised with source!");
    return false;
  }


        tvs::LinearModel *smodel = &_center_smodel;
  cv::Mat_<float> refShape = _center_refShape;
  std::vector<tvs::LinearShapeRegressor> *predictor = &_center_predictor;
  tvs::LinearShapeClassifier *classifier = &_center_classifier;
  if (/*_pose(1)*/ _yawAngle < CV_PI * -10.0f / 180.0f ) {
    smodel = &_right_smodel;
    refShape = _right_refShape;
    predictor = &_right_predictor;
    classifier = &_right_classifier;
  } else if (/*_pose(1)*/ _yawAngle > CV_PI * 10.f / 180.0f) {
    smodel = &_left_smodel;
    refShape = _left_refShape;
    predictor = &_left_predictor;
    classifier = &_left_classifier;
  }

  cv::Mat_<float> p = smodel->project(_shape);
  smodel->clamp(p, 2.0f);
  _shape = smodel->reconstruct(p);

  cv::Mat_<float> M = geo::alignPoints2d(_shape, predictor->at(0)._rpts);
  cv::Mat_<float> T = geo::alignPoints2d(refShape, _shape);
  cv::Point2f pt(T(0,2),T(1,2));
  float theta = atan2f(T(1,0),T(0,0));
  float scale = T(0,0) / cosf(theta);

  M = this->calcCrop(pt,scale,theta);
  cv::Mat_<float> Mogl = _featComp.convertCVSimTransfToOpenGL(M);
  const cv::Mat &mat = _featComp.setup(_srcTexture, Mogl);

  _feature.setup(mat);

  if (smooth) {
    //convert to grayscale here while we do the regression
    _colorConv.startConverting(_srcTexture);
  }

  cv::Mat_<float> Mi = geo::invertTransform2d(M);
  _shape = geo::transform2d(_shape, Mi);

  int levels = 3;
  if (highPrecision) levels = predictor->size();

  mtx.lock();
  for (int k = 0; k < levels; k++) {
    _shape = predictor->at(k).predict(_feature, _shape);
  }
  const cv::Mat_<float>& feat = predictor->at(levels-1).getLastFeature();
  _confidence = classifier->predict(feat, _shape);
  _confidence = cv::min(cv::max(_confidence, 0.0f), 1.0f);
  mtx.unlock();

  if (sum(_confidence)[0] / _confidence.rows < _health_thresh) {
    _shape.release();
	_shapeRotated.release();
    _pose.release();
	_poseRollRotated.release();
    _confidence.release();
    _pupils.release();
    _gaze.release();
    _tracking = false;
    printInfo("Confidence too low");
    if (smooth) {
      // make sure it finishes - otherwise the rendering of the GUI
      // has the wrong FBO set.
      _colorConv.finishConversion();
    }
    return false;
  }

  if (predictPupils && fabsf(_pose(1)) < CV_PI/9.0f) {
    cv::Mat_<float> e;
    if (!_left_eye_idx.empty() && !_right_eye_idx.empty()) {
      e = Mat_<float>::zeros(4, 1);
      for (int i = 0; i < _left_eye_idx.rows; i++) { int j = _left_eye_idx(i);
        e(0) += _shape(2 * j); e(1) += _shape(2 * j + 1);
      }
      e(0) /= _left_eye_idx.rows; e(1) /= _left_eye_idx.rows;
      for (int i = 0; i < _right_eye_idx.rows; i++) { int j = _right_eye_idx(i);
        e(2) += _shape(2 * j); e(3) += _shape(2 * j + 1);
      }
      e(2) /= _right_eye_idx.rows; e(3) /= _right_eye_idx.rows;
    } else if (_shape.rows == 2 * 66) {
      e = Mat_<float>::zeros(4, 1);
      for (int i = 0; i < 6; i++) {
        e(0) += _shape(2*(36+i)  ); e(1) += _shape(2*(36+i)+1);
        e(2) += _shape(2*(42+i)  ); e(3) += _shape(2*(42+i)+1);
      }
      e *= 1.0f / 6.0f;
    }
    if (!e.empty()) {
        mtx.lock();
      for (size_t level = 0; level < _pupil_predictor.size(); level++) {
        e = _pupil_predictor.at(level).predict(_feature, e);
      }
      mtx.unlock();
      e = geo::transform2d(e, M);
      _pupils = e;
    }
  } else {
    _pupils.release();
    _gaze.release();
  }

  _shape = geo::transform2d(_shape, M);

  if (smooth) {
    int n = _shape.rows;
    Mat_<float> se(n + 4, 1);
    Mat_<float> s1 = se(cv::Rect(0, 0, 1, n));
    Mat_<float> sp = se(cv::Rect(0, n, 1, 4));
    _shape.copyTo(s1);
    if (!_pupils.empty()) {
      _pupils.copyTo(sp);
    } else if (!_left_eye_idx.empty() && !_right_eye_idx.empty()) {
      sp = 0.0f;
      for (int i = 0; i < _left_eye_idx.rows; i++) { int j = _left_eye_idx(i);
        sp(0) += _shape(2 * j); sp(1) += _shape(2 * j + 1);
      }
      sp(0) /= _left_eye_idx.rows; sp(1) /= _left_eye_idx.rows;
      for (int i = 0; i < _right_eye_idx.rows; i++) { int j = _right_eye_idx(i);
        sp(2) += _shape(2 * j); sp(3) += _shape(2 * j + 1);
      }
      sp(2) /= _right_eye_idx.rows; sp(3) /= _right_eye_idx.rows;
    } else if (_shape.rows == 2 * 66) {
      sp = 0.0f;
      for (int i = 0; i < 6; i++) {
        sp(0) += _shape(2*(36+i)); sp(1) += _shape(2*(36+i)+1);
        sp(2) += _shape(2*(42+i)); sp(3) += _shape(2*(42+i)+1);
      }
      sp *= 1.0f/6.0f;
    } else {
      //use the first 2 points of the shape - it doesn't matter.
      _shape.rowRange(0, 4).copyTo(sp);
    }

    _colorConv.finishConversion();
    const cv::Mat& im = _colorConv.getResult();
    _smoother.calcFlow(im, se);
    _colorConv.releaseResult();

    s1.copyTo(_shape);
    if (!_pupils.empty()) sp.copyTo(_pupils);
  }

  //estimate pose here
  _pose = _pose_predictor.calcPose(_shape);

  //ignore it if the roll is under 20 degrees
  if (fabsf(_pose(2)) > 20.0 * M_PI / 180.0){
    cv::Mat_<float> Mp = geo::alignPoints2d(_shape, predictor->at(0)._rpts);
    cv::Mat_<float> shp = geo::transform2d(_shape, Mp);
    cv::Mat_<float> p = _pose_predictor.calcPose(shp);
    _yawAngle = p(1);
  } else {
    _yawAngle = _pose(1);
  }

  calcShape3D(_shape, _shape3D, _pose);

  if (!_pupils.empty() && fabsf(_pose(1)) < CV_PI/ 9.0f) {
    cv::Point3f ldir, rdir;
    this->calcGaze(_shape3D, _pose, cv::Point2f(_pupils(0), _pupils(1)),
                   cv::Point2f(_pupils(2), _pupils(3)), ldir, rdir);
    _gaze.create(6, 1);
    _gaze(0) = ldir.x; _gaze(1) = ldir.y; _gaze(2) = ldir.z;
    _gaze(3) = rdir.x; _gaze(4) = rdir.y; _gaze(5) = rdir.z;
  } else {
    _pupils.release();
    _gaze.release();
  }
  //printInfo("Done updating");
  return true;
}
//==============================================================================
//==============================================================================
//==============================================================================
cv::Mat_<float> FaceTracker::computeTransformation() {
  if (!_tracking || _shape.empty()) {
    if (!_tracking) printInfo("Not tracking");
    return cv::Mat_<float>();
  }


        tvs::LinearModel *smodel = &_center_smodel;
  cv::Mat_<float> refShape = _center_refShape;
  std::vector<tvs::LinearShapeRegressor> *predictor = &_center_predictor;
  //tvs::LinearShapeClassifier *classifier = &_center_classifier;
  if (/*_pose(1)*/ _yawAngle < CV_PI * -10.0f / 180.0f ) {
    smodel = &_right_smodel;
    refShape = _right_refShape;
    predictor = &_right_predictor;
    //classifier = &_right_classifier;
  } else if (/*_pose(1)*/ _yawAngle > CV_PI * 10.f / 180.0f) {
    smodel = &_left_smodel;
    refShape = _left_refShape;
    predictor = &_left_predictor;
    //classifier = &_left_classifier;
  }

  cv::Mat_<float> p = smodel->project(_shape);
  smodel->clamp(p, 2.0f);
  _shape = smodel->reconstruct(p);

  cv::Mat_<float> M = geo::alignPoints2d(_shape, predictor->at(0)._rpts);
  cv::Mat_<float> T = geo::alignPoints2d(refShape, _shape);
  cv::Point2f pt(T(0,2),T(1,2));
  float theta = atan2f(T(1,0),T(0,0));
  float scale = T(0,0) / cosf(theta);

  M = this->calcCrop(pt,scale,theta);
  return M;
}
//==============================================================================
cv::Mat_<float> FaceTracker::computeTransformationWA() {
  if (!_tracking || _shape.empty()) {
    if (!_tracking) printInfo("Not tracking");
    return cv::Mat_<float>();
  }

  tvs::LinearModel *smodel = &_center_smodel;
  std::vector<tvs::LinearShapeRegressor> *predictor = &_center_predictor;
  if (/*_pose(1)*/ _yawAngle < CV_PI * -10.0f / 180.0f ) {
    smodel = &_right_smodel;
    predictor = &_right_predictor;
  } else if (/*_pose(1)*/ _yawAngle > CV_PI * 10.f / 180.0f) {
    smodel = &_left_smodel;
    predictor = &_left_predictor;
  }

  cv::Mat_<float> p = smodel->project(_shape);
  smodel->clamp(p, 2.0f);
  _shape = smodel->reconstruct(p);

  cv::Mat_<float> M = geo::alignPoints2d(predictor->at(0)._rpts, _shape);
  return M;
}
//==============================================================================
//==============================================================================
//==============================================================================
bool _use_mixture = true;   // using left/right and center predictions mixture or not.

bool FaceTracker::update(const cv::Mat& crop, const cv::Mat_<float>& M_,
                         const bool predictPupils, const bool highPrecision,
                         GrayInputConversion *smoothData) {
  if (crop.empty()) return false;
  if (crop.channels() == 1) {
    crop_ = crop;
  } else {
    printErr("Multi channel not supported yet!");
    return false;
  }

  tvs::LinearModel *smodel = &_center_smodel;
  cv::Mat_<float> refShape = _center_refShape;
  std::vector<tvs::LinearShapeRegressor> *predictor = &_center_predictor;
  tvs::LinearShapeClassifier *classifier = &_center_classifier;
  if (/*_pose(1)*/ _yawAngle < CV_PI * -10.0f / 180.0f ) {		//default CV_PI * -10.0f / 180.0f
    smodel = &_right_smodel;
    refShape = _right_refShape;
    predictor = &_right_predictor;
    classifier = &_right_classifier;
  } else if (/*_pose(1)*/ _yawAngle > CV_PI * 10.f / 180.0f) {	//default CV_PI * -10.0f / 180.0f
    smodel = &_left_smodel;
    refShape = _left_refShape;
    predictor = &_left_predictor;
    classifier = &_left_classifier;
  }

  cv::Mat_<float> M;
  if (!M_.empty()) {
    M = M_.clone();
  } else {
    printInfo("Recomputing M!");
    cv::Mat_<float> p = smodel->project(_shape);
    smodel->clamp(p, 2.0f);
    _shape = smodel->reconstruct(p);

    //M = geo::alignPoints2d(_shape, predictor->at(0)._rpts);
    cv::Mat_<float> T = geo::alignPoints2d(refShape, _shape);
    cv::Point2f pt(T(0,2),T(1,2));
    float theta = atan2f(T(1,0),T(0,0));
    float scale = T(0,0) / cosf(theta);

    M = this->calcCrop(pt,scale,theta);
  }

  _feature.setup(crop_);
  cv::Mat_<float> Mi = geo::invertTransform2d(M);
  _shape = geo::transform2d(_shape, Mi);

  int levels = 3;
  if (highPrecision) levels = predictor->size();

  if( !_use_mixture )
  {
#ifdef USE_SHARE_MEMORY
	  mtx.lock();
#endif
	  for (int k = 0; k < levels; k++) {
		_shape = predictor->at(k).predict(_feature, _shape);
	  }
	  const cv::Mat_<float>& feat = predictor->at(levels-1).getLastFeature();
	  _confidence = classifier->predict(feat, _shape);
	  _confidence = cv::min(cv::max(_confidence, 0.0f), 1.0f);
#ifdef USE_SHARE_MEMORY
	  mtx.unlock();
#endif
  }
  else
  {
    Mat_<float> v;
    // mixture
    if (_yawAngle < CV_PI * -15.0f / 180.0f )
    {
      // Using _right_model.
#ifdef USE_SHARE_MEMORY
      mtx.lock();
#endif
      for (int k = 0; k < levels; k++) {
        _shape = predictor->at(k).predict(_feature, _shape);
      }
#ifdef USE_SHARE_MEMORY
      mtx.unlock();
#endif
      const cv::Mat_<float>& feat = predictor->at(levels-1).getLastFeature();
      v = classifier->predict(feat,_shape);
    }
    else if (_yawAngle > CV_PI * 15.0f / 180.0f )
    {
      // Using _left_model.
#ifdef USE_SHARE_MEMORY
      mtx.lock();
#endif
      for (int k = 0; k < levels; k++) {
        _shape = predictor->at(k).predict(_feature, _shape);
      }
#ifdef USE_SHARE_MEMORY
      mtx.unlock();
#endif
      const cv::Mat_<float>& feat = predictor->at(levels-1).getLastFeature();
      v = classifier->predict(feat,_shape);
    }
    else
    {   // mix the left/right and center predictions (slower!)
      Mat_<float> _sc;
      _shape.copyTo(_sc);

      predictor = &_center_predictor;
      classifier = &_center_classifier;

#ifdef USE_SHARE_MEMORY
      mtx.lock();
#endif
      for (int k = 0; k < levels; k++) {
        _sc = predictor->at(k).predict(_feature, _sc);
      }
#ifdef USE_SHARE_MEMORY
      mtx.unlock();
#endif

      const cv::Mat_<float>& feat = predictor->at(levels-1).getLastFeature();
      Mat_<float> _vc = classifier->predict(feat, _sc);

      const float yaw = _yawAngle * 180.0f / CV_PI;

      Mat_<float> _sr, _sl;
      Mat_<float> _vr, _vl;
      if ( yaw < -5.0f )
      {
        // mix right and center predictions!
        _shape.copyTo(_sr);

        predictor = &_right_predictor;
        classifier = &_right_classifier;

#ifdef USE_SHARE_MEMORY
        mtx.lock();
#endif
        for (int k = 0; k < levels; k++) {
          _sr = predictor->at(k).predict(_feature, _sr);
        }
#ifdef USE_SHARE_MEMORY
        mtx.unlock();
#endif

        const cv::Mat_<float>& feat = predictor->at(levels-1).getLastFeature();
        _vr = classifier->predict(feat, _sr);

        float w = (-yaw - 5.0f) / (15.0f - 5.0f);
        v = w * _vr + (1.0f - w) * _vc;
        _shape = w * _sr + (1.0f - w) * _sc;
      }
      else if( yaw > 5.0f )
      {
        // mix left and center predictions!
        _shape.copyTo(_sl);

        predictor = &_left_predictor;
        classifier = &_left_classifier;

#ifdef USE_SHARE_MEMORY
        mtx.lock();
#endif
        for (int k = 0; k < levels; k++) {
          _sl = predictor->at(k).predict(_feature, _sl);
        }
#ifdef USE_SHARE_MEMORY
        mtx.unlock();
#endif

        const cv::Mat_<float>& feat = predictor->at(levels-1).getLastFeature();
        _vl = classifier->predict(feat, _sl);

        float w = (yaw - 5.0f) / (15.0f - 5.0f);
        v = w * _vl + (1.0f - w) * _vc;
        _shape = w * _sl + (1.0f - w) * _sc;
      }
      else
      {
        v = _vc;
        _shape = _sc;
      }
    }

    _confidence = cv::min(cv::max(v, 0.0f), 1.0f);
  }

  // Modified.
//  char szBuf[500];
//  sprintf(szBuf, "(Confidence, threshold) =  (%f, %f).", sum(_confidence)[0] / _confidence.rows, _health_thresh);
//  __android_log_print(ANDROID_LOG_DEBUG, kLogTag, szBuf);

  if (sum(_confidence)[0] / _confidence.rows < _health_thresh) {
    release();
    printInfo("Confidence too low");
    return false;
  }

  if (predictPupils && fabsf(_pose(1)) < CV_PI/4.0f) {	//default CV_PI/9.0f
    cv::Mat_<float> e;
    if (!_left_eye_idx.empty() && !_right_eye_idx.empty()) {
      e = Mat_<float>::zeros(4, 1);
      for (int i = 0; i < _left_eye_idx.rows; i++) { int j = _left_eye_idx(i);
        e(0) += _shape(2 * j); e(1) += _shape(2 * j + 1);
      }
      e(0) /= _left_eye_idx.rows; e(1) /= _left_eye_idx.rows;
      for (int i = 0; i < _right_eye_idx.rows; i++) { int j = _right_eye_idx(i);
        e(2) += _shape(2 * j); e(3) += _shape(2 * j + 1);
      }
      e(2) /= _right_eye_idx.rows; e(3) /= _right_eye_idx.rows;
    } else if (_shape.rows == 2 * 66) {
      e = Mat_<float>::zeros(4, 1);
      for (int i = 0; i < 6; i++) {
        e(0) += _shape(2*(36+i)  ); e(1) += _shape(2*(36+i)+1);
        e(2) += _shape(2*(42+i)  ); e(3) += _shape(2*(42+i)+1);
      }
      e *= 1.0f / 6.0f;
    }
    if (!e.empty()) {
        mtx.lock();
      for (size_t level = 0; level < _pupil_predictor.size(); level++) {
        e = _pupil_predictor.at(level).predict(_feature, e);
      }
      mtx.unlock();
      e = geo::transform2d(e, M);
      _pupils = e;
    }
  } else {
    _pupils.release();
    _gaze.release();
  }

  _shape = geo::transform2d(_shape, M);

  if (smoothData != NULL) {
    int n = _shape.rows;
    Mat_<float> se(n + 4, 1);
    Mat_<float> s1 = se(cv::Rect(0, 0, 1, n));
    Mat_<float> sp = se(cv::Rect(0, n, 1, 4));
    _shape.copyTo(s1);
    if (!_pupils.empty()) {
      _pupils.copyTo(sp);
    } else if (!_left_eye_idx.empty() && !_right_eye_idx.empty()) {
      sp = 0.0f;
      for (int i = 0; i < _left_eye_idx.rows; i++) { int j = _left_eye_idx(i);
        sp(0) += _shape(2 * j); sp(1) += _shape(2 * j + 1);
      }
      sp(0) /= _left_eye_idx.rows; sp(1) /= _left_eye_idx.rows;
      for (int i = 0; i < _right_eye_idx.rows; i++) { int j = _right_eye_idx(i);
        sp(2) += _shape(2 * j); sp(3) += _shape(2 * j + 1);
      }
      sp(2) /= _right_eye_idx.rows; sp(3) /= _right_eye_idx.rows;
    } else if (_shape.rows == 2 * 66) {
      sp = 0.0f;
      for (int i = 0; i < 6; i++) {
        sp(0) += _shape(2*(36+i)); sp(1) += _shape(2*(36+i)+1);
        sp(2) += _shape(2*(42+i)); sp(3) += _shape(2*(42+i)+1);
      }
      sp *= 1.0f/6.0f;
    } else {
      //use the first 2 points of the shape - it doesn't matter.
      _shape.rowRange(0, 4).copyTo(sp);
    }


    std::unique_lock<std::mutex> lock(*(smoothData->mutex));
    while (!smoothData->ready) {
      smoothData->condition->wait(lock);
    }
    lock.unlock();

    _smoother.calcFlow(*(smoothData->gray), se);

    s1.copyTo(_shape);
    if (!_pupils.empty()) sp.copyTo(_pupils);
  }

  //estimate pose here
  _pose = _pose_predictor.calcPose(_shape);

  //ignore it if the roll is under 20 degrees
  if (fabsf(_pose(2)) > 20.0 * M_PI / 180.0){	//default 20.0 * M_PI / 180.0
    cv::Mat_<float> Mp = geo::alignPoints2d(_shape, predictor->at(0)._rpts);
    cv::Mat_<float> shp = geo::transform2d(_shape, Mp);
    cv::Mat_<float> p = _pose_predictor.calcPose(shp);
    _yawAngle = p(1);
  } else {
    _yawAngle = _pose(1);
  }

  calcShape3D(_shape, _shape3D, _pose);

  if (!_pupils.empty() && fabsf(_pose(1)) < CV_PI/ 4.0f) {		   //default CV_PI/ 9.0f

    cv::Point3f ldir, rdir;
    this->calcGaze(_shape3D, _pose, cv::Point2f(_pupils(0), _pupils(1)),
                   cv::Point2f(_pupils(2), _pupils(3)), ldir, rdir);
    _gaze.create(6, 1);
    _gaze(0) = ldir.x; _gaze(1) = ldir.y; _gaze(2) = ldir.z;
    _gaze(3) = rdir.x; _gaze(4) = rdir.y; _gaze(5) = rdir.z;
  } else {
    _pupils.release();
    _gaze.release();
  }
  //printInfo("Done updating");
  return true;
}
//==============================================================================
void FaceTracker::calcGaze(const Mat_<float> &s3D, const Mat_<float> &pose,
                           const cv::Point2f leye, const cv::Point2f reye,
                           cv::Point3f &ldir, cv::Point3f &rdir) {
  Mat_<float> lpose = pose.clone();
  Mat_<float> rpose = pose.clone();
  float lrad(0); cv::Point3f lcent(0, 0, 0);
  if (!_left_eye_idx.empty()) {
    float xmin = s3D(3 * _left_eye_idx(0));
    float xmax = s3D(3 * _left_eye_idx(0));
    for (int i = 1; i < _left_eye_idx.rows; i++) {
      int j = _left_eye_idx(i);
      xmin = cv::min(xmin, s3D(3 * j));
      xmax = cv::max(xmax, s3D(3 * j));
    }
    lrad = (xmax - xmin) / 2;
    for(int j = 0; j < _left_eye_idx.rows; j++){
      int k = _left_eye_idx(j);
      lcent.x += s3D(3 * k    );
      lcent.y += s3D(3 * k + 1);
      lcent.z += s3D(3 * k + 2);
    }
    lcent *= 1.0f / _left_eye_idx.rows;
  } else if (s3D.rows / 3 == 66) { //use default location
    lrad = static_cast<float>(cv::norm(s3D(cv::Rect(0, 3 * 36, 1, 3)),
                                       s3D(cv::Rect(0, 3 * 39, 1, 3))) / 2);
    lcent = cv::Point3f(0.5f * (s3D(3 * 36    ) + s3D(3 * 39)    ),
                        0.5f * (s3D(3 * 36 + 1) + s3D(3 * 39 + 1)),
                        0.5f * (s3D(3 * 36 + 2) + s3D(3 * 39 + 2)) + lrad);
  } else {
    // unsupported number of points.
    return;
  }
  float rrad(0); cv::Point3f rcent(0, 0, 0);
  if (!_right_eye_idx.empty()) {
    float xmin = s3D(3 * _right_eye_idx(0));
    float xmax = s3D(3 * _right_eye_idx(0));
    for (int i = 1; i < _right_eye_idx.rows; i++) { int j = _right_eye_idx(i);
      xmin = cv::min(xmin, s3D(3 * j));
      xmax = cv::max(xmax, s3D(3 * j));
    }
    rrad = (xmax - xmin) / 2;
    for(int j = 0; j < _right_eye_idx.rows; j++) { int k = _right_eye_idx(j);
      rcent.x += s3D(3 * k    );
      rcent.y += s3D(3 * k + 1);
      rcent.z += s3D(3 * k + 2);
    }
    rcent *= 1.0f / _right_eye_idx.rows;
  } else if (s3D.rows / 3 == 66) {
    rrad = static_cast<float>(cv::norm(s3D(cv::Rect(0, 3 * 42, 1, 3)),
                                       s3D(cv::Rect(0, 3 * 45, 1, 3))) / 2);
    rcent = cv::Point3f(0.5f * (s3D(3 * 42    ) + s3D(3 * 45)    ),
                        0.5f * (s3D(3 * 42 + 1) + s3D(3 * 45 + 1)),
                        0.5f * (s3D(3 * 42 + 2) + s3D(3 * 45 + 2)) + rrad);
  } else {
    // unsupported number of points.
    return;
  }
  Mat_<float> Rl; cv::Rodrigues(lpose(cv::Rect(0, 0, 1, 3)), Rl);
  Mat_<float> Rr; cv::Rodrigues(rpose(cv::Rect(0, 0, 1, 3)), Rr);

  // left eye
  {
    Mat_<float> Rxy = Rl(cv::Rect(0, 0, 2, 2)), R = Rl(cv::Rect(0, 0, 3, 2));
    Mat_<float> t = lpose(cv::Rect(0, 3, 1, 2));
    Mat_<float> c = (Mat_<float>(3, 1) << lcent.x, lcent.y, lcent.z - lrad);
    float a = lpose(5);
    Mat_<float> e = (Mat_<float>(2, 1) << leye.x, leye.y);
    e -= a * R * c + t; e /= a;
    Mat_<float> H = Rxy.t() * Rxy, Hi;
    cv::invert(H, Hi, cv::DECOMP_SVD);
    Mat_<float> xy = Hi * Rxy.t() * e;
    Mat_<float> X = (Mat_<float>(3, 1) << xy(0), xy(1), -lrad);
    cv::normalize(X, X);
    Mat_<float> Y = Rl * X;
    ldir.x = Y(0);
    ldir.y = Y(1);
    ldir.z = Y(2);
  }
  // right eye
  {
    Mat_<float> Rxy = Rl(cv::Rect(0, 0, 2, 2)), R = Rr(cv::Rect(0, 0, 3, 2));
    Mat_<float> t = rpose(cv::Rect(0, 3, 1, 2));
    Mat_<float> c = (Mat_<float>(3, 1) << rcent.x, rcent.y, rcent.z - rrad);
    float a = rpose(5);
    Mat_<float> e = (Mat_<float>(2, 1) << reye.x, reye.y);
    e -= a * R * c + t; e /= a;
    Mat_<float> H = Rxy.t() * Rxy, Hi;
    cv::invert(H, Hi, cv::DECOMP_SVD);
    Mat_<float> xy = Hi * Rxy.t() * e;
    Mat_<float> X = (Mat_<float>(3, 1) << xy(0), xy(1), -rrad);
    cv::normalize(X, X);
    Mat_<float> Y = Rr * X;
    rdir.x = Y(0);
    rdir.y = Y(1);
    rdir.z = Y(2);
  }
  return;
}
//==============================================================================
void FaceTracker::calcShape3D(const cv::Mat_<float> &s2D, Mat_<float> &s3D,
                          Mat_<float> &pose) {
  Mat_<float> s2D_nojaw;
  int n;
  if (_nojaw_idx.empty()) {
    if (s2D.rows / 2 != 66) { return; }
    s2D_nojaw = s2D(cv::Rect(0, 2 * 17, 1, 2 * (66 - 17)));
    n = 66 - 17;
  } else {
    n = _nojaw_idx.rows;
    s2D_nojaw.create(2 * n, 1);
    for (int i = 0; i < n; i++) { int j = _nojaw_idx(i);
      s2D_nojaw(2 * i    ) = s2D(2 * j    );
      s2D_nojaw(2 * i + 1) = s2D(2 * j + 1);
    }
  }

  Mat_<float> params = Mat_<float>::zeros(_smodel3d_nojaw._evec.cols, 1);
  Mat_<float> params_old;
  Mat_<float> s3D_nojaw = _smodel3d_nojaw._mean.clone();
  Mat_<float> xyz(3 * n, 1);
  for (int iter = 0; iter < 5; iter++) {
    Mat_<float> R; Rodrigues(pose(cv::Rect(0, 0, 1, 3)), R);
    for (int i = 0; i < n; i++) {
      const float x = (s2D_nojaw(2*i  ) - pose(3)) / pose(5);
      const float y = (s2D_nojaw(2*i+1) - pose(4)) / pose(5);
      const float z = R(2, 0) * s3D_nojaw(3*i  ) +
      R(2, 1) * s3D_nojaw(3*i+1) + R(2, 2) * s3D_nojaw(3*i+2);
      s3D_nojaw(3*i  ) = R(0, 0) * x + R(1, 0) * y + R(2, 0) * z;
      s3D_nojaw(3*i+1) = R(0, 1) * x + R(1, 1) * y + R(2, 1) * z;
      s3D_nojaw(3*i+2) = R(0, 2) * x + R(1, 2) * y + R(2, 2) * z;
    }
    params = _smodel3d_nojaw._evec.t() * (s3D_nojaw - _smodel3d_nojaw._mean);
    _smodel3d_nojaw.clamp(params, 3.0f);
    if (iter > 0) {
      if(cv::norm(params - params_old) < 1.0e-3) { break; }
    }
    s3D_nojaw = _smodel3d_nojaw._mean + _smodel3d_nojaw._evec * params;
    params.copyTo(params_old);
    for (int i = 0; i < n; i++) {
      xyz(i) = s3D_nojaw(3 * i);
      xyz(i+n) = s3D_nojaw(3 * i+1);
      xyz(i+n*2) = s3D_nojaw(3 * i+2);
    }
    pose = _pose_predictor.calcPose(s2D_nojaw, xyz);
  }
  Mat_<float> R;  Rodrigues(pose(cv::Rect(0, 0, 1, 3)), R);
  for (int i = 0; i < n; i++) {
    const float x = (s2D_nojaw(2*i  ) - pose(3)) / pose(5);
    const float y = (s2D_nojaw(2*i+1) - pose(4)) / pose(5);
    const float z = R(2, 0) * s3D_nojaw(3*i  ) +
    R(2, 1) * s3D_nojaw(3*i+1) + R(2, 2) * s3D_nojaw(3*i+2);
    s3D_nojaw(3*i  ) = R(0, 0) * x + R(1, 0) * y + R(2, 0) * z;
    s3D_nojaw(3*i+1) = R(0, 1) * x + R(1, 1) * y + R(2, 1) * z;
    s3D_nojaw(3*i+2) = R(0, 2) * x + R(1, 2) * y + R(2, 2) * z;
  }
  s3D = _smodel3d._mean.clone();
  if (_nojaw_idx.empty()) {
    for (int i = 17; i < 66; i++) {
      Mat_<float> x = s3D(cv::Rect(0, 3 * i, 1, 3));
      Mat_<float> y = s3D_nojaw(cv::Rect(0, 3 * (i - 17), 1, 3));
      y.copyTo(x);
    }
    for (int i = 0; i < 17; i++) {
      const float x = (s2D(2*i  ) - pose(3)) / pose(5);
      const float y = (s2D(2*i+1) - pose(4)) / pose(5);
      const float z = R(2, 0) * s3D(3*i  ) +
      R(2, 1) * s3D(3*i+1) + R(2, 2) * s3D(3*i+2);
      s3D(3*i  ) = R(0, 0) * x + R(1, 0) * y + R(2, 0) * z;
      s3D(3*i+1) = R(0, 1) * x + R(1, 1) * y + R(2, 1) * z;
      s3D(3*i+2) = R(0, 2) * x + R(1, 2) * y + R(2, 2) * z;
    }
  } else {
    for (int i = 0; i < n; i++) { int j = _nojaw_idx(i);
      Mat_<float> x = s3D(cv::Rect(0, 3 * j, 1, 3));
      Mat_<float> y = s3D_nojaw(cv::Rect(0, 3 * i, 1, 3)); y.copyTo(x);
    }
    for (int i = 0; i < s2D.rows / 2; i++) {
      int j = 0; for (; j < n; j++) { if (_nojaw_idx(j) == i) { break; } }
      if (j < n) { continue; }
      const float x = (s2D(2*i  ) - pose(3)) / pose(5);
      const float y = (s2D(2*i+1) - pose(4)) / pose(5);
      const float z = R(2, 0) * s3D(3*i  ) +
      R(2, 1) * s3D(3*i+1) + R(2, 2) * s3D(3*i+2);
      s3D(3*i  ) = R(0, 0) * x + R(1, 0) * y + R(2, 0) * z;
      s3D(3*i+1) = R(0, 1) * x + R(1, 1) * y + R(2, 1) * z;
      s3D(3*i+2) = R(0, 2) * x + R(1, 2) * y + R(2, 2) * z;
    }
  }  return;
}
//==============================================================================
cv::Mat_<float> FaceTracker::calcCrop(const cv::Point2f p, const float scale,
                                  const float theta) {
  float ct = scale*cosf(theta),st = scale*sinf(theta);
  float cx = (_feature_params.size.width-1.0f)/2.0f;
  float cy = (_feature_params.size.height-1.0f)/2.0f;
  float tx = p.x - (ct*cx - st*cy);
  float ty = p.y - (st*cx + ct*cy);
  Mat_<float> M = (Mat_<float>(2,3) << ct,-st,tx,st,ct,ty);
	return M;
}
//==============================================================================
int FaceTracker::loadTrackerFromFile(const std::string& fn, const std::string& sPackageName) {
  FILE *f = fopen(fn.c_str(), "r");
  if (!f) {
    printErr("Can't open model file\n");
    return -1;
  }

  // Extract the path and pass it to TimeBomb.
  size_t position = fn.find_last_of("/");

  int nlevels;
  if (fread(&nlevels, sizeof(nlevels), 1, f) != 1) { fclose(f); return -1; }
  _center_predictor.resize(nlevels);
  _left_predictor.resize(nlevels);
  _right_predictor.resize(nlevels);
  for (int i = 0; i < nlevels; i++) {
    if (_center_predictor[i].read(f) < 0) { fclose(f); return -1; }
    if (_left_predictor[i].read(f) < 0) { fclose(f); return -1; }
    if (_right_predictor[i].read(f) < 0) { fclose(f); return -1; }
  }

  _center_refShape = centerShape(_center_predictor.at(0)._rpts);
  _left_refShape = centerShape(_left_predictor.at(0)._rpts);
  _right_refShape = centerShape(_right_predictor.at(0)._rpts);

  int nlevels_eyes;
  if (fread(&nlevels_eyes, sizeof(nlevels_eyes), 1, f) != 1) {
    fclose(f); return -1;
  }
  _pupil_predictor.resize(nlevels_eyes);
  for (int i = 0; i < nlevels_eyes; i++) {
    if (_pupil_predictor[i].read(f) < 0) {
      fclose(f);
      return -1;
    }
  }
  if (_center_classifier.read(f) < 0) { fclose(f); return -1; }
  if (_left_classifier.read(f) < 0) { fclose(f); return -1; }
  if (_right_classifier.read(f) < 0) { fclose(f); return -1; }

  if (_center_smodel.read(f) < 0) { fclose(f); return -1; }
  if (_left_smodel.read(f) < 0) { fclose(f); return -1; }
  if (_right_smodel.read(f) < 0) { fclose(f); return -1; }

  int with_feats = 0;
  if (fread(&with_feats, sizeof(int), 1, f) != 1) { return 0; }
  if (with_feats > 0) {
    if (readFeatParameters(f, _feature_params) < 0) { return -1; }
    _has_feat = true;
  }
  int with_smodel3d = 0;
  if (fread(&with_smodel3d, sizeof(int), 1, f) != 1) { return 0; }
  if (with_smodel3d > 0) {
    if (_smodel3d.read(f) < 0) { return -1; }
    if (_smodel3d_nojaw.read(f) < 0) { return -1; }
    _has_smodel3d = true;
    _pose_predictor.setShape(_smodel3d._mean);
  }
  {
    int with_idx = 0;
    if (fread(&with_idx, sizeof(int), 1, f) != 1) { return 0; }
    if (with_idx > 0) {
      if (io::readMat(_nojaw_idx, f) < 0) { return -1; }
    }
  }
  {
    int with_idx = 0;
    if (fread(&with_idx, sizeof(int), 1, f) != 1) { return 0; }
    if (with_idx > 0) {
      if (io::readMat(_left_eye_idx, f) < 0) { return -1; }
    }
  }
  {
    int with_idx = 0;
    if (fread(&with_idx, sizeof(int), 1, f) != 1) { return 0; }
    if (with_idx > 0) {
      if (io::readMat(_right_eye_idx, f) < 0) { return -1; }
    }
  }

  fclose(f);
  return 0;
}

int FaceTracker::load3DPointsFromFile(const std::string& fn) {
  FILE *ptf = fopen(fn.c_str(), "r");
  if (!ptf) {
    printErr("Error loading model");
    return -3;
  }
  if (_smodel3d.read(ptf) < 0) {
    printErr("Error loading 3d model");
    fclose(ptf);
    return -3;
  }
  _pose_predictor.setShape(_smodel3d._mean);
  if (_smodel3d_nojaw.read(ptf) < 0) {
    printErr("Error loading 3d model - ");
    fclose(ptf);
    return -3;
  }
  fclose(ptf);
  _has_smodel3d = true;
  return 0;
}

void FaceTracker::release() {
    _shape.release();
    _shapeRotated.release();
    _pose.release();
    _poseRollRotated.release();
    _confidence.release();
    _pupils.release();
    _gaze.release();
    _tracking = false;
}
}  // namespace tvs
