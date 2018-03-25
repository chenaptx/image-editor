#pragma once
#include<opencv2\core.hpp>
#include <iostream>
#include <vector>
#include<math.h>


using namespace std;

enum type_scale_rotate {
	LINEAR,
	NN,
	CUBIC
};


//algebra.cpp
cv::Mat Divide(cv::Mat &src1, cv::Mat &src2);
cv::Mat Add(cv::Mat &src1, cv::Mat &src2);
cv::Mat Multiply(cv::Mat &src1, cv::Mat &src2);
cv::Mat Subtract(cv::Mat &src1, cv::Mat &src2);
cv::Mat Cut(cv::Mat& src, int x, int y, int height, int width);// get a new  img (x,y) (x+width,y)(x,y+height) (x+width,y+height)
cv::Mat Scale(cv::Mat& src, float scale, type_scale_rotate type = LINEAR);
void Resize(cv::Mat&src, cv::Mat&dst, type_scale_rotate type = LINEAR);
cv::Mat Scalexy(cv::Mat&src, float xscale, float yscale, type_scale_rotate type = LINEAR);
cv::Mat Rotate(cv::Mat& src, float a = 0, type_scale_rotate type = LINEAR);
