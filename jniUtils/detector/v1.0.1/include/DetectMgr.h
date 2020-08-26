#ifndef PROJECT_DETECTMGR_H
#define PROJECT_DETECTMGR_H


#include <vector>
#include <opencv/cxcore.h>
#include "infra-utils/uls_infra_utils.h"
#include "ModelLoader.h"

#ifdef __ANDROID__
class AAssetManager;
#endif

namespace ulsee{
class DLL_PUBLIC FaceDetectMgr {
public:
    FaceDetectMgr();
    ~FaceDetectMgr();

    /**
     * initialization
     * @param minSize   [in] Minimum face size of detection
     * @param dir       [in] directory of model
     * @return  <0 : error
     */
    int initDetector(float minSize, const char * dir);

#ifdef __ANDROID__
    int initDetector(float minSize, AAssetManager * assetManager, const char *szModelDir = "");
    void usePfl(bool use) { m_bUsePfl = use; };
#endif

    /**
     * detect face
     * @param img      [in]    image in BGR format
     * @param rects    [out]   rect of face;
     * @param points   [out]   five point of face.(eye,nose,mouth).please read demo.
     * @return <0:error
     */
    int detectFace(const cv::Mat &img, std::vector<std::vector<float>> & rects, std::vector<std::vector<float>> & points);

private:
    void * m_detector;
#ifdef __ANDROID__
    ModelLoader * loader1;
    ModelLoader * loader2;
    ModelLoader * loader3;

    bool m_bUsePfl;
    void * m_detectorPfl;
#endif
};
}

#endif //PROJECT_DETECTMGR_H
