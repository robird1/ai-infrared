//
// Created by uriah on 2017/11/2.
//
#include <Log.h>
#include "ModelLoader.h"
#include "recofea.h"
#include "faceFeatureMgr.h"

#define TAG "FaceFeatureMgr"

ulsee::FaceFeatureMgr::FaceFeatureMgr() {
    rec = nullptr;
    loader = nullptr;
}

ulsee::FaceFeatureMgr::~FaceFeatureMgr() {
    if (nullptr != rec) {
        delete rec;
    }

    if (nullptr != loader) {
        delete loader;
    }
}

int ulsee::FaceFeatureMgr::initModel(AAssetManager* mgr) {
    std::string model = "mbv3FR-S2.4.bin";
    loader = new ModelLoader();
    loader->initAssertManager(mgr);
    loader->getAssertData(model);
    return 0;
}

int ulsee::FaceFeatureMgr::initExtractor() {

    if (nullptr == loader) {
        LOGD(TAG, "please init model first");
        return -1;
    }

    rec = new recofea(loader->m_vecStackCache.data(), 112, 112);
    return 0;

}

int ulsee::FaceFeatureMgr::extractFeature(cv::Mat &image, std::vector<cv::Point2f> facial5points,
                                          std::vector<float> &features) {
    if (nullptr == rec) {
        LOGD(TAG, "please init Extractor");
        return -1;
    }

    features = rec->extract_feature(image, facial5points);
    return 0;
}