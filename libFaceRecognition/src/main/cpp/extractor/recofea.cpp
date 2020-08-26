//
// Created by kun on 16-11-1.
//

//#include <Logging.h>
#include <Log.h>
#include <vector>
#include <Bench.h>
#include "util.h"
#include "recofea.h"
#include <jni.h>
#include <CommonUtil.h>
#include <sys/file.h>
#include <sys/stat.h>
#include "ProcessUtils.h"

#define TAG "recofea_native"

using namespace std;

cv::Mat recofea::preprocess(cv::Mat image, std::vector<cv::Point2f> facial5points) {
    if (facial5points.size() == 5) {
        trans = cv_transform_warpper(facial5points, coord5points);
        if (trans.cols == 0) {
            return Mat();
        }
    }
    cv::Mat input;
    cv::warpAffine(image, input, trans, sz);

    return input;
}

/**
 * 相机/图片检测人脸都会调用此方法
 * @param facial5points 格式: x1,y1,x2,y2,x3,y3,x4,y4,x5,y5
 */
std::vector<float>
recofea::extract_feature(cv::Mat img, std::vector<cv::Point2f> facial5points) {
    std::vector<float> feas;

    //直接读取文件打印Feature 这时候需要把下面四行注释掉 cv::Mat input_data = imread("/sdcard/features/img3.bmp");
    //img = cv::imread("/sdcard/1.png");
    cv::Mat input_data = preprocess(img, facial5points);
    if (input_data.empty()) {
        return feas;
    }

    //input_data = imread("/sdcard/1_aligned.bmp");
    //cv::imwrite("/sdcard/1_aligned.bmp", input_data);

    ncnn::Mat out;
    ncnn::Mat in = ncnn::Mat::from_pixels(input_data.data, ncnn::Mat::PIXEL_BGR, input_data.cols, input_data.rows);

    bench_start();

    ncnn::Extractor ex = net.create_extractor();
    ex.set_light_mode(true);
    ex.set_num_threads(num_threads);// set the active threads
    ex.input(mbv3FR_S2_4_param_id::BLOB_data, in);
    ex.extract(mbv3FR_S2_4_param_id::BLOB_fc1, out);

    bench_end("face recognition");

    //FILE *fpp; fpp = fopen("/sdcard/out128.txt", "w+");
    //LOGD(TAG, "w[%d], h[%d], c[%d]", out.w, out.h, out.c);
    feas.resize(out.w);
    for (int j = 0; j < out.w; j++) {
        //feas[j] = out.channel(j)[0];
        feas[j] = out.channel(0)[j];
        //LOGE(TAG, "feas[%d]=%f", j, feas[j]);
        //fprintf(fpp, "%f\n", feas[j]);
    }
    //fclose(fpp);
    return feas;
}





