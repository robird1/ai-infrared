#include "util.h"
#include <iostream>

std::vector<std::vector<float >>
generateBoundingBox(ncnn::Mat data_all, float scale, float t) {
    int stride = 2, cellsize = 12;
    std::vector<std::vector<float>> res;

    //Important! because of the matlab read data by column, while opencv read data by row. We transpose input first
    //and transpose output later.

    //for example, shape is {1, 4, 72, 53}
    // x is shorter than y

    //score p
    int rc = 0;
    float *p = data_all.channel(5);//score.data + score.cstep;
    for (int row = 0; row < data_all.h; row++) {
        for (int col = 0; col < data_all.w; col++) {
            float sc = *p;
            if (sc > t) {
                std::vector<float> tmp;
                tmp.push_back(static_cast<int>((stride * col + 1) / scale));
                tmp.push_back(static_cast<int>((stride * row + 1) / scale));

                tmp.push_back(static_cast<int>((stride * col + cellsize) / scale));
                tmp.push_back(static_cast<int>((stride * col + cellsize) / scale));

                tmp.push_back(sc);
                //This reg sequence maybe consist some errors, should be verified with matlab code
//                tmp.push_back(reg_blob->data_at({0, 0, y, x}));
//                tmp.push_back(reg_blob->data_at({0, 1, y, x}));
//                tmp.push_back(reg_blob->data_at({0, 2, y, x}));
//                tmp.push_back(reg_blob->data_at({0, 3, y, x}));
                tmp.push_back(data_all.channel(0)[rc]);
                tmp.push_back(data_all.channel(1)[rc]);
                tmp.push_back(data_all.channel(2)[rc]);
                tmp.push_back(data_all.channel(3)[rc]);
                res.push_back(tmp);
            }
            p++;
            rc++;
        }
    }
    return res;
}

std::vector<std::vector<float >>
generateBoundingBox(ncnn::Mat score_mat, ncnn::Mat reg_mat, float scale, float t) {
    int stride = 2, cellsize = 12;
    std::vector<std::vector<float>> res;

    //score p
    int rc = 0;
    float *p = score_mat.channel(1);//score.data + score.cstep;
    for (int row = 0; row < score_mat.h; row++) {
        for (int col = 0; col < score_mat.w; col++) {
            float sc = *p;
            if (sc > t) {
                std::vector<float> tmp;
                tmp.push_back(static_cast<int>((stride * col + 1) / scale));
                tmp.push_back(static_cast<int>((stride * row + 1) / scale));

                tmp.push_back(static_cast<int>((stride * col + cellsize) / scale));
                tmp.push_back(static_cast<int>((stride * row + cellsize) / scale));

                tmp.push_back(sc);
                tmp.push_back(reg_mat.channel(0)[rc]);
                tmp.push_back(reg_mat.channel(1)[rc]);
                tmp.push_back(reg_mat.channel(2)[rc]);
                tmp.push_back(reg_mat.channel(3)[rc]);
                res.push_back(tmp);
            }
            p++;
            rc++;
        }
    }
    return res;
}

std::vector<int> nms(std::vector<std::vector<float >> boxes, float threshold, std::string type) {
    std::vector<int> pick(boxes.size(), 0);
    if (boxes.empty()) {
        return pick;
    }

    std::vector<int> I(boxes.size());
    std::vector<int> area(boxes.size());
    int n = 0;
    std::generate(std::begin(area), std::end(area),
                  [&] {
                      return static_cast<int>(boxes[n][2] - boxes[n][0] + 1) *
                             static_cast<int>(boxes[n][3] - boxes[n++][1] + 1);
                  });
    n = 0;
    std::generate(std::begin(I), std::end(I), [&] { return n++; });
    std::sort(I.begin(), I.end(),
              [&](int i1, int i2) { return boxes[i1][4] < boxes[i2][4]; });

    int counter = 0;
    while (!I.empty()) {
        int last = I.size();
        int i = I[last - 1];
        pick[counter] = i;
        counter++;
        std::vector<int> xx1(last - 1), yy1(last - 1), xx2(last - 1), yy2(last - 1), w(last - 1), h(last - 1);
        n = 0;
        std::generate(std::begin(xx1), std::end(xx1),
                      [&] { return static_cast<int>(std::max(boxes[I[n++]][0], boxes[i][0])); });
        n = 0;
        std::generate(std::begin(yy1), std::end(yy1),
                      [&] { return static_cast<int>(std::max(boxes[I[n++]][1], boxes[i][1])); });
        n = 0;
        std::generate(std::begin(xx2), std::end(xx2),
                      [&] { return static_cast<int>(std::min(boxes[I[n++]][2], boxes[i][2])); });
        n = 0;
        std::generate(std::begin(yy2), std::end(yy2),
                      [&] { return static_cast<int>(std::min(boxes[I[n++]][3], boxes[i][3])); });

        n = 0;
        std::generate(std::begin(w), std::end(w),
                      [&] { return static_cast<int>(std::max(xx2[n] - xx1[n++] + 1, 0)); });
        n = 0;
        std::generate(std::begin(h), std::end(h),
                      [&] { return static_cast<int>(std::max(yy2[n] - yy1[n++] + 1, 0)); });
        std::vector<float> inter;
        std::vector<float> o(last - 1);
        std::transform(w.begin(), w.end(), h.begin(), std::back_inserter(inter),
                       std::multiplies<int>());
        n = 0;
        if (type == "Min") {
            std::generate(std::begin(o), std::end(o),
                          [&] { return inter[n] / std::min(area[i], area[I[n++]]); });
        } else {
            std::generate(std::begin(o), std::end(o),
                          [&] { return inter[n] / (area[i] + area[I[n]] - inter[n++]); });
        }
        std::vector<int> results;
        auto it = std::find_if(std::begin(o), std::end(o),
                               [&](float ele) { return ele <= threshold; });
        while (it != std::end(o)) {
            results.push_back(I[std::distance(std::begin(o), it)]);
            it = std::find_if(std::next(it), std::end(o),
                              [&](float ele) { return ele <= threshold; });
        }
        I = results;
    }
    std::vector<int> res(pick.begin(), pick.begin() + counter);
    return res;
}

int matlabFix(float n) {
    return (0 < n) ? std::floor(n) : std::ceil(n);
}

std::vector<std::vector<float>> rerec(std::vector<std::vector<float>> bboxA) {
    int size = bboxA.size();
    std::vector<float> h(size), w(size), l(size);
    int n = 0;
    std::generate(std::begin(h), std::end(h),
                  [&] { return bboxA[n][3] - bboxA[n++][1]; });
    n = 0;
    std::generate(std::begin(w), std::end(w),
                  [&] { return bboxA[n][2] - bboxA[n++][0]; });
    /*n = 0;
    std::generate(std::begin(l), std::end(l),
                  [&]{return std::max(h[n], w[n++]);});*/

    for (int i = 0; i < size; i++) {
        l[i] = std::max(h[i], w[i]);
        bboxA[i][0] = bboxA[i][0] + w[i] * 0.5 - l[i] * 0.5;
        bboxA[i][1] = bboxA[i][1] + h[i] * 0.5 - l[i] * 0.5;
        bboxA[i][2] = bboxA[i][0] + l[i];
        bboxA[i][3] = bboxA[i][1] + l[i];
    }
    return bboxA;
}

std::map<std::string, std::vector<int>> pad(std::vector <std::vector<float >> total_boxes, int w, int h) {
    int len = total_boxes.size();
    std::map<std::string, std::vector<int>> res;
    res["dy"] = std::vector<int>(len, 1);
    res["edy"] = std::vector<int>(len);
    res["dx"] = std::vector<int>(len, 1);
    res["edx"] = std::vector<int>(len);
    res["y"] = std::vector<int>(len);
    res["ey"] = std::vector<int>(len);
    res["x"] = std::vector<int>(len);
    res["ex"] = std::vector<int>(len);
    res["tmpw"] = std::vector<int>(len);
    res["tmph"] = std::vector<int>(len);
    int n = 0;
    std::generate(std::begin(res["tmpw"]), std::end(res["tmpw"]),
                  [&] { return total_boxes[n][2] - total_boxes[n++][0] + 1; });

    n = 0;
    std::generate(std::begin(res["tmph"]), std::end(res["tmph"]),
                  [&] { return total_boxes[n][3] - total_boxes[n++][1] + 1; });

    n = 0;
    res["edx"] = res["tmpw"];
    res["edy"] = res["tmph"];
    n = 0;
    std::generate(std::begin(res["x"]), std::end(res["x"]),
                  [&] { return total_boxes[n++][0]; });
    n = 0;
    std::generate(std::begin(res["y"]), std::end(res["y"]),
                  [&] { return total_boxes[n++][1]; });
    n = 0;
    std::generate(std::begin(res["ex"]), std::end(res["ex"]),
                  [&] { return total_boxes[n++][2]; });
    n = 0;
    std::generate(std::begin(res["ey"]), std::end(res["ey"]),
                  [&] { return total_boxes[n++][3]; });
    for (int i = 0; i < len; i++) {
        if (res["ex"][i] > w) {
            res["edx"][i] = -res["ex"][i] + w + res["tmpw"][i];
            res["ex"][i] = w;
        }
        if (res["ey"][i] > h) {
            res["edy"][i] = -res["ey"][i] + h + res["tmph"][i];
            res["ey"][i] = h;
        }
        if (res["x"][i] < 1) {
            res["dx"][i] = 2 - res["x"][i];
            res["x"][i] = 1;
        }
        if (res["y"][i] < 1) {
            res["dy"][i] = 2 - res["y"][i];
            res["y"][i] = 1;
        }
    }
    return res;

}

std::vector<std::vector < float>> bbreg(std::vector<std::vector<float>>boundingBox, std::vector<std::vector<float>> reg) {

    int size = boundingBox.size();
    std::vector<float> h(size);
    std::vector<float> w(size);

    int n = 0;
    std::generate(std::begin(h), std::end(h), [&]{
                                                 return boundingBox[n][3] - boundingBox[n++][1] + 1;
                                                }
                 );
    n = 0;
    std::generate(std::begin(w), std::end(w), [&]{
                                                 return boundingBox[n][2] - boundingBox[n++][0] + 1;
                                                }
                 );

    for(int i = 0; i<size; i++) {
        boundingBox[i][0] = boundingBox[i][0] + w[i] * reg[i][0];
        boundingBox[i][1] = boundingBox[i][1] + h[i] * reg[i][1];
        boundingBox[i][2] = boundingBox[i][2] + w[i] * reg[i][2];
        boundingBox[i][3] = boundingBox[i][3] + h[i] * reg[i][3];
    }

    return boundingBox;
}


void test_mat_value(const Mat &img) {
    auto type = img.type();
    if (type == 16) {
        std::vector<Vec3b> x;
        for (auto size : test_sizes) {
            x.push_back(img.at<Vec3b>(size[0], size[1]));
        }
        int y = 1;
    } else if (type == 0) {
        std::vector<uchar> x;
        for (auto size : test_sizes) {
            x.push_back(img.at<uchar>(size[0], size[1]));
        }
    } else if (type == 4) {
        std::vector<float> x;
        for (auto size : test_sizes) {
            x.push_back(img.at<float>(size[0], size[1]));
        }
    } else if (type == 6) {

    } else if (type == 21) {
        std::vector<Vec3f> x;
        for (auto size: test_sizes) {
            x.push_back(img.at<Vec3f>(size[0], size[1]));
        }
        int y = 1;
    }
    int y = 1;
}

void
ULsee_rigid_transform(const cv::Mat &A, const cv::Mat &B, cv::Mat &R, cv::Mat &T, float &scale) {
    assert(A.cols == B.cols);
    assert(A.rows == B.rows);
    Mat meanA, centroidA, meanB, centroidB;
    cv::reduce(A, meanA, 0, CV_REDUCE_AVG);
    cv::repeat(meanA, A.rows, 1, centroidA);
    cv::Mat AA = A - centroidA;
    cv::reduce(B, meanB, 0, CV_REDUCE_AVG);
    cv::repeat(meanB, B.rows, 1, centroidB);
    cv::Mat BB = B - centroidB;
    cv::Mat H = AA.t() * BB / A.rows;
    SVD decomp = SVD(H);
    cv::Mat S, U, V_t;
    S = decomp.w;
    U = decomp.u;
    V_t = decomp.vt;
    if (cv::determinant(U) * cv::determinant(V_t) < 0) {
        S.row(S.rows - 1) *= -1.;
        U.col(U.cols - 1) *= -1.;
    }
    R = U * V_t;
    float varP = 0;
    std::cout << A << std::endl;
    for (int i = 0; i < A.size().width; i++) {
        cv::Scalar mean, var;
        cv::meanStdDev(A.col(i), mean, var);
        varP += cv::sum(var * var)[0];
    }
    scale = 1.0 / varP * cv::sum(S)[0];
    T = meanB - meanA * (scale * R);
}


cv::Mat
cv_transform_warpper(std::vector<cv::Point2f> facialPoints, std::vector<cv::Point2f> coordPoints) {
    int len = facialPoints.size();
    float aData[len][2], bData[len][2];
    for (int i = 0; i < len; i++) {
        aData[i][0] = facialPoints[i].x;
        aData[i][1] = facialPoints[i].y;
        bData[i][0] = coordPoints[i].x;
        bData[i][1] = coordPoints[i].y;
    }
    //cv::Mat A(facialPoints), B(coordPoints);
    cv::Mat A = cv::Mat(len, 2, CV_32FC1, &aData), B = cv::Mat(len, 2, CV_32FC1, &bData);
    std::cout << "A is" << A << std::endl;
    std::cout << "B is" << B << std::endl;
    CV_Assert(A.type() == CV_32F || A.type() == CV_64F);
    cv::Mat R, T;
    float scale;
    ULsee_rigid_transform(A, B, R, T, scale);
    R = R * scale;
    std::cout << "R is" << R << std::endl;
    std::cout << "T is" << T << std::endl;
    cv::Mat tt;
    cv::repeat(T, len, 1, tt);
    cv::Mat diff = (A * R + tt) - B;
    std::cout << "diff is" << diff << std::endl;
    cv::hconcat(R.t(), T.t(), R);
    std::cout << "the result is R" << R << std::endl;
    return R;
}

