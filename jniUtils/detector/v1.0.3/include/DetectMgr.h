#ifndef PROJECT_DETECTMGR_H
#define PROJECT_DETECTMGR_H


#include <vector>
#include <opencv/cxcore.h>
#include <android/asset_manager.h>
#include <ModelLoader.h>
#include "uls_infra_utils.h"

namespace ulsee {
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
        int initDetector(float minSize, const char *dir);
        void setMinSize(float minSize);

#ifdef __ANDROID__

        int initDetector(float minSize, AAssetManager *assetManager);
#endif

        /**
         * detect face
         * @param img      [in]    image in BGR format
         * @param rects    [out]   rect of face;
         * @param points   [out]   five point of face.(eye,nose,mouth).please read demo. 点的顺序:x0,x1,x2...y0,y1,y2...
         * @return <0:error
         */
        int detectFace(const cv::Mat &img, std::vector<std::vector<float>> &rects,
                       std::vector<std::vector<float>> &points);

    private:
        void *m_detector;
#ifdef __ANDROID__
        ModelLoader *loader1;
        ModelLoader *loader2;
        ModelLoader *loader3;
#endif
    };
}

#endif //PROJECT_DETECTMGR_H
