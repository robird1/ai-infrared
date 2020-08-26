//
// Created by kun on 17-6-24.
//

#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "detector.h"
#include "util.h"

#include "det1.mem.h"
#include "det2.mem.h"
#include "det3.mem.h"

#include <Log.h>
#define TAG "zhangchao_native"

using namespace cv;
const float mean_vals[3] = {127.5, 127.5, 127.5};
const float norm_vals[3] = {0.0078125, 0.0078125, 0.0078125};


detector::detector(float min_size, std::vector<float> thresholds, float factor, std::string P_param_path, std::string P_model_path,
         std::string R_param_path, std::string R_model_path, std::string O_param_path, std::string O_model_path,
         bool fast_resize, bool gpu) : min_size(min_size), thresholds(thresholds), factor(factor), fast_resize(fast_resize), gpu(gpu) {
    PNet.load_param(det1_param_bin);
    PNet.load_model(P_model_path.c_str());
    RNet.load_param(det2_param_bin);
    RNet.load_model(R_model_path.c_str());
    ONet.load_param(det3_param_bin);
    ONet.load_model(O_model_path.c_str());
    if (ncnn::get_cpu_count() <= 6) //rk3399 6个核，　rk3288 4个核
        num_threads = 2;
    else
        num_threads = 4;
}

#ifdef __ANDROID__
detector::detector(float min_size, std::vector<float> thresholds, float factor, unsigned char * mem1,
                   unsigned char * mem2, unsigned char * mem3,
                   bool fast_resize, bool gpu) : min_size(min_size), thresholds(thresholds), factor(factor), fast_resize(fast_resize), gpu(gpu) {
    PNet.load_param(det1_param_bin);
    PNet.load_model(mem1);

    RNet.load_param(det2_param_bin);
    RNet.load_model(mem2);

    ONet.load_param(det3_param_bin);
    ONet.load_model(mem3);
}
#endif

std::tuple<std::vector<std::vector<float>>, std::vector<std::vector<float>>> detector::detect_face(const cv::Mat &iimg) {
    LOGD(TAG, "detect_face 0");
    Mat img, timg;
    //Important! because of the matlab read data by column, while opencv read data by row. We transpose input first
    //and transpose output later.
    transpose(iimg, timg);
    LOGD(TAG, "detect_face 1");

    ncnn::Mat n_input = ncnn::Mat::from_pixels(timg.data, ncnn::Mat::PIXEL_BGR2RGB, timg.cols, timg.rows);


    //timg.convertTo(img, CV_32FC3);
    LOGD(TAG, "detect_face 2");

    if (this->fast_resize) {
        img = (img - 127.5) * 0.0078125;
    }
    int factor_count = 0;
    std::vector<std::vector<float>> total_boxes;
    std::vector<std::vector<float>> points;
    int h = timg.size().height, w = timg.size().width;
    float minl = std::min(h, w);

    float m = 12 / this->min_size;
    minl = minl * m;
    // create scale pyramid
    std::vector<float> scales;
    while (minl >= 12) {
        scales.push_back(m * std::pow(this->factor, factor_count));
        minl = minl * factor;
        factor_count++;
    }
    LOGD(TAG, "detect_face 3");
    // first stage
    for (const float &scale : scales) {
        int hs = std::ceil(h * scale),
                ws = std::ceil(w * scale);
        ncnn::Mat in;

        resize_bilinear(n_input, in, ws, hs);
        in.substract_mean_normalize(mean_vals, norm_vals);
        ncnn::Extractor ex = PNet.create_extractor();
        ex.set_light_mode(true);
        ex.set_num_threads(num_threads);
        //ex.input("data", in);
        LOGD(TAG, "detect_face 3.1");
        ex.input(0, in);
        LOGD(TAG, "detect_face 3.2");

        //test the input
//        float *i = in.channel(0);//score.data + score.cstep;
//        for(int row=0; row < in.h; row++){
//            for(int col=0; col < in.w; col++){
//                float in = *i;
//                i++;
//            }
//        }

        ncnn::Mat score_, location_;
        //ex.extract("prob1", score_);
       // ex.extract("conv4-2", location_);
        LOGD(TAG, "detect_face 3.3");
        ex.extract(12, score_);
        LOGD(TAG, "detect_face 3.4");
        ex.extract(11, location_);
        LOGD(TAG, "detect_face 3.5");

        std::vector<std::vector<float >> boxes = generateBoundingBox(score_, location_, scale,
                                                                     thresholds[0]);
        std::vector<int> pick = nms(boxes, 0.5, "Union");
        for (int i = 0; i < pick.size(); i++) { total_boxes.push_back(boxes[pick[i]]); }
        LOGD(TAG, "detect_face 3.6");
    }

    //now we get the total_boxes, second state
    int numbox = total_boxes.size();
    std::map<std::string, std::vector<int>> pad_res;
    LOGD(TAG, "detect_face 4");
    if (numbox > 0) {
        std::vector<int> pick = nms(total_boxes, 0.7, "Union");
        LOGD(TAG, "detect_face 4.1");
        std::vector<std::vector<float>> total_tmp(pick.size());
        // for(int i = 0; i < pick.size(); i++) { total_tmp.push_back(total_boxes[pick[i]]);}
        std::vector<float> regw(pick.size()), regh(pick.size());
        int n = 0;
        std::generate(std::begin(regw), std::end(regw),
                      [&] { return (total_boxes[pick[n]][2] - total_boxes[pick[n++]][0]); });
        n = 0;
        std::generate(std::begin(regh), std::end(regh),
                      [&] { return (total_boxes[pick[n]][3] - total_boxes[pick[n++]][1]); });
        n = 0;
        // float temp = getFloatPrecision(total_boxes[pick[n]][5], 4);
        std::generate(std::begin(total_tmp), std::end(total_tmp),
                      [&] {
                          std::vector<float> element;
                          element.push_back(total_boxes[pick[n]][0] + total_boxes[pick[n]][5] * regw[n]);
                          element.push_back(total_boxes[pick[n]][1] + total_boxes[pick[n]][6] * regh[n]);
                          element.push_back(total_boxes[pick[n]][2] + total_boxes[pick[n]][7] * regw[n]);
                          element.push_back(total_boxes[pick[n]][3] + total_boxes[pick[n]][8] * regh[n]);
                          element.push_back(total_boxes[pick[n++]][4]);
                          return element;
                      });
        LOGD(TAG, "detect_face 4.2");
        total_boxes = rerec(total_tmp);
        //!important: we have transpose the img, so w is swap with h
        for (int i = 0; i < total_boxes.size(); i++) {
            for (int j = 0; j < 4; j++) {
                total_boxes[i][j] = matlabFix(total_boxes[i][j]);
            }
        }
        LOGD(TAG, "detect_face 4.3");
        pad_res = pad(total_boxes, h, w);
    }
    //now we get the total_boxes, second state
    int len = total_boxes.size();
    LOGD(TAG, "detect_face 5");
    if (len > 0) {

        std::vector<std::vector<float>> fine_boxes, mv;
        std::vector<float> ss;
        std::vector<Mat> temp_imgs;
        LOGD(TAG, "detect_face 5.1");
        for (int i = 0; i < len; i++) {
            Mat temp(Size(pad_res["tmph"][i], pad_res["tmpw"][i]), CV_8UC3, CV_RGB(0, 0, 0));
            Rect r(pad_res["y"][i] - 1, pad_res["x"][i] - 1,
                   pad_res["ey"][i] - pad_res["y"][i] + 1,
                   pad_res["ex"][i] - pad_res["x"][i] + 1);
            Mat dst_roi = temp(Rect(pad_res["dy"][i] - 1, pad_res["dx"][i] - 1,
                                    pad_res["edy"][i] - pad_res["dy"][i] + 1,
                                    pad_res["edx"][i] - pad_res["dx"][i] + 1));
            timg(r).convertTo(dst_roi, dst_roi.type(), 1, 0);
            LOGD(TAG, "detect_face 5.1.1");
            ncnn::Mat resize_mat;

            ncnn::Mat temp_ncnn = ncnn::Mat::from_pixels(temp.data, ncnn::Mat::PIXEL_BGR2RGB, temp.cols, temp.rows);
            resize_bilinear(temp_ncnn, resize_mat, 24, 24);
            resize_mat.substract_mean_normalize(mean_vals, norm_vals);

            ncnn::Extractor ex = RNet.create_extractor();
            ex.set_light_mode(true);
            ex.set_num_threads(num_threads);
            //ex.input("data", resize_mat);
            LOGD(TAG, "detect_face 5.1.2");
            ex.input(0, resize_mat);
            LOGD(TAG, "detect_face 5.1.3");
            ncnn::Mat score, bbox;
            //ex.extract("prob1", score);
            //ex.extract("conv5-2", bbox);
            ex.extract(15, score);
            LOGD(TAG, "detect_face 5.1.4");
            ex.extract(14, bbox);
            LOGD(TAG, "detect_face 5.1.5");

#ifdef __ANDROID__
            float sc = score.channel(1)[0];
#else
            float sc = *(score.data+score.cstep);
#endif
            if (sc > thresholds[1]) {
                std::vector<float> temp(total_boxes[i].begin(), total_boxes[i].begin() + 4), mv_ele;
                temp.push_back(sc);
                fine_boxes.push_back(temp);

                mv_ele.push_back(bbox.channel(0)[0]);
                mv_ele.push_back(bbox.channel(1)[0]);
                mv_ele.push_back(bbox.channel(2)[0]);
                mv_ele.push_back(bbox.channel(3)[0]);
                mv.push_back(mv_ele);
            }

        }

        LOGD(TAG, "detect_face 5.2");
        total_boxes.clear();
        len = fine_boxes.size();
        std::vector<std::vector<float>> finer_boxes, finer_mv;
        if (len > 0) {
            std::vector<int> pick = nms(fine_boxes, 0.7, "Union");
            pick_vector(finer_boxes, fine_boxes, pick);
            pick_vector(finer_mv, mv, pick);
            finer_boxes = bbreg(finer_boxes, finer_mv);
            finer_boxes = rerec(finer_boxes);
        }
        // finally, we go into the third stage;
        len = finer_boxes.size();
        // reuse total_boxes, points, mv
        total_boxes.clear();
        points.clear();
        mv.clear();
        std::vector<float> vw, vh;
        ss.clear();
        LOGD(TAG, "detect_face 5.3");
        if (len > 0) {
            for (int i = 0; i < len; i++) {
                for (int j = 0; j < 5; j++) {
                    finer_boxes[i][j] = matlabFix(finer_boxes[i][j]);
                }
            }
            pad_res = pad(finer_boxes, h, w);
            temp_imgs.clear();
            for (int i = 0; i < len; i++) {
                Mat temp(Size(pad_res["tmph"][i], pad_res["tmpw"][i]), CV_8UC3, CV_RGB(0, 0, 0));
                Rect r(pad_res["y"][i] - 1, pad_res["x"][i] - 1,
                       pad_res["ey"][i] - pad_res["y"][i] + 1,
                       pad_res["ex"][i] - pad_res["x"][i] + 1);
                Mat dst_roi = temp(Rect(pad_res["dy"][i] - 1, pad_res["dx"][i] - 1,
                                        pad_res["edy"][i] - pad_res["dy"][i] + 1,
                                        pad_res["edx"][i] - pad_res["dx"][i] + 1));
                timg(r).convertTo(dst_roi, dst_roi.type(), 1, 0);
                //std::cout << "temp is: " << temp << std::endl;
                ncnn::Mat resize_mat;
//                Mat resize_mat;
//                resize(temp, resize_mat, Size(48, 48));
//                resize_mat = (resize_mat - 127.5) * 0.0078125;
//                temp_imgs.push_back(resize_mat);


                ncnn::Mat temp_ncnn = ncnn::Mat::from_pixels(temp.data, ncnn::Mat::PIXEL_BGR2RGB, temp.cols, temp.rows);
                resize_bilinear(temp_ncnn, resize_mat, 48, 48);
                resize_mat.substract_mean_normalize(mean_vals, norm_vals);

//                for (int i = 0; i < 10; i++) {
//                    std::cout << resize_mat.channel(0)[i] << std::endl;
//                }

                ncnn::Extractor ex = ONet.create_extractor();
                ex.set_light_mode(true);
                ex.set_num_threads(num_threads);
                //ex.input("data", resize_mat);
                ex.input(0, resize_mat);
                ncnn::Mat score, bbox, keyPoint;
               /* ex.extract("prob1", score);
                ex.extract("conv6-2", bbox);
                ex.extract("conv6-3", keyPoint);*/
                ex.extract(21, score);
                ex.extract(19, bbox);
                ex.extract(20, keyPoint);

                float sc = score.channel(1)[0];
                ss.push_back(sc);
                if (sc > thresholds[2]) {
                    //each point contains 10 element because of 5 landmarks
                    std::vector<float> point(10);
                    int n = 0;
                    std::generate(std::begin(point), std::end(point),
                                  [&] { return keyPoint.channel(n++)[0]; });

                    std::vector<float> temp(finer_boxes[i].begin(), finer_boxes[i].begin() + 4), mv_ele;
                    vw.push_back(finer_boxes[i][2] - finer_boxes[i][0] + 1);
                    vh.push_back(finer_boxes[i][3] - finer_boxes[i][1] + 1);

                    temp.push_back(sc);
                    total_boxes.push_back(temp);
                    mv_ele.push_back(bbox.channel(0)[0]);
                    mv_ele.push_back(bbox.channel(1)[0]);
                    mv_ele.push_back(bbox.channel(2)[0]);
                    mv_ele.push_back(bbox.channel(3)[0]);
                    mv.push_back(mv_ele);
                    n = 0;
                    std::generate(std::begin(point), std::end(point),
                                  [&] {
                                      if (n < 5) {
                                          return point[n++] * vw[vw.size() - 1] + temp[0] - 1;
                                      } else {
                                          return point[n++] * vh[vh.size() - 1] + temp[1] - 1;
                                      }
                                  });
                    points.push_back(point);
                }

            }

            len = total_boxes.size();
            if (len > 0) {
                total_boxes = bbreg(total_boxes, mv);
                std::vector<int> pick = nms(total_boxes, 0.7, "Min");
                auto copy_boxes = total_boxes, copy_points = points;
                pick_vector(total_boxes, copy_boxes, pick);
                pick_vector(points, copy_points, pick);
            }
        }
        LOGD(TAG, "detect_face 5.4");


    }
    LOGD(TAG, "detect_face 6");
    //LOGD("uriah","total_boxes = %d, Points = %d", total_boxes.size(), points.size());
    return std::tie(total_boxes, points);
}
