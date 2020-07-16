/**
   OpticalFlow tracker efficient: an efficient OF tracker
   TrueVisionSolutions Face Tracker
   Copyright 2014 TrueVisionSolution Pty. Ltd.
   All rights reserved
   www.truevisionsolutions.net
*/

#include "optical_flow_tracker_efficient.h"
#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>

using cv::Mat_;
using cv::Point2f;
namespace tvs {
int OpticalFlowTrackerEfficient::calcFlow(const cv::Mat_<uchar> &img,
                                          cv::Mat_<float> &pts) {
  if (this->switchCurrForPrev() < 0) { 
    this->reset();
    return this->setupCurrentImage(img, pts);
  }
  if (this->setupCurrentImage(img, pts) < 0) { return -1; }

  std::vector<cv::Mat> prevPyrd, currPyrd;
  this->getPyramids(prevPyrd, currPyrd);
  std::vector<uchar> status;
  std::vector<float> error;
  cv::calcOpticalFlowPyrLK(prevPyrd, currPyrd, _prevPts, _currPts, 
                           status, error, cv::Size(_wsize, _wsize),
                           _nlevels - 1,
                           cv::TermCriteria(cv::TermCriteria::COUNT + 
                                            cv::TermCriteria::EPS,
                                            _itol, _ftol),
                           cv::OPTFLOW_USE_INITIAL_FLOW);
  int n = pts.rows / 2, n_success = 0;
  for (int i = 0; i < n; i++) {
    if (status[i] && (error[i] < _etol)) {
      Point2f q(pts(2 * i) - _currOffset.x, pts(2 * i + 1) - _currOffset.y);
      float v = 1.0f - cv::norm(q - _currPts[i]) / _dthresh;
      if (v > 0.0f) {
        _currPts[i] *= v; _currPts[i] += (1.0f - v) * q; 
        pts(2 * i    ) = _currPts[i].x + _currOffset.x;
        pts(2 * i + 1) = _currPts[i].y + _currOffset.y;
        ++n_success;
      } else {
        _currPts[i].x = pts(2 * i    ) - _currOffset.x;
        _currPts[i].y = pts(2 * i + 1) - _currOffset.y;
      }
    } else {
      _currPts[i].x = pts(2 * i    ) - _currOffset.x;
      _currPts[i].y = pts(2 * i + 1) - _currOffset.y;
    }
  }
  //check if scale changes by too much
  cv::Size size = this->calcSize(pts);
  if ((std::fabs(size.width  - currPyrd[0].cols) / size.width  > 0.1f) || 
      (std::fabs(size.height - currPyrd[0].rows) / size.height > 0.1f)) {
    this->reset();
    if (this->setupCurrentImage(img, pts) < 0) { return -1; }
  }
  return n_success;
}

void OpticalFlowTrackerEfficient::getPyramids( std::vector<cv::Mat> &prevPyrd,
                                               std::vector<cv::Mat> &currPyrd) {
  this->getCurrentPyramid(currPyrd);
  this->getPreviousPyramid(prevPyrd); 
  return;
}

void OpticalFlowTrackerEfficient::getCurrentPyramid(
    std::vector<cv::Mat> &currPyrd) {
  if (_ordered) {
    currPyrd.resize(_pyrd2.size());
    for (size_t i = 0; i < _pyrd2.size(); i++) {
      currPyrd[i] = _pyrd2[i];
    }
  } else {
    currPyrd.resize(_pyrd1.size());
    for (size_t i = 0; i < _pyrd1.size(); i++) {
      currPyrd[i] = _pyrd1[i];
    }
  }
  return;
}

void OpticalFlowTrackerEfficient::getPreviousPyramid(
    std::vector<cv::Mat> &prevPyrd) {
  if (_ordered) {
    prevPyrd.resize(_pyrd1.size());
    for (size_t i = 0; i < _pyrd1.size(); i++) {
      prevPyrd[i] = _pyrd1[i];
    }
  } else {
    prevPyrd.resize(_pyrd2.size());
    for (size_t i = 0; i < _pyrd2.size(); i++) {
      prevPyrd[i] = _pyrd2[i];
    }
  }
  return;
}

int OpticalFlowTrackerEfficient::switchCurrForPrev() {
  if (_currPts.size() == 0) { return -1; }
  if (_ordered) {
    if (static_cast<int>(_pyrd2.size()) != 2 * _nlevels) { return -1; }
  } else {
    if (static_cast<int>(_pyrd1.size()) != 2 * _nlevels) { return -1; }
  }
  _ordered = !_ordered;
  _prevPts = _currPts; _prevOffset = _currOffset; return 0;
}

int OpticalFlowTrackerEfficient::setupCurrentImage(const cv::Mat_<uchar> &img, 
                                                   const cv::Mat_<float> &pts) {
  const int n = pts.rows / 2;
  cv::Size size;
  if (static_cast<int>(_prevPts.size()) == n) { 
    if (_ordered) { size = _pyrd1[0].size(); }
    else          { size = _pyrd2[0].size(); }
  } else {
    size = this->calcSize(pts, &_wsize, &_dthresh);
  }
  float xmin = pts(0), xmax = pts(0), ymin = pts(1), ymax = pts(1);
  for (int i = 1; i < n; i++) {
    float x = pts(2 * i), y = pts(2 * i + 1);
    xmin = cv::min(xmin, x); ymin = cv::min(ymin, y);
    xmax = cv::max(xmax, x); ymax = cv::max(ymax, y);
  }

  _currOffset.x =
      static_cast<int>(xmin - 0.5f * (size.width  - (xmax - xmin)));
  _currOffset.y = 
      static_cast<int>(ymin - 0.5f * (size.height - (ymax - ymin)));
  if (_currOffset.x < 0) { _currOffset.x = 0; }
  if (_currOffset.y < 0) { _currOffset.y = 0; }
  if (_currOffset.x + size.width >= img.cols) { 
    _currOffset.x = img.cols - size.width - 1; 
  }
  if (_currOffset.y + size.height >= img.rows) { 
    _currOffset.y = img.rows - size.height - 1; 
  }
  //check if size is too large
  if (_currOffset.x < 0) { return -1; }
  if (_currOffset.y < 0) { return -1; }
  if (_currOffset.x + size.width  >= img.cols) { return -1; }
  if (_currOffset.y + size.height >= img.rows) { return -1; }

  if (static_cast<int>(_currPts.size()) != n) { _currPts.resize(n); }
  for (int i = 0; i < n; i++) {
    _currPts[i].x = pts(2 * i    ) - _currOffset.x;
    _currPts[i].y = pts(2 * i + 1) - _currOffset.y;
  }
  cv::Rect region(_currOffset.x, _currOffset.y, size.width, size.height);
  if (_ordered) {
    if (static_cast<int>(_pyrd2.size()) != 2 * _nlevels){ 
      _pyrd2.resize(2 * _nlevels); 
    }
    cv::buildOpticalFlowPyramid(img(region), _pyrd2, 
                                cv::Size(_wsize, _wsize), _nlevels - 1, 
                                true, cv::BORDER_REFLECT_101, 
                                cv::BORDER_CONSTANT, false);
  } else {
    if (static_cast<int>(_pyrd1.size()) != 2 * _nlevels){ 
      _pyrd1.resize(2 * _nlevels);
    }
    cv::buildOpticalFlowPyramid(img(region), _pyrd1, 
                                cv::Size(_wsize, _wsize), _nlevels - 1, 
                                true, cv::BORDER_REFLECT_101, 
                                cv::BORDER_CONSTANT, false);
  }

  return 0;
}

cv::Size OpticalFlowTrackerEfficient::calcSize(const cv::Mat_<float> &pts,
                                               int *w_size, float *d_size) {
  int n = pts.rows / 2;
  float xmin = pts(0), xmax = pts(0), ymin = pts(1), ymax = pts(1);
  for (int i = 1; i < n; i++) {
    float x = pts(2 * i), y = pts(2 * i + 1);
    xmin = cv::min(xmin, x); ymin = cv::min(ymin, y);
    xmax = cv::max(xmax, x); ymax = cv::max(ymax, y);
  }
  cv::Rect rect(xmin, ymin, xmax - xmin, ymax - ymin);
  int wsize = _w_ratio * cv::max(rect.width, rect.height);
  float dsize = _d_ratio * cv::max(rect.width, rect.height);
  if (wsize % 2 == 0) { wsize += 1; }
  wsize = cv::max(5, cv::min(wsize, 21));
  if (w_size != NULL) { *w_size = wsize; }
  if (d_size != NULL) { *d_size = dsize; }
  int w = rect.width  + 2 * _w_border * wsize; if (w % 2 != 0) { w -= 1; }
  int h = rect.height + 2 * _w_border * wsize; if (h % 2 != 0) { h -= 1; }
  return cv::Size(w, h);
}
}  // namespace tvs
