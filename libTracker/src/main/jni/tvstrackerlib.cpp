#include <stdlib.h>
#include <sys/stat.h>
#ifdef _OPENMP
#include <omp.h>
#endif

#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

//#include <cpu-features.h>
//#include <prof.h>

#include <string>
#include <Eigen/Core>
#include <opencv2/imgproc/imgproc.hpp>

#include "TVSTrackerEngine.h"
#include "TVSMultiTrackerEngine.h"
#include "uls-dense-points/dense_points.h"

#include <android/bitmap.h>
#include "jniWrapper/JniUtils.h"
#include "jniWrapper/JniBitmap.h"
#include "authorization.h"
#include "ProcessUtils.h"
#include "blurEvaluation/blur_evaluation.h"

using std::vector;
using std::string;
using Eigen::MatrixXf;
//using namespace ulsee;
const char kLogTag[] = "UlsMultiFaceTracker";
bool authResult;
#define printErr(...) {__android_log_print(ANDROID_LOG_ERROR, kLogTag, __VA_ARGS__);}
#define printDebug(...) {__android_log_print(ANDROID_LOG_DEBUG, kLogTag, __VA_ARGS__);}

enum byteDataFormat {
    BYTE_NV21 = 0,
    BYTE_BGR = 1,
};

// Modified. Restrict the number of faces that can be added.
int iAliveFaceCount = 0;

int *piFaceRect = NULL,
    *piFaceRectBrightness = NULL,
	iNumTrackers = 0;

class TVSMultiTrackerJava {
public:
  jclass javaClass = 0;
  jfieldID mShapeFieldID = 0;
  jfieldID mShape3DFieldID = 0;
  jfieldID mConfidenceFieldID = 0;
  jfieldID mPoseFieldID = 0, mPoseQualityFieldID = 0;
  jfieldID mEulerAnglesFieldID = 0;
  jfieldID mPupilsFieldID = 0, mGazeFieldID = 0;
  jfieldID mAliveFielID = 0;

  jfieldID nativeTrackerPtrFieldID = 0;
};

static TVSMultiTrackerJava multiTrackerDataCache;

TVSMultiTrackerEngine *getTVSMultiTrackerEnginePtr(JNIEnv *env, jobject obj) {
  jlong handle = env->GetLongField(
      obj, multiTrackerDataCache.nativeTrackerPtrFieldID);
  if (handle==0) {
    return NULL;
  }
  TVSMultiTrackerEngine *engine =
      reinterpret_cast<TVSMultiTrackerEngine*>(handle);
  return engine;
}


void initialiseJavaFieldsMulti(JNIEnv *env, jobject jobj) {
  multiTrackerDataCache.nativeTrackerPtrFieldID =
      env->GetFieldID(multiTrackerDataCache.javaClass, "nativeTrackerPtr", "J");
  multiTrackerDataCache.mShapeFieldID =
      env->GetFieldID(multiTrackerDataCache.javaClass, "mShape",
                      "[[F");
  multiTrackerDataCache.mShape3DFieldID =
      env->GetFieldID(multiTrackerDataCache.javaClass, "mShape3D",
                      "[[F");
  multiTrackerDataCache.mConfidenceFieldID =
      env->GetFieldID(multiTrackerDataCache.javaClass, "mConfidence",
                      "[[F");
  multiTrackerDataCache.mPoseFieldID =
      env->GetFieldID(multiTrackerDataCache.javaClass, "mPose",
                      "[[F");
  multiTrackerDataCache.mPoseQualityFieldID =
      env->GetFieldID(multiTrackerDataCache.javaClass, "mPoseQuality", "[F");
  multiTrackerDataCache.mEulerAnglesFieldID =
      env->GetFieldID(multiTrackerDataCache.javaClass, "mEulerAngles",
                      "[[F");
  multiTrackerDataCache.mPupilsFieldID =
      env->GetFieldID(multiTrackerDataCache.javaClass, "mPupils",
                      "[[F");
  multiTrackerDataCache.mGazeFieldID =
      env->GetFieldID(multiTrackerDataCache.javaClass, "mGaze",
                      "[[F");
  multiTrackerDataCache.mAliveFielID =
      env->GetFieldID(multiTrackerDataCache.javaClass, "mAlive", "[Z");
}

jboolean naMultiInitialiseFromAssets (JNIEnv *env, jobject jobj,
                                      jobject context, jint count, jboolean useOGL,
                                      jobject manager, jint minSize, jstring jcacheDir) {
  initialiseJavaFieldsMulti(env, jobj);

  TVSMultiTrackerEngine* engine = new TVSMultiTrackerEngine(count, useOGL);
  if (!engine) {
    printErr("No multi engine!");
    return JNI_FALSE;
  }
  jlong handle = reinterpret_cast<jlong>(engine);
  env->SetLongField(jobj, multiTrackerDataCache.nativeTrackerPtrFieldID,
                    handle);

  char const *str = env->GetStringUTFChars(jcacheDir, NULL);
  std::string cacheDir = std::string(str) + "/";
  env->ReleaseStringUTFChars(jcacheDir, str);

  // Get app's package name.
  jclass appContext = env->GetObjectClass(context);
  jmethodID methodIdGetPackageName = env->GetMethodID(appContext,
                                    "getPackageName", "()Ljava/lang/String;");
  jstring jsPackageName = static_cast<jstring>(env->CallObjectMethod(context, methodIdGetPackageName));
  str = env->GetStringUTFChars(jsPackageName, NULL);
  std::string sPackageName = std::string(str);
  env->ReleaseStringUTFChars(jsPackageName, str);

  AAssetManager *mgr = AAssetManager_fromJava(env, manager);
  bool ok = engine->loadModels(mgr, minSize, cacheDir, sPackageName);
  if (!ok) return JNI_FALSE;

  //set the number of points
  jfieldID ptCountField = env->GetFieldID(multiTrackerDataCache.javaClass,
                                          "mShapePointCount", "[I");
  jintArray pcount = (jintArray)env->GetObjectField(jobj, ptCountField);
  jint *ptr = env->GetIntArrayElements(pcount, NULL);
  for (int i = 0; i < count; i++) {
    ptr[i] = engine->getPointCount(i);
  }
  env->ReleaseIntArrayElements(pcount, ptr, 0);

  iNumTrackers = count;

  return JNI_TRUE;
}

jboolean naMultiInitialiseFromPath(JNIEnv *env, jobject jobj,
                                    jobject context, jint count,
                                   jboolean useOGL, jstring path) {
  initialiseJavaFieldsMulti(env, jobj);

  TVSMultiTrackerEngine* engine = new TVSMultiTrackerEngine(count, useOGL);
  if (!engine) {printErr("No engine!"); return JNI_FALSE;}
  jlong handle = reinterpret_cast<jlong>(engine);
  env->SetLongField(jobj, multiTrackerDataCache.nativeTrackerPtrFieldID,
                    handle);

  char const *str = env->GetStringUTFChars(path, NULL);
  std::string modelPath = std::string(str) + "/";
  env->ReleaseStringUTFChars(path, str);

  // Get app's package name.
  jclass appContext = env->GetObjectClass(context);
  jmethodID methodIdGetPackageName = env->GetMethodID(appContext,
                                    "getPackageName", "()Ljava/lang/String;");
  jstring jsPackageName = static_cast<jstring>(env->CallObjectMethod(context, methodIdGetPackageName));
  str = env->GetStringUTFChars(jsPackageName, NULL);
  std::string sPackageName = std::string(str);
  env->ReleaseStringUTFChars(jsPackageName, str);

  bool ok = engine->loadModels(modelPath, sPackageName);
  if (!ok) return JNI_FALSE;

  //set the number of points
  jfieldID ptCountField = env->GetFieldID(multiTrackerDataCache.javaClass,
                                          "mShapePointCount", "[I");
  jintArray pcount = (jintArray)env->GetObjectField(jobj, ptCountField);
  jint *ptr = env->GetIntArrayElements(pcount, NULL);
  for (int i = 0; i < count; i++) {
    ptr[i] = engine->getPointCount(i);
  }
  env->ReleaseIntArrayElements(pcount, ptr, 0);

  iNumTrackers = count;

  return JNI_TRUE;
}

void naMultiDispose(JNIEnv *env, jobject jobj) {
    if (piFaceRect != NULL) {
      delete[] piFaceRect;
      piFaceRect = NULL;
    }

    if (piFaceRectBrightness != NULL) {
      delete[] piFaceRectBrightness;
      piFaceRectBrightness = NULL;
    }

    TVSMultiTrackerEngine *engine = getTVSMultiTrackerEnginePtr(env, jobj);
    if (engine == nullptr) return;
    delete engine;
    //clear the value in the Java side
    env->SetLongField(jobj, multiTrackerDataCache.nativeTrackerPtrFieldID, 0);
    return;
}

jint
naMultiActivate(JNIEnv *env, jobject jobj, jstring jkey, jstring jkeySecret, jstring macString) {
#ifndef NO_AUTH
    /*
    char const *pkey = env->GetStringUTFChars(jkey, NULL);
    char const *pkeySecret = env->GetStringUTFChars(jkeySecret, NULL);

    string androidId = JniUtils::getAndroidId(env);
    string macAddress = JniUtils::jstring2string(env, macString);
    string appId = JniUtils::getPackageName(env);
    string licensePath = JniUtils::getLicensePath(env);

    int platform = 2;
    uls::Authorization auth;
    auth.initLicenseFile(licensePath.c_str());

    auth.initDevice(androidId.c_str(), macAddress.c_str(), appId.c_str(), platform);

    std::string auth_group = "tracker";

    uls_license_key_status key_status = auth.getAuthRet(pkey, pkeySecret,
                                                        auth_group.c_str());
    env->ReleaseStringUTFChars(jkey, pkey);
    env->ReleaseStringUTFChars(jkeySecret, pkeySecret);

    if (key_status != ULS_KEY_SUCCESS) {
        printErr("key验证失败，请联系ulsee申请合法key ErrorCode: %d", key_status);
        return key_status;
    }*/
#endif
    authResult = true;
    return ULS_KEY_SUCCESS;
}

jboolean naMultiSetupOpenGL (JNIEnv *env, jobject jobj, jint jsrcTexture,
                             jint width, jint height) {
/*
  TVSMultiTrackerEngine* engine = getTVSMultiTrackerEnginePtr(env, jobj);
  if (engine == nullptr) return false;

  bool ok = engine->setupOpenGL(jsrcTexture, width, height);
  return ok ? JNI_TRUE : JNI_FALSE;
*/
  printErr("naMultiSetupOpenGL(int srcTextureName, int width, int height) is not supported any more.");
  return JNI_FALSE;
}

void naMultiEGLContextInvalidated(JNIEnv *env, jobject jobj) {
/*
  TVSMultiTrackerEngine* engine = getTVSMultiTrackerEnginePtr(env, jobj);
  if (engine == nullptr) return;

  engine->eglContextInvalidated();
  return;
*/
  printErr("naMultiEGLContextInvalidated() is not supported any more.");
  return;
}

jboolean naMultiFindFacesAndAdd(JNIEnv *env, jobject jobj, jint rotation) {
/*
    TVSMultiTrackerEngine* engine = getTVSMultiTrackerEnginePtr(env, jobj);
    if (engine == nullptr) return JNI_FALSE;
    std::vector<cv::Rect> rect = engine->findFaces(rotation);
    if (rect.size() > 0) {
        std::vector<int> rot (rect.size(), rotation);
        bool ok = engine->addFaces(rect, rot);
        return ok ? JNI_TRUE : JNI_FALSE;
    }
    return JNI_FALSE;
*/
    printErr("naMultiFindFacesAndAdd(int rotation) is not supported any more.");
    return JNI_FALSE;
}

jboolean naMultiAddFaces (JNIEnv *env, jobject jobj, jintArray rectangles) {
  // Modified. Restrict the number of faces that can be added.
//  if (iAliveFaceCount >= 1) return JNI_FALSE;

  TVSMultiTrackerEngine* engine = getTVSMultiTrackerEnginePtr(env, jobj);
  if (engine == nullptr) return JNI_FALSE;

  std::vector<cv::Rect> rect;
  std::vector<int> rot;

  int count = env->GetArrayLength(rectangles);
  if (count % 5) {
    printErr("Bad rectangles array size!");
    return JNI_FALSE;
  }
  jint * ptr = env->GetIntArrayElements(rectangles, NULL);
  count /= 5;
  rect.resize(count);
  rot.resize(count);

  for (int i = 0; i < count; i++) {
    rect.at(i) = cv::Rect(ptr[i*5], ptr[i*5 + 1], ptr[i*5 + 2], ptr[i*5 + 3]);
    rot.at(i) = ptr[i*5 + 4];
  }
  env->ReleaseIntArrayElements(rectangles, ptr, JNI_ABORT);

  // Modified.
//  bool ok = engine->addFaces(rect, rot);
  bool ok = engine->addFacesByApi(rect, rot);

  return ok ? JNI_TRUE : JNI_FALSE;
}

bool updateJavaSideWithResults(JNIEnv *env, jobject jobj,
                               TVSMultiTrackerEngine *engine,
                               int alive,
                               jboolean predictPupils) {
  jbooleanArray aliveArr = (jbooleanArray)env->GetObjectField(
      jobj, multiTrackerDataCache.mAliveFielID);
  const int count = env->GetArrayLength(aliveArr);

  jboolean *alivePtr = env->GetBooleanArrayElements(aliveArr, NULL);
  for (int i = 0; i < count; i++) {
    if (engine->getShape(i).empty()) {
      alivePtr[i] = JNI_FALSE;
    } else {
      alivePtr[i] = JNI_TRUE;
    }
  }
  env->ReleaseBooleanArrayElements(aliveArr, alivePtr, 0);
  if (alive == 0) return false;

  jobjectArray shapeArr, shape3DArr, confArr, poseArr, eulerArr;
  jobjectArray pupilArr = NULL, gazeArr = NULL;
  shapeArr= (jobjectArray)env->GetObjectField(
      jobj, multiTrackerDataCache.mShapeFieldID);
  shape3DArr = (jobjectArray)env->GetObjectField(
      jobj, multiTrackerDataCache.mShape3DFieldID);
  confArr = (jobjectArray)env->GetObjectField(
      jobj, multiTrackerDataCache.mConfidenceFieldID);
  eulerArr = (jobjectArray)env->GetObjectField(
      jobj, multiTrackerDataCache.mEulerAnglesFieldID);
  poseArr = (jobjectArray)env->GetObjectField(
      jobj, multiTrackerDataCache.mPoseFieldID);

  if (predictPupils) {
    pupilArr = (jobjectArray)env->GetObjectField(
        jobj, multiTrackerDataCache.mPupilsFieldID);
    gazeArr = (jobjectArray)env->GetObjectField(
        jobj, multiTrackerDataCache.mGazeFieldID);
  }

  for (int i = 0; i < count; i++) {
    // Shape 2D
    const cv::Mat_<float> sh = engine->getShape(i);
    if (sh.empty()) {
      continue;
    }
    jfloatArray array = (jfloatArray)env->GetObjectArrayElement(shapeArr, i);
    jfloat *ptr = env->GetFloatArrayElements(array, NULL);

    cv::Mat dst(sh.size(), CV_32FC1, ptr);
    sh.copyTo(dst);
	dst = engine->getScaleDownFactor() * dst;
    env->ReleaseFloatArrayElements(array, ptr, 0);

    // Shape 3D
    const cv::Mat_<float> sh3D = engine->getShape3D(i);
    array = (jfloatArray)env->GetObjectArrayElement(shape3DArr, i);
    ptr = env->GetFloatArrayElements(array, 0);

    if (sh3D.empty()) return false;
    dst = cv::Mat(sh3D.size(), CV_32FC1, ptr);
    sh3D.copyTo(dst);
	dst = engine->getScaleDownFactor() * dst;
    env->ReleaseFloatArrayElements(array, ptr, 0);

    // Confidence

    const cv::Mat_<float> conf = engine->getConfidence(i);
    array = (jfloatArray)env->GetObjectArrayElement(confArr, i);
    ptr = env->GetFloatArrayElements(array, NULL);

    dst = cv::Mat(conf.size(), CV_32FC1, ptr);
    conf.copyTo(dst);
    env->ReleaseFloatArrayElements(array, ptr, 0);

    // Pose

    const cv::Mat_<float> pose = engine->getPose(i);
    array = (jfloatArray)env->GetObjectArrayElement(poseArr, i);

    ptr = env->GetFloatArrayElements(array, NULL);

    dst = cv::Mat(pose.size(), CV_32FC1, ptr);
    pose.copyTo(dst);
	dst = engine->getScaleDownFactor() * dst;
    env->ReleaseFloatArrayElements(array, ptr, 0);

    float pitch, yaw, roll;
    array = (jfloatArray)env->GetObjectArrayElement(eulerArr, i);
    ptr = env->GetFloatArrayElements(array, NULL);

    engine->getRotationAngles(i, &pitch, &yaw, &roll);
    ptr[0] = pitch;
    ptr[1] = yaw;
    ptr[2] = roll;
    env->ReleaseFloatArrayElements(array, ptr, 0);

    const float poseQuality = engine->getPoseQuality(i);
    array = (jfloatArray) env->GetObjectField(
        jobj, multiTrackerDataCache.mPoseQualityFieldID);
    ptr = env->GetFloatArrayElements(array, NULL);
    ptr[i] = poseQuality;
    env->ReleaseFloatArrayElements(array, ptr, 0);

    // Pupils
    if (predictPupils) {
      const cv::Mat_<float> pupils = engine->getPupilLocation(i);

      array = (jfloatArray)env->GetObjectArrayElement(pupilArr, i);
      ptr = env->GetFloatArrayElements(array, NULL);
      dst = cv::Mat(4, 1, CV_32FC1, ptr);
      if (!pupils.empty()) {
        pupils.copyTo(dst);
		dst = engine->getScaleDownFactor() * dst;
      } else {
        dst = -128;
      }
      env->ReleaseFloatArrayElements(array, ptr, 0);

      const cv::Mat_<float> gaze = engine->getGaze(i);
      array = (jfloatArray)env->GetObjectArrayElement(gazeArr, i);
      ptr = env->GetFloatArrayElements(array, NULL);

      dst = cv::Mat(6, 1, CV_32FC1, ptr);
      if (!gaze.empty()) {
        gaze.copyTo(dst);
      } else {
        dst = -128;
      }
      env->ReleaseFloatArrayElements(array, ptr, 0);
    }
  }
  return true;
}


jint naMultiUpdateShape(JNIEnv * env, jobject jobj, jboolean predictPupils,
                  jboolean highPrecision, jboolean faceGlue) {
/*
  TVSMultiTrackerEngine* engine = getTVSMultiTrackerEnginePtr(env, jobj);
  if (engine == nullptr) return JNI_FALSE;

  int alive = engine->update(predictPupils, highPrecision, faceGlue);

  updateJavaSideWithResults(env, jobj, engine, alive, predictPupils);

  return alive;
*/
  printErr("naMultiUpdateShapes(boolean predictPupils, boolean highPrecision, boolean smooth) is not supported any more.");
  return 0;
}

jboolean naMultiResetTracker(JNIEnv *env, jobject jobj, jint index) {
  TVSMultiTrackerEngine* engine = getTVSMultiTrackerEnginePtr(env, jobj);
  if (engine == nullptr) return JNI_FALSE;
  return engine->resetTracker(index);
}

///****************************************************************************/
/// NV21 interface for multiple faces

jboolean naMultiSetupByteArray(JNIEnv *env, jobject jobj) {
  TVSMultiTrackerEngine* engine = getTVSMultiTrackerEnginePtr(env, jobj);
  if (engine == nullptr) return JNI_FALSE;
  return engine->setupForByteArrayInput();
}

jboolean naMultiFindFacesAndAddByte(JNIEnv *env, jobject jobj, jbyteArray data,
                                    jint width, jint height, jint rotation, jint format) {
    TVSMultiTrackerEngine* engine = getTVSMultiTrackerEnginePtr(env, jobj);
    if (engine == nullptr) return JNI_FALSE;
    jbyte *ptr = nullptr;
    if (data == nullptr) return JNI_FALSE;
    ptr = env->GetByteArrayElements(data, NULL);

#if FACE_DETECTION_METHOD == 1   // opencv

    // Modified.
//    cv::Mat gray (height, width, CV_8U, ptr);
    cv::Mat gray;
    if (format == BYTE_NV21) {   // nv21.
//        printDebug("Image data format is NV21.");
        cv::Mat tempMat(height, width, CV_8U, ptr);
        gray = tempMat.clone();
        tempMat.release();
    } else {
        printErr("Unsupported byteArray data format:%d", (int)format);
        return JNI_FALSE;
    }

    std::vector<cv::Rect> rect = engine->findFaces(gray, rotation);
    gray.release();

#elif FACE_DETECTION_METHOD == 2   // dlib.

    cv::Mat matColorImg;
    if (format == BYTE_NV21) {   // nv21.
//        printDebug("Image data format is NV21.");
        cv::Mat tempMat(static_cast<int>(height*1.5), width, CV_8UC1, ptr);
        cvtColor(tempMat, matColorImg, CV_YUV2RGB_NV21);
        tempMat.release();
    } else {
        printErr("Unsupported byteArray data format:%d", (int)format);
        return JNI_FALSE;
    }

    std::vector<cv::Rect> rect = engine->findFacesByDlib(matColorImg, rotation);
    matColorImg.release();

#elif FACE_DETECTION_METHOD == 3   // NPD.

    cv::Mat gray;
    if (format == BYTE_NV21) {   // nv21.
//        printDebug("Image data format is NV21.");
        cv::Mat tempMat(height, width, CV_8U, ptr);
        gray = tempMat.clone();
        tempMat.release();
    } else {
        printErr("Unsupported byteArray data format:%d", (int)format);
        return JNI_FALSE;
    }

    std::vector<cv::Rect> rect = engine->findFacesByNpd(gray, rotation);
    gray.release();

#elif FACE_DETECTION_METHOD == 4   // facelib.

    cv::Mat matColorImg;
    if (format == BYTE_NV21) {   // nv21.
//        printDebug("Image data format is NV21.");
        cv::Mat tempMat(static_cast<int>(height*1.5), width, CV_8UC1, ptr);
        cvtColor(tempMat, matColorImg, CV_YUV2RGB_NV21);
        tempMat.release();
    } else {
        printErr("Unsupported byteArray data format:%d", (int)format);
        return JNI_FALSE;
    }

    std::vector<cv::Rect> rect = engine->findFacesByFacelib(matColorImg, rotation);
    matColorImg.release();

#elif FACE_DETECTION_METHOD == 5   // ACF.

    unsigned char * a = (unsigned char *)ptr;
    //LOGE("zc------------------------123");
  //  LOGE("zhangc", "acf[]={%d,%d,%d,%d,%d,%d,%d,%d}", a[0], a[1], a[2], a[3],
        //                                  a[4], a[5], a[6], a[7]);

    cv::Mat matColorImg;
    if (format == BYTE_NV21) {   // nv21.
//        printDebug("Image data format is NV21.");
        cv::Mat tempMat(static_cast<int>(height*1.5), width, CV_8UC1, ptr);
        cvtColor(tempMat, matColorImg, CV_YUV2RGB_NV21);
        tempMat.release();
    } else {
        printErr("Unsupported byteArray data format:%d", (int)format);
        return JNI_FALSE;
    }

  //  LOGE("zhangc", "matColorImg[]={%d,%d,%d,%d,%d,%d,%d,%d}", matColorImg.data[0], matColorImg.data[1], matColorImg.data[2], matColorImg.data[3],
    //                                                                matColorImg.data[4], matColorImg.data[5], matColorImg.data[6], matColorImg.data[7]);

    std::vector<cv::Rect> rect = engine->findFacesByAcf(matColorImg, rotation);
    matColorImg.release();

#elif FACE_DETECTION_METHOD == 6   // mtcnn/ncnn.
    cv::Mat matColorImg;

    if (format == BYTE_NV21) {   // nv21.
        cv::Mat tempMat(static_cast<int>(height*1.5), width, CV_8UC1, ptr);
        cvtColor(tempMat, matColorImg, CV_YUV2BGR_NV21);
        tempMat.release();
    } else if (format == BYTE_BGR) {   // bgr.
        cv::Mat tempMat(height, width, CV_8UC3, ptr);
        matColorImg = tempMat;
    } else {
        printErr("Unsupported byteArray data format:%d", (int)format);
        return JNI_FALSE;
    }

    std::vector<cv::Rect> rect;
    std::vector<std::vector<cv::Point2f>> points;
    std::tie(rect, points) = engine->findFacesByMtcnn(matColorImg, rotation);
    matColorImg.release();
#endif

    env->ReleaseByteArrayElements(data, ptr, JNI_ABORT);

    if (rect.size() > 0) {
        if (piFaceRect != NULL)
            delete[] piFaceRect;

        piFaceRect = new int[1 + rect.size() * 4];   // x, y, width, height for each rect.
        piFaceRect[0] = rect.size();   // Face number.
		int scaleDownFactor = engine->getScaleDownFactor();
        for (int i = 0; i < rect.size(); i++) {
            cv::Rect r = rect.at(i);
            piFaceRect[i * 4 + 1] = r.x * scaleDownFactor;
            piFaceRect[i * 4 + 2] = r.y * scaleDownFactor;
            piFaceRect[i * 4 + 3] = r.width * scaleDownFactor;
            piFaceRect[i * 4 + 4] = r.height * scaleDownFactor;


        }
        for(auto fivePoint : points) {
          for(auto point : fivePoint) {
            point.x = point.x * scaleDownFactor;
            point.y = point.y * scaleDownFactor;
          }
        }

        std::vector<int> rot (rect.size(), rotation);
        bool ok = engine->addFaces(rect, rot, points);
        return ok ? JNI_TRUE : JNI_FALSE;
    } else {
        if (piFaceRect != NULL)
            delete[] piFaceRect;

        piFaceRect = new int[1];
        piFaceRect[0] = 0;   // Face number is 0.
        return JNI_FALSE;
    }
}

jint
naMultiTrackerPic(JNIEnv *env, jobject jobj, jobject bitmap, jboolean pupils, jboolean highPrec,
                  jboolean smooth) {

    if (!authResult) {
        printErr("请先初始化key")
        return JNI_FALSE;
    }
    TVSMultiTrackerEngine *engine = getTVSMultiTrackerEnginePtr(env, jobj);
    if (engine == nullptr) return JNI_FALSE;

    JniBitmap jniBitmap(env, bitmap);

    if(jniBitmap.ret != 0) {
        return JNI_FALSE;
    }

    cv::Mat bgrMat = ProcessUtils::jnibitmap2bgrmat(jniBitmap);
    if (bgrMat.empty()) {
        printErr("Bitmap failed convert to Mat!");
        return 0;
    }

    int status = JNI_FALSE;
//    engine->setReset(true);
    std::vector<cv::Rect> rect;
    std::vector<std::vector<cv::Point2f>> points;
    std::tie(rect, points) = engine->findFacesByMtcnn(bgrMat, 0);
    if (rect.size() > 0) {
        std::vector<int> rot(rect.size(), 0);
        bool ok = engine->addFaces(rect, rot, points);
        if (ok) {
            cv::Mat gray;
            cv::cvtColor(bgrMat, gray, CV_BGR2GRAY);
            iAliveFaceCount = engine->update(gray, pupils, highPrec, smooth);
            gray.release();
            updateJavaSideWithResults(env, jobj, engine, iAliveFaceCount, false);
            status = JNI_TRUE;
        }
    } else {
        if (piFaceRect != NULL)
            delete[] piFaceRect;

        piFaceRect = new int[1];
        piFaceRect[0] = 0;   // Face number is 0.
        status = JNI_FALSE;
    }
    bgrMat.release();

    return status;
}
jint naMultiUpdateShapesByte(JNIEnv *env, jobject jobj, jbyteArray data,
                             jint width, jint height,
                             jboolean pupils, jboolean highPrec,
                             jboolean smooth, jint format) {
    if (!authResult) {
        printErr("请先初始化key")
        return JNI_FALSE;
    }
  TVSMultiTrackerEngine* engine = getTVSMultiTrackerEnginePtr(env, jobj);
  if (engine == nullptr) return JNI_FALSE;

  jbyte *ptr = nullptr;
  if (data == nullptr) return -1;
  ptr = env->GetByteArrayElements(data, NULL);

    // Modified.
//    cv::Mat gray (height, width, CV_8U, ptr);
    cv::Mat gray;
    if (format == BYTE_NV21) {   // nv21.
        cv::Mat tempMat(height, width, CV_8U, ptr);
        gray = tempMat.clone();
        tempMat.release();
    } else if (format == BYTE_BGR) {   // bgr.
        cv::Mat tempMat(height, width, CV_8UC3, ptr);
        cvtColor(tempMat, gray, CV_BGR2GRAY);
        tempMat.release();
    } else {
        printErr("Unsupported byteArray data format:%d", (int)format);
        return JNI_FALSE;
    }

//  int alive = engine->update(gray, pupils, highPrec, smooth);
  iAliveFaceCount = engine->update(gray, pupils, highPrec, smooth);

  gray.release();
  env->ReleaseByteArrayElements(data, ptr, JNI_ABORT);

//  updateJavaSideWithResults(env, jobj, engine, alive, pupils);
  updateJavaSideWithResults(env, jobj, engine, iAliveFaceCount, pupils);

//  return alive;
  return iAliveFaceCount;
}

jintArray naMultiGetFaceRect(JNIEnv *env, jobject jobj) {
    jintArray result;

    if (piFaceRect == NULL) {
        piFaceRect = new int[1];
        piFaceRect[0] = 0;   // Face number is 0.
    }

    int itemNum = 1 + piFaceRect[0] * 4;

    result = env->NewIntArray(itemNum);

    env->SetIntArrayRegion(result, 0, itemNum, piFaceRect);

    return result;
}

jintArray naMultiGetFaceRectBrightness(JNIEnv *env, jobject jobj) {
	TVSMultiTrackerEngine* engine = getTVSMultiTrackerEnginePtr(env, jobj);

	if (piFaceRectBrightness == NULL)
		piFaceRectBrightness = new int[iNumTrackers];

	engine->getFaceRectBrightness(piFaceRectBrightness);

    jintArray result;

    result = env->NewIntArray(iNumTrackers);

    env->SetIntArrayRegion(result, 0, iNumTrackers, piFaceRectBrightness);

    return result;
}

///****************************************************************************/
/// ULSee Dense-106-Points APIs.

CDenseptns *pCDenseptns = NULL;

void naUlsDensePtsInitialize(JNIEnv *env, jobject jobj) {
    pCDenseptns = new CDenseptns();
}

jfloatArray naUlsDensePtsDenseShape(JNIEnv *env, jobject jobj,
                                jfloatArray pts2d, jint pointNum) {
    if (pCDenseptns == NULL) {
        return NULL;
    }
    int denseNum = 0;
    float *cPts2d = env->GetFloatArrayElements(pts2d, 0);
    float *tmp = pCDenseptns->ulsee_dense_shape(cPts2d, pointNum, denseNum);
    jfloatArray result = env->NewFloatArray(denseNum*2);
    if (result == NULL) {
        return NULL;
    }
    env->SetFloatArrayRegion(result, 0, denseNum*2, tmp);
    return result;
}

jfloatArray naUlsDensePtsJawline(JNIEnv *env, jobject jobj,
                                jfloatArray pts2d, jint pointNum) {
    if (pCDenseptns == NULL) {
        return NULL;
    }
    int denseNum = 0;
    float *cPts2d = env->GetFloatArrayElements(pts2d, 0);
    float *tmp = pCDenseptns->ulsee_jawline(cPts2d, pointNum, denseNum);
    jfloatArray result = env->NewFloatArray(denseNum*2);
    if (result == NULL) {
        return NULL;
    }
    env->SetFloatArrayRegion(result, 0, denseNum*2, tmp);
    return result;
}

jfloatArray naUlsDensePtsDenseEyebrow(JNIEnv *env, jobject jobj,
                                    jfloatArray pts2d, jint pointNum) {
    if (pCDenseptns == NULL) {
        return NULL;
    }
    int denseNum = 0;
    float *cPts2d = env->GetFloatArrayElements(pts2d, 0);
    float *tmp = pCDenseptns->ulsee_dense_eyebrow(cPts2d, pointNum, denseNum);
    jfloatArray result = env->NewFloatArray(denseNum*2);
    if (result == NULL) {
        return NULL;
    }
    env->SetFloatArrayRegion(result, 0, denseNum*2, tmp);
    return result;
}

jfloatArray naUlsDensePtsDenseNose(JNIEnv *env, jobject jobj,
                                jfloatArray pts2d, jint pointNum) {
    if (pCDenseptns == NULL) {
        return NULL;
    }
    int denseNum = 0;
    float *cPts2d = env->GetFloatArrayElements(pts2d, 0);
    float *tmp = pCDenseptns->ulsee_dense_nose(cPts2d, pointNum, denseNum);
    jfloatArray result = env->NewFloatArray(denseNum*2);
    if (result == NULL) {
        return NULL;
    }
    env->SetFloatArrayRegion(result, 0, denseNum*2, tmp);
    return result;
}

jfloatArray naUlsDensePtsDenseEye(JNIEnv *env, jobject jobj,
                                jfloatArray pts2d, jint pointNum) {
    if (pCDenseptns == NULL) {
        return NULL;
    }
    int denseNum = 0;
    float *cPts2d = env->GetFloatArrayElements(pts2d, 0);
    float *tmp = pCDenseptns->ulsee_denseeye(cPts2d, pointNum, denseNum);
    jfloatArray result = env->NewFloatArray(denseNum*2);
    if (result == NULL) {
        return NULL;
    }
    env->SetFloatArrayRegion(result, 0, denseNum*2, tmp);
    return result;
}

jfloatArray naUlsDensePtsDenseMouthShape1(JNIEnv *env, jobject jobj,
                                    jfloatArray pts2d, jint pointNum) {
    if (pCDenseptns == NULL) {
        return NULL;
    }
    int denseNum = 0;
    float *cPts2d = env->GetFloatArrayElements(pts2d, 0);
    float *tmp = pCDenseptns->ulsee_dense_mouth_shape_1(cPts2d, pointNum, denseNum);
    jfloatArray result = env->NewFloatArray(denseNum*2);
    if (result == NULL) {
        return NULL;
    }
    env->SetFloatArrayRegion(result, 0, denseNum*2, tmp);
    return result;
}

jfloatArray naUlsDensePtsDenseMouthShape2(JNIEnv *env, jobject jobj,
                                    jfloatArray pts2d, jint pointNum) {
    if (pCDenseptns == NULL) {
        return NULL;
    }
    int denseNum = 0;
    float *cPts2d = env->GetFloatArrayElements(pts2d, 0);
    float *tmp = pCDenseptns->ulsee_dense_mouth_shape_2(cPts2d, pointNum, denseNum);
    jfloatArray result = env->NewFloatArray(denseNum*2);
    if (result == NULL) {
        return NULL;
    }
    env->SetFloatArrayRegion(result, 0, denseNum*2, tmp);
    return result;
}

void naUlsDensePtsRelease(JNIEnv *env, jobject jobj) {
    if (pCDenseptns != NULL) {
        delete[] pCDenseptns;
        pCDenseptns = NULL;
    }
}

void naSetThreshold (JNIEnv *env, jobject jobj, jfloat first, jfloat others) {
    TVSMultiTrackerEngine* engine = getTVSMultiTrackerEnginePtr(env, jobj);
    engine->setThreshold(first, others);
}

void naSetScaleDownFactor(JNIEnv *env, jobject jobj, jint scaleDownFactor) {
  TVSMultiTrackerEngine* engine = getTVSMultiTrackerEnginePtr(env, jobj);
  if (engine != nullptr)
    engine->setScaleDownFactor(scaleDownFactor);
}

void naEnableAutoScaleDown(JNIEnv *env, jobject jobj) {
  TVSMultiTrackerEngine* engine = getTVSMultiTrackerEnginePtr(env, jobj);
  if (engine != nullptr)
    engine->setScaleDownFactor(0);
}

int naGetTrackerState(JNIEnv *env, jobject jobj, jint index) {
    TVSMultiTrackerEngine* engine = getTVSMultiTrackerEnginePtr(env, jobj);
    return engine->getTrackerState(index);
}

jboolean naCheckBlur(JNIEnv *env, jobject jobj, jobject bitmap, jfloat firstparams,jfloat secondparams
    ,jint left, jint top, jint right, jint bottom) {
    // 封装为JniBitmap对象
    JniBitmap jniBitmapSrc(env, bitmap);
    // 转化为Mat
    cv::Mat srcMat = ProcessUtils::jnibitmap2bgrmat(jniBitmapSrc);
    cv::Rect extractRect(left, top, right-left, bottom-top);
    //裁剪框是否在图片内(入参验证)
    bool is_inside = (extractRect & cv::Rect(0, 0, srcMat.cols, srcMat.rows)) == extractRect;
    if(is_inside){
        srcMat = srcMat(extractRect);
    }
    pair<double, double> ssim_lap = blur_eval(srcMat);
   /* if (!env->IsSameObject(blurResults, NULL)) {
        env->SetDoubleArrayRegion(blurResults,0,1,&ssim_lap.first);
        env->SetDoubleArrayRegion(blurResults,1,1,&ssim_lap.second);
    }*/
    /*printErr("naCheckBlur %lf#%lf",ssim_lap.first,ssim_lap.second);*/
//    if (ssim_lap.first  < 0.968 && ssim_lap.second > 6.0){
    if (ssim_lap.first  < firstparams && ssim_lap.second > secondparams){
        return true;
    }
    printErr("naCheckBlur FALSE");
    return false;
}

jobjectArray nativeProcessDetectorImage(JNIEnv *env, jobject jobj,jobject image) {
    TVSMultiTrackerEngine* engine = getTVSMultiTrackerEnginePtr(env, jobj);
    if (engine == nullptr) {
        return NULL;
    }
    return engine->processDetectorImageByNcnn(env, image);
}

void naCrop(JNIEnv *env, jobject jobj,jobject srcBitmap, jobject outBitmap,jint left, jint top, jint right, jint bottom) {
    // 封装为JniBitmap对象
    JniBitmap jniBitmapSrc(env, srcBitmap);
    // 转化为Mat
    cv::Mat srcMat;
    ProcessUtils::bitmapToMat(env,srcBitmap,srcMat);
    cv::Rect extractRect(left, top, right-left, bottom-top);
    //裁剪框是否在图片内(入参验证)
    bool is_inside = (extractRect & cv::Rect(0, 0, srcMat.cols, srcMat.rows)) == extractRect;
    if(is_inside){
        printDebug("naCrop is_inside %d", srcMat.type());
        ProcessUtils::matToBitmap(env,srcMat(extractRect),outBitmap);
    } else{
        printDebug("naCrop not_inside");
        outBitmap = srcBitmap;
    }
}
///****************************************************************************/


static JNINativeMethod nativeMultiTrackerMethods[] = {
        {"naMultiInitialiseFromAssets",
                                          "(Landroid/content/Context;IZLandroid/content/res/AssetManager;ILjava/lang/String;)Z",
                                                                                                                                           (void *) naMultiInitialiseFromAssets},
        {"naMultiInitialiseFromPath",    "(Landroid/content/Context;IZLjava/lang/String;)Z",
                                                                                                                                          (void *) naMultiInitialiseFromPath},
        {"naMultiDispose",               "()V",                                                                                           (void *) naMultiDispose},
        {"naMultiActivate",              "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I", (void *) naMultiActivate},
        {"naMultiTrackerPic",            "(Landroid/graphics/Bitmap;ZZZ)I",                                                               (void *) naMultiTrackerPic},
        {"naMultiSetupOpenGL",           "(III)Z",                                                                                        (void *) naMultiSetupOpenGL},
        {"naMultiEGLContextInvalidated", "()V",                                                                                           (void *) naMultiEGLContextInvalidated},
        {"naMultiFindFacesAndAdd",       "(I)Z",                                                                                          (void *) naMultiFindFacesAndAdd},
        {"naMultiAddFaces",               "([I)Z",                                                                                         (void *) naMultiAddFaces},
        {"naMultiUpdateShapes",           "(ZZZ)I",                                                                                        (void *) naMultiUpdateShape},
        {"naMultiResetTracker",           "(I)Z",                                                                                          (void *) naMultiResetTracker},
        {"naMultiSetupByteArray",         "()Z",                                                                                           (void *) naMultiSetupByteArray},
        {"naMultiFindFacesAndAddByte",    "([BIIII)Z",                                                                                     (void *) naMultiFindFacesAndAddByte},
        {"naMultiUpdateShapesByte",       "([BIIZZZI)I",                                                                                   (void *) naMultiUpdateShapesByte},
        {"naMultiGetFaceRect",            "()[I",                                                                                          (void *) naMultiGetFaceRect},
//  {"naMultiGetFaceRectBrightness", "()[I", (void*)naMultiGetFaceRectBrightness},
  {      "naUlsDensePtsInitialize",       "()V",                                                                                           (void*)naUlsDensePtsInitialize},
        {"naUlsDensePtsDenseShape",       "([FI)[F",                                                                                       (void *) naUlsDensePtsDenseShape},
        {"naUlsDensePtsJawline",          "([FI)[F",                                                                                       (void *) naUlsDensePtsJawline},
        {"naUlsDensePtsDenseEyebrow",     "([FI)[F",                                                                                       (void *) naUlsDensePtsDenseEyebrow},
        {"naUlsDensePtsDenseNose",        "([FI)[F",                                                                                       (void *) naUlsDensePtsDenseNose},
        {"naUlsDensePtsDenseEye",         "([FI)[F",                                                                                       (void *) naUlsDensePtsDenseEye},
        {"naUlsDensePtsDenseMouthShape1", "([FI)[F",                                                                                       (void *) naUlsDensePtsDenseMouthShape1},
        {"naUlsDensePtsDenseMouthShape2", "([FI)[F",                                                                                       (void *) naUlsDensePtsDenseMouthShape2},
        {"naUlsDensePtsRelease",          "()V",                                                                                           (void *) naUlsDensePtsRelease},
        {"naSetThreshold",                "(FF)V",                                                                                         (void *) naSetThreshold},
        {"naSetScaleDownFactor",          "(I)V",                                                                                          (void *) naSetScaleDownFactor},
        {"naEnableAutoScaleDown",         "()V",                                                                                           (void *) naEnableAutoScaleDown},
        {"naCheckBlur",                  "(Landroid/graphics/Bitmap;FFIIII)Z",                                                             (void *) naCheckBlur},
        {"nativeProcessDetectorImage",   "(Landroid/graphics/Bitmap;)[Lcom/uls/multifacetrackerlib/bean/FaceInfo;",                       (void *) nativeProcessDetectorImage},
        {"naCrop",                       "(Landroid/graphics/Bitmap;Landroid/graphics/Bitmap;IIII)V",                                      (void *) naCrop},
        {"naGetTrackerState",             "(I)I",                                                                                          (void *) naGetTrackerState}
 };

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *pVM, void* reserved) {
  JNIEnv *env;
  if ((pVM)->GetEnv((void **)&env, JNI_VERSION_1_6) != JNI_OK) {
    return -1;
  }
  Eigen::initParallel();

   //TVSMultiTracker class
  jclass mtclass = env->FindClass(
      "com/uls/multifacetrackerlib/UlsMultiTracker");
  multiTrackerDataCache.javaClass = (jclass) env->NewGlobalRef(mtclass);
  env->RegisterNatives(multiTrackerDataCache.javaClass,
                       nativeMultiTrackerMethods,
                       sizeof(nativeMultiTrackerMethods) / sizeof(nativeMultiTrackerMethods[0]));

  if (env->ExceptionCheck()) {
    //jthrowable except = env->ExceptionOccurred();
    return JNI_FALSE;
  }

  return JNI_VERSION_1_6;
}
