//
// Created by kun on 16-11-1.
//
#include "util.h"

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
    //std::cout << A << std::endl;
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
    //std::cout << "A is" << A << std::endl;
    //std::cout << "B is" << B << std::endl;
    CV_Assert(A.type() == CV_32F || A.type() == CV_64F);
    cv::Mat R, T;
    float scale;
    ULsee_rigid_transform(A, B, R, T, scale);
    R = R * scale;
    //std::cout << "R is" << R << std::endl;
    //std::cout << "T is" << T << std::endl;
    cv::Mat tt;
    cv::repeat(T, len, 1, tt);
    cv::Mat diff = (A * R + tt) - B;
    //std::cout << "diff is" << diff << std::endl;
    cv::hconcat(R.t(), T.t(), R);
    //std::cout << "the result is R" << R << std::endl;
    return R;
}
