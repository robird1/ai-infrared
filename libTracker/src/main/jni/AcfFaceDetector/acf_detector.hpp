/**--------------------------------------------------
* ULSEE-ACF-Detector
*
* Copyright (c) 2016
* Written by Jiaolong Xu
*--------------------------------------------------*/

#ifndef _ACF_DETECTOR_HPP_
#define _ACF_DETECTOR_HPP_

#include <vector>

#include "feature_pyramid.hpp"
#include "cascade_model.hpp"
#include "common.hpp"

using namespace std;

namespace ulsee {

class AcfDetector {
public:
    // Consructor
    AcfDetector();

    // Destructor
    ~AcfDetector();

    /* Reture true if successfully load pretrained model, false if not
     * 
     * @param modelFile: file name of the binary model
     */
    bool loadModel(const char* modelFile);

    /* Returns the detected bounding boxes and scores
     *
     * @param img: unint8_t pointer to the image data, column major order
     * @param h: height of the image
     * @param w: width of the image
     * @param d: number of channels
     */
    //vector<Detection> detect(const uint8_t* img, const int h, const int w, const int d);
    
    /* Returns the detected bounding boxes and scores
     *
     * @param img: input image
     */
    // vector<Detection> detect(const Image &img);
    vector<Detection> detect(const ImageData &img);

    /* Set cascade model paramters
     *
     * @param cascThr: cascade threshold (default: -1)
     * @param cascCal: cascade calibration threshold
     * @note keep cascThr and tune cascCal to change the recall
     */
    void setCascParams(const float cascThr, const float cascCal);

    /* Set detector options
     *
     * @param opts: options
     */
    void setOptions(const Options &opts) {
        mOpts = opts;
        mPyd.setParameters(opts.mFeatParams);
    }

    /** If current image size is larger than maxSize, rescale it to maxSize
     * @param maxSize: maximum size of image to detect
     */
    void setMaxImageSize(const int maxSize) {
        mOpts.mMaxImageSize = maxSize;
    }

    FeaturePyramid getFeaturePyramid() { return mPyd; }

private:
    // Returns the detections on the channel features
    // vector<Detection> acfDetect(const vector<ChnFeat> &chnFeat);
    vector<Detection> acfDetect(const ChnData &chnData);
    
    FeaturePyramid mPyd;
    CascadeModel mModel;
    Options mOpts;
};

}
#endif // _ACF_DETECTOR_HPP_
