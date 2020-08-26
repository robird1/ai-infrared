/**--------------------------------------------------
* ULSEE-ACF-Detector
*
* Copyright (c) 2016
* Written by Jiaolong Xu
*--------------------------------------------------*/
#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include <iostream>
#include <stdio.h>
#include <cmath>

#include <opencv2/highgui/highgui.hpp>

#include "common.hpp"

using namespace cv;
using namespace ulsee;

ImageData loadImageData(const char* imgFile) {
    cv::Mat m = imread(imgFile);
    assert(m.channels() == 3);
    uint32_t imH = m.rows;
    uint32_t imW = m.cols;
    ImageData im(imH, imW, 3);

    for (int x = 0; x < m.cols; x++) {
        for (int y = 0; y < m.rows; y++) {
            Vec3b p = m.at<Vec3b>(y, x);
            // BGR -> RGB
            im.data[0 * m.cols * m.rows + m.rows * x + y] = p.val[2];// R
            im.data[1 * m.cols * m.rows + m.rows * x + y] = p.val[1];// G
            im.data[2 * m.cols * m.rows + m.rows * x + y] = p.val[0];// B
        }
    }
    return im;
};

/* Return byte array of the image
* @param m: image data load by opencv imread
*/
uint8_t* cvToByteArray(const cv::Mat m) {
    assert(m.channels() == 3);
    uint32_t imH = m.rows;
    uint32_t imW = m.cols;
    uint8_t* im = new uint8_t*[imH * imW * 3];

    for (int x = 0; x < m.cols; x++) {
        for (int y = 0; y < m.rows; y++) {
            Vec3b p = m.at<Vec3b>(y, x);
            // BGR -> RGB
            im.data[0 * m.cols * m.rows + m.rows * x + y] = p.val[2];// R
            im.data[1 * m.cols * m.rows + m.rows * x + y] = p.val[1];// G
            im.data[2 * m.cols * m.rows + m.rows * x + y] = p.val[0];// B
        }
    }
    return im;
};
#endif// _UTILS_IM_LOADER_HPP_
