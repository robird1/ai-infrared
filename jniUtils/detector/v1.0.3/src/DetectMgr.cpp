#include <iostream>
#include <tuple>
#include "DetectMgr.h"
#include "detector.h"
#include "Bench.h"

ulsee::FaceDetectMgr::FaceDetectMgr() {
    m_detector = nullptr;
    loader1 = nullptr;
    loader2 = nullptr;
    loader3 = nullptr;
}

ulsee::FaceDetectMgr::~FaceDetectMgr() {
    if (m_detector != nullptr) {
        delete static_cast<detector *>(m_detector);
    }
#ifdef __ANDROID__
    if (loader1 != nullptr) {
        delete loader1;
        loader1 = nullptr;
    }

    if (loader2 != nullptr) {
        delete loader2;
        loader2 = nullptr;
    }

    if (loader3 != nullptr) {
        delete loader3;
        loader3 = nullptr;
    }
#endif
}

int ulsee::FaceDetectMgr::initDetector(float minSize, const char *dir) {
    //防止多次init,造成内存泄漏
    if (m_detector != nullptr) {
        delete static_cast<detector *>(m_detector);
    }

    std::string modelDir = dir;

    std::string p_model_path = modelDir + "model1.bin";
    std::string p_param_path = "";
    std::string r_model_path = modelDir + "model2.bin";
    std::string r_param_path = "";
    std::string o_model_path = modelDir + "model3.bin";
    std::string o_param_path = "";
    std::vector<float> thresholds{0.6, 0.7, 0.5};
    float factor = 0.709;

    detector *det = new detector(minSize, thresholds, factor, p_param_path, p_model_path,
                                 r_param_path, r_model_path, o_param_path, o_model_path);

    m_detector = static_cast<void *>(det);
    return 0;
}

#ifdef __ANDROID__

int
ulsee::FaceDetectMgr::initDetector(float minSize, AAssetManager *pManager) {
    //防止多次init,造成内存泄漏
    if (m_detector != nullptr) {
        delete static_cast<detector *>(m_detector);
    }


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


//    std::string modelDir = dir;
//
//    std::string p_model_path = modelDir + "model1.bin";
//    std::string p_param_path = "";
//    std::string r_model_path = modelDir + "model2.bin";
//    std::string r_param_path = "";
//    std::string o_model_path = modelDir + "model3.bin";
//    std::string o_param_path = "";
    std::vector<float> thresholds{0.6, 0.7, 0.5};
    float factor = 0.709;

//    detector * det = new detector(minSize, thresholds, factor ,p_param_path, p_model_path, r_param_path, r_model_path, o_param_path, o_model_path );
    detector *det = new detector(minSize, thresholds, factor, loader1->m_vecStackCache.data(),
                                 loader2->m_vecStackCache.data(), loader3->m_vecStackCache.data());

    m_detector = static_cast<void *>(det);
    return 0;
}

#endif

/**
 *
 * @param img
 * @param rects
 * @param points 点的顺序:x0,x1,x2...y0,y1,y2...
 * @return
 */
int ulsee::FaceDetectMgr::detectFace(const cv::Mat &img, std::vector<std::vector<float>> &rects,
                                     std::vector<std::vector<float>> &points) {
    if (m_detector == nullptr) {
        std::cout << "please check key or initDetector" << std::endl;
        return -1;
    }
    detector *det = static_cast<detector *>(m_detector);

    bench_start();
    std::tie(rects, points) = det->detect_face(img);
    bench_end("face-detector");

    return rects.size();
}

void ulsee::FaceDetectMgr::setMinSize(float minSize) {
    if (m_detector != nullptr) {
        detector *det = static_cast<detector *>(m_detector);
        det->setMinSize(minSize);
    }
}
