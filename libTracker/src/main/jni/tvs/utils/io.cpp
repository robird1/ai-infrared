/**
   tvsTracker: TrueVisionSolutions Face Tracker Interface
   Copyright 2014 TrueVisionSolutions Pty. Ltd.
   All rights reserved
   www.truevisionsolutions.net
*/
#include "tvs/utils/io.h"

#include <stdio.h>
#include <fstream>
#include <arm_neon.h>
using cv::Mat_;
using cv::Mat;

namespace tvs {
namespace io {
//=============================================================================
template<class T>
int writeMat(const Mat_<T> &X, FILE* f) {
  int r = X.rows;
  if (fwrite(&r, sizeof(int), 1, f) != 1) { return -1; }
  int c = X.cols;
  if (fwrite(&c, sizeof(int), 1, f) != 1) { return -1; }
  if ((int)fwrite((T*)X.data, sizeof(int), r*c, f) != r*c) { return -1; }
  return 0;
}
//=============================================================================
template<class T>
int readMat(Mat_<T> &X, FILE* f) {
  int r; if (fread(&r, sizeof(int), 1, f) != 1) { return -1; }
  int c; if (fread(&c, sizeof(int), 1, f) != 1) { return -1; }
  X.create(r, c);
  if ((int)fread((T*)X.data, sizeof(T), r*c, f) != r*c) { return -1; }
  return 0;
}
//=============================================================================
template int writeMat<uchar>(const Mat_<uchar>&, FILE* f);
template int writeMat<int>(const Mat_<int>&, FILE* f);
template int writeMat<float>(const Mat_<float>&, FILE* f);
template int writeMat<double>(const Mat_<double>&, FILE* f);
template int readMat<uchar>(Mat_<uchar>&, FILE* f);
template int readMat<int>(Mat_<int>&, FILE* f);
template int readMat<float>(Mat_<float>&, FILE* f);
template int readMat<double>(Mat_<double>&, FILE* f);

#ifdef ULS_USE_NEON
template int writeMat<float16_t >(const Mat_<float16_t >&, FILE * f);
template int readMat<float16_t >(Mat_<float16_t >&, FILE* f);
#endif

//=============================================================================
}  // namespace io
}  // namespace tvs
