#include <jniWrapper/JniFloatArray2D.h>
#include "ProcessUtils.h"
#include "Log.h"
#include "CommonUtil.h"
#include <android/log.h>

const char kLogTag[] = "ProcessUtils";
#define printErr(...) {__android_log_print(ANDROID_LOG_ERROR, kLogTag, __VA_ARGS__);}
#define printDebug(...) {__android_log_print(ANDROID_LOG_DEBUG, kLogTag, __VA_ARGS__);}

using namespace cv;

void ProcessUtils::processMatAndPoints(const cv::Mat &mat, cv::Mat &resMat, JniFloatArray2D &jniFloatArray2D,
                                    int rotation) {

    int w = mat.cols;
    int h = mat.rows;
    processMat(mat, resMat, rotation);
    processPoints(jniFloatArray2D, w, h, rotation);
}

/**
 *
 * @param mat
 * @param resMat
 * @param rotation 逆时针旋转的度数
 */
void ProcessUtils::processMat(const cv::Mat &mat, cv::Mat &resMat, int rotation) {
    switch (rotation) {
        case 90:
            cv::transpose(mat, resMat);
            cv::flip(resMat, resMat, 0);
            break;
        case 180:
            cv::flip(mat, resMat, -1);
            break;
        case 270:
            cv::transpose(mat, resMat);
            cv::flip(resMat, resMat, 1);
            break;
        default:
            resMat = mat;
            break;
    }
}

/**
 *
 * @param jniFloatArray2D
 * @param w
 * @param h
 * @param rotation 逆时针旋转的度数
 */
void ProcessUtils::processPoints(JniFloatArray2D &jniFloatArray2D, int w, int h, int rotation) {
    switch (rotation) {
        case 90:
            for (int i = 0; i < jniFloatArray2D.size(); i++) {
                JniFloatArray *pJniFloatArray = jniFloatArray2D.get(i);
                float *array = pJniFloatArray->get();
                int half = pJniFloatArray->size() / 2;
                for (int j = 0; j < half; j++) {
                    float x = array[2 * j];
                    float y = array[2 * j + 1];
                    array[2 * j] = y;
                    array[j * 2 + 1] = w-x;
                 /*   LOGD("processPoints", "w: %d h: %d rotation: %d x: %f y: %f newX: %f newY: %f",
                         w,h,rotation,x,y, array[2 * j], array[j * 2 + 1]);*/
                }
            }
            break;
        case 180:
            for (int i = 0; i < jniFloatArray2D.size(); i++) {
                JniFloatArray *pJniFloatArray = jniFloatArray2D.get(i);
                float *array = pJniFloatArray->get();
                int half = pJniFloatArray->size() / 2;
                for (int j = 0; j < half; j++) {
                    float x = array[2 * j];
                    float y = array[j * 2 + 1];
                    array[2 * j] = w - x;
                    array[j * 2 + 1] = h - y;
                }
            }
            break;
        case 270:
            for (int i = 0; i < jniFloatArray2D.size(); i++) {
                JniFloatArray *pJniFloatArray = jniFloatArray2D.get(i);
                float *array = pJniFloatArray->get();
                int half = pJniFloatArray->size() / 2;
                for (int j = 0; j < half; j++) {
                    float x = array[2 * j];
                    float y = array[j * 2 + 1];
                    array[2 * j] = h-y;
                    array[j * 2 + 1] = x;
                }
            }
            break;
        default:
            break;
    }
}

void ProcessUtils::backupPointsAndRects(std::vector<std::vector<float>> &rects_tmp,
                                        std::vector<std::vector<float>> &points_tmp,
                                        int w, int h, int rotation) {
    backupPoints(points_tmp, w, h, rotation);
    backupRects(rects_tmp, w, h, rotation);
}

/**
 * 顺时针旋转的度数
 * @param points_tmp
 * @param w
 * @param h
 * @param rotation 顺时针旋转的度数
 */
void ProcessUtils::backupPoints(std::vector<std::vector<float>> &points_tmp, int w, int h,
                                  int rotation) {
    std::vector<std::vector<float >> points;

    switch (rotation) {
        case 90:

            //points
            for (int i = 0; i < points_tmp.size(); i++) {
                std::vector<float> point;
                int half = points_tmp[i].size() / 2;
                for (int j = 0; j < half; j++) {
                    float x = points_tmp[i][2 * j];
                    float y = points_tmp[i][j * 2 + 1];
                    float newX = h - y;
                    float newY = x;
                    point.push_back(newX);
                    point.push_back(newY);
                }
                points.push_back(point);
            }
            break;
        case 180:

            //points
            for (int i = 0; i < points_tmp.size(); i++) {
                std::vector<float> point;
                int half = points_tmp[i].size() / 2;
                for (int j = 0; j < half; j++) {
                    float x = points_tmp[i][2 * j];
                    float y = points_tmp[i][j * 2 + 1];
                    float newX = w - x;
                    float newY = h - y;
                    point.push_back(newX);
                    point.push_back(newY);
                }
                points.push_back(point);
            }
            break;
        case 270:

            //points
            for (int i = 0; i < points_tmp.size(); i++) {
                std::vector<float> point;
                int half = points_tmp[i].size() / 2;
                for (int j = 0; j < half; j++) {
                    float x = points_tmp[i][2 * j];
                    float y = points_tmp[i][j * 2 + 1];
                    float newX = y;
                    float newY = w - x;
                    point.push_back(newX);
                    point.push_back(newY);
                }
                points.push_back(point);
            }
            break;
        default:
            points = points_tmp;

            break;
    }
    points_tmp = points;
}

void ProcessUtils::backupRects(std::vector<std::vector<float>> &rects_tmp, int w, int h, int rotation) {
    backupPoints(rects_tmp, w, h, rotation);
    int l, t, r, b;
    for (int i = 0; i < rects_tmp.size(); i++) {
        std::vector<float>& rect = rects_tmp[i];
        l = min(rect[0], rect[2]);
        t = min(rect[1], rect[3]);
        r = max(rect[0], rect[2]);
        b = max(rect[1], rect[3]);
        rect[0] = l;
        rect[1] = t;
        rect[2] = r;
        rect[3] = b;
    }
}

void ProcessUtils::pointsToPointXy(std::vector<std::vector<float>> &points_tmp) {
    std::vector<std::vector<float >> points;
    for (int i = 0; i < points_tmp.size(); i++) {
        std::vector<float> point;
        int half = points_tmp[i].size() / 2;
        for (int j = 0; j < half; j++) {
            float x = points_tmp[i][j];
            float y = points_tmp[i][j + half];

            point.push_back(x);
            point.push_back(y);
        }
        points.push_back(point);
    }
    points_tmp = points;
}

void ProcessUtils::pointXyXyToPointXXYY(std::vector<std::vector<float>> &points_tmp) {
    std::vector<std::vector<float >> pointss;
    for (int i = 0; i < points_tmp.size(); i++) {
        std::vector<float> points;
        int half = points_tmp[i].size() / 2;
        //push x values
        for (int j = 0; j < half; j++) {
            points.push_back(points_tmp[i][2*j]);
        }
        // push y values
        for (int j = 0; j < half; j++) {
            points.push_back(points_tmp[i][2*j+1]);
        }
        pointss.push_back(points);
    }
    points_tmp = pointss;
}

cv::Mat ProcessUtils::jnibitmap2bgrmat(const JniBitmap& jniBitmap) {
    int matType = CV_8UC4;
    int colorCvtCode = 0;
    if (jniBitmap.imageInfo.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        matType = CV_8UC4;
        colorCvtCode = COLOR_RGBA2BGR;
    } else if(jniBitmap.imageInfo.format == ANDROID_BITMAP_FORMAT_RGB_565){
        matType = CV_8UC2;
        colorCvtCode = COLOR_BGR5652BGR;
    }
    cv::Mat srcMat(jniBitmap.getHeight(), jniBitmap.getWidth(), matType,
                   jniBitmap.imagePixels);
    cv::cvtColor(srcMat, srcMat, colorCvtCode);
    return srcMat;
}

cv::Mat ProcessUtils::jnibitmap2rgbmat(const JniBitmap& jniBitmap) {
    int matType = CV_8UC4;
    int colorCvtCode = 0;
    if (jniBitmap.imageInfo.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        matType = CV_8UC4;
        colorCvtCode = COLOR_RGBA2RGB;
    } else if(jniBitmap.imageInfo.format == ANDROID_BITMAP_FORMAT_RGB_565){
        matType = CV_8UC2;
        colorCvtCode = COLOR_BGR5652RGB;
    }
    cv::Mat srcMat(jniBitmap.getHeight(), jniBitmap.getWidth(), matType,
                   jniBitmap.imagePixels);
    cv::cvtColor(srcMat, srcMat, colorCvtCode);
    return srcMat;
}

/**
 * 无失真转换
 * @param env
 * @param srcBitmap
 * @param srcMat
 */
void ProcessUtils::bitmapToMat(JNIEnv *env, const jobject &srcBitmap, Mat &srcMat) {
    void *srcPixels = 0;
    AndroidBitmapInfo srcBitmapInfo;
    try {
        AndroidBitmap_getInfo(env, srcBitmap, &srcBitmapInfo);
        AndroidBitmap_lockPixels(env, srcBitmap, &srcPixels);
        uint32_t srcHeight = srcBitmapInfo.height;
        uint32_t srcWidth = srcBitmapInfo.width;
        srcMat.create(srcHeight, srcWidth, CV_8UC4);
        if (srcBitmapInfo.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
            Mat tmp(srcHeight, srcWidth, CV_8UC4, srcPixels);
            tmp.copyTo(srcMat);
        } else {
            Mat tmp = Mat(srcHeight, srcWidth, CV_8UC2, srcPixels);
            cvtColor(tmp, srcMat, COLOR_BGR5652RGBA);
        }
        AndroidBitmap_unlockPixels(env, srcBitmap);
        return;
    } catch (cv::Exception &e) {
        AndroidBitmap_unlockPixels(env, srcBitmap);
        jclass je = env->FindClass("java/lang/Exception");
        env -> ThrowNew(je, e.what());
        return;
    } catch (...) {
        AndroidBitmap_unlockPixels(env, srcBitmap);
        jclass je = env->FindClass("java/lang/Exception");
        env -> ThrowNew(je, "unknown");
        return;
    }
}

/**
 * 无失真转换
 * @param env
 * @param srcMat
 * @param dstBitmap
 */
void ProcessUtils::matToBitmap(JNIEnv *env, const Mat &srcMat, const jobject &dstBitmap, int convertColor) {
    void *dstPixels = 0;
    AndroidBitmapInfo dstBitmapInfo;
    try {
        AndroidBitmap_getInfo(env, dstBitmap, &dstBitmapInfo);
        AndroidBitmap_lockPixels(env, dstBitmap, &dstPixels);
        uint32_t dstHeight = dstBitmapInfo.height;
        uint32_t dstWidth = dstBitmapInfo.width;
        if (dstBitmapInfo.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
            Mat tmp(dstHeight, dstWidth, CV_8UC4, dstPixels);
            if(convertColor != -1){
                cvtColor(srcMat, tmp, convertColor);
            } else if(srcMat.type() == CV_8UC1) {
                cvtColor(srcMat, tmp, COLOR_GRAY2RGBA);
            } else if (srcMat.type() == CV_8UC3) {
                cvtColor(srcMat, tmp, COLOR_RGB2RGBA);
            } else if (srcMat.type() == CV_8UC4) {
                srcMat.copyTo(tmp);
            }
        } else {
            Mat tmp = Mat(dstHeight, dstWidth, CV_8UC2, dstPixels);
            if(convertColor != 0){
                cvtColor(srcMat, tmp, convertColor);
            } else if(srcMat.type() == CV_8UC1) {
                cvtColor(srcMat, tmp, COLOR_GRAY2BGR565);
            } else if (srcMat.type() == CV_8UC3) {
                cvtColor(srcMat, tmp, COLOR_RGB2BGR565);
            } else if (srcMat.type() == CV_8UC4) {
                cvtColor(srcMat, tmp, COLOR_RGBA2BGR565);
            }
        }
        AndroidBitmap_unlockPixels(env, dstBitmap);
    }catch (cv::Exception &e) {
        AndroidBitmap_unlockPixels(env, dstBitmap);
        jclass je = env->FindClass("java/lang/Exception");
        env -> ThrowNew(je, e.what());
        return;
    } catch (...) {
        AndroidBitmap_unlockPixels(env, dstBitmap);
        jclass je = env->FindClass("java/lang/Exception");
        env -> ThrowNew(je, "unknown");
        return;
    }
}