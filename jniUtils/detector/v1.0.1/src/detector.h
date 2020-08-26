//
// Created by kun on 17-6-24.
//

#ifndef CAFFE2FR_DETECTOR_H
#define CAFFE2FR_DETECTOR_H

#include "net.h"
#include "cpu.h"

using namespace std;
using namespace cv;

class detector final {
public:
    detector(float min_size, std::vector<float> thresholds, float factor, std::string P_param_path, std::string P_model_path,
           std::string R_param_path, std::string R_model_path, std::string O_param_path, std::string O_model_path,
           bool fast_resize=false, bool gpu=false);

#ifdef __ANDROID__
    detector(float min_size, std::vector<float> thresholds, float factor, unsigned char * mem1,
             unsigned char * mem2, unsigned char * mem3,
             bool fast_resize=false, bool gpu=false);

#endif

    // detect face method, return the rectangle box(and a point) and five landmarks
    std::tuple<std::vector<std::vector<float>>, std::vector<std::vector<float>>> detect_face(const cv::Mat& img);
private:
    float min_size;
    // thresholds should have length 3
    std::vector<float> thresholds;
    bool fast_resize;
    float factor;
    ncnn::Net PNet;
    ncnn::Net RNet;
    ncnn::Net ONet;
    bool gpu;
    int num_threads = 4;
};


#endif //CAFFE2FR_DETECTOR_H
