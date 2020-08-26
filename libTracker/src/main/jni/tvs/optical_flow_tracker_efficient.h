/**
   OpticalFlow tracker efficient: an efficient OF tracker
   TrueVisionSolutions Face Tracker
   Copyright 2014 TrueVisionSolution Pty. Ltd.
   All rights reserved
   www.truevisionsolutions.net
*/

#ifndef __OPTICAL_FLOW_TRACKER_EFFICIENT_H__
#define __OPTICAL_FLOW_TRACKER_EFFICIENT_H__
#include <opencv2/core/core.hpp>
namespace tvs {
class OpticalFlowTrackerEfficient {
public:
  OpticalFlowTrackerEfficient() {
    _nlevels = 2;
    _itol = 5;
    _ftol = 0.01f;
    _etol = 10.0f;
    _w_border = 2.0f;
    _w_ratio = 0.075f;
    _d_ratio = 0.05f;
    this->reset();
  }
  void setLevels(const int nlevels) { _nlevels = nlevels; }
  void reset() {
    _prevPts.clear(); _pyrd1.clear(); _prevOffset = cv::Point(0, 0);
    _currPts.clear(); _pyrd2.clear(); _currOffset = cv::Point(0, 0);
    _ordered = false;
  }
  int calcFlow(const cv::Mat_<uchar> &img, cv::Mat_<float> &pts);

protected:
  int _itol;
  float _ftol, _etol;
  float _w_border, _w_ratio, _d_ratio;
  int _wsize;
  float _dthresh;
  int _nlevels;
  cv::Point _prevOffset;
  cv::Point _currOffset;
  std::vector<cv::Point2f> _prevPts;
  std::vector<cv::Point2f> _currPts;
  std::vector<cv::Mat> _pyrd1;
  std::vector<cv::Mat> _pyrd2;
  bool _ordered;

  void getPyramids(std::vector<cv::Mat> &prevPyrd,
		   std::vector<cv::Mat> &currPyrd);
  void getCurrentPyramid(std::vector<cv::Mat> &currPyrd);
  void getPreviousPyramid(std::vector<cv::Mat> &prevPyrd);
  int switchCurrForPrev();
  int setupCurrentImage(const cv::Mat_<uchar> &img, 
			const cv::Mat_<float> &pts);
  cv::Size calcSize(const cv::Mat_<float> &pts,
		    int *w_size = NULL, float *d_size = NULL);
};
}  // namespace tvs
#endif  // __OPTICAL_FLOW_TRACKER_EFFICIENT_H__
