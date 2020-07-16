#ifndef PRE_PROCESS_UTILS_XY
#define PRE_PROCESS_UTILS_XY

#include <android/bitmap.h>
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <jniWrapper/JniBitmap.h>
#include "jniWrapper/JniFloatArray2D.h"

class ProcessUtils {
public:
    /**
     * 将mat points逆时针旋转指定角度
     * @param mat
     * @param resMat
     * @param jniFloatArray2D (x=pt[2*i] y=pt[2*i+1])
     * @param rotation
     */
    static void processMatAndPoints(const cv::Mat &mat, cv::Mat &resMat, JniFloatArray2D &jniFloatArray2D,
                                    int rotation);

    /**
     * mat 逆时针旋转指定角度
     * @param mat
     * @param resMat
     * @param rotation
     */
    static void processMat(const cv::Mat &mat, cv::Mat &resMat, int rotation);

    /**
     * points 逆时针旋转指定角度
     * @param jniFloatArray2D (x=pt[2*i] y=pt[2*i+1])
     * @param w
     * @param h
     * @param rotation
     */
    static void processPoints(JniFloatArray2D &jniFloatArray2D, int w, int h, int rotation);

    /**
     * point rect 还原
     * @param rects_tmp
     * @param points_tmp
     * @param w
     * @param h
     * @param rotation
     */
    static void backupPointsAndRects(std::vector<std::vector<float>> &rects_tmp,
                                     std::vector<std::vector<float>> &points_tmp,
                                     int w, int h, int rotation);

    /**
     * point 还原
     * @param points_tmp
     * @param w
     * @param h
     * @param rotation
     */
    static void backupPoints(std::vector<std::vector<float>> &points_tmp, int w, int h, int rotation);

    /**
     * rect 还原
     * @param rects_tmp
     * @param w
     * @param h
     * @param rotation
     */
    static void backupRects(std::vector<std::vector<float>> &rects_tmp, int w, int h, int rotation);

    /**
     * 点位排序转换, 将x0,x1,x2...y0,y1,y2...格式转换成x0,y0,x1,y1...的格式
     * (x=pt[i] y=pt[i+half])->(x=pt[2*i] y=pt[2*i+1])
     * @param points_tmp
     */
    static void pointsToPointXy(std::vector<std::vector<float>> &points_tmp);

    /**
     * 点位排序转换, 将x0,y0,x1,y1...格式转换成x0,x1,x2...y0,y1,y2...的格式
     * @param points_tmp
     */
    static void pointXyXyToPointXXYY(std::vector<std::vector<float>> &points_tmp);

    static cv::Mat jnibitmap2bgrmat(const JniBitmap& jniBitmap);

    static cv::Mat jnibitmap2rgbmat(const JniBitmap& jniBitmap);
    static void matToBitmap(JNIEnv *env, const cv::Mat &mat, const jobject &dstBitmap, int convertColor = -1);
    static void bitmapToMat(JNIEnv *env, const jobject &srcBitmap, cv::Mat &srcMat);
public:
    // cv
//    static void transpose(cv::Mat& in, cv::Mat& out, JniFloatArray2D& jniFloatArray2D);
//    static void flip(cv::Mat& in, cv::Mat& out, int flag, JniFloatArray2D& jniFloatArray2D);
};

#endif // PRE_PROCESS_UTILS
