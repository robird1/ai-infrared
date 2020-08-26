//
// Created by kun on 16-11-1.
//

#ifndef DETECTDEMO_RECOFEA_H
#define DETECTDEMO_RECOFEA_H

#include <vector>
#include <string>

#include <memory>
#include <opencv/cv.hpp>

#include "mbv3FR-S2.4.mem.h"
#include "mbv3FR-S2.4.id.h"

#include "util.h"
#include "cpu.h"
#include "net.h"

class recofea {
public:
    recofea(unsigned char *mem, int width = 112, int height = 112) {
        trans = cv::estimateRigidTransform(lmkPoints, coord5points, false);
        sz.width = width;
        sz.height = height;

        net.load_param(mbv3FR_S2_4_param_bin);
        net.load_model(mem);
        if (ncnn::get_cpu_count() <= 6) //rk3399 6个核，　rk3288 4个核
            num_threads = 2;
        else
            num_threads = 4;
    }

    std::vector<float> extract_feature(cv::Mat img, std::vector<cv::Point2f> facial5points);

private:
    cv::Mat preprocess(cv::Mat image, std::vector<cv::Point2f> facial5points);

    ncnn::Net net;

    cv::Size sz;
    //patch7 default param
    std::vector<cv::Point2f> coord5points = {
        {
            // patch 7 (width: 112, height: 112)
            cv::Point2f(30.2946+8.0, 51.6963),
            cv::Point2f(65.5318+8.0, 51.5014),
            cv::Point2f(48.0252+8.0, 71.7366),
            cv::Point2f(33.5493+8.0, 92.3655),
            cv::Point2f(62.7299+8.0, 92.2041)
        }
    };
    std::vector<cv::Point2f> lmkPoints = {
        cv::Point2f(69.2596, 111.872),
        cv::Point2f(109.409, 112.196),
        cv::Point2f(89.369, 139.533),
        cv::Point2f(73.2612, 152.362),
        cv::Point2f(104.563, 152.069),
    };
    cv::Mat trans;

private:
    int num_threads = 4;

};


#endif //DETECTDEMO_RECOFEA_H
