#ifndef __TVSFACETRACKER_ENGINE_H__
#define __TVSFACETRACKER_ENGINE_H__

#include <string>

#include <android/log.h>
#include <android/asset_manager.h>

#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include "tvs/ColorConverter.h"
#include "tvs/face_tracker.h"

class TVSTrackerEngine {
 public:
  TVSTrackerEngine();
  /*Loads models and data from the apk assets*/
  bool loadModels(AAssetManager* mgr, const std::string& cacheDir, const std::string& sPackageName);
  /*Loads models and data from a given folder*/
  bool loadModels(const std::string& path, const std::string& sPackageName);
  /* Initialise OpenGL elements with the current EGLContext (setup in
   * the Java side).
   */
  bool setupOpenGL(const int srcTexture, const int width, const int height);
  void eglContextInvalidated();
  bool activate(const std::string& key);
  
  /*  Rotation is that of the input texture (0, 90, 180 or 270)
   */
  bool resetWithFaceRect(const cv::Rect& faceRect, const int rotation);
  /*  Rotation is that of the input texture (0, 90, 180 or 270)
   */
  cv::Rect findFace(int rotation);
  bool update(const bool predictPupils, const bool highPrecision,
              const bool smooth);
  const unsigned int getPointCount() {return _tracker.getPointCount(); }
  const cv::Mat_<float> getShape() { return _tracker.getShape(); }
  const cv::Mat_<float> getShape3D() { return _tracker.getShape3D(); }

  const cv::Mat_<float> getPose() { return _tracker.getPose(); }
  bool getRotationAngles(float *pitch, float *yaw, float *roll);

  const cv::Mat_<float> getConfidence() { return _tracker.getConfidence(); }
  float getPoseQuality() { return _tracker.getPoseQuality(); }
  const cv::Mat_<float> getPupilLocation() {
    return _tracker.getPupilLocation();
  }
  const cv::Mat_<float> getGaze() { return _tracker.getGaze(); }
  
 private:
  tvs::FaceTracker _tracker;
  EGLContext _eglContext = EGL_NO_CONTEXT;
  bool _modelsReady = false;

  cv::CascadeClassifier _face_det;
  ColorConverter _colorConv;
  bool _colorConvInitialised = false;
  int _srcTexture;
  cv::Size _srcTextureSize;
};

#endif //  __TVSFACETRACKER_ENGINE_H__
