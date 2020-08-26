/**--------------------------------------------------
* ULSEE-ACF-Detector
*
* Copyright (c) 2016
* Written by Jiaolong Xu
*--------------------------------------------------*/
#ifndef _FEATURE_PYRAMID_HPP_
#define _FEATURE_PYRAMID_HPP_

#include <vector>
#include "common.hpp"

using namespace std;

namespace ulsee {

class FeaturePyramid {

public:
    FeaturePyramid();
    ~FeaturePyramid();

    // get number of scales
    size_t numScales() { return mScales.size(); }

    // compute scales
    void getScales(const int h, const int w);

    // get real and approximate scale indices
    void getRealApproxScaleIndex(vector<int> &idsR,
            vector<int> &idsA, vector<int> &idsN);

    // add channel feature
    void addChannelFeat(const float* feat, int inH, int inW, int inD,
            const int smpH, const int smpW, float* pChnData);

    // add channel feature
    // void addChnFeat(const float* feat, int inH, int inW, int inD,
    //    ChnFeat &chnFeat, const int smpH, const int smpW);

    // compute channel features
    // void computeChannels(const ChnFeat &imgLUV, vector<ChnFeat> &chnFeats);

    // compute channel features
    void computeChannels(const ChnData &imgLUV, ChnData &acf);

    // compute channel features in pyramid
    // PyramidFeat chnsPyramid(const Image &img);
    
    // compute channel features in pyramid
    PyramidData chnsPyramid(const ImageData &img);

    // set parameters
    void setParameters(const FeatureParameters &params) {
        mParams = params;
    }

    // get parameters
    FeatureParameters getParameters() { return mParams; }

    // set minimum detection size
    void setMinDsWH(const int w, const int h) {
        mParams.mMinDsW = w;
        mParams.mMinDsH = h;
    }

    int PadX() { return mParams.mPadX; }
    int PadY() { return mParams.mPadY; }
public:
    // scale factors
    vector<float> mScales;
    // scale factors in height and width
    vector< vector<float> > mScalesHW;
    
private:
    FeatureParameters mParams;
};

}
#endif // _FEATURE_PYRAMID_HPP_
