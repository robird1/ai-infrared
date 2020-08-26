#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "detector.h"
#include "util.h"
#include "stich.h"
#include "infra-utils/uls_infra_log.h"
#include "infra-utils/uls_infra_time.h"
#include "det1.mem.h"
#include "det2.mem.h"
#include "det3.mem.h"

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
}

#ifdef __ANDROID__

detector::detector(float min_size, std::vector<float> thresholds, float factor, unsigned char *mem1,
                   unsigned char *mem2, unsigned char *mem3,
                   bool fast_resize, bool gpu) : min_size(min_size), thresholds(thresholds),
                                                 factor(factor), fast_resize(fast_resize),
                                                 gpu(gpu) {
    PNet.load_param(det1_param_bin);
    PNet.load_model(mem1);

    RNet.load_param(det2_param_bin);
    RNet.load_model(mem2);

    ONet.load_param(det3_param_bin);
    ONet.load_model(mem3);

    if (ncnn::get_cpu_count()  <= 6) //rk3399 6个核，　rk3288 4个核
        num_threads = 2;
    else
        num_threads = 4;
}

#endif

void detector::print_ncnn(ncnn::Mat &in) {
    for(int i = 0;i<in.c;i++){
        for(int j=0;j<in.h;j++){
            for(int k=0;k<in.w;k++){
                printf("the output[%d][%d][%d]is%f\n",i,j,k,*((float*)in.channel(i).data+j*in.w+k));
            }
        }
    }
}

std::tuple<std::vector<std::vector<float>>, std::vector<std::vector<float>>> detector::detect_face(const cv::Mat &iimg) {
    //Mat timg;
    //Important! because of the matlab read data by column, while opencv read data by row. We transpose input first
    //and transpose output later.
    //PERF_START();
    //transpose(iimg, timg);
    //PERF_END();
    //PERF_PROFILE("transpose");

    const float m = 12.0f / this->min_size;
    cv::Mat scale_img;
    resize(iimg, scale_img, cv::Size(iimg.cols*m, iimg.rows*m));


    std::vector<std::pair<Rect, double>> location_and_scale;
    cv::Mat big_img = getPyramidStitchingImage2(scale_img, location_and_scale);
    //LOGD("stich big_img size[%dx%d].\n", big_img.cols, big_img.rows);
#ifdef _SHOW_
    imshow("big_img", big_img);
    waitKey(0);
#endif
//    cv::imshow("a", big_img);
//    cv::waitKey(0);
    ncnn::Mat n_input = ncnn::Mat::from_pixels(big_img.data, ncnn::Mat::PIXEL_BGR, big_img.cols, big_img.rows);

    std::vector<std::vector<float>> total_boxes;
    std::vector<std::vector<float>> points;

    n_input.substract_mean_normalize(mean_vals, norm_vals);
    ncnn::Extractor ex = PNet.create_extractor();
    ex.set_light_mode(true);
    ex.set_num_threads(num_threads);
    ex.input(0, n_input);

    ncnn::Mat score;
    ncnn::Mat reg_mat;

    //PERF_START();
    ex.extract(12, score);
    //print_ncnn(score);
    //PERF_END();
    //PERF_PROFILE("ex.extractx_score");

    for(int i = 0; i < score.h * score.w; i++) {
        if(thresholds[0] < score.channel(1)[i]) {
            //LOGD("sc[%f] is bigger than threshold for PNet.\n", score.channel(1)[i]);
            ex.extract(11, reg_mat);
            break;
        }
    }
    //print_ncnn(reg_mat);
    std::vector<std::vector<float >> boxes;
    if(!reg_mat.empty()) {
       boxes = generateBoundingBox(score, reg_mat, 1.0f, thresholds[0]);
    } else {
      /* LOGD("reg_mat is empty.\n");*/
       return std::tie(total_boxes, points);
    }


    std::vector<int> pick = nms(boxes, 0.5, "Union");
    for(int i = 0; i < pick.size(); i++) {
       total_boxes.push_back(boxes[pick[i]]);
    }

#ifdef _SHOW_
    cv::Mat mapA = big_img.clone();
    for(int i = 0; i < total_boxes.size(); i++) {
        rectangle(mapA, cv::Point2f(total_boxes[i][0] - 1, total_boxes[i][1] - 1),
        cv::Point2f(total_boxes[i][2] - 1, total_boxes[i][3] - 1), cv::Scalar(0, 0, 255));
    }
    imshow("mapA", mapA);
    waitKey(0);
#endif
    //LOGD("stage 1 detected %d faces.\n", total_boxes.size());

    //now we get the total_boxes, second state
    std::map<std::string, std::vector<int>> pad_res;
    if (!total_boxes.empty()) {
        std::vector<int> pick = nms(total_boxes, 0.7, "Union");
        std::vector<std::vector<float>> total_tmp(pick.size());
        std::vector<float> regw(pick.size()), regh(pick.size());
        int n = 0;
        std::generate(std::begin(regw), std::end(regw),
                      [&] { return (total_boxes[pick[n]][2] - total_boxes[pick[n++]][0]); });
        n = 0;
        std::generate(std::begin(regh), std::end(regh),
                      [&] { return (total_boxes[pick[n]][3] - total_boxes[pick[n++]][1]); });
        n = 0;
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
        for (int i = 0; i < total_boxes.size(); i++) {
            for (int j = 0; j < 4; j++) {
                total_boxes[i][j] = matlabFix(total_boxes[i][j]);
            }
        }
        pad_res = pad(total_boxes, big_img.cols, big_img.rows);
    }

#ifdef _SHOW_
    cv::Mat mapB = big_img.clone();;
    for(int i = 0; i < total_boxes.size(); i++) {
        rectangle(mapB, cv::Point2f(pad_res["y"][i] - 1, pad_res["x"][i] - 1),
        cv::Point2f(pad_res["ey"][i] - 1, pad_res["ex"][i] - 1), cv::Scalar(0, 0, 255));
    }
    imshow("mapB", mapB);
    waitKey(0);
#endif

//  total_boxes.clear();
    //calculate coordinates in original timg for total_boxes and pad_res
    const int nScales = location_and_scale.size();
    const int nBox = pad_res["x"].size();
    for(int i = 0; i < nBox; i++) {
       const int  x = pad_res["x"][i];//x1
       const int  y = pad_res["y"][i];//y1
       const int dx = pad_res["ex"][i];//x2
       const int dy = pad_res["ey"][i];//y2
       for(int s = 0; s < nScales; s++) {
         const Rect location = location_and_scale[s].first;
         const double scale = location_and_scale[s].second;
         if(location.x < x and x < location.x+location.width and location.y < y and y < location.y+location.height) {
            total_boxes[i][1] = pad_res["y"][i] = (pad_res["y"][i] - location.y)/scale/m;
            total_boxes[i][0] = pad_res["x"][i] = (pad_res["x"][i] - location.x)/scale/m;
            total_boxes[i][3] = pad_res["ey"][i] = (pad_res["ey"][i] - location.y)/scale/m;
            total_boxes[i][2] = pad_res["ex"][i] = (pad_res["ex"][i] - location.x)/scale/m;
         }
       }
    }
 
#ifdef _SHOW_
    cv::Mat mapC = timg.clone();
    for(int i = 0; i < pad_res["x"].size(); i++) {
       rectangle(mapC, cv::Point2f(pad_res["y"][i] - 1, pad_res["x"][i] - 1),
                 cv::Point2f(pad_res["ey"][i] - 1, pad_res["ex"][i] - 1), cv::Scalar(0, 0, 255));
    }
    imshow("mapC", mapC);
    waitKey(0);
#endif

    auto boxs = total_boxes;
    //LOGD("total_boxes.size[%d], boxs.size[%d].\n", total_boxes.size(), boxs.size());
    total_boxes.clear();
    //LOGD("total_boxes.size[%d], boxs.size[%d].\n", total_boxes.size(), boxs.size());
    pick = nms(boxs, 0.5, "Union");
    //LOGD("nms return pick.size[%d].\n", pick.size());
    for(int i = 0; i < pick.size(); i++) {
        total_boxes.push_back(boxs[pick[i]]);
    }

    //LOGD("total_boxes.size[%d], boxs.size[%d].\n", total_boxes.size(), boxs.size());
    //update pad_res in original timg coordinates
    pad_res = pad(total_boxes,  iimg.cols, iimg.rows);
#ifdef _SHOW_
    cv::Mat mapD = timg.clone();
//    for(int i = 0; i < total_boxes.size(); i++) {
  //    rectangle(mapD, cv::Point2f(total_boxes[i][0] - 1, total_boxes[i][1] - 1),
    //            cv::Point2f(total_boxes[i][2] - 1, total_boxes[i][3] - 1), cv::Scalar(0, 255, 0));
//   }
    for(int i = 0; i < pad_res["x"].size(); i++) {
      rectangle(mapD, cv::Point2f(pad_res["y"][i] - 1, pad_res["x"][i] - 1),
                cv::Point2f(pad_res["ey"][i] - 1, pad_res["ex"][i] - 1), cv::Scalar(0, 0, 255));
    }
    imshow("mapD", mapD);
    waitKey(0);
#endif


    int len = total_boxes.size();
    if (len > 0) {

        std::vector<std::vector<float>> fine_boxes, mv;
        std::vector<Mat> temp_imgs;
        for (int i = 0; i < len; i++) {
            Mat temp(Size(pad_res["tmpw"][i], pad_res["tmph"][i]), CV_8UC3, CV_RGB(0, 0, 0));
            Rect r(pad_res["x"][i] - 1, pad_res["y"][i] - 1,
                   pad_res["ex"][i] - pad_res["x"][i] + 1,
                   pad_res["ey"][i] - pad_res["y"][i] + 1);
            Mat dst_roi = temp(Rect(pad_res["dx"][i] - 1, pad_res["dy"][i] - 1,
                                    pad_res["edx"][i] - pad_res["dx"][i] + 1,
                                    pad_res["edy"][i] - pad_res["dy"][i] + 1));
            iimg(r).convertTo(dst_roi, dst_roi.type(), 1, 0);
            ncnn::Mat resize_mat;

            ncnn::Mat temp_ncnn = ncnn::Mat::from_pixels(temp.data, ncnn::Mat::PIXEL_BGR, temp.cols, temp.rows);
            resize_bilinear(temp_ncnn, resize_mat, 24, 24);
            resize_mat.substract_mean_normalize(mean_vals, norm_vals);

            ncnn::Extractor ex = RNet.create_extractor();
            ex.set_light_mode(true);
            ex.set_num_threads(num_threads);

            ex.input(0, resize_mat);

            ncnn::Mat score;
            ex.extract(15, score);

            float sc = score.channel(1)[0];
            if (sc > thresholds[1]) {
                std::vector<float> temp(total_boxes[i].begin(), total_boxes[i].begin() + 4), mv_ele;
                temp.push_back(sc);
                fine_boxes.push_back(temp);

                ncnn::Mat bbox;
                ex.extract(14, bbox);
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
        if (len > 0) {
            for (int i = 0; i < len; i++) {
                for (int j = 0; j < 5; j++) {
                    finer_boxes[i][j] = matlabFix(finer_boxes[i][j]);
                }
            }
            pad_res = pad(finer_boxes, iimg.cols, iimg.rows);
            temp_imgs.clear();
            for (int i = 0; i < len; i++) {
                Mat temp(Size(pad_res["tmpw"][i], pad_res["tmph"][i]), CV_8UC3, CV_RGB(0, 0, 0));
                Rect r(pad_res["x"][i] - 1, pad_res["y"][i] - 1,
                       pad_res["ex"][i] - pad_res["x"][i] + 1,
                       pad_res["ey"][i] - pad_res["y"][i] + 1);
                Mat dst_roi = temp(Rect(pad_res["dx"][i] - 1, pad_res["dy"][i] - 1,
                                        pad_res["edx"][i] - pad_res["dx"][i] + 1,
                                        pad_res["edy"][i] - pad_res["dy"][i] + 1));
                iimg(r).convertTo(dst_roi, dst_roi.type(), 1, 0);
                //std::cout << "temp is: " << temp << std::endl;
                ncnn::Mat resize_mat;

                ncnn::Mat temp_ncnn = ncnn::Mat::from_pixels(temp.data, ncnn::Mat::PIXEL_BGR, temp.cols, temp.rows);
                resize_bilinear(temp_ncnn, resize_mat, 48, 48);
                resize_mat.substract_mean_normalize(mean_vals, norm_vals);

                ncnn::Extractor ex = ONet.create_extractor();
                ex.set_light_mode(true);
                ex.set_num_threads(num_threads);

                ex.input(0, resize_mat);
                ncnn::Mat score;
                ex.extract(20, score);

                float sc = score.channel(1)[0];
                if (sc > thresholds[2]) {
                    ncnn::Mat bbox, keyPoint;
                    ex.extract(18, bbox);
                    ex.extract(19, keyPoint);

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
                                      if (n % 2 == 0) {
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
    //LOGD("total_boxes = %d, Points = %d.\n", total_boxes.size(), points.size());
    return std::tie(total_boxes, points);
}
