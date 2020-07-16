#include <jni.h>
#include <cmath>
#include <iostream>
#include <android/asset_manager_jni.h>
#include <extractor/faceFeatureMgr.h>
#include <opencv/cv.hpp>
#include <DetectMgr.h>
#include <jniWrapper/JniBitmap.h>
#include <jniWrapper/JniString.h>
#include <jniWrapper/JniByteArray.h>
#include "Log.h"
#include "YuvUtils.h"
#include "jniWrapper/JniFloatArray2D.h"
#include "jniWrapper/JniUtils.h"
#include "ProcessUtils.h"
#include <authorization.h>

using namespace std;
using namespace uls;
#define Method(x) Java_com_ulsee_sdk_faceverification_ULSeeFaceVerificationMgr_##x

#define TAG "ulseeFaceRecognition"


extern "C"
{

enum byteDataFormat {
    BYTE_NV21 = 0,
    BYTE_BGR = 1,
};


jobjectArray handleResult(JNIEnv *env, const vector<vector<float>> &rects,
                          const vector<vector<float>> &points,
                          const vector<vector<float>> &features) {
    LOGD(TAG, "handleResult 0");
    jobjectArray retInfos = NULL;
    if (rects.size() != points.size() || rects.size() != features.size()) {
        return retInfos;
    }
    LOGD(TAG, "handleResult 1");
    const char *pClassNameFaceInfo = "com/uls/multifacetrackerlib/bean/FaceInfo";
    jclass jclsFaceInfo = env->FindClass(pClassNameFaceInfo);
    jclass jclsRectF = env->FindClass("android/graphics/RectF");
    jfieldID jfleft = env->GetFieldID(jclsRectF, "left", "F");
    jfieldID jfright = env->GetFieldID(jclsRectF, "right", "F");
    jfieldID jftop = env->GetFieldID(jclsRectF, "top", "F");
    jfieldID jfbottom = env->GetFieldID(jclsRectF, "bottom", "F");
    jfieldID jfrect = env->GetFieldID(jclsFaceInfo, "rect", "Landroid/graphics/RectF;");
    jfieldID jfpoints = env->GetFieldID(jclsFaceInfo, "points", "[F");
    jfieldID jffeatures = env->GetFieldID(jclsFaceInfo, "features", "[F");

    LOGD(TAG, "handleResult 2");
    retInfos = env->NewObjectArray(rects.size(), jclsFaceInfo, NULL);
    LOGD(TAG, "handleResult 3");
    for (int i = 0; i < rects.size(); i++) {
        LOGD(TAG, "handleResult 3.1");
        jobject jobjRect = env->AllocObject(jclsRectF);
        env->SetFloatField(jobjRect, jfleft, rects[i][0]);
        env->SetFloatField(jobjRect, jftop, rects[i][1]);
        env->SetFloatField(jobjRect, jfright, rects[i][2]);
        env->SetFloatField(jobjRect, jfbottom, rects[i][3]);

        LOGD(TAG, "handleResult 3.2");
        jfloatArray jffeaturesarr = env->NewFloatArray(features[i].size());
        env->SetFloatArrayRegion(jffeaturesarr, 0, features[i].size(), &features[i][0]);

        LOGD(TAG, "handleResult 3.3");
        int ptSize = points[i].size();
        jfloatArray jfpointsarr = env->NewFloatArray(points[i].size());
        for (int j = 0; j < ptSize; j++) {
            env->SetFloatArrayRegion(jfpointsarr, j, 1, &points[i][j]);
        }
        LOGD(TAG, "handleResult 3.4");
        jobject jobjInfo = env->AllocObject(jclsFaceInfo);
        env->SetObjectField(jobjInfo, jfrect, jobjRect);
        env->SetObjectField(jobjInfo, jffeatures, jffeaturesarr);
        env->SetObjectField(jobjInfo, jfpoints, jfpointsarr);

        LOGD(TAG, "handleResult 3.5");
        env->DeleteLocalRef(jffeaturesarr);
        env->DeleteLocalRef(jfpointsarr);

        LOGD(TAG, "handleResult 3.6");
        env->SetObjectArrayElement(retInfos, i, jobjInfo);
    }

    LOGD(TAG, "handleResult 4");

    return retInfos;
}


JNIEXPORT jlong JNICALL
Method(nativeInitDetector)(JNIEnv *env, jobject obj, jobject assetManager, jfloat minSize) {

    LOGD(TAG, "nativeInitDetector 2");

    AAssetManager *manager = AAssetManager_fromJava(env, assetManager);
    LOGD(TAG, "nativeInitDetector 3");
    ulsee::FaceDetectMgr *faceDetectMgr = new ulsee::FaceDetectMgr();
    LOGD(TAG, "nativeInitDetector 4");
    faceDetectMgr->initDetector(minSize, manager);
    LOGD(TAG, "nativeInitDetector 5");
    return (jlong) faceDetectMgr;
}


JNIEXPORT jlong JNICALL
Method(nativeInitExtractor)(JNIEnv *env, jobject obj, jobject assetManager, jstring key,
                            jstring keySecret, jstring macString) {
#ifndef NO_AUTH
    /*
    LOGD(TAG, "start auth");
    string androidId = JniUtils::getAndroidId(env);
    LOGD(TAG, "androidId = %s", androidId.c_str());
    string macAddress = JniUtils::jstring2string(env, macString);
    LOGD(TAG, "macAddress = %s", macAddress.c_str());
    string appId = JniUtils::getPackageName(env);
    LOGD(TAG, "appId = %s", appId.c_str());
    string licensePath = JniUtils::getLicensePath(env);
    LOGD(TAG, "licensePath = %s", licensePath.c_str());
    int platform = 2;
    uls::Authorization auth;
    auth.initLicenseFile(licensePath.c_str());
    auth.initDevice(androidId.c_str(),macAddress.c_str(), appId.c_str(), platform);

    JniString jkey(env, key);
    JniString jkeySecret(env, keySecret);
    std::string auth_group = "faceRecognition";

    uls_license_key_status key_status = auth.getAuthRet(jkey.get(), jkeySecret.get(),
                                                        auth_group.c_str());
    if (key_status != ULS_KEY_SUCCESS) {
        LOGE(TAG, "key验证失败，请联系ulsee申请合法key ErrorCode: %d", key_status);
        return key_status;
    }
    LOGD(TAG, "key验证成功");
     */
#endif

    AAssetManager *manager = AAssetManager_fromJava(env, assetManager);
    LOGD(TAG, "nativeInitExtractor 3");
    ulsee::FaceFeatureMgr *faceFeatureMgr = new ulsee::FaceFeatureMgr();
    LOGD(TAG, "nativeInitExtractor 4");
    faceFeatureMgr->initModel(manager);
    LOGD(TAG, "nativeInitExtractor 5");
    faceFeatureMgr->initExtractor();
    LOGD(TAG, "nativeInitExtractor 6");
    return (jlong) faceFeatureMgr;
}

JNIEXPORT jobjectArray JNICALL
Method(nativeExtactFeatureData)(JNIEnv *env,
                                jobject obj,
                                jlong extractorHandle,
                                jbyteArray data_,
                                jint srcWidth, jint srcHeight,
                                jobjectArray points_, jint imageDataType) {


    LOGD(TAG, "nativeExtactFeatureData 0");
    jobjectArray array2D = NULL;
    JniByteArray byteData(env, data_);
    JniFloatArray2D points(env, points_);
    int len = points.size();
    do {
        try {
            LOGD(TAG, "nativeExtactFeatureData 0.1");
            if (extractorHandle == 0) {
                LOGE(TAG, "extractorHandle == 0");
                break;
            }
            if (len == 0) {
                LOGE(TAG, "face five points size == 0");
                break;
            }
            ulsee::FaceFeatureMgr *pFaceFeatureMgr = (ulsee::FaceFeatureMgr *) extractorHandle;

            LOGD(TAG, "nativeExtactFeatureData 1");

            cv::Mat bgrMat;
            if (imageDataType == BYTE_NV21) {
                bgrMat = nv21ByteToBgrMat(byteData.get(), srcWidth, srcHeight);
            } else if (imageDataType == BYTE_BGR){
                cv::Mat tmpMat(srcHeight, srcWidth, CV_8UC3, byteData.get());
                bgrMat = tmpMat;
            } else {
                LOGE(TAG, "unsupported byteArray format: %d", (int)imageDataType);
                break;
            }

            LOGD(TAG, "nativeExtactFeatureData 3 len=%d", len);
            array2D = env->NewObjectArray(len, env->FindClass("[F"), NULL);
            for (int i = 0; i < len; i++) {
                float *fivepoint_buf;
                JniFloatArray *pJniFloatArray = points.get(i);
                fivepoint_buf = pJniFloatArray->get();

                LOGD(TAG, "nativeExtactFeatureData 3.1");
                //make sure the five points form Java is [x0,y0,x1,y1,x2,y2,x3,y3,x4,y4]
                std::vector<cv::Point2f> facial5points;
                facial5points.push_back(cv::Point2f(fivepoint_buf[0], fivepoint_buf[1]));
                facial5points.push_back(cv::Point2f(fivepoint_buf[2], fivepoint_buf[3]));
                facial5points.push_back(cv::Point2f(fivepoint_buf[4], fivepoint_buf[5]));
                facial5points.push_back(cv::Point2f(fivepoint_buf[6], fivepoint_buf[7]));
                facial5points.push_back(cv::Point2f(fivepoint_buf[8], fivepoint_buf[9]));

                LOGD(TAG, "nativeExtactFeatureData 3.2");
                std::vector<float> feature_tmp;
                pFaceFeatureMgr->extractFeature(bgrMat, facial5points, feature_tmp);
                LOGD(TAG, "nativeExtactFeatureData 3.3 pFaceFeatureMgr->extractFeature");
                jfloatArray array1D = env->NewFloatArray(feature_tmp.size());
                if (array1D != NULL) {
                    env->SetFloatArrayRegion(array1D, 0, feature_tmp.size(), &feature_tmp[0]);
                    env->SetObjectArrayElement(array2D, i, array1D);
                    env->DeleteLocalRef(array1D);
                }
            }
        } catch (exception &e) {
            LOGD(TAG, "nativeExtactFeatureData 3.999 e=%s", e.what());
        }
    } while (0);

    LOGD(TAG, "nativeExtactFeatureData 4");
    return array2D;
}

JNIEXPORT jobjectArray JNICALL
Method(nativeExtactFeatureDataSync)(JNIEnv *env,
                                    jobject obj,
                                    jlong detectHandle,
                                    jlong extractorHandle,
                                    jbyteArray data_,
                                    jint srcWidth, jint srcHeight,
                                    jint imageRotate, jint imageDataType) {
    LOGD(TAG, "nativeExtactFeatureDataSync 0, imageRotate=%d", imageRotate);
    jobjectArray faceInfoArray = NULL;
    JniByteArray byteData(env, data_);
    do {
        try {
            LOGD(TAG, "nativeExtactFeatureDataSync 0");
            if (detectHandle == 0 || extractorHandle == 0) {
                LOGE(TAG, "detectHandle == 0x%08x || extractorHandle == 0x%08x", detectHandle, extractorHandle);
                break;
            }
            ulsee::FaceFeatureMgr *pFaceFeatureMgr = (ulsee::FaceFeatureMgr *) extractorHandle;
            ulsee::FaceDetectMgr *pFaceDetectMgr = (ulsee::FaceDetectMgr *) (long) detectHandle;

            LOGD(TAG, "nativeExtactFeatureDataSync 1");
            cv::Mat bgrMat;
            if (imageDataType == BYTE_NV21) {
                bgrMat = nv21ByteToBgrMat(byteData.get(), srcWidth, srcHeight);
            } else if (imageDataType == BYTE_BGR){
                cv::Mat tmpMat(srcHeight, srcWidth, CV_8UC3, byteData.get());
                bgrMat = tmpMat;
            } else {
                LOGE(TAG, "unsupported byteArray format: %d", (int)imageDataType);
                break;
            }

            Mat flipMat;
            ProcessUtils::processMat(bgrMat, flipMat, imageRotate);

            std::vector<std::vector<float>> rects_tmp;
            std::vector<std::vector<float>> points_tmp;
            std::vector<std::vector<float>> features_tmp;
            int len = pFaceDetectMgr->detectFace(flipMat, rects_tmp, points_tmp);
            LOGD(TAG, "nativeExtactFeatureDataSync 3 len=%d", len);
            for (int i = 0; i < len; i++) {
                float *fivepoint_buf;
                fivepoint_buf = &points_tmp[i][0];
                LOGD(TAG, "nativeExtactFeatureDataSync 3.1");
                //the five points form big detector are [x0,x1,x2,x3,x4,y0,y1,y2,y3,y4]
                std::vector<cv::Point2f> facial5points;
                facial5points.push_back(cv::Point2f(fivepoint_buf[0], fivepoint_buf[5]));
                facial5points.push_back(cv::Point2f(fivepoint_buf[1], fivepoint_buf[6]));
                facial5points.push_back(cv::Point2f(fivepoint_buf[2], fivepoint_buf[7]));
                facial5points.push_back(cv::Point2f(fivepoint_buf[3], fivepoint_buf[8]));
                facial5points.push_back(cv::Point2f(fivepoint_buf[4], fivepoint_buf[9]));

                LOGD(TAG, "nativeExtactFeatureDataSync 3.2");
                float spend;
                int64 begin = cv::getTickCount();
                std::vector<float> feature_tmp;
                pFaceFeatureMgr->extractFeature(flipMat, facial5points, feature_tmp);
                int64 end = cv::getTickCount();
                spend = (end - begin) * 1000 / cv::getTickFrequency();
                LOGD(TAG,
                     "nativeExtactFeatureDataSync 3.3 pFaceFeatureMgr->extractFeature duration=%f",
                     spend);
                features_tmp.push_back(feature_tmp);
            }

            ProcessUtils::pointsToPointXy(points_tmp);
            ProcessUtils::backupPointsAndRects(rects_tmp, points_tmp, flipMat.cols, flipMat.rows, imageRotate);

            faceInfoArray = handleResult(env, rects_tmp, points_tmp, features_tmp);
        } catch (exception &e) {
            LOGD(TAG, "nativeExtactFeatureDataSync 3.999 e=%s", e.what());
        }
    } while (0);
    LOGD(TAG, "nativeExtactFeatureDataSync 4");
    return faceInfoArray;
}

JNIEXPORT jobjectArray JNICALL
Method(nativeExtactFeatureBitmap)(JNIEnv *env,
                                  jobject obj,
                                  jlong extractorHandle,
                                  jobject image,
                                  jobjectArray points_) {
    LOGD(TAG, "nativeExtactFeatureBitmap 0");
    jobjectArray array2D = NULL;
    JniFloatArray2D points(env, points_);
    int len = points.size();
    do {
        try {
            LOGD(TAG, "nativeExtactFeatureBitmap 0.1");
            if (extractorHandle == 0) {
                LOGE(TAG, "extractorHandle == 0");
                break;
            }
            if (len == 0) {
                LOGE(TAG, "len == 0");
                break;
            }
            ulsee::FaceFeatureMgr *pFaceFeatureMgr = (ulsee::FaceFeatureMgr *) extractorHandle;

            LOGD(TAG, "nativeExtactFeatureBitmap 1");
            JniBitmap jniBitmapSrc(env, image);
            if (jniBitmapSrc.ret != 0) {
                LOGE(TAG, "bitmap is not qualified");
                break;
            }
            cv::Mat mat = ProcessUtils::jnibitmap2bgrmat(jniBitmapSrc);

            LOGD(TAG, "nativeExtactFeatureBitmap 3 len=%d", len);
            array2D = env->NewObjectArray(len, env->FindClass("[F"), NULL);
            for (int i = 0; i < len; i++) {
                float *fivepoint_buf;
                JniFloatArray *pJniFloatArray = points.get(i);
                fivepoint_buf = pJniFloatArray->get();

                LOGD(TAG, "nativeExtactFeatureBitmap 3.1");
                //make sure the five points form Java are [x0,y0,x1,y1,x2,y2,x3,y3,x4,y4]
                std::vector<cv::Point2f> facial5points;
                facial5points.push_back(cv::Point2f(fivepoint_buf[0], fivepoint_buf[1]));
                facial5points.push_back(cv::Point2f(fivepoint_buf[2], fivepoint_buf[3]));
                facial5points.push_back(cv::Point2f(fivepoint_buf[4], fivepoint_buf[5]));
                facial5points.push_back(cv::Point2f(fivepoint_buf[6], fivepoint_buf[7]));
                facial5points.push_back(cv::Point2f(fivepoint_buf[8], fivepoint_buf[9]));

                LOGD(TAG, "nativeExtactFeatureBitmap 3.2");
                std::vector<float> feature_tmp;
                pFaceFeatureMgr->extractFeature(mat, facial5points, feature_tmp);
                LOGD(TAG, "nativeExtactFeatureBitmap 3.3 pFaceFeatureMgr->extractFeature");
                jfloatArray array1D = env->NewFloatArray(feature_tmp.size());
                if (array1D != NULL) {
                    env->SetFloatArrayRegion(array1D, 0, feature_tmp.size(), &feature_tmp[0]);
                    env->SetObjectArrayElement(array2D, i, array1D);
                    env->DeleteLocalRef(array1D);
                }
            }
        } catch (exception &e) {
            LOGD(TAG, "nativeExtactFeatureBitmap 3.999 e=%s", e.what());
        }
    } while (0);

    LOGD(TAG, "nativeExtactFeatureBitmap 4");
    return array2D;
}

cv::Rect boundingSquare(const float *pts, int n) {

    float minx = pts[0], maxx = pts[0];

    float miny = pts[1], maxy = pts[1];

    for (int i = 1; i < n; i++) {

        float x = pts[2*i];

        float y = pts[2*i +1];

        if (minx > x) minx = x;

        else if (maxx < x) maxx = x;


        if (miny > y) miny = y;

        else if (maxy < y) maxy = y;

    }

    cv::Rect bbox = cv::Rect(static_cast<int>(minx), static_cast<int>(miny),

                             static_cast<int>(maxx-minx), static_cast<int>(maxy-miny));

    return bbox;
}

cv::Rect extentRect(cv::Rect fr, float h, float w, float factor) {

    float eh = fr.height * factor, ew = fr.width * factor;

    float ex_1 = fr.x - (ew - fr.width)/ 2, ey_1 = fr.y - (eh - fr.height) / (1.25);

    float ex_2 = std::min(ex_1 + ew, w - 1), ey_2 =  std::min(ey_1 + eh, h - 1);

    cv::Rect res;

    res.x = std::max(int(ex_1), 0), res.y = std::max(int(ey_1), 0);

    res.height = ey_2 - res.y, res.width = ex_2 - res.x;

    return res;
}

JNIEXPORT jobjectArray JNICALL
Method(nativeExtactFeatureDataUsingDetector)(JNIEnv *env,
                                             jobject instance,
                                             jlong detectHandle, jlong extractorHandle, jbyteArray data_,
                                             jint srcWidth, jint srcHeight, jint imageRoate,jobjectArray pointsSixSix, jint imageDataType) {
    LOGD(TAG, "nativeExtactFeatureData 0");
    jobjectArray array2D = NULL;
    JniByteArray byteData(env, data_);
    JniFloatArray2D points66(env, pointsSixSix);

    cv::Mat bgrMat;
    if (imageDataType == BYTE_NV21) {
        bgrMat = nv21ByteToBgrMat(byteData.get(), srcWidth, srcHeight);
    } else if (imageDataType == BYTE_BGR){
        cv::Mat tmpMat(srcHeight, srcWidth, CV_8UC3, byteData.get());
        bgrMat = tmpMat;
    } else {
        LOGE(TAG, "unsupported byteArray format: %d", (int)imageDataType);
        return array2D;
    }

    //将66点和mat都转成正方向
    ProcessUtils::processMatAndPoints(bgrMat, bgrMat, points66, imageRoate);
    LOGD(TAG, "nativeExtactFeatureData 0.1");
    if (detectHandle == 0) {
        LOGE(TAG, "detectHandle == 0");
        return array2D;
    }
    if (extractorHandle == 0) {
        LOGE(TAG, "extractorHandle == 0");
        return array2D;
    }
    try {
        ulsee::FaceDetectMgr *pFaceDetectMgr = (ulsee::FaceDetectMgr *) (long) detectHandle;
        ulsee::FaceFeatureMgr *pFaceFeatureMgr = (ulsee::FaceFeatureMgr *) extractorHandle;
        array2D = env->NewObjectArray(points66.size(), env->FindClass("[F"), NULL);
        for (int i = 0; i < points66.size(); i++) {
            cv::Rect fr = boundingSquare(points66.get(i)->get(), points66.get(i)->size() / 2);
            cv::Rect efr = extentRect(fr, bgrMat.rows, bgrMat.cols, 2.2);
            cv::Mat croppedFace = bgrMat(efr).clone();
            LOGD(TAG, "nativeExtactFeatureData 0.2");
            std::vector<std::vector<float>> rects_tmp;
            std::vector<std::vector<float>> points_tmp;
            int len = pFaceDetectMgr->detectFace(croppedFace, rects_tmp, points_tmp);
            if (len > 0) { //因为已经是裁剪的图,只有一个人脸, 所以只取第一个
                float *fivepoint_buf;
                fivepoint_buf = &points_tmp[0][0];
                LOGD(TAG, "nativeExtactFeatureData 3.1");
                //the five points form big detector are [x0,x1,x2,x3,x4,y0,y1,y2,y3,y4]
                std::vector<cv::Point2f> facial5points;
                facial5points.push_back(cv::Point2f(fivepoint_buf[0], fivepoint_buf[5]));
                facial5points.push_back(cv::Point2f(fivepoint_buf[1], fivepoint_buf[6]));
                facial5points.push_back(cv::Point2f(fivepoint_buf[2], fivepoint_buf[7]));
                facial5points.push_back(cv::Point2f(fivepoint_buf[3], fivepoint_buf[8]));
                facial5points.push_back(cv::Point2f(fivepoint_buf[4], fivepoint_buf[9]));
                LOGD(TAG, "nativeExtactFeatureData 3.2");
                std::vector<float> feature_tmp;
                pFaceFeatureMgr->extractFeature(croppedFace, facial5points, feature_tmp);
                LOGD(TAG, "nativeExtactFeatureData 3.3");
                jfloatArray array1D = env->NewFloatArray(feature_tmp.size());
                if (array1D != NULL) {
                    env->SetFloatArrayRegion(array1D, 0, feature_tmp.size(), &feature_tmp[0]);
                    env->SetObjectArrayElement(array2D, i, array1D);
                    env->DeleteLocalRef(array1D);
                }
            }

        }
    } catch (exception &e) {
        LOGD(TAG, "nativeExtactFeatureData 3.999 e=%s", e.what());
    }
    LOGD(TAG, "nativeExtactFeatureData 4");
    return array2D;
}



JNIEXPORT void JNICALL
Method(nativeReleaseExtractor)(JNIEnv *env, jobject instance, jlong handle) {
    if (handle != 0) {
        delete (ulsee::FaceFeatureMgr *) handle;
    }
}

JNIEXPORT void JNICALL
Method(nativeReleaseDetector)(JNIEnv *env, jobject instance, jlong handle) {
    if (handle != 0) {
        delete (ulsee::FaceDetectMgr *) handle;
    }
}

};