/**--------------------------------------------------
* ULSEE-ACF-Detector
*
* Copyright (c) 2016
* Written by Jiaolong Xu
*--------------------------------------------------*/

#ifndef _COMMON_HPP_
#define _COMMON_HPP_
#include <typeinfo>
#include <vector>
#include <iostream>
#include <fstream>
#include <string.h>

#include "ndarray.hpp"

#ifdef USE_OPENMP
#include <omp.h>
#define NUM_THREADS 4
#endif

using namespace std;

namespace ulsee {

enum ChnPadType{
	PAD_INVLD,
	PAD_REPLICATE,
	PAD_SYMMETRIC,
	PAD_CIRCULAR,
	PAD_VAL
};

enum ConvFilterType{
	CONV_BOX,
	CONV_TRI,
	CONV_TRI1,
	CONV_11,
	CONV_MAX
};

struct Rect {
    int32_t x; // left
    int32_t y; // top
    int32_t w; // width
    int32_t h; // height
};

struct Detection {
    ulsee::Rect bbox;
    double score;
};

/*
 * Feature pyramid parameters
 */
class FeatureParameters {
public:
    // lambdas
    vector<float> mLambdas = {0, 0.1671, 0.1676};
    // padding in height and width
    int mPadX = 4;
    int mPadY = 4;
    // minimum detection model size
    int mMinDsW = 72;
    int mMinDsH = 72;
    // number of layers per octave
    int mNPerOct = 8;
    // number of approx. scales
    int mNApprox = 7;
    // number of upscaled octaves
    int mNOctUp = 0;
    // pooling size
    int mShrink = 4;
    // radius for color smoothing
    int mSmooth = 1;
    
    // color channel related parameters
    int mColorSmooth = 0;

    // magnitude related parameters
    int mMagChnId = 0;
    float mMagNormConst = 0.005;
    int mMagNormRad = 5;
    int mMagIsFull = 0;
   
    // gradient historgram related parameters
    int mHistBinSize = 4;
    int mHistOrients = 6;
    int mHistSoftBin = 0;
    int mHistUseHOG = 0;
    float mHistClipHOG = 0.2f;
};

/*
 * Detector options
 */
class Options {
public:
    FeatureParameters mFeatParams;
    int mShrink  = 4;
    int mStride = 4;
    int mModelHt = 72;
    int mModelWd = 72;
    int mModelHtPad = 80;
    int mModelWdPad = 80;
    float mNMSOverlap = 0.3;
    int mMaxImageSize = -1;
};

//typedef NDArray<uint8_t> Image;
//typedef NDArray<float> ChnFeat;
typedef NDMat<uint8_t> ImageData;
typedef NDMat<float> ChnData;
//typedef vector< vector<ChnFeat> > PyramidFeat;
typedef vector< ChnData > PyramidData;
}
#endif // _COMMON_HPP_
