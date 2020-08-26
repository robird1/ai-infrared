#include <chrono>
#include <map>
#include "stich.h"

//Only support 2 scaling
Mat getPyramidStitchingImage2(Mat &input_image, std::vector<std::pair<Rect, double>> &location_and_scale,
                              double scaling, Scalar background_color, int min_side, int interval) {
    using namespace std;
    bool stitch_x = input_image.cols < input_image.rows;
    Size current_size = input_image.size();
    Point left_top = Point(0, 0);
    int width, height;
    if (stitch_x) {
		//printf("current_size[%dx%d]\n", current_size.width, current_size.height);
        width = ceil(input_image.cols * (1 + scaling)) + interval * 2;
        height = 0;
        map<int, pair<int, int>> height_index; // (width_start, width, height)
        height_index[0] = pair<int, int>(width, 0);
        do {
            int min_h = INT_MAX, min_start = 0;
            for (auto h : height_index) {
                if (h.second.first > current_size.width + interval && h.second.second < min_h) {
                    min_h = h.second.second;
                    min_start = h.first;
                }
            }
            location_and_scale.push_back(
                    make_pair(Rect(min_start, height_index[min_start].second, current_size.width, current_size.height),
                              (double) current_size.height / (double) input_image.rows));
            height_index[min_start + current_size.width + interval] = height_index[min_start];
            height_index[min_start + current_size.width + interval].first -= current_size.width + interval;
            height_index[min_start].first = current_size.width;
            height_index[min_start].second += current_size.height + interval;
            if (height_index[min_start].second > height) height = height_index[min_start].second;
            //for (auto h : height_index) {
            //  std::cout << h.first << " " << h.second.first << " " << h.second.second << std::endl;
            //}
            //std::cout << "===================="<<std::endl;
            current_size.width *= scaling;
            current_size.height *= scaling;
        } while (current_size.width > min_side);
        height += interval;
    } else {
        height = ceil(input_image.rows * (1 + scaling)) + interval * 2;
        width = 0;
        map<int, pair<int, int>> width_index; // (height_start, height, width)
        width_index[0] = pair<int, int>(height, 0);
        do {
            int min_w = INT_MAX, min_start = 0;
            for (auto w : width_index) {
                if (w.second.first > current_size.height + interval && w.second.second < min_w) {
                    min_w = w.second.second;
                    min_start = w.first;
                }
            }
            location_and_scale.push_back(
                    make_pair(Rect(width_index[min_start].second, min_start, current_size.width, current_size.height),
                              (double) current_size.width / (double) input_image.cols));
            width_index[min_start + current_size.height + interval] = width_index[min_start];
            width_index[min_start + current_size.height + interval].first -= current_size.height + interval;
            width_index[min_start].first = current_size.height;
            width_index[min_start].second += current_size.width + interval;
            if (width_index[min_start].second > width) width = width_index[min_start].second;
            //for (auto h : width_index) {
            //  std::cout << h.first << " " << h.second.first << " " << h.second.second << std::endl;
            //}
            //std::cout << "====================" << std::endl;
            current_size.width *= scaling;
            current_size.height *= scaling;
        } while (current_size.height > min_side);
        width += interval;
    }

    Mat big_image = Mat::zeros(height, width, input_image.type());
    big_image = background_color;
    Mat resized_image = input_image;
    //std::chrono::time_point<std::chrono::system_clock> t0 = std::chrono::system_clock::now();
//#pragma omp parallel for // not very fast.
    for (auto ls : location_and_scale) {
        resize(resized_image, resized_image, Size(ls.first.width, ls.first.height));
        resized_image.copyTo(big_image(ls.first));
    }
    //std::cout << "resize time:" << (float)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - t0).count() / 1000 << "ms" << std::endl;
    return big_image;
}
