//
// Created by uriah on 2017/11/2.
//

#ifndef FV_ANDROID_CAFFE2DEMO_FACEDETECTMGR_H
#define FV_ANDROID_CAFFE2DEMO_FACEDETECTMGR_H

#include <opencv2/core/mat.hpp>
class detector;
class ModelLoader;
class AAssetManager;
namespace ulsee {
    class FaceDetectMgr {
    public:
        FaceDetectMgr();

        ~FaceDetectMgr();

        int initModel(AAssetManager *pManager);
        int initDetector(float minSize);

        int detectFace(cv::Mat &image, std::vector<std::vector<float>> &rects,
                       std::vector<std::vector<float>> &points);

    private:
        detector * det;
        ModelLoader * loader1;
        ModelLoader * loader2;
        ModelLoader * loader3;
    };
}


#endif //FV_ANDROID_CAFFE2DEMO_FACEDETECTMGR_H
