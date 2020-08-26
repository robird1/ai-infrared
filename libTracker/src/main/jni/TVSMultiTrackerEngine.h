#ifndef __TVSMULTITRACKER_ENGINE_H__
#define __TVSMULTITRACKER_ENGINE_H__

// Modified.
#define FACE_DETECTION_METHOD    6   // 1: opencv; 2: dlib; 3: NPD; 4: facelib; 5: ACF; 6:mtcnn/ncnn.

#include <string>
#include <mutex>
#include <condition_variable>

#include <android/asset_manager.h>
#include <opencv2/core/core.hpp>

// Modified.
#if FACE_DETECTION_METHOD == 1   //opencv
#include <opencv2/objdetect/objdetect.hpp>
#elif FACE_DETECTION_METHOD == 2   // dlib.
#include "dlib/image_processing/frontal_face_detector.h"
#include "dlib/opencv.h"
#elif FACE_DETECTION_METHOD == 3   // NDP.
#include "NPDFdt/LearnGAB.hpp"
#elif FACE_DETECTION_METHOD == 4   // facelib. (another dlib?)
#include "facelib/image_processing/frontal_face_detector.h"
#include "facelib/opencv.h"
#elif FACE_DETECTION_METHOD == 5   // ACF.
#include "AcfFaceDetector/acf_detector.hpp"
#elif FACE_DETECTION_METHOD == 6   // mtcnn/ncnn.
#include "DetectMgr.h"
#endif

#include "tvs/ColorConverter.h"
#include "tvs/face_tracker.h"
#include "tvs/utils/thread_pool.h"

// Modified.
#include "tvs/ColorConverterBGRA.h"

class TrackerData_t;
struct Detection_t {
    cv::Rect rect;
    int rotation;
    std::vector<cv::Point2f> fivePoints;
};

class TVSMultiTrackerEngine {
 public:
  TVSMultiTrackerEngine(int count, bool useOGL);
  ~TVSMultiTrackerEngine();

  /*Loads models and data from the apk assets*/
  bool loadModels(AAssetManager* mgr, int minSize, const std::string& cacheDir, const std::string& sPackageName);
  /*Loads models and data from a given folder*/
  bool loadModels(const std::string& path, const std::string& sPackageName);
  /* Initialise OpenGL elements with the current EGLContext (setup in
   * the Java side).
   */
  bool setupOpenGL(const int srcTexture, const int width, const int height);
  void eglContextInvalidated();

  bool setupForByteArrayInput();


  const unsigned int getPointCount(int index) {
    if (index >= _maxTrackers || index < 0) return 0;
    return _trackers.at(index).getPointCount();
  }
  const cv::Mat_<float> getShape(int index) {
    if (index >= _maxTrackers || index < 0) return cv::Mat_<float>();
    return _trackers.at(index).getShape();
  }
  const cv::Mat_<float> getShape3D(int index) {
    if (index >= _maxTrackers || index < 0) return cv::Mat_<float>();
    return _trackers.at(index).getShape3D();
  }

  const cv::Mat_<float> getPose(int index) {
    if (index >= _maxTrackers || index < 0) return cv::Mat_<float>();
    return _trackers.at(index).getPose();
  }
  bool getRotationAngles(int index, float *pitch, float *yaw, float *roll);

  const cv::Mat_<float> getConfidence(int index) {
    if (index >= _maxTrackers || index < 0) return cv::Mat_<float>();
    return _trackers.at(index).getConfidence();
  }
  float getPoseQuality(int index) {
    if (index >= _maxTrackers || index < 0) return 0.0f;
    return _trackers.at(index).getPoseQuality();
  }

  const cv::Mat_<float> getPupilLocation(int index) {
    if (index >= _maxTrackers || index < 0) return cv::Mat_<float>();
    return _trackers.at(index).getPupilLocation();
  }
  const cv::Mat_<float> getGaze(int index) {
    if (index >= _maxTrackers || index < 0) return cv::Mat_<float>();
    return _trackers.at(index).getGaze();
  }

  /*  Rotation is that of the input texture (0, 90, 180 or 270)
   */
  bool addFaces(const std::vector<cv::Rect>& faces,
                const std::vector<int>& rotations);

  bool addFaces(const std::vector<cv::Rect>& faces,
                const std::vector<int>& rotations,
                std::vector<std::vector<cv::Point2f>> &points);
  //bool resetWithFaceRect(const cv::Rect& faceRect, const int rotation);
  /*  Rotation is that of the input texture (0, 90, 180 or 270)
   */
  //this one works with texture input
  std::vector<cv::Rect> findFaces(const int rotation);
  //this one works with matrix input
#if FACE_DETECTION_METHOD == 1   // opencv
  std::vector<cv::Rect> findFaces(const cv::Mat_<uchar>& gray,
                                    const int rotation);
#elif FACE_DETECTION_METHOD == 2   // dlib.
  std::vector<cv::Rect> findFacesByDlib(const cv::Mat &matColorImage,
                                        const int rotation);
#elif FACE_DETECTION_METHOD == 3   // NDP.
  std::vector<cv::Rect> findFacesByNpd(const cv::Mat_<uchar> &gray,
                                        const int rotation);
#elif FACE_DETECTION_METHOD == 4   // facelib.
  std::vector<cv::Rect> findFacesByFacelib(const cv::Mat &matColorImage,
                                        const int rotation);
#elif FACE_DETECTION_METHOD == 5   // ACF.
  std::vector<cv::Rect> findFacesByAcf(const cv::Mat &matColorImage,
                                        const int rotation);
#elif FACE_DETECTION_METHOD == 6   // mtcnn/ncnn.
  std::pair<std::vector<cv::Rect>, std::vector<std::vector<cv::Point2f>> > findFacesByMtcnn(const cv::Mat &matColorImage,
                                         const int rotation);
#endif

  int update(const bool predictPupils, const bool highPrecision,
              const bool smooth);
  int update(const cv::Mat_<uchar>& grayOri, const bool predictPupils,
             const bool highPrecision, const bool smooth);

  bool resetTracker(int index) {
    if (index >= _maxTrackers || index < 0) return false;
    _alive.at(index) = false;
    _trackers.at(index).release();
    return true;
  }

  void setThreshold(float first, float others);

  void setScaleDownFactor(int scaleDownFactor) {
    _scaleDownFactorByApi = scaleDownFactor;
  }

  int getScaleDownFactor() {
    return _scaleDownFactor;
  }

  bool addFacesByApi(const std::vector<cv::Rect>& faces,
                     const std::vector<int>& rotations);

  void getFaceRectBrightness(int *pBuf);

  jobjectArray processDetectorImageByNcnn(JNIEnv *env,jobject image);

    //-1 not working, 0: just working, 1 : still working
    int getTrackerState(int index) {
      int state = -1;
      if(index >= 0 && index < _maxTrackers) {
        state = _state[index];
      }
      return state;
    }

 private:
  int getAliveTrackerCount() {
    int count = 0;
    for (size_t i = 0; i < _alive.size(); i++) {
      if (_alive.at(i)) count++;
    }
    return count;
  }

  bool _useOpenGL;
  int _maxTrackers;
  std::vector<tvs::FaceTracker> _trackers;
  std::vector<bool> _alive;
  //std::vector<EGLContext> _eglContexts;
  //EGLDisplay _eglInternalDisplay;
  EGLContext _eglJavaContext = EGL_NO_CONTEXT;
  //std::vector<EGLSurface> _eglSurfaces;
  bool _modelsReady = false;

  std::unique_ptr<ThreadPool> _tpool;
  cv::Size _feature_size;
  cv::Mat_<uchar> _grayInput;

  std::mutex _mutex;
  std::condition_variable _condition_var;

  std::vector<Detection_t> _detections;

  std::vector<TrackerData_t> _trackerData; //temps for tracker data.

    // for smooth tracing
  ColorConverter _colorConv;
  bool _colorConvInitialised = false;

  // for face detection
  ColorConverter _faceDetColorConv;
  bool _faceDetColorConvInitialised = false;

  std::unique_ptr<FeatureComp> _featComp;
  int _srcTexture;
  cv::Size _srcTextureSize;

  int _scaleDownFactor = 1, _scaleDownFactorByApi = 0;
  unsigned char *_pColorImgBuf = NULL,
                *_pGrayImgBuf = NULL;
  int _colorImgBufSize = 0,
      _grayImgBufSize = 0;

  void determineScaleDownFactor(const int w, const int h) {

#if FACE_DETECTION_METHOD == 5   // ACF.

	if (_scaleDownFactorByApi > 0) {
		_scaleDownFactor = _scaleDownFactorByApi;
		return;
	}

	const int MIN_WH = 200;

	if (w <= MIN_WH || h <= MIN_WH)
	  _scaleDownFactor = 1;
	else {
	  int wScale = w / MIN_WH,
		  hScale = h / MIN_WH;
	  _scaleDownFactor = wScale < hScale ? wScale : hScale;
	}

#else

	_scaleDownFactor = 1;

#endif
  }

  // Store the face rectangles input by api.
  // These rectangles will be scaled with _scaleDownFactor.
  std::vector<cv::Rect> _facesByApi;
  std::vector<int> _faceRotationsByApi;

  void loadFacesAddedByApi();

  // Modified.
#if FACE_DETECTION_METHOD == 1   // opencv
  cv::CascadeClassifier _face_det;
#elif FACE_DETECTION_METHOD == 2   // dlib.
  dlib::frontal_face_detector _dlibFaceDetector;
#elif FACE_DETECTION_METHOD == 3   // NPD face detector.
  GAB _gab;
#elif FACE_DETECTION_METHOD == 4   // facelib.
  dlib::frontal_face_detector _dlibFaceDetector;
#elif FACE_DETECTION_METHOD == 5   // ACF.
  std::shared_ptr<ulsee::AcfDetector> _pAcfFaceDet;
#elif FACE_DETECTION_METHOD == 6   // ncnn.
  std::shared_ptr<ulsee::FaceDetectMgr> _pDetectionNcnn;
#endif

    std::vector<int> _state;// -1 means not working, 0 means just working, 1 means working.
};

#endif //  __TVSMULTITRACKER_ENGINE_H__
