#include "edgedetect.h"
#include <opencv2\highgui.hpp>
#include "color.h"
#include"filters.h"

using namespace cv;
int sobelX[3][3] = { {-1,0,1},{ -2,0,2 },{ -1,0,1 } };
int sobelY[3][3] = { { 1,2,1 },{ 0,0,0},{ -1,-2,-1 } };
int sobel_scale = 4;
int laplace[3][3] = { { 0,1,0 },{ 1,-4,1 },{ 0,1,0 } };

void EdgeDetect(cv::Mat & src, cv::Mat& dst, type_edge type) {
	cv::Mat src_gray;
	dst.create(src.size(), CV_8UC1);
	if (src.type() == CV_8UC3) {
		convertCorlor(src, src_gray);
	}
	else {
		src_gray = src;
	}
	
	if (type == SOBEL) {
		cv::Mat gradient(src.size(), CV_32FC1);
		for (int row = 0; row < src.size().height; row++) {
			for (int col = 0; col < src.size().width; col++)
			{
				int gx=0,gy=0;
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						if ((row + i - 1) < src.size().height
							&& (row + i - 1) >= 0
							&& (col + j - 1) >= 0
							&& (col + j - 1) < src.size().width) {
							gx += sobelX[i][j] * src_gray.ptr(row - 1 + i)[col - 1 + j];
						}
					}
				}
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						if ((row + i - 1) < src.size().height
							&& (row + i - 1) >= 0
							&& (col + j - 1) >= 0
							&& (col + j - 1) < src.size().width) {
							gy += sobelY[i][j] * src_gray.ptr(row - 1 + i)[col - 1 + j];
						}
					}
				}
				float g = sqrt(gx*gx + gy*gy);
				gradient.ptr(row)[col] = g;
			}
		}
		float average=sobel_scale*mean(dst)[0];
		
		for (int row = 0; row < src.size().height; row++) {
			for (int col = 0; col < src.size().width; col++)
			{
				if (gradient.ptr(row)[col] > average) {
					dst.ptr(row) [col] = 255;
				}
				else {
					dst.ptr(row)[col] = 0;
				}
			}
		}
	}
	
	else if (type == LAPLACE) {
		cv::Mat temp(src_gray.size(), src_gray.type());
		GausFilter(src_gray, temp,0.3);
		imshow("??", temp);
		waitKey(1000);
		for (int row = 0; row < src.size().height; row++) {
			for (int col = 0; col < src.size().width; col++)
			{
				int pixel = 0;
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						if ((row + i - 1) < src.size().height
							&& (row + i - 1) >= 0
							&& (col + j - 1) >= 0
							&& (col + j - 1) < src.size().width) {
							pixel += laplace[i][j] * temp.ptr(row - 1 + i)[col - 1 + j];
						}
					}
				}
				if (pixel == 0) {
					dst.ptr(row)[col] = 255;
				}
				else {
					dst.ptr(row)[col] = 0;
				}

			}
		}
		imshow("?", dst);
		waitKey(1000);
	}
	else if (type == CANNY) {
		cv::Mat temp(src_gray.size(), src_gray.type());
		GausFilter(src_gray, temp, 0.5);

		cv::Mat gradient(src.size(), CV_8UC1);
		cv::Mat theta(src.size(), CV_16SC1);
		imshow("高斯滤镜", temp);
		waitKey(1000);
		for (int row = 0; row < src.size().height; row++) {
			for (int col = 0; col < src.size().width; col++)
			{
				int gx = 0, gy = 0;
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						if ((row + i - 1) < src.size().height
							&& (row + i - 1) >= 0
							&& (col + j - 1) >= 0
							&& (col + j - 1) < src.size().width) {
							gx += sobelX[i][j] * temp.ptr(row - 1 + i)[col - 1 + j];
						}
					}
				}
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						if ((row + i - 1) < src.size().height
							&& (row + i - 1) >= 0
							&& (col + j - 1) >= 0
							&& (col + j - 1) < src.size().width) {
							gy += sobelY[i][j] * temp.ptr(row - 1 + i)[col - 1 + j];
						}
					}
				}
				uchar g = sqrt(gx*gx + gy*gy);
				gradient.ptr(row)[col] = g;
				float t =atan2(gy, gx);
				theta.ptr(row)[col] =floor(( t / PI) * 180);
			}
		}

		imshow("梯度", gradient);
		waitKey(1000);
		//非极大值抑制
		for (int row = 0; row < src.size().height; row++) {
			for (int col = 0; col < src.size().width; col++)
			{
				int t = theta.ptr(row)[col];
				float g = gradient.ptr(row)[col];
				if ((t < 22.5 &&t >= -22.5) ||(t>=157.5||t<=-157.5)) {
					if (col<( src.size().width -1) &&(gradient.ptr(row)[col + 1] <= g) && (col>1 &&gradient.ptr(row)[col - 1] <= g)) {
						dst.ptr(row)[col] = 255;
					}
					
				}
				else if ((t >= 22.5&&t < 67.5)||(t<-112.5&&t>=-57.5)) {
					if (col<(src.size().width - 1) &&(row<(src.size().height - 1))&& (gradient.ptr(row+1)[col + 1] <= g) && (col>1 && row>1 && gradient.ptr(row-1)[col - 1] <= g)) {
						dst.ptr(row)[col] = 255;
					}
					
				}
				else if ((t >= 67.5 &&t < 112.5)||(t>=-112.5&&t<-67.5)) {
					if (row<(src.size().height - 1) && (gradient.ptr(row+1)[col ] <= g) && (row>1 && gradient.ptr(row-1)[col] <= g)) {
						dst.ptr(row)[col] = 255;
					}
					
				}
				else if ((t >= 112.5&&t < 157.5)||(t>=-67.5&&t<-22.5)) {
					if ((row>1 && col<(src.size().width - 1) && (gradient.ptr(row-1)[col + 1] <= g) )&&( (col>1 && row<(src.size().height - 1 )&&gradient.ptr(row+1)[col - 1] <= g)) ){
						dst.ptr(row)[col] = 255;
					}
					
				}
			}
		}
		imshow("最大值抑制之后", dst);
		waitKey(1000);
		uchar thresholdHigh = 200;//????????????????
		int num = 0;
		vector<int> counts(256, 0);
		histogramGray(src_gray, counts);
		int h = 0;
		while (num <= src.size().width*src.size().height*0.7) {
			num += counts[h];
			h++;
		}
		thresholdHigh = h-1;
		uchar thresholdLow = 0.4*thresholdHigh;

		for (int row = 0; row < src.size().height; row++) {
			for (int col = 0; col < src.size().width; col++)
			{
				if (dst.ptr(row)[col] <= thresholdLow) {
					dst.ptr(row)[col] = 0;
				}
				else if (dst.ptr(row)[col] > thresholdLow &&dst.ptr(row)[col] <= thresholdHigh) {
					dst.ptr(row)[col] = 128;
				}
				else {
					dst.ptr(row)[col] = 255;
				}
			}
		}
		//滞后边界跟踪 暂无
	}
}