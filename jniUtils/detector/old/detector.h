//
// Created by kun on 17-6-24.
//

#ifndef CAFFE2FR_DETECTOR_H
#define CAFFE2FR_DETECTOR_H

//#include <cxcore.h>
#include <Log.h>
#include "3rdparty/ncnn/ncnn20180124/include/net.h"
#include "det1.mem.h"
#include "det2.mem.h"
#include "det3.mem.h"
#include <android/asset_manager_jni.h>
using namespace std;
using namespace cv;

class detector {
public:
    detector(float min_size, std::vector<float> thresholds, float factor, unsigned char * mem1,
             unsigned char * mem2, unsigned char * mem3,
           bool fast_resize=false, bool gpu=false)
            : min_size(min_size), thresholds(thresholds), factor(factor), fast_resize(fast_resize), gpu(gpu) {
        PNet.load_param(det1_param_bin);
        PNet.load_model(mem1);

        RNet.load_param(det2_param_bin);
        RNet.load_model(mem2);

        ONet.load_param(det3_param_bin);
        ONet.load_model(mem3);

    }


    // detect face method, return the rectangle box(and a point) and five landmarks
    std::tuple<std::vector<std::vector<float>>, std::vector<std::vector<float>>> detect_face(cv::Mat& img);
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
};


#endif //CAFFE2FR_DETECTOR_H
