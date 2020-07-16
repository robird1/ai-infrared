#ifndef _HELPER_H
#define _HELPER_H

#include <vector>
#include <string>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>

// functions to get blur evaluation scores (ssim score and laplacian score)
std::pair<double, double> blur_eval(cv::Mat img); // returns scores only

// helper functions for blur_eval()
cv::Mat get_luminance_channel(const cv::Mat& src);
double get_blur_laplacian(const cv::Mat& luminance, cv::Mat& luminance_lap);
double compute_std(const cv::Mat& src);
void get_blur(const cv::Mat& luminance, cv::Mat& blur_hor, cv::Mat& blur_ver);
double getMSSIM( const cv::Mat& i1, const cv::Mat& i2, const cv::Mat& i3);

//L2_Norm function
double l2_norm(std::vector<double> vec);

// function to visualise results (for tuning threshold uasge)
void show_result(const std::string& img_dir, const std::vector<std::string>& img_files, const double& ssim_threshold, const double& lap_threshold);
void add_to_bg(const cv::Mat& img_, const std::pair<double, double>& scores, const std::string& filename, const double& ssim_threshold, const double& lap_threshold, bool is_all);
std::vector<std::string> read_list(const std::string& list);
std::vector<std::string> ls_img(const std::string& dir_name);
std::pair<double, double> blur_eval(const std::string& filename, cv::Mat& lum); //returns the luminance channel and scores


#endif