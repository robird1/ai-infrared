//
// Created by kun on 16-10-10.
//
/*
  define tool function bbreg callbox generate_boundingBox
  and nms, pad, rerec,
  if need some custom struct(s), I need to define it(them) in other files
*/

#ifndef DETECTDEMO_UTIL_H
#define DETECTDEMO_UTIL_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <tuple>
#include <map>
#include "net.h"

using namespace cv;


std::vector<std::vector<float >>
generateBoundingBox(ncnn::Mat score_mat, ncnn::Mat reg_mat, float scale, float t);

std::vector<int> nms(std::vector<std::vector<float >> boxes, float threshold, std::string type);

int matlabFix(float n);

std::vector<std::vector<float>> rerec(std::vector<std::vector<float>> bboxA);

std::map<std::string, std::vector<int>>
pad(std::vector<std::vector<float >> total_boxes, int w, int h);

std::vector<std::vector<float>>
bbreg(std::vector<std::vector<float>> boundingBox, std::vector<std::vector<float>> reg);

template<typename T>
void pick_vector(std::vector<T> &result, const std::vector<T> &in, const std::vector<int> &s) {
    if (!result.empty())
        result.clear();
    result.reserve(s.size());
    std::transform(s.begin(), s.end(), std::back_inserter(result),
                   [&in](typename std::vector<T>::size_type idx) {
                       return in.at(idx);
                   });
}

static std::vector<std::vector<int>> test_sizes = {{0, 1},
                                                   {0, 2},
                                                   {0, 3},
                                                   {1, 1},
                                                   {1, 2},
                                                   {1, 3}};

#endif //DETECTDEMO_UTIL_H
