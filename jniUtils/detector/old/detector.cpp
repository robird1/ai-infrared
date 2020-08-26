//
// Created by kun on 17-6-24.
//

#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "detector.h"
#include "util.h"
#include "Log.h"

#define TAG "ulseeDetector"

using namespace cv;
const float mean_vals[3] = {127.5, 127.5, 127.5};
const float norm_vals[3] = {0.0078125, 0.0078125, 0.0078125};

std::tuple<std::vector<std::vector<float>>, std::vector<std::vector<float>>> detector::detect_face(cv::Mat &iimg) {
    Mat img, timg;
    //Important! because of the matlab read data by column, while opencv read data by row. We transpose input first
    //and transpose output later.
    transpose(iimg, timg);
    timg.convertTo(timg, CV_8UC3);
    ncnn::Mat n_input = ncnn::Mat::from_pixels(timg.data, ncnn::Mat::PIXEL_BGR2RGB, timg.cols, timg.rows);


    //timg.convertTo(img, CV_32FC3);

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
    // first stage
    for (const float &scale : scales) {
        int hs = std::ceil(h * scale),
                ws = std::ceil(w * scale);
        ncnn::Mat in;

        resize_bilinear(n_input, in, ws, hs);
        in.substract_mean_normalize(mean_vals, norm_vals);
        ncnn::Extractor ex = PNet.create_extractor();
        ex.set_light_mode(false);
        ex.input(0, in);

        //test the input
//        float *i = in.channel(0);//score.data + score.cstep;
//        for(int row=0; row < in.h; row++){
//            for(int col=0; col < in.w; col++){
//                float in = *i;
//                i++;
//            }
//        }

        ncnn::Mat score_, location_;
        /*ex.extract("prob1", score_);
        ex.extract("conv4-2", location_);*/
        ex.extract(12, score_);
        ex.extract(11, location_);


        std::vector<std::vector<float >> boxes = generateBoundingBox(score_, location_, scale,
                                                                     thresholds[0]);
        std::vector<int> pick = nms(boxes, 0.5, "Union");
        for (int i = 0; i < pick.size(); i++) { total_boxes.push_back(boxes[pick[i]]); }

    }

    //now we get the total_boxes, second state
    int numbox = total_boxes.size();
    std::map<std::string, std::vector<int>> pad_res;
    if (numbox > 0) {
        std::vector<int> pick = nms(total_boxes, 0.7, "Union");
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
        total_boxes = rerec(total_tmp);
        //!important: we have transpose the img, so w is swap with h
        for (int i = 0; i < total_boxes.size(); i++) {
            for (int j = 0; j < 4; j++) {
                total_boxes[i][j] = matlabFix(total_boxes[i][j]);
            }
        }
        pad_res = pad(total_boxes, h, w);
    }
    //now we get the total_boxes, second state
    int len = total_boxes.size();
    if (len > 0) {

        std::vector<std::vector<float>> fine_boxes, mv;
        std::vector<float> ss;
        std::vector<Mat> temp_imgs;
        for (int i = 0; i < len; i++) {
            Mat temp(Size(pad_res["tmph"][i], pad_res["tmpw"][i]), CV_8UC3, CV_RGB(0, 0, 0));
            Rect r(pad_res["y"][i] - 1, pad_res["x"][i] - 1,
                   pad_res["ey"][i] - pad_res["y"][i] + 1,
                   pad_res["ex"][i] - pad_res["x"][i] + 1);
            Mat dst_roi = temp(Rect(pad_res["dy"][i] - 1, pad_res["dx"][i] - 1,
                                    pad_res["edy"][i] - pad_res["dy"][i] + 1,
                                    pad_res["edx"][i] - pad_res["dx"][i] + 1));
            timg(r).convertTo(dst_roi, dst_roi.type(), 1, 0);
            ncnn::Mat resize_mat;

            ncnn::Mat temp_ncnn = ncnn::Mat::from_pixels(temp.data, ncnn::Mat::PIXEL_BGR2RGB, temp.cols, temp.rows);
            resize_bilinear(temp_ncnn, resize_mat, 24, 24);
            resize_mat.substract_mean_normalize(mean_vals, norm_vals);

            ncnn::Extractor ex = RNet.create_extractor();
            //ex.set_light_mode(true);
            ex.input(0, resize_mat);
            ncnn::Mat score, bbox;
           /* ex.extract("prob1", score);
            ex.extract("conv5-2", bbox);
*/
            ex.extract(15, score);
            ex.extract(14, bbox);
            float sc = score.channel(1)[0];
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
                //ex.set_light_mode(true);
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


    }
    LOGD(TAG, "total_boxes = %zu, Points size = %zu", total_boxes.size(), points.size());
    return std::tie(total_boxes, points);
}