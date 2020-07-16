//
// Created by uriah on 2017/11/2.
//

#ifndef FV_ANDROID_CAFFE2DEMO_FACEFEATUREMGR_H
#define FV_ANDROID_CAFFE2DEMO_FACEFEATUREMGR_H

#include <opencv2/core/mat.hpp>

class recofea;

class ModelLoader;

class AAssetManager;
namespace ulsee {
    class FaceFeatureMgr {
    public:
        FaceFeatureMgr();

        ~FaceFeatureMgr();

        int initModel(AAssetManager* mgr);

        int initExtractor();

        int extractFeature(cv::Mat &image, std::vector<cv::Point2f> facial5points,
                           std::vector<float> &features);

    private:
        recofea* rec;
        ModelLoader* loader;
    };
}


#endif //FV_ANDROID_CAFFE2DEMO_FACEFEATUREMGR_H
