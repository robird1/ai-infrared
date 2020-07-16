//
// Created by kun on 16-10-10.
//
/*
  define tool function bbreg callbox generate_boundingBox
  and nms, pad, rerec,
  if need some custom struct(s), I need to define it(them) in other files
*/

#ifndef EXTRACTOR_UTIL_H
#define EXTRACTOR_UTIL_H

#include <opencv2/opencv.hpp>

using namespace cv;

void ULsee_rigid_transform(const cv::Mat &A, const cv::Mat &B, cv::Mat &R, cv::Mat &T, float &scale);

cv::Mat
cv_transform_warpper(std::vector<cv::Point2f> facialPoints, std::vector<cv::Point2f> coordPoints);

#endif //DETECTDEMO_UTIL_H
