/**
   GBMatWrap: A class to wrap data from an Android Graphic buffers
   inside an OpenCV cv::Mat
   
   Copyright 2014 TrueVisionSolution Pty. Ltd.
   All rights reserved
*/
#include "GBMatWrapping.h"

#include <android/log.h>
#include <opencv2/core/core.hpp>
#ifdef TVS_INTERNAL_DEBUGGING
const char kLogTag[] = "GBMatWrapping";
#define printInfo(...) {__android_log_print(ANDROID_LOG_INFO, kLogTag, \
                                            __VA_ARGS__);}
#else
const char kLogTag[] = "UlsFaceTracker";
#define printInfo(...)
#endif
#define printErr(...) __android_log_print(ANDROID_LOG_ERROR, kLogTag, \
                                          __VA_ARGS__)

namespace tvs {
namespace gpgpu {
bool GBMatWrapping::wrap(AndGraphicBuffer *agb, cv::Mat& matrix,
                         const bool lockRO) {
  if (!agb) return false;

  int mattype;
  GLenum agbtype = agb->getImageFormat();
  if (agbtype == GL_RGBA) {
    mattype = CV_8UC4;
  } else {
    printErr("Unsupported Graphic Buffer format: %x", agbtype);
    return false;
  }
  //get the pointer to the AGB

  unsigned char *ptr;
  bool ok = agb->lockBuffer(lockRO, &ptr);
  if (ok) {
    matrix = cv::Mat(agb->getHeight(), agb->getWidth(), mattype,
                     ptr, agb->getBytesPerRow());
  } else {
    matrix = cv::Mat();
    return false;
  }

  _agb = agb;
  _dataptr = ptr;
  _isLocked = true;
  return true;  
}

bool GBMatWrapping::unwrap(cv::Mat& matrix) {
  assert(_dataptr == matrix.ptr());
  if (!_isLocked) return false;
  
  _agb->unlockBuffer();
  matrix = cv::Mat();
  this->clear();
  return true;
}

}  // namespace gpgpu
}  // namespace tvs
