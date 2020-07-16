#include <iostream>
#include <tuple>
#include <infra-utils/uls_infra_log.h>
#include "DetectMgr.h"
#include "detector.h"
#include "Bench.h"

bool g_uls_detect_auth_ret = false;

ulsee::FaceDetectMgr::FaceDetectMgr() {
    m_detector = nullptr;
}

ulsee::FaceDetectMgr::~FaceDetectMgr() {
    if(m_detector != nullptr) {
        delete static_cast<detector *>(m_detector);
    }
}

int ulsee::FaceDetectMgr::initDetector(float minSize, const char * dir) {

    //防止多次init,造成内存泄漏
    if(m_detector != nullptr) {
        delete static_cast<detector *>(m_detector);
    }

    std::string modelDir = dir;

    std::string p_model_path = modelDir + "ulsdet1.bin";
    std::string p_param_path = "";
    std::string r_model_path = modelDir + "ulsdet2.bin";
    std::string r_param_path = "";
    std::string o_model_path = modelDir + "ulsdet3.bin";
    std::string o_param_path = "";
    std::vector<float> thresholds{0.6, 0.5, 0.7};
    float factor = 0.709;

    detector * det = new detector(minSize, thresholds, factor ,p_param_path, p_model_path, r_param_path, r_model_path, o_param_path, o_model_path );

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


    std::string model1 = "ulsdet1.bin";
    std::string model2 = "ulsdet2.bin";
    std::string model3 = "ulsdet3.bin";
    loader1 = new ModelLoader();
    loader1->initAssertManager(pManager);
    loader1->getAssertData(model1);

    loader2 = new ModelLoader();
    loader2->initAssertManager(pManager);
    loader2->getAssertData(model2);

    loader3 = new ModelLoader();
    loader3->initAssertManager(pManager);
    loader3->getAssertData(model3);

    std::vector<float> thresholds{0.6, 0.7, 0.5};
    float factor = 0.709;

//    detector * det = new detector(minSize, thresholds, factor ,p_param_path, p_model_path, r_param_path, r_model_path, o_param_path, o_model_path );
    detector *det = new detector(minSize, thresholds, factor, loader1->m_vecStackCache.data(),
                                 loader2->m_vecStackCache.data(), loader3->m_vecStackCache.data());

    m_detector = static_cast<void *>(det);
    return 0;
}

#endif

int ulsee::FaceDetectMgr::activate(const char *key) {
    std::string authGroup = "faceDetector";
    g_uls_detect_auth_ret = true; //;uls::AuthWrapper::Instance().getAuthRet(key,authGroup.c_str()) == ULS_KEY_SUCCESS;
    LOGD("activate is %d\n", g_uls_detect_auth_ret);
//    if(!g_uls_detect_auth_ret) {
//      return -1;
//    }else {
//      return 0;
//    }
    return 0;
}

/**
 *
 * @param img
 * @param rects
 * @param points 点的顺序:x0,y0,x1,y1....
 * @return
 */
int ulsee::FaceDetectMgr::detectFace(const cv::Mat &img, std::vector<std::vector<float>> & rects, std::vector<std::vector<float>> & points ) {

    if(m_detector == nullptr) {
        std::cout<<"please check initDetector"<<std::endl;
        return -1;
    }
    if(!g_uls_detect_auth_ret) {
        LOGE("please input the correct key ");
        //return -1;
    }

    detector * det = static_cast<detector *>(m_detector);

    bench_start();
    std::tie(rects, points) = det->detect_face(img);
    bench_end("face-detectorS");

    return 0;

}

void ulsee::FaceDetectMgr::setMinSize(float minSize) {
    if (m_detector != nullptr) {
        detector *det = static_cast<detector *>(m_detector);
        det->setMinSize(minSize);
    }
}