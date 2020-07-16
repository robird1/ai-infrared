//
//  GrayInputConversion.h
//  TVStrackeriOS
//
//  Created by Jesus Nuevo Chiquero on 11/09/2015.
//  Copyright (c) 2015 TrueVisionSolutions Pty Ltd. All rights reserved.
//

#ifndef TVStrackeriOS_GrayInputConversion_h
#define TVStrackeriOS_GrayInputConversion_h
namespace tvs {
class GrayInputConversion {
public:
  GrayInputConversion() {
    mutex = NULL;
    condition = NULL;
    ready = false;
    gray = NULL;
  }
  std::mutex *mutex;
  std::condition_variable *condition;
  bool ready;
  const cv::Mat_<uchar> *gray;
};
}  // namespace tvs

#endif  // TVStrackeriOS_GrayInputConversion_h
