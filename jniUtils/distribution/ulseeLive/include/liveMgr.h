//
// Created by uriah on 2017/11/2.
//

#ifndef FV_ANDROID_CAFFE2DEMO_LIVEMGR_H
#define FV_ANDROID_CAFFE2DEMO_LIVEMGR_H

#include <opencv2/core/mat.hpp>

class Live;

class AAssetManager;

class ModelLoader;
namespace ulsee {
    class LiveMgr {
    public:
        LiveMgr();

        ~LiveMgr();

        int initModel(AAssetManager *mgr);

        int initLive();

        float getLiveRate(cv::Mat &mat);

    private:
        Live *live;
        ModelLoader *loader;

    };
}

#endif //FV_ANDROID_CAFFE2DEMO_LIVEMGR_H
