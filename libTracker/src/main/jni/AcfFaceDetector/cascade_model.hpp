/**--------------------------------------------------
* ULSEE-ACF-Detector
*
* Copyright (c) 2016
* Written by Jiaolong Xu
*--------------------------------------------------*/
#ifndef _CASCADE_MODEL_HPP_
#define _CASCADE_MODEL_HPP_

#include <vector>
#include "common.hpp"

using namespace std;

namespace ulsee {

class CascadeModel {

public:
    CascadeModel() {}
    ~CascadeModel() {}

    bool loadModel(const char* modelFile);
    
    float classify(const float* chns, const uint32_t* indices);
    
    void getChild(const float* chns, const uint32_t* indices,
            uint32_t offset, uint32_t &k0, uint32_t &k);
   
    void setCascadeThreshold(const float cascThr) { mCascThr = cascThr; }
    void setCascadeCalibration(const float cascCal) { mCascCal = cascCal; }
    float CascThr() { return mCascThr; }
    
    Options getOptions() { return mOpts; }

public:
    uint32_t mTreeDepth;
    uint32_t mNumTrees;
    uint32_t mNumNodes;
    vector<float> mThrs;
    vector<float> mHs;
    vector<uint32_t> mFids;
    vector<uint32_t> mChild;
    vector<uint32_t> mDepth;

    // options
    Options mOpts;

private:
    float mCascThr = -1.0; 
    float mCascCal = 0.05; 
};
}
#endif // _CASCADE_MODEL_HPP_
