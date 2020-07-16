//
// Created by whb on 18-1-10.
//
#include "YuvUtils.h"
#include <Log.h>

cv::Mat getBgrMatCV(jbyte *nv21, int width, int height) {
    cv::Mat matNv21(height + (height >> 1), width, CV_8UC1, nv21);
    cv::Mat bgrMat;
    cv::cvtColor(matNv21, bgrMat, CV_YUV2BGR_NV21);
    return bgrMat;
}

cv::Mat getRgbMatCV(jbyte *nv21, int width, int height) {
    cv::Mat matNv21(height + (height >> 1), width, CV_8UC1, nv21);
    cv::Mat bgrMat;
    cv::cvtColor(matNv21, bgrMat, CV_YUV2RGB_NV21);
    return bgrMat;
}

cv::Mat nv21ByteToBgrMat(jbyte *nv21, int width, int height) {
    cv::Mat nv21Mat(height + (height >> 1), width, CV_8UC1, nv21);
    cv::Mat bgrMat(height, width, CV_8UC3);

    cv::cvtColor(nv21Mat, bgrMat, CV_YUV2BGR_NV21);

    return bgrMat;
}

cv::Mat nv21ByteToRgbMat(jbyte *nv21, int width, int height) {
    cv::Mat nv21Mat(height + (height >> 1), width, CV_8UC1, nv21);
    cv::Mat rgbMat(height, width, CV_8UC3);

    cv::cvtColor(nv21Mat, rgbMat, CV_YUV2RGB_NV21);

    return rgbMat;
}
