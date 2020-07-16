#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include "blur_evaluation.h"
using namespace std;
template <typename T>
std::string to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}
std::pair<double, double> blur_eval(cv::Mat img){
    cv::Mat img_resized, luminance, luminance_lap, blur_hor, blur_ver;
    // preprocess
    cv::resize(img, img_resized, cv::Size(120, 144), 0, 0, cv::INTER_LANCZOS4);
    img_resized.convertTo(img_resized, CV_64F);
    // get luminanace channel
    luminance = get_luminance_channel(img_resized);
    // get laplacian std
    double lap_val = get_blur_laplacian(luminance, luminance_lap);
    // apply blur horizontally and vertically to the luminanace channel
    get_blur(luminance, blur_hor, blur_ver);
    luminance.convertTo(luminance, CV_8U);
    // compute ssim score
    double ssim_val = getMSSIM(luminance, blur_hor, blur_ver);
    return make_pair(ssim_val, lap_val);
}


std::pair<double, double> blur_eval(const std::string& filename, cv::Mat& lum){
    cv::Mat img, img_resized, luminance, luminance_lap, blur_hor, blur_ver;
    img = cv::imread(filename);
    // preprocess
    cv::resize(img, img_resized, cv::Size(120, 144), 0, 0, cv::INTER_LANCZOS4);
    img_resized.convertTo(img_resized, CV_64F);
    // get luminanace channel
    luminance = get_luminance_channel(img_resized);
    // get laplacian std
    double lap_val = get_blur_laplacian(luminance, luminance_lap);
    lum = luminance;
    // apply blur horizontally and vertically to the luminanace channel
    get_blur(luminance, blur_hor, blur_ver);
    luminance.convertTo(luminance, CV_8U);
    // compute ssim score
    double ssim_val = getMSSIM(luminance, blur_hor, blur_ver);
    return make_pair(ssim_val, lap_val);
}


cv::Mat get_luminance_channel(const cv::Mat& src){
    // Get luminance (Y) channel in the YCbCr mode, where
    // Y = 16 + 0.183*R + 0.614*G + 0.062*B
    // This formula is the same as the one used in MATLAB fucntion: rgb2ycbcr(), 
    // but differs from the one in OpenCV function: cvtColor(img, img, CV_BGR2YCrCb)
    cv::Mat luminance;
    cv::Mat bgr[3];
    cv::split(src, bgr);
    luminance  = 16.0 + 0.062*bgr[0]+ 0.614*bgr[1] + 0.183*bgr[2];
    return luminance;
}


double get_blur_laplacian(const cv::Mat& luminance, cv::Mat& luminance_lap){
    // compute the laplacian filter 
    double lap_f[9] = {0, 1, 0, 1, -4, 1, 0, 1, 0};
    cv::Mat kern_lap = cv::Mat(3, 3, CV_64F, lap_f);
    cv::filter2D(luminance, luminance_lap, luminance.depth(), kern_lap, cv::Point(-1, -1), 0, cv::BORDER_REFLECT);
    double std = compute_std(luminance_lap);
    return std;
}


double compute_std(const cv::Mat& src){
    // compute standard deviation for a 2D cv::Mat
    cv::Mat src_flat = src.reshape(1, src.rows * src.cols);
    double mean = cv::mean(src_flat)[0];
    double var =  (cv::sum((src_flat - mean).mul(src_flat - mean))[0]) / (src_flat.rows-1);
    return sqrt(var);
}


void get_blur(const cv::Mat& luminance, cv::Mat& blur_hor, cv::Mat& blur_ver){
    // create horizontal and vertical linear kernels
    cv::Mat kern_hor = cv::Mat::ones(1, 9, CV_64F);
    cv::Mat kern_ver = cv::Mat::ones(9, 1, CV_64F);
    kern_hor /= 9;
    kern_ver /= 9;
    // filter luminance channel by the kernels created above
    cv::filter2D(luminance, blur_hor, luminance.depth(), kern_hor, cv::Point(-1, -1), 0, cv::BORDER_REFLECT);
    cv::filter2D(luminance, blur_ver, luminance.depth(), kern_ver, cv::Point(-1, -1), 0, cv::BORDER_REFLECT);
    return;
}


double getMSSIM( const cv::Mat& i1, const cv::Mat& i2, const cv::Mat& i3){
    // This function is modified from: https://docs.opencv.org/2.4/doc/tutorials/gpu/gpu-basics-similarity/gpu-basics-similarity.html
    const double C1 = pow(255.0*0.01, 2.0);
    const double C2 = pow(255.0*0.03, 2.0);
    const int h1 = 5; // h1 = kernel_size/2, kernel_size is 11*11
    const int h2 = i1.size().height - h1; // h2 = image.height - h1
    const int w1 = 5; // w1 = kernel_size/2
    const int w2 = i1.size().width - w1; // w2 = image.width -w1

    /***************************** INITS **********************************/
    int d = CV_64F;
    cv::Mat I1, I2, I3;
    i1.convertTo(I1, d);           // cannot calculate on one byte large values
    i2.convertTo(I2, d);
    i3.convertTo(I3, d);
    cv::Mat I1_2 = I1.mul(I1);        // I1^2
    cv::Mat I2_2 = I2.mul(I2);        // I2^2
    cv::Mat I3_2 = I3.mul(I3);          // I3^2
    cv::Mat I1_I2 = I1.mul(I2);        // I1 * I2
    cv::Mat I1_I3 = I1.mul(I3);          // I1 * I3

    /*************************** END INITS **********************************/
    cv::Mat mu1, mu2, mu3;   // PRELIMINARY COMPUTING
    cv::GaussianBlur(I1, mu1, cv::Size(11, 11), 1.5);
    cv::GaussianBlur(I2, mu2, cv::Size(11, 11), 1.5);
    cv::GaussianBlur(I3, mu3, cv::Size(11, 11), 1.5);
    mu1 = mu1(cv::Range(h1, h2), cv::Range(w1, w2)); // crop to achieve "valid" convolution (since cv::GaussianBlur adds border to the image before conv)
    mu2 = mu2(cv::Range(h1, h2), cv::Range(w1, w2)); // crop to achieve "valid" convolution (since cv::GaussianBlur adds border to the image before conv)
    mu3 = mu3(cv::Range(h1, h2), cv::Range(w1, w2)); // crop to achieve "valid" convolution (since cv::GaussianBlur adds border to the image before conv)

    cv::Mat mu1_2   =   mu1.mul(mu1);
    cv::Mat mu2_2   =   mu2.mul(mu2);
    cv::Mat mu3_2   =   mu3.mul(mu3);
    cv::Mat mu1_mu2 =   mu1.mul(mu2);
    cv::Mat mu1_mu3 =   mu1.mul(mu3);

    cv::Mat sigma1_2, sigma2_2, sigma3_2, sigma12, sigma13;

    cv::GaussianBlur(I1_2, sigma1_2, cv::Size(11, 11), 1.5);
    sigma1_2 = sigma1_2(cv::Range(h1, h2), cv::Range(w1, w2)); // crop to achieve "valid" convolution (since cv::GaussianBlur adds border to the image before conv)
    sigma1_2 -= mu1_2;

    cv::GaussianBlur(I2_2, sigma2_2, cv::Size(11, 11), 1.5);
    sigma2_2 = sigma2_2(cv::Range(h1, h2), cv::Range(w1, w2)); // crop to achieve "valid" convolution (since cv::GaussianBlur adds border to the image before conv)
    sigma2_2 -= mu2_2;

    cv::GaussianBlur(I3_2, sigma3_2, cv::Size(11, 11), 1.5);
    sigma3_2 = sigma3_2(cv::Range(h1, h2), cv::Range(w1, w2)); // crop to achieve "valid" convolution (since cv::GaussianBlur adds border to the image before conv)
    sigma3_2 -= mu3_2;

    cv::GaussianBlur(I1_I2, sigma12, cv::Size(11, 11), 1.5);
    sigma12 = sigma12(cv::Range(h1, h2), cv::Range(w1, w2)); // crop to achieve "valid" convolution (since cv::GaussianBlur adds border to the image before conv)
    sigma12 -= mu1_mu2;

    cv::GaussianBlur(I1_I3, sigma13, cv::Size(11, 11), 1.5);
    sigma13 = sigma13(cv::Range(h1, h2), cv::Range(w1, w2)); // crop to achieve "valid" convolution (since cv::GaussianBlur adds border to the image before conv)
    sigma13 -= mu1_mu3;

    ///////////////////////////////// FORMULA  for image 1 & 2 ////////////////////////////////
    cv::Mat t1, t2, t3;
    t1 = 2 * mu1_mu2 + C1;
    t2 = 2 * sigma12 + C2;
    t3 = t1.mul(t2);              // t3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))

    t1 = mu1_2 + mu2_2 + C1;
    t2 = sigma1_2 + sigma2_2 + C2;
    t1 = t1.mul(t2);               // t1 =((mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2))
    cv::Mat ssim_map1;
    cv::divide(t3, t1, ssim_map1);      // ssim_map =  t3./t1;
    cv::Scalar mssim1 = mean( ssim_map1 ); // mssim = average of ssim map

    ///////////////////////////////// FORMULA  for image 1 & 3 ////////////////////////////////

    cv::Mat t4, t5, t6;
    t4 = 2 * mu1_mu3 + C1;
    t5 = 2 * sigma13 + C2;
    t6 = t4.mul(t5);

    t4 = mu1_2 + mu3_2 + C1;
    t5 = sigma1_2 + sigma3_2 + C2;
    t4 = t4.mul(t5);
    cv::Mat ssim_map2;
    cv::divide(t6, t4, ssim_map2);
    cv::Scalar mssim2 = mean( ssim_map2);

    return std::max(mssim1[0], mssim2[0]);
}


std::vector<std::string> ls_img(const std::string& dir_name){
    // find all .jpg and .png files in the directory
    std::vector<std::string> files;
    DIR* dirp = opendir(dir_name.c_str());
    struct dirent* dp;

    while((dp=readdir(dirp))  != NULL){
        std::string file = dp->d_name;
        size_t ext_pos = file.find_last_of('.');
        if(ext_pos != std::string::npos){
            std::string ext = file.substr(ext_pos, file.length());
            if(ext == ".png" || ext == "jpg"){
                files.push_back(file);
            }
        }
    }
    closedir(dirp);
    return files;
}


void add_to_bg(const cv::Mat& img_, const std::pair<double, double>& scores, const std::string& filename, const double& ssim_threshold, const double& lap_threshold, bool is_all){
    // function to display 40 images and their ssim+lap scores in one plot
    static int space_hor = 50;
    static int space_ver = 100;
    static int num_row = 4;
    static int num_col = 10;
    static int num_img = num_row * num_col;
    static int img_width = img_.size().width;
    static int img_height = img_.size().height;
    static int bg_width = space_hor*(num_col+1) + img_width*num_col;
    static int bg_height = space_ver*(num_row+1) + img_height*num_row;
    static int img_counter = 0;
    static cv::Scalar green(0, 255, 0);
    static cv::Scalar red(0, 0, 255);
    static cv::Mat background(cv::Size(bg_width, bg_height), CV_8UC3, cv::Scalar::all(200.0));

    cv::Mat img;
    int tl_y = space_ver + (space_ver + img_height) * ( ((img_counter) % num_img) / num_col); //top left x 
    int tl_x = space_hor + (space_hor + img_width) * ( ((img_counter) % num_img) % num_col);  //top right
    double ssim = scores.first;
    double lap = scores.second;

    img_.convertTo(img, CV_8U);
    cv::cvtColor(img, img, cv::COLOR_GRAY2BGR);
    cv::Rect ROI(cv::Point(tl_x, tl_y), img.size());

    if (lap<lap_threshold){
        cv::putText(background, "lap: " + to_string(lap), cv::Point(tl_x, tl_y-40), cv::FONT_HERSHEY_DUPLEX, 0.7, red, 2);
    }else{
        cv::putText(background, "lap: " + to_string(lap), cv::Point(tl_x, tl_y-40), cv::FONT_HERSHEY_DUPLEX, 0.7, green, 2);
    }
    if (ssim>ssim_threshold){
        cv::putText(background, "sim: " + to_string(ssim), cv::Point(tl_x, tl_y-12), cv::FONT_HERSHEY_DUPLEX, 0.7, red, 2);
    }else{
        cv::putText(background, "sim: " + to_string(ssim), cv::Point(tl_x, tl_y-12), cv::FONT_HERSHEY_DUPLEX, 0.7, green, 2);
    }

    img.copyTo(background(ROI));
    if((img_counter+1) % num_img == 0 || is_all){
        cv::namedWindow("mywindow", 0);
        cv::imshow("mywindow", background);
        cv::waitKey();
        background.setTo(cv::Scalar::all(200.0));
    }
    img_counter++;
    return;
}


void show_result(const std::string& img_dir, const std::vector<std::string>& img_files, const double& ssim_threshold, const double& lap_threshold){
    // show blur eval results (ssim and lap scores) for all images in img_dir. 
    for(size_t i=0; i<img_files.size(); i++){
        cv::Mat luminance;
        pair<double, double> scores = blur_eval(img_dir+img_files[i], luminance);

        if (i<img_files.size()-1){
            add_to_bg(luminance, scores, img_files[i], ssim_threshold, lap_threshold, false);
        }else{
            add_to_bg(luminance, scores, img_files[i], ssim_threshold, lap_threshold, true);
        }
    }
    return;
}


double l2_norm(std::vector<double> vec){
    double sum = 0;
    for (size_t i= 0; i < vec.size(); i++){
        sum += vec[i] * vec[i];
    }
    return sqrt(sum);
}