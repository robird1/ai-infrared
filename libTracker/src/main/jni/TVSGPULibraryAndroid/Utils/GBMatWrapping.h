/**
   GBMatWrap: A class to wrap data from an Android Graphic buffers
   inside an OpenCV cv::Mat
   
   Copyright 2014 TrueVisionSolution Pty. Ltd.
   All rights reserved
*/
#ifndef __ANDROID_GRAPHIC_BUFFER_CV_MAT_WRAPPING_H__
#define __ANDROID_GRAPHIC_BUFFER_CV_MAT_WRAPPING_H__


#include <opencv2/core/core.hpp>
#include "Objects/AndGraphicBuffer.h"

namespace tvs {
namespace gpgpu {
typedef struct GBMatWrapping {
  GBMatWrapping() { clear(); }

  bool wrap(AndGraphicBuffer *agb, cv::Mat& matrix, const bool lockRO = false);
  bool unwrap(cv::Mat& matrix);
  
  void clear() {      
    _agb = nullptr;
    _dataptr = nullptr;
    _isLocked = false;
  }
  AndGraphicBuffer *_agb;
  bool _isLocked;
  // pointer to the data
  unsigned char *_dataptr;
} GBMatWrapping;

}  // namespcae gpgpu
}  // namespace tvs
#endif
