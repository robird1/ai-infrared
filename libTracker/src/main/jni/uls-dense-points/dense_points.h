#ifndef DENSE_POINTS_H
#define DENSE_POINTS_H

#include <vector>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>

using namespace cv;

class CDenseptns
{
public:
	CDenseptns() {};
	~CDenseptns() {};

	float *ulsee_dense_shape(const float *pts2d, int &pointNum, int &denseNum);
	int get_denseNbr() {
		return m_denseNbr;
	};
	float *ulsee_denseeye(const float *pts2d, int &pointNum, int &denseNum);
	float *ulsee_dense_mouth_shape_1(const float *pts2d, int &pointNum, int &denseNum);
	float *ulsee_dense_mouth_shape_2(const float *pts2d, int &pointNum, int &denseNum);
	float *ulsee_jawline(const float *pts2d, int &pointNum, int &denseNum);
	float *ulsee_dense_eyebrow(const float *pts2d, int &pointNum, int &denseNum);
	float *ulsee_dense_nose(const float *pts2d, int &pointNum, int &denseNum);
private:
	void ulsee_rotate(const Mat &src_x, const Mat &src_y, Mat &dst_x, Mat &dst_y, float& rad, bool isback);
	void ulsee_polyfit(const Mat& src_x, const Mat& src_y, Mat& dst, int order);
	std::vector<float> ulsee_linspace(float a, float b, int n);
	std::vector<float> ulsee_addpoints(cv::Mat srcx, int n, bool end_inc);

	//float g_denseptns[200];
	float m_dense_1[200], m_dense_2[200], m_dense_3[200], m_dense_4[200];
	float m_dense_5[200], m_dense_6[200], m_denseShape[300];
	int m_denseNbr;
};
#endif