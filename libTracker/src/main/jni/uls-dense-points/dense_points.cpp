//#include "stdafx.h"
#include "dense_points.h"
//code written by Chris Yang chris@ulsee.com
//copyright, ULSee. Inc.
//code extended by Jethro@20160914, 0922

using namespace cv;

void CDenseptns::ulsee_rotate(const Mat &src_x, const Mat &src_y, Mat &dst_x, Mat &dst_y,float& rad, bool isback)
{
    CV_Assert(src_x.rows == src_y.rows);
    cv::Mat mag;
    cv::Mat agl;
    cv::cartToPolar(src_x, src_y, mag, agl);
    if (isback){
        cv::polarToCart(mag, agl + rad, dst_x, dst_y);
    }
    else{
        rad  = agl.at<float>(agl.rows - 1, 0);
        cv::polarToCart(mag, agl - rad, dst_x, dst_y);
    }
    
}



void CDenseptns::ulsee_polyfit(const Mat& src_x, const Mat& src_y, Mat& dst, int order)
{
    // to calculate the parameters of a 2nd order curve fitted from four points
    CV_Assert((src_x.rows>0)&&(src_y.rows>0)&&(src_x.cols==1)&&(src_y.cols==1)
              &&(dst.cols==1)&&(dst.rows==(order+1))&&(order>=1));
    Mat X;
    X = Mat::zeros(src_x.rows, order+1,CV_32FC1);
    Mat copy;
    for(int i = 0; i <=order;i++)
    {
        copy = src_x.clone();
        pow(copy,i,copy);
        Mat M1 = X.col(i);
        copy.col(0).copyTo(M1);
    }
    Mat X_t, X_inv;
    transpose(X,X_t);
    Mat temp = X_t*X;
    Mat temp2;
    invert (temp,temp2);
    Mat temp3 = temp2*X_t;
    Mat W = temp3*src_y;
    W.copyTo(dst);
}
// ----------------------------------------------------------------------------------------
std::vector<float> CDenseptns::ulsee_linspace(float a, float b, int n) {
    // get a vector of x locations given two end points and the number of points expected
    CV_Assert(n >=1);
    std::vector<float> array;
    float step = (b-a) / (n+1);
    for (int k = 0; k < n; k++) {
        a += step;
        array.push_back(a);
    }
    return array;
}
std::vector<float> CDenseptns::ulsee_addpoints(cv::Mat srcx, int n, bool end_inc){
    //CV_Assert ((srcx.rows == 4) && (n > 0));
	int t = srcx.rows - 1; // 3
    std::vector<float> v_rt;
    if (end_inc)
        v_rt.push_back(srcx.at<float>(0,0));
    for (int k = 0; k < t; k ++){
        std::vector<float> temp =  ulsee_linspace(srcx.at<float>(k, 0), srcx.at<float>(k+1, 0), n);
        if (k > 0)
            v_rt.push_back(srcx.at<float>(k,0));
        for (int i = 0; i < temp.size(); i ++)
            v_rt.push_back(temp[i]);
    }
    if (end_inc)
        v_rt.push_back(srcx.at<float>(t,0));
    return v_rt;
    
}



float *CDenseptns::ulsee_dense_nose(const float *pts2d, int &pointNum, int &denseNum)
{
	if (pointNum != 66) {
		return NULL;
	}
	int nbrCom = 2;
	int pntNbr = 5;
	//        CV_Assert(faceshape.size()==68);//ulsee tracker has 66 points
	const int num_densep = 1;//This is the number of interpolated points between two landmarks;
	const Mat eye_indexes = (Mat_<int>(nbrCom, pntNbr) << 31, 32, 33, 34, 35,
												27, 28, 29, 30, 33);// 5 indexes per arc


	std::vector< cv::Point2f> faceshape;

	cv::Point2f temppoint;
	for (int i = 0; i < pointNum; i++) {
		temppoint.x = pts2d[2 * i];
		temppoint.y = pts2d[2 * i + 1];
		faceshape.push_back(temppoint);
	}
	denseNum = 0;
	memset(m_dense_1, 0, sizeof(m_dense_1));


	std::vector<cv::Point2f> v_dense;
	Mat src_x = Mat::zeros(pntNbr, 1, CV_32F);
	Mat src_y = Mat::zeros(pntNbr, 1, CV_32F);
	for (int i = 0; i < nbrCom; i++) {
		for (int j = 0; j < pntNbr; j++) {
			src_x.at<float>(j, 0) = faceshape[eye_indexes.at<int>(i, j)].x;
			src_y.at<float>(j, 0) = faceshape[eye_indexes.at<int>(i, j)].y;
		}
		Mat dst = Mat::zeros(3, 1, CV_32FC1);
		float basex0 = src_x.at<float>(0, 0);
		float basey0 = src_y.at<float>(0, 0);
		src_x -= basex0;
		src_y -= basey0;
		float angle = 0;

		cv::Mat src_x_r;
		cv::Mat src_y_r;
		ulsee_rotate(src_x, src_y, src_x_r, src_y_r, angle, false);
		ulsee_polyfit(src_x_r, src_y_r, dst, 2);
		std::vector<float> v = ulsee_addpoints(src_x_r, num_densep, i % 2 == 0);// for upper side we will include end points, and not for lower side
		cv::Mat densex = Mat::zeros(v.size(), 1, CV_32F);

		for (int np = 0; np < v.size(); np++) {
			densex.at<float>(np, 0) = v[np];
		}
		cv::Mat tem_power2;
		cv::pow(densex, 2.0, tem_power2);
		cv::Mat densey = dst.at<float>(2, 0) * tem_power2 + dst.at<float>(1, 0) * densex + dst.at<float>(0, 0);
		cv::Mat densex_dst, densey_dst;
		ulsee_rotate(densex, densey, densex_dst, densey_dst, angle, true);
		for (int np = 0; np < densex_dst.rows; np++) {
			v_dense.push_back(cv::Point2f(densex_dst.at<float>(np, 0) + basex0, densey_dst.at<float>(np, 0) + basey0));
		}

		if (i == 0) {
			v_dense.push_back(cv::Point2f(pts2d[27 * 2], pts2d[27 * 2 + 1]));
		}

	}
	// copy to buffer 
	denseNum = v_dense.size();
	for (int i = 0; i < v_dense.size(); i++) {
		m_dense_1[2 * i] = v_dense[i].x;
		m_dense_1[2 * i + 1] = v_dense[i].y;
		//printf("%d %f %f\n", i, g_denseptns[2 * i], g_denseptns[2 * i + 1]);
	}
	//printf("nose =================== different number: %d\n", v_dense.size() - 10);
	//total number should be 12 x num_densep + 12 (12 interpolation places and 12 original landmark locations)
	//CV_Assert(v_dense.size() == 12 * (num_densep + 1));

	return m_dense_1;// v_dense;
}

float *CDenseptns::ulsee_dense_eyebrow(const float *pts2d, int &pointNum, int &denseNum)
{
	if (pointNum != 66) {
		return NULL;
	}

	//        CV_Assert(faceshape.size()==68);//ulsee tracker has 66 points
	const int num_densep = 1;//This is the number of interpolated points between two landmarks;
#if 0
	const Mat eye_indexes = (Mat_<int>(4, 4) << 17, 18, 19, 20, 		
		18, 19, 20, 21,
		22, 23, 24, 25,
		23, 24, 25, 26
		 );// 4 indexes per arc
#else
	const Mat eye_indexes = (Mat_<int>(2, 5) << 17, 18, 19, 20, 21, 22, 23, 24, 25, 26 );// 5 indexes per arc
#endif


	std::vector< cv::Point2f> faceshape;

	cv::Point2f temppoint;
	for (int i = 0; i < pointNum; i++) {
		temppoint.x = pts2d[2 * i];
		temppoint.y = pts2d[2 * i + 1];
		faceshape.push_back(temppoint);
	}
	denseNum = 0;
	memset(m_dense_2, 0, sizeof(m_dense_2));


	std::vector<cv::Point2f> v_dense;
	Mat src_x = Mat::zeros(5, 1, CV_32F);
	Mat src_y = Mat::zeros(5, 1, CV_32F);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 5; j++) {
			src_x.at<float>(j, 0) = faceshape[eye_indexes.at<int>(i, j)].x;
			src_y.at<float>(j, 0) = faceshape[eye_indexes.at<int>(i, j)].y;
		}
		Mat dst = Mat::zeros(3, 1, CV_32FC1);
		float basex0 = src_x.at<float>(0, 0);
		float basey0 = src_y.at<float>(0, 0);
		src_x -= basex0;
		src_y -= basey0;
		float angle = 0;

		cv::Mat src_x_r;
		cv::Mat src_y_r;
		ulsee_rotate(src_x, src_y, src_x_r, src_y_r, angle, false);
		ulsee_polyfit(src_x_r, src_y_r, dst, 2);
		std::vector<float> v = ulsee_addpoints(src_x_r, num_densep, i % 2 == 0);// for upper side we will include end points, and not for lower side
		cv::Mat densex = Mat::zeros(v.size(), 1, CV_32F);

		for (int np = 0; np < v.size(); np++) {
			densex.at<float>(np, 0) = v[np];
		}
		cv::Mat tem_power2;
		cv::pow(densex, 2.0, tem_power2);
		cv::Mat densey = dst.at<float>(2, 0) * tem_power2 + dst.at<float>(1, 0) * densex + dst.at<float>(0, 0);
		cv::Mat densex_dst, densey_dst;
		ulsee_rotate(densex, densey, densex_dst, densey_dst, angle, true);
		for (int np = 0; np < densex_dst.rows; np++) {
			v_dense.push_back(cv::Point2f(densex_dst.at<float>(np, 0) + basex0, densey_dst.at<float>(np, 0) + basey0));
		}
		if (i == 0) {			
			v_dense.push_back(cv::Point2f(pts2d[22 * 2], pts2d[22 * 2+1]));
		}
		if (i == 1) {
			v_dense.push_back(cv::Point2f(pts2d[26 * 2], pts2d[26 * 2 + 1]));
		}

	}
	// copy to buffer 
	denseNum = v_dense.size();
	for (int i = 0; i < v_dense.size(); i++) {
		m_dense_2[2 * i] = v_dense[i].x;
		m_dense_2[2 * i + 1] = v_dense[i].y;
		//printf("%d %f %f\n", i, g_denseptns[2 * i], g_denseptns[2 * i + 1]);
	}
	//printf("eyebrow =================== different number: %d\n", v_dense.size() - 10);

	//total number should be 12 x num_densep + 12 (12 interpolation places and 12 original landmark locations)
	//CV_Assert(v_dense.size() == 12 * (num_densep + 1));

	return m_dense_2;// v_dense;
}

float *CDenseptns::ulsee_denseeye(const float *pts2d, int &pointNum, int &denseNum)
{
	if (pointNum != 66) {
		return NULL;
	}

    //        CV_Assert(faceshape.size()==68);//ulsee tracker has 66 points
    const int num_densep  = 1;//This is the number of interpolated points between two landmarks;
    const Mat eye_indexes = (Mat_<int>(4,4) << 36, 37, 38, 39, 36, 41, 40, 39, 42, 43 , 44, 45, 42, 47, 46, 45);// 4 indexes per arc

	std::vector< cv::Point2f> faceshape;

	cv::Point2f temppoint;
	for (int i = 0; i < pointNum; i++) {
		temppoint.x = pts2d[2 * i];
		temppoint.y = pts2d[2 * i + 1];
		faceshape.push_back(temppoint);
	}
	denseNum = 0;
	memset(m_dense_3, 0, sizeof(m_dense_3));


	std::vector<cv::Point2f> v_dense;
    Mat src_x = Mat::zeros(4,1, CV_32F);
    Mat src_y = Mat::zeros(4,1, CV_32F);
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            src_x.at<float>(j, 0) = faceshape[eye_indexes.at<int>(i,j)].x;
            src_y.at<float>(j, 0) = faceshape[eye_indexes.at<int>(i,j)].y;
        }
        Mat dst = Mat::zeros(3,1,CV_32FC1);
        float basex0 = src_x.at<float>(0,0);
        float basey0 = src_y.at<float>(0,0);
        src_x -= basex0;
        src_y -= basey0;
        float angle = 0;
        
        cv::Mat src_x_r;
        cv::Mat src_y_r;
        ulsee_rotate(src_x, src_y, src_x_r, src_y_r, angle, false);
        ulsee_polyfit(src_x_r, src_y_r ,dst,2);
        std::vector<float> v = ulsee_addpoints(src_x_r , num_densep, i%2==0);// for upper side we will include end points, and not for lower side
        cv::Mat densex = Mat::zeros(v.size(), 1, CV_32F);
        
        for (int np = 0; np < v.size(); np++){
            densex.at<float>(np,0) = v[np];
        }
        cv::Mat tem_power2;
        cv::pow(densex, 2.0, tem_power2);
        cv::Mat densey = dst.at<float>(2,0) * tem_power2 +  dst.at<float>(1,0) * densex + dst.at<float>(0,0);
        cv::Mat densex_dst, densey_dst;
        ulsee_rotate(densex, densey, densex_dst, densey_dst, angle, true);
        for (int np = 0; np < densex_dst.rows; np++){
            v_dense.push_back(cv::Point2f(densex_dst.at<float>(np,0) + basex0, densey_dst.at<float>(np,0) + basey0 ));
        }
        
        
    }
	// copy to buffer 
	denseNum = v_dense.size();
	for (int i = 0; i < v_dense.size(); i++) {
		m_dense_3[2 * i] = v_dense[i].x;
		m_dense_3[2 * i + 1] = v_dense[i].y;
		//printf("%d %f %f\n", i, g_denseptns[2 * i], g_denseptns[2 * i + 1]);
	}
	//printf("===================different number: %d\n", v_dense.size() - 12);
    //total number should be 12 x num_densep + 12 (12 interpolation places and 12 original landmark locations)
    CV_Assert(v_dense.size() == 12 * (num_densep+1));
    
	return m_dense_3;// v_dense;
}

float *CDenseptns::ulsee_jawline(const float *pts2d, int &pointNum, int &denseNum)
{
	if (pointNum != 66) {
		return NULL;
	}
	int nbrCom = 4;
	int pntNbr = 5;
	//        CV_Assert(faceshape.size()==68);//ulsee tracker has 66 points
	const int num_densep = 1;//This is the number of interpolated points between two landmarks;
	const Mat eye_indexes = (Mat_<int>(nbrCom, pntNbr) << 0, 1, 2, 3, 4, 
														  4, 5, 6, 7, 8,
														  8, 9, 10, 11, 12, 
														  12, 13, 14, 15, 16 );// 5 indexes per arc

	std::vector< cv::Point2f> faceshape;

	cv::Point2f temppoint;
	for (int i = 0; i < pointNum; i++) {
		temppoint.x = pts2d[2 * i];
		temppoint.y = pts2d[2 * i + 1];
		faceshape.push_back(temppoint);
	}
	denseNum = 0;
	memset(m_dense_4, 0, sizeof(m_dense_4));


	std::vector<cv::Point2f> v_dense;
	Mat src_x = Mat::zeros(pntNbr, 1, CV_32F);
	Mat src_y = Mat::zeros(pntNbr, 1, CV_32F);
	for (int i = 0; i < nbrCom; i++) {
		for (int j = 0; j < pntNbr; j++) {
			src_x.at<float>(j, 0) = faceshape[eye_indexes.at<int>(i, j)].x;
			src_y.at<float>(j, 0) = faceshape[eye_indexes.at<int>(i, j)].y;
		}
		Mat dst = Mat::zeros(3, 1, CV_32FC1);
		float basex0 = src_x.at<float>(0, 0);
		float basey0 = src_y.at<float>(0, 0);
		src_x -= basex0;
		src_y -= basey0;
		float angle = 0;

		cv::Mat src_x_r;
		cv::Mat src_y_r;
		ulsee_rotate(src_x, src_y, src_x_r, src_y_r, angle, false);
		ulsee_polyfit(src_x_r, src_y_r, dst, 2);
		std::vector<float> v = ulsee_addpoints(src_x_r, num_densep, i % 2 == 0);// for upper side we will include end points, and not for lower side
		cv::Mat densex = Mat::zeros(v.size(), 1, CV_32F);

		for (int np = 0; np < v.size(); np++) {
			densex.at<float>(np, 0) = v[np];
		}
		cv::Mat tem_power2;
		cv::pow(densex, 2.0, tem_power2);
		cv::Mat densey = dst.at<float>(2, 0) * tem_power2 + dst.at<float>(1, 0) * densex + dst.at<float>(0, 0);
		cv::Mat densex_dst, densey_dst;
		ulsee_rotate(densex, densey, densex_dst, densey_dst, angle, true);
			
		for (int np = 0; np < (densex_dst.rows); np++) {
			v_dense.push_back(cv::Point2f(densex_dst.at<float>(np, 0) + basex0, densey_dst.at<float>(np, 0) + basey0));
		}
		if (i == (nbrCom - 1)) {
			v_dense.push_back(cv::Point2f(pts2d[16 * 2], pts2d[16 * 2 + 1]));
		
	}
	}

	// copy to buffer 
	denseNum = v_dense.size();
	for (int i = 0; i < v_dense.size(); i++) {
		m_dense_4[2 * i] = v_dense[i].x;
		m_dense_4[2 * i + 1] = v_dense[i].y;
		//printf("%d %f %f\n", i, g_denseptns[2 * i], g_denseptns[2 * i + 1]);
	}
	//printf("jawline =================== different number: %d %d\n", v_dense.size() , (v_dense.size() - 17) );
	//total number should be 12 x num_densep + 12 (12 interpolation places and 12 original landmark locations)
	//CV_Assert(v_dense.size() == 12 * (num_densep + 1));

	return m_dense_4;// v_dense;
}


float *CDenseptns::ulsee_dense_mouth_shape_1(const float *pts2d, int &pointNum, int &denseNum)
{
	if (pointNum != 66) {
		return NULL;
	}
	int nbrCom = 4;
	int pntNbr = 4;

	//        CV_Assert(faceshape.size()==68);//ulsee tracker has 66 points
	const int num_densep = 1;//This is the number of interpolated points between two landmarks;
	const Mat eye_indexes = (Mat_<int>(nbrCom, pntNbr) << 48, 49, 50, 51,
												54, 53, 52, 51,
												54, 55, 56, 57,
												48, 59, 58, 57);// 4 indexes per arc

	std::vector< cv::Point2f> faceshape;

	cv::Point2f temppoint;
	for (int i = 0; i < pointNum; i++) {
		temppoint.x = pts2d[2 * i];
		temppoint.y = pts2d[2 * i + 1];
		faceshape.push_back(temppoint);
	}
	denseNum = 0;
	memset(m_dense_5, 0, sizeof(m_dense_5));

	std::vector<cv::Point2f> v_dense;
	Mat src_x = Mat::zeros(pntNbr, 1, CV_32F);
	Mat src_y = Mat::zeros(pntNbr, 1, CV_32F);
	for (int i = 0; i < nbrCom; i++) {
		for (int j = 0; j < pntNbr; j++) {
			src_x.at<float>(j, 0) = faceshape[eye_indexes.at<int>(i, j)].x;
			src_y.at<float>(j, 0) = faceshape[eye_indexes.at<int>(i, j)].y;
		}
		Mat dst = Mat::zeros(3, 1, CV_32FC1);
		float basex0 = src_x.at<float>(0, 0);
		float basey0 = src_y.at<float>(0, 0);
		src_x -= basex0;
		src_y -= basey0;
		float angle = 0;

		cv::Mat src_x_r;
		cv::Mat src_y_r;
		ulsee_rotate(src_x, src_y, src_x_r, src_y_r, angle, false);
		ulsee_polyfit(src_x_r, src_y_r, dst, 2);
		std::vector<float> v = ulsee_addpoints(src_x_r, num_densep, i % 2 == 0);// for upper side we will include end points, and not for lower side
		cv::Mat densex = Mat::zeros(v.size(), 1, CV_32F);

		for (int np = 0; np < v.size(); np++) {
			densex.at<float>(np, 0) = v[np];
		}
		cv::Mat tem_power2;
		cv::pow(densex, 2.0, tem_power2);
		cv::Mat densey = dst.at<float>(2, 0) * tem_power2 + dst.at<float>(1, 0) * densex + dst.at<float>(0, 0);
		cv::Mat densex_dst, densey_dst;
		ulsee_rotate(densex, densey, densex_dst, densey_dst, angle, true);
		for (int np = 0; np < densex_dst.rows; np++) {
			v_dense.push_back(cv::Point2f(densex_dst.at<float>(np, 0) + basex0, densey_dst.at<float>(np, 0) + basey0));
		}

	}
	// copy to buffer 
	denseNum = v_dense.size();
	for (int i = 0; i < v_dense.size(); i++) {
		m_dense_5[2 * i] = v_dense[i].x;
		m_dense_5[2 * i + 1] = v_dense[i].y;
		//printf("%d %f %f\n", i, g_denseptns[2 * i], g_denseptns[2 * i + 1]);
	}
	//printf("mouth shape 1 =================== different number: %d %d\n", v_dense.size() , (v_dense.size() - 12) );

	//total number should be 12 x num_densep + 12 (12 interpolation places and 12 original landmark locations)
	CV_Assert(v_dense.size() == 12 * (num_densep + 1));

	return m_dense_5;// v_dense;
}

float *CDenseptns::ulsee_dense_mouth_shape_2(const float *pts2d, int &pointNum, int &denseNum)
{
	//        CV_Assert(faceshape.size()==68);//ulsee tracker has 66 points
	if (pointNum != 66) {
		return NULL;
	}
	const int num_densep = 1;//This is the number of interpolated points between two landmarks;

	int nbrCom = 2;
	int pntNbr = 5;
	const Mat eye_indexes = (Mat_<int>(nbrCom, pntNbr) << 48, 60, 61, 62, 54,
														  48, 65, 64, 63, 54);// 5 indexes per arc
	std::vector< cv::Point2f> faceshape;

	cv::Point2f temppoint;
	for (int i = 0; i < pointNum; i++) {
		temppoint.x = pts2d[2 * i];
		temppoint.y = pts2d[2 * i + 1];
		faceshape.push_back(temppoint);
	}

	denseNum = 0;
	memset(m_dense_6, 0, sizeof(m_dense_6));


	std::vector<cv::Point2f> v_dense;
	Mat src_x = Mat::zeros(pntNbr, 1, CV_32F);
	Mat src_y = Mat::zeros(pntNbr, 1, CV_32F);
	for (int i = 0; i < nbrCom; i++) {
		for (int j = 0; j < pntNbr; j++) {
			src_x.at<float>(j, 0) = faceshape[eye_indexes.at<int>(i, j)].x;
			src_y.at<float>(j, 0) = faceshape[eye_indexes.at<int>(i, j)].y;
		}
		Mat dst = Mat::zeros(3, 1, CV_32FC1);
		float basex0 = src_x.at<float>(0, 0);
		float basey0 = src_y.at<float>(0, 0);
		src_x -= basex0;
		src_y -= basey0;
		float angle = 0;

		cv::Mat src_x_r;
		cv::Mat src_y_r;
		ulsee_rotate(src_x, src_y, src_x_r, src_y_r, angle, false);
		ulsee_polyfit(src_x_r, src_y_r, dst, 2);
		std::vector<float> v = ulsee_addpoints(src_x_r, num_densep, i % 2 == 0);// for upper side we will include end points, and not for lower side
		cv::Mat densex = Mat::zeros(v.size(), 1, CV_32F);

		for (int np = 0; np < v.size(); np++) {
			densex.at<float>(np, 0) = v[np];
		}
		cv::Mat tem_power2;
		cv::pow(densex, 2.0, tem_power2);
		cv::Mat densey = dst.at<float>(2, 0) * tem_power2 + dst.at<float>(1, 0) * densex + dst.at<float>(0, 0);
		cv::Mat densex_dst, densey_dst;
		ulsee_rotate(densex, densey, densex_dst, densey_dst, angle, true);
		for (int np = 0; np < densex_dst.rows; np++) {
			v_dense.push_back(cv::Point2f(densex_dst.at<float>(np, 0) + basex0, densey_dst.at<float>(np, 0) + basey0));
		}
		if (i == 0) {
			v_dense.erase(v_dense.begin());
		}
		if (i == 1) {
			v_dense.erase(v_dense.end()-8);
		}

	}
	// copy to buffer 
	denseNum = v_dense.size();
	for (int i = 0; i < v_dense.size(); i++) {
		m_dense_6[2 * i] = v_dense[i].x;
		m_dense_6[2 * i + 1] = v_dense[i].y;
		//printf("%d %f %f\n", i, g_denseptns[2 * i], g_denseptns[2 * i + 1]);
	}
	//printf("mouth shape 2 =================== different number: %d %d\n", v_dense.size() , (v_dense.size() - 10) );
	//total number should be 12 x num_densep + 12 (12 interpolation places and 12 original landmark locations)
	//CV_Assert(v_dense.size() == 12 * (num_densep + 1));

	return m_dense_6;// v_dense;
}

const int new_order[] = {
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,
	33,34,35,36,37,38,39,40,41,
	42,43,44,45,46,47,48,49,50,
	//51,52,53,54,55,56,57,58,59, //==> 59-67
	//60,61,62,63,64,65,66,67, //51-58
	59,60,61,62,63,64,65,66,67,
	51,52,53,54,55,56,57,58,
	68,69,70,71,72,73,74,
	//75,76,77,78,79,//=>79-75
	79,78,77,76,75,
	80,81,82,83,84,85,86,
	//87,88,89,90,91,//91-87
	91,90,89,88,87,
	92,93,94,95,96,97,98,
	//99,100,101,102,103,
	103,102,101,100,99,
	104,105,106,107,108,109,110,
	//111,112,113,114,115,
	115,114,113,112,111,
	116,117,118,119,120,121,122,
	//123,124,125,126,127,128,129
	129,128,127,126,125,124,123
};
float *CDenseptns::ulsee_dense_shape(const float *pts2d, int &pointNum, int &denseNum)
{
	if (pointNum != 66) {
		return NULL;
	}

	float fbuffer[300] = { 0 };
	memset(m_denseShape, 0, sizeof(m_denseShape));
	m_denseNbr = 0;

	float *rtnpts = ulsee_jawline(pts2d, pointNum, denseNum);
	memcpy(m_denseShape, rtnpts, sizeof(float) * 2 * denseNum);
	m_denseNbr += denseNum;
	
	rtnpts = ulsee_dense_eyebrow(pts2d, pointNum, denseNum);
	memcpy(m_denseShape + m_denseNbr * 2, rtnpts, sizeof(float) * 2 * denseNum);
	m_denseNbr += denseNum;

	rtnpts = ulsee_dense_nose(pts2d, pointNum, denseNum);
	memcpy(m_denseShape + m_denseNbr * 2, rtnpts, sizeof(float) * 2 * denseNum);
	m_denseNbr += denseNum;
	
	rtnpts = ulsee_denseeye(pts2d, pointNum, denseNum);
	memcpy(m_denseShape+ m_denseNbr*2, rtnpts, sizeof(float) * 2 * denseNum);
	m_denseNbr += denseNum;

	
	rtnpts = ulsee_dense_mouth_shape_1(pts2d, pointNum, denseNum);
	memcpy(m_denseShape + m_denseNbr * 2, rtnpts, sizeof(float) * 2 * denseNum);
	m_denseNbr += denseNum;

	rtnpts = ulsee_dense_mouth_shape_2(pts2d, pointNum, denseNum);
	memcpy(m_denseShape + m_denseNbr * 2, rtnpts, sizeof(float) * 2 * denseNum);
	m_denseNbr += denseNum;

	denseNum = m_denseNbr;
	memcpy(fbuffer, m_denseShape, sizeof(m_denseShape));
	memset(m_denseShape, 0, sizeof(m_denseShape));

	for (int i = 0; i < m_denseNbr; i++) {
		m_denseShape[2 * new_order[i]    ] = fbuffer[2*i  ];
		m_denseShape[2 * new_order[i] + 1] = fbuffer[2*i+1];
	}
	return(&m_denseShape[0]);
}
