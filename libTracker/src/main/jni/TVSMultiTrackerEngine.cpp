

#include <android/log.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <algorithm>

#include "TVSMultiTrackerEngine.h"
#include "tvs/utils/android_assets.h"
#include "tvs/utils/geometry.h"

#include "ProcessUtils.h"

using std::string;

#ifdef TVS_INTERNAL_DEBUGGING
const char kLogTag[] = "UlsMultiTrackerEngine";
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

class TrackerData_t {
 public:
  tvs::FaceTracker* tracker;
  bool pupils = false, highPrec = false, smooth = false;

  cv::Mat_<uchar> image, crop; //depending on usage, image may be empty
  cv::Mat_<float> M;
  tvs::GrayInputConversion grayInput;
  bool state = false;
};

TVSMultiTrackerEngine::TVSMultiTrackerEngine(int count, bool useOGL) {
  _maxTrackers = std::max(count, 1);
  _trackers.resize(_maxTrackers);
  _alive.resize(_maxTrackers, false);
  _state.resize(_maxTrackers);
  for(int i = 0; i < _maxTrackers; i++) {
    _state[i] = -1;
  }
  _tpool = std::unique_ptr<ThreadPool>(new ThreadPool(_maxTrackers));
  _trackerData.resize(_maxTrackers);

  _useOpenGL = useOGL;
  if (_useOpenGL)
    _featComp = std::unique_ptr<FeatureComp>(new FeatureComp(_maxTrackers));

  // Modified.
#if FACE_DETECTION_METHOD == 2   // dlib.
  _dlibFaceDetector = dlib::get_frontal_face_detector();
#elif FACE_DETECTION_METHOD == 4   // facelib.
  _dlibFaceDetector = dlib::get_frontal_face_detector();
#elif FACE_DETECTION_METHOD == 5   // ACF.
  _pAcfFaceDet = std::make_shared<ulsee::AcfDetector>();
#elif FACE_DETECTION_METHOD == 6   // ncnn.
  _pDetectionNcnn = std::make_shared<ulsee::FaceDetectMgr>();
#endif
}

TVSMultiTrackerEngine::~TVSMultiTrackerEngine() {
  if (_pColorImgBuf != NULL)
    delete _pColorImgBuf;

  if (_pGrayImgBuf != NULL)
    delete _pGrayImgBuf;
}

bool TVSMultiTrackerEngine::loadModels(AAssetManager *mgr, int minSize,
                                       const string& cacheDir, const string& sPackageName) {
  for (int k = 0; k < _maxTrackers; k++) {
    // traker models
    int ret =  _trackers.at(k).loadModels(mgr, cacheDir, sPackageName);
    if (ret < 0) {
      printErr("Error loading models from assets: %d", ret);
      _modelsReady = false;
      return false;
    } else {
      _modelsReady = true;
    }
  }
  _feature_size = _trackers.at(0).getFeatureSize();

  const std::string tmpFN = cacheDir + "/tracker.tmp";

    /*
     * models
     * opencv:  face.xml
     * NPD:     faceModel.dat
     * ACF:     ulsFace.model
     * mtcnn:   model(1-3).bin
     * */
#if FACE_DETECTION_METHOD == 1   // opencv
  if (!copyFileFromAsset(mgr, "ULSFaceTrackerAssets/face.xml", tmpFN)) {
    printErr("Error copying face.xml from assets");
    return false;
  }
  if (!_face_det.load(tmpFN)) {
    unlink(tmpFN.c_str());
    printErr("Error loading face detector file from assets");
    return false;
  }
#elif FACE_DETECTION_METHOD == 3   // NPD face detector.
  if (!copyFileFromAsset(mgr, "ULSFaceTrackerAssets/faceModel.dat", tmpFN)) {
    unlink(tmpFN.c_str());
    printErr("Error copying faceModel.dat from assets");
    return false;
  }

  _gab.LoadModel(tmpFN);
  printDebug("NPD face model is loaded.");
#elif FACE_DETECTION_METHOD == 5   // ACF face detector.
  // Copy the face model file from project’s asset to a temp file and then load it.
  if (!copyFileFromAsset(mgr, "ULSFaceTrackerAssets/ulsFace.model", tmpFN)) {
    unlink(tmpFN.c_str());
    printErr("Error copying ulsFace.model from assets");
    return false;
  }

  bool bSucceed = _pAcfFaceDet->loadModel(tmpFN.c_str());
  if (bSucceed)
	printDebug("ACF face model is loaded.");
  else {
    unlink(tmpFN.c_str());
	printErr("ACF face model is NOT loaded.");
	return false;
  }

  _pAcfFaceDet->setCascParams(-1.0f, -0.01f);
//  _pAcfFaceDet->setMaxImageSize(800);
#elif FACE_DETECTION_METHOD == 6   // mtcnn/ncnn face detector.
    _pDetectionNcnn->initDetector(minSize, mgr);
#endif

  unlink(tmpFN.c_str());

  return _modelsReady;
}

bool TVSMultiTrackerEngine::loadModels(const string& path, const string& sPackageName) {
   // tracker models
  for (int k = 0; k < _maxTrackers; k++) {
    int ret =  _trackers.at(k).loadModels(path, sPackageName);
    if (ret < 0) {
      printErr("Error loading models from path %s: %d", path.c_str(), ret);
      _modelsReady = false;
      return false;
    } else {
      _modelsReady = true;
    }
  }
  _feature_size = _trackers.at(0).getFeatureSize();

#if FACE_DETECTION_METHOD == 1   // opencv
  if (!_face_det.load(path + "/face.xml")) {
    printErr("Error loading face detector file");
    return false;
  }
#elif FACE_DETECTION_METHOD == 3   // NPD face detector.
  std::string npdModelFile = path + "/faceModel.dat";
  _gab.LoadModel(npdModelFile);
  printDebug("NPD face model is loaded.");
#elif FACE_DETECTION_METHOD == 5   // ACF face detector.
  // Load face model file.
  std::string sModelFile = path + "/ulsFace.model";
  bool bSucceed = _pAcfFaceDet->loadModel(sModelFile.c_str());
  if (bSucceed)
	printDebug("ACF face model is loaded.");
  else {
	printErr("ACF face model is NOT loaded.");
	return false;
  }

  _pAcfFaceDet->setCascParams(-1.0f, -0.01f);
//  _pAcfFaceDet->setMaxImageSize(800);
#endif

  return true;
}

bool TVSMultiTrackerEngine::setupOpenGL(const int srcTexture, const int width,
                                        const int height) {
  if (!_useOpenGL) {
    printErr("Error - called setupOpenGL in a tracker using nv21 input");
    return false;
  }
  //check we have a context setup
  EGLContext ctxt = eglGetCurrentContext();
  if (ctxt == EGL_NO_CONTEXT) {
    printErr("No EGL context");
    return false;
  }
  if (_eglJavaContext != EGL_NO_CONTEXT) {
    printWarn("An EGL context was already setup. It will be discarded");
  }

  //printInfo("Setting up oGL. javaCtxt: %p", ctxt);

  _feature_size = _trackers.at(0).getFeatureSize();

  _eglJavaContext = ctxt;
  _srcTexture = srcTexture;
  _srcTextureSize = cv::Size(width, height);

  _featComp->setSizes(_srcTextureSize, _feature_size);
  _featComp->initialiseGPU();

  _colorConv.initialise(_srcTextureSize);
  _colorConvInitialised = true;
  _faceDetColorConvInitialised = false; // this is done later!

  bool initOK = true;

  // for (int i = 0; i < _maxTrackers; i++) {
  //   _trackers.at(i).initialiseWithSrcTexture(srcTexture, _srcTextureSize);
  // }
  /*
  EGLDisplay edisplay = eglGetCurrentDisplay();
  EGLSurface eDrawSurf = eglGetCurrentSurface(EGL_DRAW);
  EGLSurface eReadSurf = eglGetCurrentSurface(EGL_READ);

  EGLint configID;
  EGLConfig config;
  eglQueryContext(edisplay, ctxt, EGL_CONFIG_ID, &configID);
  printInfo("EGLConfigID: %d", configID);

  _eglInternalDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  eglInitialize(_eglInternalDisplay, NULL, NULL);
  EGLint eglerr = eglGetError();
  if (eglerr != EGL_SUCCESS) {
    printErr("Error getting display: %d", eglerr);
  }

  const EGLint ctxAttr[] = {
    EGL_CONTEXT_CLIENT_VERSION, 2,
    EGL_NONE
  };
  const EGLint surfaceAttr[] = {
    EGL_WIDTH, 1,
    EGL_HEIGHT, 1,
    EGL_NONE
  };

  const EGLint confAttr[] = {
    EGL_CONFIG_ID, configID,
    EGL_NONE
  };
  int confCount = 0;
  eglChooseConfig(_eglInternalDisplay, confAttr, NULL, 0, &confCount);
  printInfo("Configurations available: %d", confCount);
  if (eglChooseConfig(_eglInternalDisplay, confAttr, &config, 1, &confCount)
      != EGL_TRUE) {
    printErr("Error choosing config: %d - confCount: %d", eglGetError(),
             confCount);
  }

  bool initOK = false;
  for (int i = 0; i < _maxTrackers; i++) {
    //create a context with shared context for each tracker
    _eglContexts.at(i) = eglCreateContext(_eglInternalDisplay, config, ctxt,
                                          ctxAttr);
    _eglSurfaces.at(i) = eglCreatePbufferSurface(_eglInternalDisplay, config,
                                                 surfaceAttr);
    eglMakeCurrent(_eglInternalDisplay, _eglSurfaces.at(i), _eglSurfaces.at(i),
                   _eglContexts.at(i));
    EGLint err = eglGetError();
    if (err != EGL_SUCCESS) {
      printErr("EGL error: %d\n", err);
    }

    initOK = _trackers.at(i).initialiseWithSrcTexture(
        srcTexture, cv::Size(width, height));
    if (!initOK) {
      printErr("Error initialising tracker %d with texture", i);
    }
  }

  //reset the context
  if (eglMakeCurrent(edisplay, eReadSurf, eDrawSurf, ctxt) != EGL_TRUE) {
    printErr("Error reseting context: %d\n", eglGetError());
    }*/



  printInfo("Done setupOpenGL");
  return initOK;
}

bool TVSMultiTrackerEngine::setupForByteArrayInput() {

  return true;
}

void TVSMultiTrackerEngine::eglContextInvalidated() {
  if (!_useOpenGL) {
    printErr("Error - trying to invalidate an EGL context in a tracker using nv21 input");
    return;
  }
#ifdef DEBUG
  if (_eglJavaContext != EGL_NO_CONTEXT) {
    printWarn("EGL context was already EGL_NO_CONTEXT");
  }
#endif
  _eglJavaContext = EGL_NO_CONTEXT;

  _colorConvInitialised = false;

  _faceDetColorConvInitialised = false;
  _srcTexture = -1;
  //  printInfo("Release egl context");
}


std::vector<cv::Rect> TVSMultiTrackerEngine::findFaces(const int rotation) {
  if (!_useOpenGL) {
    printErr("Error - called findFaces() with a GL texture in a tracker using nv21 input!!");
    return std::vector<cv::Rect>();
  }
  //check the correct EGL context is current
  EGLContext ctxt = eglGetCurrentContext();
  if (ctxt == EGL_NO_CONTEXT || ctxt != _eglJavaContext) {
    printErr("No EGL context or wrong context");
    return std::vector<cv::Rect>();
  }

  if (!_faceDetColorConvInitialised ||
      _faceDetColorConv.getRotation() != rotation) {
    printInfo("(Re)Initialising (faceDet) color converter, for rotation: %d",
              rotation);
    cv::Size sz = _srcTextureSize;
    if (rotation == 90 || rotation == 270) {
      sz = cv::Size(_srcTextureSize.height, _srcTextureSize.width);
    }
    _faceDetColorConv.initialise(sz, rotation);
    _faceDetColorConvInitialised = true;
    // printInfo("color converter initialisation done");
  }
  _faceDetColorConv.startConverting(_srcTexture);
  _faceDetColorConv.finishConversion();

  // to be confirmed, mat from Color Convertion is supposed to be gray image
  const cv::Mat& mat = _faceDetColorConv.getResult();
  std::vector<cv::Rect> faces;

#if 0 // it should be invalidated
  // Modified. Get color image from OpenGL texture.
  if (!_colorConvBGRAInitialised) {
    // printInfo("Reinitialising color converter, for rotation: %d", rotation);
    cv::Size sz = _srcTextureSize;
    if (rotation == 90 || rotation == 270)
      sz = cv::Size(_srcTextureSize.height, _srcTextureSize.width);
    _colorConvBGRA.initialise(sz, rotation);
    _colorConvBGRAInitialised = true;
    // printInfo("color converter initialisation done");
  }
  _colorConvBGRA.startConverting(_srcTexture);
  _colorConvBGRA.finishConversion();
  const cv::Mat& matColorImg = _colorConvBGRA.getResult();
  _colorConvBGRA.releaseResult();
#endif

  //to be validated
#if FACE_DETECTION_METHOD == 1   // opencv.
  cv::Mat eqImg;
  cv::equalizeHist(mat, eqImg);

  _face_det.detectMultiScale(eqImg, faces, 1.1, 2,
//                             CV_HAAR_FIND_BIGGEST_OBJECT | CV_HAAR_SCALE_IMAGE,
                             CV_HAAR_DO_CANNY_PRUNING | CV_HAAR_SCALE_IMAGE,
                             cv::Size(120,120));
  for (size_t k = 0; k < faces.size(); k++) {
    cv::Rect &r = faces.at(k);
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
    } else if (rotation == 270) {
      r = cv::Rect(r.y, _srcTextureSize.width - r.x - r.width, r.height,
                   r.width);
    }
  }
#elif FACE_DETECTION_METHOD == 2   // dlib.
  cv::Mat dst;
  //cvtColor(matColorImg, dst, CV_BGRA2BGR);
  cvtColor(mat, dst, CV_GRAY2BGR);

  // Detect faces using dlib.
  dlib::cv_image<dlib::bgr_pixel> cimg(dst);
  std::vector<dlib::rectangle> facesDlib = _dlibFaceDetector(cimg);

  // Debug only.
//  char szBuf[500];
//  static int count = 0;
//  sprintf(szBuf, "%s%d%s", "/storage/emulated/0/Pictures/ImageFromTexture", ++count, ".png");
//  imwrite(szBuf, dst);
//  sprintf(szBuf, "%s%d", "Number of faces detected by dlib: ", facesDlib.size());
//  __android_log_print(ANDROID_LOG_DEBUG, kLogTag, szBuf);

  // Copy dlib face rectangles to opencv rect.
  for (size_t k = 0; k < facesDlib.size(); k++) {
    cv::Rect r;
    r.x = facesDlib[k].left();
	r.y = facesDlib[k].top();
	r.width = facesDlib[k].right() - r.x;
	r.height = facesDlib[k].bottom() - r.y;

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
    } else if (rotation == 270) {
      r = cv::Rect(r.y, _srcTextureSize.width - r.x - r.width, r.height,
                   r.width);
    }

    faces.push_back(r);
  }
#elif FACE_DETECTION_METHOD == 6   // mtcnn/ncnn.
  cv::Mat dst;
  //cvtColor(matColorImg, dst, CV_BGRA2BGR);
  cvtColor(mat, dst, CV_GRAY2BGR);

  std::vector<std::vector<float>> total_boxes, points;
  _pDetectionNcnn->detectFace(dst, total_boxes, points);

  cv::Mat matColorImage = dst;
  auto length = total_boxes.size();
  for (size_t k = 0; k < length; k++) {

    cv::Rect r;
    std::vector<cv::Point2f> fivePoint;

    if (rotation == 270) {
//      {
//        float x0 = points[k][6];
//        float x1 = points[k][5];
//        float x2 = points[k][7];
//        float x3 = points[k][9];
//        float x4 = points[k][8];
//        float y0 = points[k][1];
//        float y1 = points[k][0];
//        float y2 = points[k][2];
//        float y3 = points[k][4];
//        float y4 = points[k][3];
//
//        fivePoint.push_back(cv::Point2f(x0,y0));
//        fivePoint.push_back(cv::Point2f(x1,y1));
//        fivePoint.push_back(cv::Point2f(x2,y2));
//        fivePoint.push_back(cv::Point2f(x3,y3));
//        fivePoint.push_back(cv::Point2f(x4,y4));
//      }

      {
        int x1 = total_boxes[k][1];
        int y1 = total_boxes[k][0];
        int x2 = total_boxes[k][3];
        int y2 = total_boxes[k][2];
        r = cv::Rect(x1,y1, x2-x1, y2- y1);
      }



    } else if (rotation == 180) {
//      {
//        float x0 = matColorImage.cols - points[k][0];
//        float x1 = matColorImage.cols - points[k][1];
//        float x2 = matColorImage.cols - points[k][2];
//        float x3 = matColorImage.cols - points[k][3];
//        float x4 = matColorImage.cols - points[k][4];
//        float y0 = matColorImage.rows - points[k][5];
//        float y1 = matColorImage.rows - points[k][6];
//        float y2 = matColorImage.rows - points[k][7];
//        float y3 = matColorImage.rows - points[k][8];
//        float y4 = matColorImage.rows - points[k][9];
//
//        fivePoint.push_back(cv::Point2f(x0,y0));
//        fivePoint.push_back(cv::Point2f(x1,y1));
//        fivePoint.push_back(cv::Point2f(x2,y2));
//        fivePoint.push_back(cv::Point2f(x3,y3));
//        fivePoint.push_back(cv::Point2f(x4,y4));
//      }
      {
        int x1 = matColorImage.cols - total_boxes[k][2];
        int y1 = matColorImage.rows - total_boxes[k][3];
        int x2 = matColorImage.cols - total_boxes[k][0];
        int y2 = matColorImage.rows - total_boxes[k][1];
        r = cv::Rect(x1,y1, x2-x1, y2- y1);
      }

    } else if (rotation == 90) {
      //  r = cv::Rect(matColorImage.cols - r.y - r.width, r.x, r.height, r.width);

      //原始
//      float x0 = matColorImage.cols - points[k][6];
//      float x1 = matColorImage.cols - points[k][5];
//      float x2 = matColorImage.cols - points[k][7];
//      float x3 = matColorImage.cols - points[k][9];
//      float x4 = matColorImage.cols - points[k][8];
//      float y0 = matColorImage.rows - points[k][1];
//      float y1 = matColorImage.rows - points[k][0];
//      float y2 = matColorImage.rows - points[k][2];
//      float y3 = matColorImage.rows - points[k][4];
//      float y4 = matColorImage.rows - points[k][3];
//
//      fivePoint.push_back(cv::Point2f(x0,y0));
//      fivePoint.push_back(cv::Point2f(x1,y1));
//      fivePoint.push_back(cv::Point2f(x2,y2));
//      fivePoint.push_back(cv::Point2f(x3,y3));
//      fivePoint.push_back(cv::Point2f(x4,y4));

      //计算框的大小
      int a1 = matColorImage.cols - total_boxes[k][3];
      int b1 = matColorImage.rows - total_boxes[k][2];
      int a2 = matColorImage.cols - total_boxes[k][1];
      int b2 = matColorImage.rows - total_boxes[k][0];
      r = cv::Rect(a1,b1, a2-a1, b2- b1);

    }else {
      fivePoint.push_back(cv::Point2f(points[k][0],points[k][5]));
      fivePoint.push_back(cv::Point2f(points[k][1],points[k][6]));
      fivePoint.push_back(cv::Point2f(points[k][2],points[k][7]));
      fivePoint.push_back(cv::Point2f(points[k][3],points[k][8]));
      fivePoint.push_back(cv::Point2f(points[k][4],points[k][9]));


      r = cv::Rect(total_boxes[k][0], total_boxes[k][1], total_boxes[k][2] - total_boxes[k][0], total_boxes[k][3] - total_boxes[k][1]);
    }

    //mtcnn no need to adjust the face rects as opencv
//    r.y += static_cast<int>(0.2f * r.height);
//    r.height = static_cast<int>(r.height * 0.9f);
//    r.x += static_cast<int>(0.05f * r.width);
//    r.width = static_cast<int>(r.width * 0.9f);

    faces.push_back(r);
//    fivepoints.push_back(fivePoint);
  }

#endif

  _faceDetColorConv.releaseResult();

  return faces;
}

// Modified.
#if FACE_DETECTION_METHOD == 1   // opencv
std::vector<cv::Rect> TVSMultiTrackerEngine::findFaces(
        const cv::Mat_<uchar>& gray, const int rotation) {
  cv::Mat eqImg, dst;

  //apply rotations
  if (rotation == 0) {
    dst = gray;
  } else if (rotation == 90) {
    cv::flip(gray.t(), dst, 0);
  } else if (rotation == 180) {
    cv::flip(gray, dst, -1);
  } else if (rotation == 270) {
    cv::flip(gray.t(), dst, 1);
  }

  cv::equalizeHist(dst, eqImg);

  std::vector<cv::Rect> faces;

  // Notes about face detection settings.
  //  _face_det.detectMultiScale(eqImg, faces, param1, param2,
  //                             CV_HAAR_FIND_BIGGEST_OBJECT | CV_HAAR_SCALE_IMAGE,
  //                             cv::Size(param3, param3));
  //
  // Candidates for param1, param2 and param3
  //  param1 = 1.3, 1.4, 1.5 (larger and faster)
  //  param2 = 2, 3, 4 (low relation with speed)
  //  param3 = 40, 50, 60, 70, 80 (low relation with speed)
  //
  // Original setting.
  _face_det.detectMultiScale(eqImg, faces, 1.1, 2,
//                             CV_HAAR_FIND_BIGGEST_OBJECT | CV_HAAR_SCALE_IMAGE,
                             CV_HAAR_DO_CANNY_PRUNING | CV_HAAR_SCALE_IMAGE,
                             cv::Size(120,120));
//  _face_det.detectMultiScale(eqImg, faces, 1.5, 4,
//                             CV_HAAR_FIND_BIGGEST_OBJECT | CV_HAAR_SCALE_IMAGE,
//                             cv::Size(80,80));

  for (size_t k = 0; k < faces.size(); k++) {
    cv::Rect &r = faces.at(k);
    r.y += static_cast<int>(0.2f * r.height);
    r.height = static_cast<int>(r.height*0.9f);
    r.x += static_cast<int>(0.05f * r.width);
    r.width = static_cast<int>(r.width*0.9f);

    if (rotation == 270) {
//      r = cv::Rect(gray.rows - r.x - r.height, r.x, r.height, r.width);   Modified.
      r = cv::Rect(r.y, gray.rows - r.x - r.height, r.height, r.width);
    } else if (rotation == 180) {
      r = cv::Rect(gray.cols - r.x - r.width, gray.rows - r.y - r.height,
                   r.width, r.height);
    } else if (rotation == 90) {
      r = cv::Rect(gray.cols - r.y - r.width, r.x, r.height, r.width);
    }
  }

  return faces;
}
#elif FACE_DETECTION_METHOD == 2   // dlib.
std::vector<cv::Rect> TVSMultiTrackerEngine::findFacesByDlib(const cv::Mat &matColorImage,
                                                             const int rotation) {
  cv::Mat dst;

  // Rotate to get upright faces.
  if (rotation == 0) {
    dst = matColorImage;
  } else if (rotation == 90) {
    cv::flip(matColorImage.t(), dst, 0);
  } else if (rotation == 180) {
    cv::flip(matColorImage, dst, -1);
  } else if (rotation == 270) {
    cv::flip(matColorImage.t(), dst, 1);
  }

  cvtColor(dst, dst, CV_BGRA2BGR);

  // Detect faces using dlib.
  dlib::cv_image<dlib::bgr_pixel> cimg(dst);
  std::vector<dlib::rectangle> facesDlib = _dlibFaceDetector(cimg);

  // Debug only.
//  char szBuf[500];
//  static int count = 0;
//  sprintf(szBuf, "%s%d%s", "/storage/emulated/0/Pictures/ImageFromMat", ++count, ".png");
//  imwrite(szBuf, dst);
//  sprintf(szBuf, "%s%d", "Number of faces detected by dlib: ", facesDlib.size());
//  __android_log_print(ANDROID_LOG_DEBUG, kLogTag, szBuf);

  // Copy dlib face rectangles to opencv rect.
  std::vector<cv::Rect> faces;

  for (size_t k = 0; k < facesDlib.size(); k++) {
    cv::Rect r;
    r.x = facesDlib[k].left();
	r.y = facesDlib[k].top();
	r.width = facesDlib[k].right() - r.x;
	r.height = facesDlib[k].bottom() - r.y;

    r.y += static_cast<int>(0.2f * r.height);
    r.height = static_cast<int>(r.height*0.9f);
    r.x += static_cast<int>(0.05f * r.width);
    r.width = static_cast<int>(r.width*0.9f);

    if (rotation == 270) {
//      r = cv::Rect(matColorImage.rows - r.x - r.height, r.x, r.height, r.width);   Modified.
      r = cv::Rect(r.y, matColorImage.rows - r.x - r.height, r.height, r.width);
    } else if (rotation == 180) {
      r = cv::Rect(matColorImage.cols - r.x - r.width, matColorImage.rows - r.y - r.height,
                   r.width, r.height);
    } else if (rotation == 90) {
      r = cv::Rect(matColorImage.cols - r.y - r.width, r.x, r.height, r.width);
    }

    faces.push_back(r);
  }

  return faces;
}
#elif FACE_DETECTION_METHOD == 3   // NPD face detector.
std::vector<cv::Rect> TVSMultiTrackerEngine::findFacesByNpd(
        const cv::Mat_<uchar>& gray, const int rotation) {
  cv::Mat dst;

  //apply rotations
  if (rotation == 0) {
    dst = gray;
  } else if (rotation == 90) {
    cv::flip(gray.t(), dst, 0);
  } else if (rotation == 180) {
    cv::flip(gray, dst, -1);
  } else if (rotation == 270) {
    cv::flip(gray.t(), dst, 1);
  }

  vector<cv::Rect> rects;
  vector<float> scores;
  vector<int> index;
  index = _gab.DetectFace(dst, rects, scores);

  std::vector<cv::Rect> faces;

  for (size_t k = 0; k < index.size(); k++) {
	if (scores[index[k]] <= 100) continue;

    cv::Rect r;
    r = rects[index[k]];

    r.x += r.width / 2;
    r.y += r.height / 2;

    r.y += static_cast<int>(0.2f * r.height);
    r.height = static_cast<int>(r.height*0.9f);
    r.x += static_cast<int>(0.05f * r.width);
    r.width = static_cast<int>(r.width*0.9f);

    if (rotation == 270) {
      r = cv::Rect(r.y, dst.rows - r.x - r.height, r.height, r.width);
    } else if (rotation == 180) {
      r = cv::Rect(dst.cols - r.x - r.width, dst.rows - r.y - r.height,
                   r.width, r.height);
    } else if (rotation == 90) {
      r = cv::Rect(dst.cols - r.y - r.width, r.x, r.height, r.width);
    }

    faces.push_back(r);
  }

  return faces;
}
#elif FACE_DETECTION_METHOD == 4   // facelib.
std::vector<cv::Rect> TVSMultiTrackerEngine::findFacesByFacelib(const cv::Mat &matColorImage,
                                                             const int rotation) {
  cv::Mat dst;

  // Rotate to get upright faces.
  if (rotation == 0) {
    dst = matColorImage;
  } else if (rotation == 90) {
    cv::flip(matColorImage.t(), dst, 0);
  } else if (rotation == 180) {
    cv::flip(matColorImage, dst, -1);
  } else if (rotation == 270) {
    cv::flip(matColorImage.t(), dst, 1);
  }

  cvtColor(dst, dst, CV_BGRA2BGR);

  int imageShortSide = std::min(dst.cols, dst.rows);
  double imageScale = 1;

  if (imageShortSide < 450 || imageShortSide > 500) {
    imageScale = 500.0 / imageShortSide;
    cv::resize(dst, dst, cv::Size(), imageScale, imageScale);
  }

  // Detect faces using facelib.
  dlib::cv_image<dlib::bgr_pixel> cimg(dst);
  std::vector<dlib::rectangle> facesDlib = _dlibFaceDetector(cimg);

  // Debug only.
//  char szBuf[500];
//  static int count = 0;
//  sprintf(szBuf, "%s%d%s", "/storage/emulated/0/Pictures/ImageFromMat", ++count, ".png");
//  imwrite(szBuf, dst);
//  sprintf(szBuf, "%s%d", "Number of faces detected by facelib: ", facesDlib.size());
//  __android_log_print(ANDROID_LOG_DEBUG, kLogTag, szBuf);

  // Copy dlib face rectangles to opencv rect.
  std::vector<cv::Rect> faces;

  for (size_t k = 0; k < facesDlib.size(); k++) {
    cv::Rect r;
    r.x = facesDlib[k].left() / imageScale;
	r.y = facesDlib[k].top() / imageScale;
	r.width = facesDlib[k].right() / imageScale - r.x;
	r.height = facesDlib[k].bottom() / imageScale - r.y;

    r.y += static_cast<int>(0.2f * r.height);
    r.height = static_cast<int>(r.height*0.9f);
    r.x += static_cast<int>(0.05f * r.width);
    r.width = static_cast<int>(r.width*0.9f);

    if (rotation == 270) {
//      r = cv::Rect(matColorImage.rows - r.x - r.height, r.x, r.height, r.width);   Modified.
      r = cv::Rect(r.y, matColorImage.rows - r.x - r.height, r.height, r.width);
    } else if (rotation == 180) {
      r = cv::Rect(matColorImage.cols - r.x - r.width, matColorImage.rows - r.y - r.height,
                   r.width, r.height);
    } else if (rotation == 90) {
      r = cv::Rect(matColorImage.cols - r.y - r.width, r.x, r.height, r.width);
    }

    faces.push_back(r);
  }

  return faces;
}
#elif FACE_DETECTION_METHOD == 5   // ACF.
std::vector<cv::Rect> TVSMultiTrackerEngine::findFacesByAcf(const cv::Mat &matColorImage,
                                                             const int rotation) {
  cv::Mat dst;
  double imgWidthBound, imgHeightBound;

  // Rotate to get upright faces.
  if (rotation == 0) {
    dst = matColorImage;

	// Parameters for resize.
//    imgWidthBound = 640;
//    imgHeightBound = 480;
  } else if (rotation == 90) {
    cv::flip(matColorImage.t(), dst, 0);

	// Parameters for resize.
//	imgWidthBound = 480;
//	imgHeightBound = 640;
  } else if (rotation == 180) {
    cv::flip(matColorImage, dst, -1);

	// Parameters for resize.
//    imgWidthBound = 640;
//    imgHeightBound = 480;
  } else if (rotation == 270) {
    cv::flip(matColorImage.t(), dst, 1);

	// Parameters for resize.
//	imgWidthBound = 480;
//	imgHeightBound = 640;
  }

  cvtColor(dst, dst, CV_BGRA2RGB);

  // Scale down image with OpenCV.
//  double widthScale = static_cast<double>(imgWidthBound) / dst.cols,
//		 heightScale = static_cast<double>(imgHeightBound) / dst.rows;
//  double scaleMin = widthScale < heightScale ? widthScale : heightScale;
//  double imgScale = 1;

//  if (scaleMin < 1) {
//	imgScale = scaleMin;
//    cv::resize(dst, dst, cv::Size(), imgScale, imgScale);
//  }

  int srcw = dst.cols;
  int srch = dst.rows;
  determineScaleDownFactor(srcw, srch);

  cv::Mat *pScaledDownImg = NULL;

  if (_scaleDownFactor == 1) {
    pScaledDownImg = &dst;
  } else {
    // Scale down image with down sampling (from Jethro).
    unsigned char *pImgData = dst.ptr<unsigned char>();
    int imgw = srcw / _scaleDownFactor;
    int imgh = srch / _scaleDownFactor;
    int bufSize = imgw * imgh * 3;
    if (_pColorImgBuf == NULL || _colorImgBufSize < bufSize) {
      if (_pColorImgBuf != NULL) delete _pColorImgBuf;
      _colorImgBufSize = bufSize;
      _pColorImgBuf = new unsigned char[_colorImgBufSize];
    }

    int mm, nn, ii, count = 0;
    int hBound = imgh*_scaleDownFactor,
        wBound = imgw*_scaleDownFactor;

    for (int m = 0; m < hBound; m += _scaleDownFactor) {
      mm = 3 * m*srcw;   // 3 for RGB.
      for (int n = 0; n < wBound; n += _scaleDownFactor) {
        nn = 3 * n;
        ii = 3 * count;
        _pColorImgBuf[ii    ] = pImgData[mm + nn];
        _pColorImgBuf[ii + 1] = pImgData[mm + nn + 1];
        _pColorImgBuf[ii + 2] = pImgData[mm + nn + 2];
        count++;
      }
    }

    pScaledDownImg = new cv::Mat(imgh, imgw, CV_8UC3, _pColorImgBuf);
  }

  uint32_t imH = pScaledDownImg->rows;
  uint32_t imW = pScaledDownImg->cols;
  ulsee::ImageData im(imH, imW, 3);

  for (int x = 0; x < pScaledDownImg->cols; x++) {
      for (int y = 0; y < pScaledDownImg->rows; y++) {
          cv::Vec3b p = pScaledDownImg->at<cv::Vec3b>(y, x);
          // BGR -> RGB
          im.data[0 * pScaledDownImg->cols * pScaledDownImg->rows + pScaledDownImg->rows * x + y] = p.val[2];// R
          im.data[1 * pScaledDownImg->cols * pScaledDownImg->rows + pScaledDownImg->rows * x + y] = p.val[1];// G
          im.data[2 * pScaledDownImg->cols * pScaledDownImg->rows + pScaledDownImg->rows * x + y] = p.val[0];// B
      }
  }

  std::vector<ulsee::Detection> facesAcf;
  if (_pAcfFaceDet)
    facesAcf = _pAcfFaceDet->detect(im);

  // Debug only.
//  char szBuf[500];
//  static int count = 0;
//  sprintf(szBuf, "%s%d%s", "/storage/emulated/0/Pictures/ImageFromMat", ++count, ".png");
//  imwrite(szBuf, *pScaledDownImg);
//  sprintf(szBuf, "%s%d", "Number of faces detected by ACF: ", facesAcf.size());
//  __android_log_print(ANDROID_LOG_DEBUG, kLogTag, szBuf);

  if (_scaleDownFactor != 1)
    delete pScaledDownImg;

  // Copy face rectangles to opencv rect.
  std::vector<cv::Rect> faces;

  for (size_t k = 0; k < facesAcf.size(); k++) {
    cv::Rect r;
    r.x = facesAcf[k].bbox.x * _scaleDownFactor;
	r.y = facesAcf[k].bbox.y * _scaleDownFactor;
	r.width = facesAcf[k].bbox.w * _scaleDownFactor;
	r.height = facesAcf[k].bbox.h * _scaleDownFactor;

    r.y += static_cast<int>(0.2f * r.height);
    r.height = static_cast<int>(r.height*0.9f);
    r.x += static_cast<int>(0.05f * r.width);
    r.width = static_cast<int>(r.width*0.9f);

    if (rotation == 270) {
//      r = cv::Rect(matColorImage.rows - r.x - r.height, r.x, r.height, r.width);   Modified.
//      r = cv::Rect(r.y, matColorImage.rows - r.x - r.height, r.height, r.width);
      r = cv::Rect(r.y / _scaleDownFactor,
                        (matColorImage.rows - r.x - r.height) / _scaleDownFactor,
                        r.height / _scaleDownFactor,
                        r.width / _scaleDownFactor);
    } else if (rotation == 180) {
//      r = cv::Rect(matColorImage.cols - r.x - r.width, matColorImage.rows - r.y - r.height,
//                   r.width, r.height);
      r = cv::Rect((matColorImage.cols - r.x - r.width) / _scaleDownFactor,
                      (matColorImage.rows - r.y - r.height) / _scaleDownFactor,
                      r.width / _scaleDownFactor,
                      r.height / _scaleDownFactor);
    } else if (rotation == 90) {
//      r = cv::Rect(matColorImage.cols - r.y - r.width, r.x, r.height, r.width);
      r = cv::Rect((matColorImage.cols - r.y - r.width) / _scaleDownFactor,
                      r.x / _scaleDownFactor,
                      r.height / _scaleDownFactor,
                      r.width / _scaleDownFactor);
    } else {
      r.x /= _scaleDownFactor;
      r.y /= _scaleDownFactor;
      r.width /= _scaleDownFactor;
      r.height /= _scaleDownFactor;
    }

    faces.push_back(r);
  }

  return faces;
}
#elif FACE_DETECTION_METHOD == 6   // mtcnn/ncnn.

std::pair<std::vector<cv::Rect>, std::vector<std::vector<cv::Point2f>> > TVSMultiTrackerEngine::findFacesByMtcnn(const cv::Mat &matColorImage,
                                                             const int rotation) {

    printDebug("findFacesByNcnn2 begin");
    cv::Mat dst;

    // Rotate to get upright faces.
    if (rotation == 0) {
        dst = matColorImage;
    } else if (rotation == 90) {
        cv::flip(matColorImage.t(), dst, -1);
    } else if (rotation == 180) {
        cv::flip(matColorImage, dst, -1);
    } else if (rotation == 270) {
        cv::transpose(matColorImage, dst);
    }

    std::vector<std::vector<float>> total_boxes, points;
    printDebug("detectFace begin");
    _pDetectionNcnn->detectFace(dst, total_boxes, points);
    printDebug("detectFace end");

    std::vector<cv::Rect> faces;
    std::vector<std::vector<cv::Point2f>>  fivepoints;

    auto length = total_boxes.size();

    for (size_t k = 0; k < length; k++) {

         cv::Rect r;
        std::vector<cv::Point2f> fivePoint;

        if (rotation == 270) {
          {
          float x0 = points[k][6];
          float x1 = points[k][5];
          float x2 = points[k][7];
          float x3 = points[k][9];
          float x4 = points[k][8];
          float y0 = points[k][1];
          float y1 = points[k][0];
          float y2 = points[k][2];
          float y3 = points[k][4];
          float y4 = points[k][3];

          fivePoint.push_back(cv::Point2f(x0,y0));
          fivePoint.push_back(cv::Point2f(x1,y1));
          fivePoint.push_back(cv::Point2f(x2,y2));
          fivePoint.push_back(cv::Point2f(x3,y3));
          fivePoint.push_back(cv::Point2f(x4,y4));
        }

        {
          int x1 = total_boxes[k][1];
          int y1 = total_boxes[k][0];
          int x2 = total_boxes[k][3];
          int y2 = total_boxes[k][2];
          r = cv::Rect(x1,y1, x2-x1, y2- y1);
        }



        } else if (rotation == 180) {
          {
          float x0 = matColorImage.cols - points[k][0];
          float x1 = matColorImage.cols - points[k][1];
          float x2 = matColorImage.cols - points[k][2];
          float x3 = matColorImage.cols - points[k][3];
          float x4 = matColorImage.cols - points[k][4];
          float y0 = matColorImage.rows - points[k][5];
          float y1 = matColorImage.rows - points[k][6];
          float y2 = matColorImage.rows - points[k][7];
          float y3 = matColorImage.rows - points[k][8];
          float y4 = matColorImage.rows - points[k][9];

          fivePoint.push_back(cv::Point2f(x0,y0));
          fivePoint.push_back(cv::Point2f(x1,y1));
          fivePoint.push_back(cv::Point2f(x2,y2));
          fivePoint.push_back(cv::Point2f(x3,y3));
          fivePoint.push_back(cv::Point2f(x4,y4));
        }
        {
          int x1 = matColorImage.cols - total_boxes[k][2];
          int y1 = matColorImage.rows - total_boxes[k][3];
          int x2 = matColorImage.cols - total_boxes[k][0];
          int y2 = matColorImage.rows - total_boxes[k][1];
          r = cv::Rect(x1,y1, x2-x1, y2- y1);
        }

        } else if (rotation == 90) {
        //  r = cv::Rect(matColorImage.cols - r.y - r.width, r.x, r.height, r.width);

          //原始
          float x0 = matColorImage.cols - points[k][6];
          float x1 = matColorImage.cols - points[k][5];
          float x2 = matColorImage.cols - points[k][7];
          float x3 = matColorImage.cols - points[k][9];
          float x4 = matColorImage.cols - points[k][8];
          float y0 = matColorImage.rows - points[k][1];
          float y1 = matColorImage.rows - points[k][0];
          float y2 = matColorImage.rows - points[k][2];
          float y3 = matColorImage.rows - points[k][4];
          float y4 = matColorImage.rows - points[k][3];

          fivePoint.push_back(cv::Point2f(x0,y0));
          fivePoint.push_back(cv::Point2f(x1,y1));
          fivePoint.push_back(cv::Point2f(x2,y2));
          fivePoint.push_back(cv::Point2f(x3,y3));
          fivePoint.push_back(cv::Point2f(x4,y4));

          //计算框的大小
          int a1 = matColorImage.cols - total_boxes[k][3];
          int b1 = matColorImage.rows - total_boxes[k][2];
          int a2 = matColorImage.cols - total_boxes[k][1];
          int b2 = matColorImage.rows - total_boxes[k][0];
          r = cv::Rect(a1,b1, a2-a1, b2- b1);

        }else {
          fivePoint.push_back(cv::Point2f(points[k][0],points[k][5]));
          fivePoint.push_back(cv::Point2f(points[k][1],points[k][6]));
          fivePoint.push_back(cv::Point2f(points[k][2],points[k][7]));
          fivePoint.push_back(cv::Point2f(points[k][3],points[k][8]));
          fivePoint.push_back(cv::Point2f(points[k][4],points[k][9]));


          r = cv::Rect(total_boxes[k][0], total_boxes[k][1], total_boxes[k][2] - total_boxes[k][0], total_boxes[k][3] - total_boxes[k][1]);
        }

        //mtcnn no need to adjust the face rects as opencv
//        r.y += static_cast<int>(0.2f * r.height);
//        r.height = static_cast<int>(r.height * 0.9f);
//        r.x += static_cast<int>(0.05f * r.width);
//        r.width = static_cast<int>(r.width * 0.9f);

        faces.push_back(r);
        fivepoints.push_back(fivePoint);
    }

    printDebug("findFacesByNcnn 4");


    return make_pair(faces, fivepoints);
}

#endif

bool TVSMultiTrackerEngine::addFaces(const std::vector<cv::Rect>& faces,
                                     const std::vector<int>& rotations) {
  size_t sz = faces.size();
  if (sz != rotations.size()) {
    printErr("Error: faces and rotation sizes differ!");
    return false;
  }
  _detections.resize(sz);
  for (size_t k = 0; k < sz; k++) {
    _detections.at(k).rect = faces.at(k);
    _detections.at(k).rotation = rotations.at(k);
  }
  //  printInfo("Added %ld faces", faces.size());
  return true;
}

// 从大到小排序
bool faceInfo_compare(const Detection_t &faceInfo1,
                      const Detection_t &faceInfo2) {
  return faceInfo1.rect.width * faceInfo1.rect.height >
        faceInfo2.rect.width * faceInfo2.rect.height;
}

bool TVSMultiTrackerEngine::addFaces(const std::vector<cv::Rect> &faces,
                                     const std::vector<int> &rotations, std::vector<std::vector<cv::Point2f>> &points) {
    size_t sz = faces.size();
    if (sz != rotations.size() || sz != points.size()) {
        //printErr("Error: faces and rotation sizes differ!");
        return false;
    }
    std::vector<Detection_t> tmpFaces;
    tmpFaces.resize(sz);
    for (size_t k = 0; k < sz; k++) {
      tmpFaces.at(k).rect = faces.at(k);
      tmpFaces.at(k).rotation = rotations.at(k);
      tmpFaces.at(k).fivePoints = points.at(k);
    }
    //在tracker这里对人脸排序, 因为排序不是detector的业务,所以在tacker的代码里排序
    std::sort(tmpFaces.begin(), tmpFaces.end(), faceInfo_compare);
    _detections.resize(sz);
    for (size_t k = 0; k < sz; k++) {
        _detections.at(k).rect = tmpFaces.at(k).rect;
        _detections.at(k).rotation = tmpFaces.at(k).rotation;
        _detections.at(k).fivePoints = tmpFaces.at(k).fivePoints;

    }
    //  printInfo("Added %ld faces", faces.size());
    return true;
}

bool TVSMultiTrackerEngine::addFacesByApi(const std::vector<cv::Rect>& faces,
										  const std::vector<int>& rotations) {
  size_t sz = faces.size();
  if (sz != rotations.size()) {
    printErr("Error: faces and rotation sizes differ!");
    return false;
  }

  // Store the input face rectangles into the buffer.
  for (size_t k = 0; k < sz; k++) {
    _facesByApi.push_back(faces.at(k));
    _faceRotationsByApi.push_back(rotations.at(k));
  }

  return true;
}

void TVSMultiTrackerEngine::loadFacesAddedByApi() {
  size_t sz = _facesByApi.size();
  if (sz == 0) return;

  std::vector<cv::Rect> scaledFaces;
  std::vector<int> rotations;
  for (size_t k = 0; k < sz; k++) {
	cv::Rect r;
	r.x = _facesByApi.at(k).x / _scaleDownFactor;
	r.y = _facesByApi.at(k).y / _scaleDownFactor;
	r.width = _facesByApi.at(k).width / _scaleDownFactor;
	r.height = _facesByApi.at(k).height / _scaleDownFactor;
    scaledFaces.push_back(r);
	rotations.push_back(_faceRotationsByApi.at(k));
  }

  // Clear the buffer.
  _facesByApi.clear();
  _faceRotationsByApi.clear();

  addFaces(scaledFaces, rotations);
}

void updateTrackerFuncCrop(TrackerData_t *data) {
  if (data->smooth) {
    data->state = data->tracker->update(data->crop, data->M, data->pupils,
                                        data->highPrec, &(data->grayInput));
  } else {
    data->state = data->tracker->update(data->crop, data->M, data->pupils,
                                        data->highPrec, nullptr);
  }
}

float mFirst = 0.25f;
float mOthers = 0.25f;
void TVSMultiTrackerEngine::setThreshold(float first, float others) {
    mFirst = first;
    mOthers = others;
}

int TVSMultiTrackerEngine::update(const bool predictPupils,
                                  const bool highPrecision,
                                  const bool smooth) {
  if (!_useOpenGL) {
    printErr("Error - called update() with a GL texture in a tracker using nv21 input");
    return -1;
  }
  //check the correct EGL context is current
  EGLContext ctxt = eglGetCurrentContext();
  if (ctxt == EGL_NO_CONTEXT || ctxt != _eglJavaContext) {
    printErr("No EGL context or wrong context");
    return -1;
  }

  std::vector<cv::Mat_<float>> Mats;
  std::vector<cv::Mat_<float>> MatsOGL;
  for (int i = 0; i < _maxTrackers; i++) {
    if (!_alive[i]) {
      continue;
      _state[i] = -1;
    }
    cv::Mat_<float> mat = _trackers.at(i).computeTransformation();
    if (mat.empty()) continue;
    Mats.push_back(mat);
    cv::Mat_<float> Mogl = _featComp->convertCVSimTransfToOpenGL(mat);
    MatsOGL.push_back(Mogl);
  }

  if (MatsOGL.size() > 0) {
    cv::Mat crop = _featComp->setupMulti(_srcTexture, MatsOGL);

    for (int i = 0, j = 0; i < _maxTrackers; i++) {
      if (!_alive.at(i)) { _state[i] = -1; continue; }
      _trackerData.at(i).tracker = &(_trackers.at(i));
      _trackerData.at(i).crop = crop.rowRange(j * _feature_size.height,
                                      (j+1) * _feature_size.height);
      _trackerData.at(i).M = Mats.at(j);
      j++;

      _trackerData.at(i).pupils = predictPupils;
      _trackerData.at(i).highPrec = highPrecision;
      _trackerData.at(i).smooth = smooth;

      if (smooth) {
        _trackerData.at(i).grayInput.mutex = &_mutex;
        _trackerData.at(i).grayInput.condition = &_condition_var;
        _trackerData.at(i).grayInput.ready = false;
        _trackerData.at(i).grayInput.gray = &_grayInput;
      }
      _tpool->enqueue(updateTrackerFuncCrop, &(_trackerData.at(i)));
    }
    if (smooth) {
      _colorConv.startConverting(_srcTexture);
      _colorConv.finishConversion();
      const cv::Mat& im = _colorConv.getResult();
      std::unique_lock<std::mutex> lock(_mutex);
      _grayInput = im;
      for (int i = 0; i < _maxTrackers; i++) {
        _trackerData.at(i).grayInput.ready = true;
      }
      lock.unlock();
      _condition_var.notify_all();
    }

    _tpool->wait();

    if (smooth) {
      _colorConv.releaseResult();
    }

    for (int i = 0; i < _maxTrackers; i++) {
      if (_trackerData.at(i).state == false) {
        _alive.at(i) = false;
        _state[i] = -1;
      } else {
        _state[i] = 1;
      }
    }
  }

  if (this->getAliveTrackerCount() == static_cast<int>(_maxTrackers)) {
    return this->getAliveTrackerCount();
  }
  int ndet = static_cast<int>(_detections.size());
  for (int i = ndet-1; i >= 0; i--) {
    cv::Rect r = _detections.at(i).rect;

    float d_x_min = r.x;
    float d_x_max = r.x + r.width;
    float d_y_min = r.y;
    float d_y_max = r.y + r.height;

    float d_area = r.width * r.height;

    bool already_tracking = false;
    for (int j = 0; j < _maxTrackers; j++) {
      if (!_alive[j]) { _state[i] = -1; continue; }
      int n = _trackers[j].getPointCount();
      const cv::Mat_<float> pts = _trackers[j].getShape();
      int nsum = 0;

      float t_x_min = 10000, t_x_max = -1, t_y_min = 10000, t_y_max = -1;

      for (int k = 0; k < n; k++) {
        float x = pts(2 * k), y = pts(2 * k + 1);
        if ( (x >= r.x) && (x <= r.x + r.width) &&
             (y >= r.y) && (y <= r.y + r.height) ) { nsum++; }

        if (x < t_x_min) t_x_min = x;
        if (x > t_x_max) t_x_max = x;
        if (y < t_y_min) t_y_min = y;
        if (y > t_y_max) t_y_max = y;
      }

      float t_area = (t_x_max - t_x_min) * (t_y_max - t_y_min);

      float x_overlap = cv::max(float(0.0), cv::min(d_x_max, t_x_max) - cv::max(d_x_min, t_x_min));
      float y_overlap = cv::max(float(0.0), cv::min(d_y_max, t_y_max) - cv::max(d_y_min, t_y_min));

      //overlap percentage of detection area and tracking area
      //to make sure the detection face is a valid face
      float overlap = (x_overlap * y_overlap) / cv::min(t_area, d_area);

      if (static_cast<float>(nsum) / n > 0.5f || overlap > 0.5) {
        already_tracking = true;
        break;
      }
    }
    if (already_tracking) { continue; }

    int j = 0;
    for (; j < _maxTrackers; j++) { if (!_alive[j]) { _state[i] = -1; break; } }
    if (j == _maxTrackers) { _detections.clear(); break; }
    // printInfo("Initialing tracker with (w: %d, h: %d)",
    //             r.width, r.height);
    if (_trackers[j].resetWithFaceRect(r, _detections.at(i).rotation)) {
      cv::Mat_<float> M = _trackers.at(j).computeTransformation();
      cv::Mat_<float> Mogl = _featComp->convertCVSimTransfToOpenGL(M);
      cv::Mat crop = _featComp->setup(_srcTexture, Mogl);

      // printInfo("Initialisation of tracker OK - %dx%d - %.2f",
      //           crop.cols, crop.rows, cv::norm(crop));
      //shared context should make this work
      _trackers[j].setThreshold(mFirst);   // Modified. 0.4f);
      int state = _trackers.at(j).update(crop, M, predictPupils, highPrecision,
                                         NULL);

      _trackers[j].setThreshold(mOthers);
//      _trackers[j].setThreshold(0.3f);

      if (state < 0) { return -1; }
      else if (state == 0) { _state[i] = -1; _alive[j] = false; }
      else {
        _state[j] = 0;
        _alive[j] = true;
        printInfo("First frame tracked OK!");
      }
    }
  }
  _detections.clear();
  return this->getAliveTrackerCount();
}

int warpCounter = 0;
void updateTrackerFunc(TrackerData_t *data) {
  //compute the transformation and warp the image
  cv::Mat_<float> matI = data->tracker->computeTransformationWA();
  //  cv::Mat_<float> matI = tvs::geo::invertTransform2d(mat);
  cv::warpAffine(data->image, data->crop, matI, data->crop.size(),
                 cv::INTER_LINEAR | cv::WARP_INVERSE_MAP);
  //
  // char file[128];
  // sprintf(file, "warped_%03d_%dx%d.jpg", warpCounter++, data->crop.cols,
  //         data->crop.rows);
  // cv::imwrite(folder + file, data->crop);
  // cv::imwrite("/storage/emulated/0/Pictures/face.png", data->crop);
  // __android_log_print(ANDROID_LOG_DEBUG, kLogTag, "Write face.png");
  cv::Scalar meanVals = mean(data->crop);
  data->tracker->setFaceRegionBrightness(meanVals.val[0]);
  // __android_log_print(ANDROID_LOG_DEBUG, kLogTag, "%f", data->tracker->getFaceRegionBrightness());

  if (data->smooth) {
    data->state = data->tracker->update(data->crop, matI, data->pupils,
                                        data->highPrec,
                                        &(data->grayInput));
  } else {
    data->state = data->tracker->update(data->crop, matI, data->pupils,
                                        data->highPrec, nullptr);
  }
}

void PrintMat(const cv::Mat_<uchar>& grayOri)
{
  for(int i=0;i<grayOri.rows;i++)
  {
    for(int j=0;j<grayOri.cols;j++){
        uchar s = grayOri.at<uchar>(i,j);
      printErr("%c",s);
    }
    /* printErr(A.at<float>(i,j));*/
  }
}

int TVSMultiTrackerEngine::update(const cv::Mat_<uchar>& grayOri,
                                  const bool predictPupils,
                                  const bool highPrecision, const bool smooth) {
  if (_useOpenGL) {
    printErr("Error - called update() with nv21 in a tracker using OpenGL");
    return -1;
  }
    /*printErr("update grey Mat");
    PrintMat(grayOri);*/
  int srcw = grayOri.cols;
  int srch = grayOri.rows;
  determineScaleDownFactor(srcw, srch);
  loadFacesAddedByApi();

  cv::Mat *pGrayImg = NULL;

  if (_scaleDownFactor == 1) {
    pGrayImg = (cv::Mat *)&grayOri;
  } else {
    // Scale down image with down sampling (from Jethro).
    unsigned char *pImgData = (unsigned char *)grayOri.ptr<unsigned char>();
    int imgw = srcw / _scaleDownFactor;
    int imgh = srch / _scaleDownFactor;
    int bufSize = imgw * imgh;
    if (_pGrayImgBuf == NULL || _grayImgBufSize < bufSize) {
      if (_pGrayImgBuf != NULL) delete _pGrayImgBuf;
      _grayImgBufSize = bufSize;
      _pGrayImgBuf = new unsigned char[_grayImgBufSize];
    }

    int mm, count = 0;
    int hBound = imgh*_scaleDownFactor,
        wBound = imgw*_scaleDownFactor;

    for (int m = 0; m < hBound; m += _scaleDownFactor) {
      mm = m*srcw;
      for (int n = 0; n < wBound; n += _scaleDownFactor){
          _pGrayImgBuf[count] = pImgData[mm + n];
          count++;
      }
    }

    pGrayImg = new cv::Mat(imgh, imgw, CV_8UC1, _pGrayImgBuf);
  }

  for (int i = 0; i < _maxTrackers; i++) {
    if (!_alive.at(i)) {
      _state[i] = -1;
      continue;
    }
    _trackerData.at(i).tracker = &(_trackers.at(i));
    _trackerData.at(i).crop.create(_feature_size);

    _trackerData.at(i).pupils = predictPupils;
    _trackerData.at(i).highPrec = highPrecision;
    _trackerData.at(i).smooth = smooth;

    _trackerData.at(i).image = *pGrayImg;

    _trackerData.at(i).grayInput.mutex = &_mutex;
    _trackerData.at(i).grayInput.condition = &_condition_var;
    _trackerData.at(i).grayInput.ready = true;
    _trackerData.at(i).grayInput.gray = (cv::Mat_<uchar> *)pGrayImg; //input, as is

    _tpool->enqueue(updateTrackerFunc, &(_trackerData.at(i)));
  }

  _tpool->wait();

  for (int i = 0; i < _maxTrackers; i++) {
    if (_trackerData.at(i).state == false) {
      _alive.at(i) = false;
      _state[i] = -1; //set to not working
    } else {
      _state[i] = 1; //working
    }
  }

  if (this->getAliveTrackerCount() == static_cast<int>(_maxTrackers)) {
    return this->getAliveTrackerCount();
  }

  int ndet = static_cast<int>(_detections.size());
  for (int i = ndet-1; i >= 0; i--) {
    cv::Rect r = _detections[i].rect;

    float d_x_min = r.x;
    float d_x_max = r.x + r.width;
    float d_y_min = r.y;
    float d_y_max = r.y + r.height;

    float d_area = r.width * r.height;

    bool already_tracking = false;
    for (int j = 0; j < _maxTrackers; j++) {
      if (!_alive[j]) {
        _trackers[j].setFaceRegionBrightness(-1);   // Negative values mean not tracking.
        continue;
	  }

      int n = _trackers[j].getPointCount();
      const cv::Mat_<float> pts = _trackers[j].getShape();
      int nsum = 0;

      float t_x_min = 10000, t_x_max = -1, t_y_min = 10000, t_y_max = -1;

      for (int k = 0; k < n; k++) {
        float x = pts(2 * k), y = pts(2 * k + 1);
        if ( (x >= r.x) && (x <= r.x + r.width) &&
             (y >= r.y) && (y <= r.y + r.height) ) { nsum++; }

        if (x < t_x_min) t_x_min = x;
        if (x > t_x_max) t_x_max = x;
        if (y < t_y_min) t_y_min = y;
        if (y > t_y_max) t_y_max = y;
      }

      float t_area = (t_x_max - t_x_min) * (t_y_max - t_y_min);

      float x_overlap = cv::max(float(0.0), cv::min(d_x_max, t_x_max) - cv::max(d_x_min, t_x_min));
      float y_overlap = cv::max(float(0.0), cv::min(d_y_max, t_y_max) - cv::max(d_y_min, t_y_min));

      //overlap percentage of detection area and tracking area
      //to make sure the detection face is a valid face
      float overlap = (x_overlap * y_overlap) / cv::min(t_area, d_area);

//      __android_log_print(ANDROID_LOG_DEBUG, "OVERLAP", "%f", overlap);

      if (static_cast<float>(nsum) / n > 0.5f || overlap > 0.5) {
        _state[j] = 1; // still working
        already_tracking = true;
        break;
      }
    }
    if (already_tracking) { continue; }

    int j = 0;
    for (; j < _maxTrackers; j++) { if (!_alive[j]) { break; } }
    if (j == _maxTrackers) { _detections.clear(); break; }

    auto fivePoint = _detections.at(i).fivePoints;
    //if (_trackers.at(j).resetWithFaceRect(r, _detections.at(i).rotation)) {
    if (_trackers.at(j).resetWith5Points(fivePoint)) {
      printDebug("_trackers.at[%d].resetWith5Points", j);
      //printInfo("shape reset OK: (%d, %d, %d, %d), rot = %d",
      //          r.x, r.y, r.width, r.height, _detections.at(i).rotation);

      cv::Mat_<float> M = _trackers.at(j).computeTransformationWA();
      cv::Mat crop;
      cv::warpAffine(*pGrayImg, crop, M, _feature_size,
                     cv::INTER_LINEAR | cv::WARP_INVERSE_MAP);

      // char file[128];
      // sprintf(file, "warped_%03d_r.jpg", warpCounter++);

      // cv::imwrite(folder + file, crop);
      // cv::imwrite("/storage/emulated/0/Pictures/face.png", crop);

      // printInfo("Initialisation of tracker OK - %dx%d - %.2f",
      //           crop.cols, crop.rows, cv::norm(crop));
      //shared context should make this work
      _trackers[j].setThreshold(mFirst);   // Modified. 0.4f);
      int state = _trackers.at(j).update(crop, M, predictPupils, highPrecision,
                                         nullptr);

      _trackers[j].setThreshold(mOthers);
//      _trackers[j].setThreshold(0.3f);

      if (state < 0) { return -1; }
      else if (state == 0) {
        //printInfo("Tracker reset failed");
        _alive[j] = false;
        _state[j] = -1;
      } else {
        _alive[j] = true;
        _state[j] = 0;
        //printInfo("First frame tracked OK!");
      }
    }
  }

  // {
  //   //printInfo("Feature size: %dx%d", _feature_size.width, _feature_size.height);
  //   cv::Mat mat = pGrayImg->clone();
  //   for (int k = 0; k < _detections.size(); k++) {
  //     cv::rectangle(mat, _detections.at(k).rect, cv::Scalar::all(255), 1);
  //     cv::circle(mat, _detections.at(k).rect.tl(), 3, cv::Scalar::all(255),
  //                -1);
  //     cv::circle(mat, _detections.at(k).rect.br(), 3, cv::Scalar::all(255),
  //                1);
  //   }

  //   char file[128];
  //   sprintf(file, "source_%03d.jpg", warpCounter++);
  //   cv::imwrite(folder + file, mat);
  // }
  _detections.clear();

  if (_scaleDownFactor != 1)
    delete pGrayImg;

  return this->getAliveTrackerCount();
}


bool TVSMultiTrackerEngine::getRotationAngles(int index, float *pitch,
                                              float *yaw, float *roll) {
  if (index >= _maxTrackers || index < 0) return 0;
  cv::Mat_<float> p = _trackers.at(index).getPose();
  if (!p.empty()) {
    cv::Mat_<float> R;
    cv::Rodrigues(p.rowRange(0, 3), R);
    rot2euler(R, pitch, yaw, roll);
    return true;
  }
  return false;
}


void TVSMultiTrackerEngine::getFaceRectBrightness(int *pBuf) {
  for (int i = 0; i < _maxTrackers; i++)
    pBuf[i] = static_cast<int>(_trackers[i].getFaceRegionBrightness());
}

jobjectArray handleResult(JNIEnv *env, vector<vector<float>> rects, vector<vector<float>> points) {
  jobjectArray retInfos = NULL;
  if (rects.size() != points.size()) {
    return retInfos;
  }
  const char *pClassNameFaceInfo = "com/uls/multifacetrackerlib/bean/FaceInfo";
  jclass jclsFaceInfo = env->FindClass(pClassNameFaceInfo);
  jclass jclsRectF = env->FindClass("android/graphics/RectF");
  jfieldID jfleft = env->GetFieldID(jclsRectF, "left", "F");
  jfieldID jfright = env->GetFieldID(jclsRectF, "right", "F");
  jfieldID jftop = env->GetFieldID(jclsRectF, "top", "F");
  jfieldID jfbottom = env->GetFieldID(jclsRectF, "bottom", "F");
  jfieldID jfrect = env->GetFieldID(jclsFaceInfo, "rect", "Landroid/graphics/RectF;");
  jfieldID jfpoints = env->GetFieldID(jclsFaceInfo, "points", "[F");

  retInfos = env->NewObjectArray(rects.size(), jclsFaceInfo, NULL);
  for (int i = 0; i < rects.size(); i++) {
    jobject jobjRect = env->AllocObject(jclsRectF);
    // 因为算法组是以(1,1)为原点,所以要减1
    rects[i][0] -= 1;
    rects[i][1] -= 1;
    rects[i][2] -= 1;
    rects[i][3] -= 1;
    env->SetFloatField(jobjRect, jfleft, rects[i][0]);
    env->SetFloatField(jobjRect, jftop, rects[i][1]);
    env->SetFloatField(jobjRect, jfright, rects[i][2]);
    env->SetFloatField(jobjRect, jfbottom, rects[i][3]);

    jfloatArray jfpointsarr = env->NewFloatArray(points[i].size());
    for (int j = 0; j < points[i].size(); j++) {
      // 因为算法组是以(1,1)为原点,所以要减1
      points[i][j] -= 1;
      env->SetFloatArrayRegion(jfpointsarr, j, 1, &points[i][j]);
    }
    jobject jobjInfo = env->AllocObject(jclsFaceInfo);
    env->SetObjectField(jobjInfo, jfrect, jobjRect);
    env->SetObjectField(jobjInfo, jfpoints, jfpointsarr);

    env->DeleteLocalRef(jfpointsarr);

    env->SetObjectArrayElement(retInfos, i, jobjInfo);
  }
  return retInfos;
}

jobjectArray TVSMultiTrackerEngine::processDetectorImageByNcnn(JNIEnv *env,jobject image) {
  printInfo("nativeProcessDetectorImage 0");
  jobjectArray faceInfoArray = NULL;
  do {
    if (_pDetectionNcnn == 0) {
      printInfo("_pDetectionNcnn == 0");
      break;
    }
    JniBitmap jniBitmapSrc(env, image);
    printInfo("nativeProcessDetectorImage 1 jniBitmapSrc.ret=%d", jniBitmapSrc.ret);
    printInfo("nativeProcessDetectorImage 1 format=%d", jniBitmapSrc.imageInfo.format);
    // 转化为Mat
    cv::Mat srcMat = ProcessUtils::jnibitmap2bgrmat(jniBitmapSrc);
    printInfo("nativeProcessDetectorImage 2");
    std::vector<std::vector<float>> rects_tmp;
    std::vector<std::vector<float>> points_tmp;
    _pDetectionNcnn->detectFace(srcMat, rects_tmp, points_tmp);
    printInfo("nativeProcessDetectorImage 3 faceNum=%d", faceNum);
    ProcessUtils::pointsToPointXy(points_tmp);
    faceInfoArray = handleResult(env, rects_tmp, points_tmp);
  } while (0);
  printInfo("nativeProcessDetectorImage 4");
  return faceInfoArray;
}


