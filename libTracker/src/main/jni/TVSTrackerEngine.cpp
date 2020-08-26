#include "TVSTrackerEngine.h"

#include <android/log.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include "tvs/utils/android_assets.h"

using std::string;

#ifdef TVS_INTERNAL_DEBUGGING
const char kLogTag[] = "UlsFaceTrackerEngine";
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

static void rot2euler(const cv::Mat_<float> &R,
                           float *pitch, float *yaw, float *roll) {
  double q[4];
  q[0] = sqrt(1 + R(0, 0) + R(1, 1) + R(2, 2)) / 2;
  q[1] = (R(2, 1) - R(1, 2)) / (4 * q[0]) ;
  q[2] = (R(0, 2) - R(2, 0)) / (4 * q[0]) ;
  q[3] = (R(1, 0) - R(0, 1)) / (4 * q[0]) ;
  if (pitch)
    *pitch = static_cast<float>(atan2(2 * (q[0] * q[1] - q[2] * q[3]),
                                      q[0] * q[0] - q[1] * q[1] - q[2]
                                      * q[2] + q[3] * q[3]));
  if (yaw)
    *yaw = static_cast<float>(asin(2 * (q[0] * q[2] + q[1] * q[3])));
  if (roll)
    *roll = static_cast<float>(atan2(2 * (q[0] * q[3] - q[1] * q[2]),
                                  q[0] * q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3]));
  return;
}


TVSTrackerEngine::TVSTrackerEngine() {
  
}

bool TVSTrackerEngine::loadModels(AAssetManager *mgr, const string& cacheDir, const string& sPackageName) {
  // traker models
  int ret =  _tracker.loadModels(mgr, cacheDir, sPackageName);
  if (ret < 0) {
    printErr("Error loading models from assets: %d", ret);
    _modelsReady = false;
    return false;
  } else {
    _modelsReady = true;
  }

  const std::string tmpFN = cacheDir + "/tracker.tmp";
  if (!copyFileFromAsset(mgr, "ULSFaceTrackerAssets/face.xml", tmpFN)) {
    printErr("Error copying face.xml from assets");
    return false;
  }

  if (!_face_det.load(tmpFN)) {
    unlink(tmpFN.c_str());
    printErr("Error loading face detector file from assets");
    return false;
  }
  unlink(tmpFN.c_str());
  return _modelsReady;
}

bool TVSTrackerEngine::loadModels(const string& path, const string& sPackageName) {
   // traker models
  int ret =  _tracker.loadModels(path, sPackageName);
  if (ret < 0) {
    printErr("Error loading models from path %s: %d", path.c_str(), ret);
    _modelsReady = false;
    return false;
  } else {
    _modelsReady = true;
  }
  if (!_face_det.load(path + "/face.xml")) {
    printErr("Error loading face detector file");
    return false;
  }
  return true;
}

bool TVSTrackerEngine::setupOpenGL(const int srcTexture, const int width,
                                   const int height) {
  //check we have a context setup
  printInfo("SetupOpenGL");
  EGLContext ctxt = eglGetCurrentContext();
  if (ctxt == EGL_NO_CONTEXT) {
    printErr("No EGL context");
    return false;
  }
  if (_eglContext != EGL_NO_CONTEXT) {
    printWarn("An EGL context was already setup. It will be discarded");
  }
  _eglContext = ctxt;
  _srcTexture = srcTexture;
  _srcTextureSize = cv::Size(width, height);
  _colorConvInitialised = false;
  bool initOK = _tracker.initialiseWithSrcTexture(srcTexture,
                                                  cv::Size(width, height));
  printInfo("Done setupOpenGL");
  return initOK;
}

void TVSTrackerEngine::eglContextInvalidated() {
#ifdef DEBUG
  if (_eglContext != EGL_NO_CONTEXT) {
    printWarn("EGL context was already EGL_NO_CONTEXT");
  }
#endif
  _eglContext = EGL_NO_CONTEXT;
  _colorConvInitialised = false;
  _srcTexture = -1;
}

bool TVSTrackerEngine::resetWithFaceRect(const cv::Rect& faceRect,
                                         const int rotation) {
  _tracker.resetWithFaceRect(faceRect, rotation);
  return _tracker.isTracking();
}

cv::Rect TVSTrackerEngine::findFace(int rotation) {
 //check the correct EGL context is current
  EGLContext ctxt = eglGetCurrentContext();
  if (ctxt == EGL_NO_CONTEXT || ctxt != _eglContext) {
    printErr("No EGL context or wrong context");
    return cv::Rect(0,0,0,0);
  }

  if (!_colorConvInitialised) {
    // printInfo("Reinitialising color converter, for rotation: %d", rotation);
    cv::Size sz = _srcTextureSize;
    if (rotation == 90 || rotation == 270)
      sz = cv::Size(_srcTextureSize.height, _srcTextureSize.width);
    _colorConv.initialise(sz, rotation);
    _colorConvInitialised = true;
    // printInfo("color converter initialisation done");
  }
  _colorConv.startConverting(_srcTexture);
  _colorConv.finishConversion();

  const cv::Mat& mat = _colorConv.getResult();
  
  cv::Mat eqImg;
  cv::equalizeHist(mat, eqImg);
  _colorConv.releaseResult();

  std::vector<cv::Rect> faces;
  _face_det.detectMultiScale(eqImg, faces, 1.1, 2,
                            CV_HAAR_FIND_BIGGEST_OBJECT | CV_HAAR_SCALE_IMAGE,
                            cv::Size(120,120));
  if (faces.size() == 0) return cv::Rect(0,0,0,0);
  //center the rectangle slightly
  cv::Rect r = faces.at(0);
  r.y += static_cast<int>(0.2f * r.height);
  r.height = static_cast<int>(r.height*0.9f);
  r.x += static_cast<int>(0.05f * r.width);
  r.width = static_cast<int>(r.width*0.9f);

  if (rotation == 90) {
    r = cv::Rect(_srcTextureSize.width - r.y - r.height, r.x, r.height,
                 r.width);
  } else if (rotation == 180) {
    r = cv::Rect(_srcTextureSize.width - r.x - r.width,
                 _srcTextureSize.height - r.y - r.height, r.width, r.height);
  }
  
  return r;
}

bool TVSTrackerEngine::update(const bool predictPupils,
                              const bool highPrecision,
                              const bool smooth) {
  //check the correct EGL context is current
  EGLContext ctxt = eglGetCurrentContext();
  if (ctxt == EGL_NO_CONTEXT || ctxt != _eglContext) {
    printErr("No EGL context or wrong context");
    return false;
  }
 
  bool ret = _tracker.update(predictPupils, highPrecision, smooth);
  return ret;
}

bool TVSTrackerEngine::getRotationAngles(float *pitch, float *yaw,
                                         float *roll) {
  cv::Mat_<float> p = _tracker.getPose();
  if (!p.empty()) {
    cv::Mat_<float> R;
    cv::Rodrigues(p.rowRange(0, 3), R);
    rot2euler(R, pitch, yaw, roll);
    return true;
  }
  return false;
}
