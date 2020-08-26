#ifndef PROJECT_DETECTMGR_H
#define PROJECT_DETECTMGR_H


#include <vector>
#include <opencv/cxcore.h>
#include "infra-utils/uls_infra_utils.h"
#include <android/asset_manager.h>
#include <ModelLoader.h>

namespace ulsee{
class DLL_PUBLIC FaceDetectMgr {
public:


    FaceDetectMgr();

    ~FaceDetectMgr();

    /**
     * initialization
     * @param minSize   [in] Minimum face size of detection
     * @param dir       [in] directory of model
     * @param licensePath [in] license file ,default is dir/license.txt
     * @return  <0 : error
     */
    int initDetector(float minSize, const char * dir);

#ifdef __ANDROID__

        int initDetector(float minSize, AAssetManager *assetManager);
#endif

    int activate(const char * key);

    /**
     * detect face
     * @param img      [in]    image in BGR format
     * @param rects    [out]   rect of face;
     * @param points   [out]   five point of face.(eye,nose,mouth).please read demo. 点的顺序:x0,y0,x1,y1....
     * @return <0:error
     */
    int detectFace(const cv::Mat &img, std::vector<std::vector<float>> & rects, std::vector<std::vector<float>> & points);

    void setMinSize(float minSize);

private:
    void * m_detector;
    std::string m_key;

#ifdef __ANDROID__
        ModelLoader *loader1;
        ModelLoader *loader2;
        ModelLoader *loader3;
#endif
};
}

#endif //PROJECT_DETECTMGR_H
