/**
   tvsTracker: TrueVisionSolutions Face Tracker Interface
   Copyright 2014 TrueVisionSolutions Pty. Ltd.
   All rights reserved
   www.truevisionsolutions.net
*/

#ifndef __IO_H__
#define __IO_H__

#include <opencv2/core/core.hpp>

namespace tvs {
namespace io {
//=============================================================================
template<class T>
int writeMat(const cv::Mat_<T> &X, FILE* f);
//=============================================================================
template<class T>
int readMat(cv::Mat_<T> &X, FILE* f);
//=============================================================================
}  // namespace io
}  // namespace tvs

#endif  // __IO_H__
