//
// Created by uriah on 2017/11/2.
//

#include <tuple>
#include "faceDetectMgr.h"
#include "detector.h"
#include "ModelLoader.h"
#include "Bench.h"

#define TAG "FaceDetectMgr"
ulsee::FaceDetectMgr::FaceDetectMgr() {
    det = nullptr;
    loader1 = nullptr;
    loader2 = nullptr;
    loader3 = nullptr;
}

ulsee::FaceDetectMgr::~FaceDetectMgr() {
    if(det != nullptr) {
        delete det;
    }

    if(loader1 != nullptr) {
        delete loader1;
    }

    if(loader2 != nullptr) {
        delete loader2;
    }

    if(loader3 != nullptr) {
        delete loader3;
    }
}

int ulsee::FaceDetectMgr::initModel(AAssetManager * pManager) {
    std::string model1 = "model1.bin";
    std::string model2 = "model2.bin";
    std::string model3 = "model3.bin";
    loader1 = new ModelLoader();
    loader1->initAssertManager(pManager);
    loader1->getAssertData(model1);

    loader2 = new ModelLoader();
    loader2->initAssertManager(pManager);
    loader2->getAssertData(model2);

    loader3 = new ModelLoader();
    loader3->initAssertManager(pManager);
    loader3->getAssertData(model3);

    return 0;
}

int ulsee::FaceDetectMgr::initDetector(float minSize) {
    if(loader1 == nullptr || loader2 == nullptr || loader3 == nullptr){
        LOGD(TAG, "please init model first");
        return -1;
    }
    std::vector<float> thresholds{0.6, 0.7, 0.5};
    det = new detector(minSize, thresholds, 0.709, loader1->m_vecStackCache.data(),
                       loader2->m_vecStackCache.data(), loader3->m_vecStackCache.data());
    return 0;
}

int ulsee::FaceDetectMgr::detectFace(cv::Mat &image, std::vector<std::vector<float>> &rects,
                                     std::vector<std::vector<float>> &points) {

    if(nullptr == det) {
        LOGD(TAG, "detector point is nullptr");
        return -1;
    }
    std::vector<std::vector<float>> rect_tmp;
    std::vector<std::vector<float>> point_tmp;

    bench_start();
    std::tie(rect_tmp, point_tmp) = det->detect_face(image);
    bench_end("face-detector");

    rects = std::move(rect_tmp);
    points = std::move(point_tmp);

    return rects.size();

}