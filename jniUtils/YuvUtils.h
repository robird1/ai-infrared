//
// Created by whb on 18-1-10.
//

#ifndef ULSEEAPPSTORE_YUVUTILS_H
#define ULSEEAPPSTORE_YUVUTILS_H

#include "opencv2/core/mat.hpp"
#include "opencv2/imgproc.hpp"
#include <jni.h>

using namespace cv;

cv::Mat getBgrMatCV(jbyte *nv21, int width, int height);

cv::Mat getRgbMatCV(jbyte *nv21, int width, int height);

cv::Mat nv21ByteToBgrMat(jbyte *nv21, int width, int height);

cv::Mat nv21ByteToRgbMat(jbyte *nv21, int width, int height);

#endif //ULSEEAPPSTORE_YUVUTILS_H
