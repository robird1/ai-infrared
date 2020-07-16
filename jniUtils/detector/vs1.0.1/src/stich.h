#ifndef _STICH_
#define _STICH_

#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

//Only support 2 scaling
Mat getPyramidStitchingImage2(Mat &input_image, std::vector<std::pair<Rect, double>> &location_and_scale,
                              double scaling = 0.709, Scalar background_color = Scalar(0,0,0), int min_side = 12, int interval = 2);

#endif // !_STICH_
