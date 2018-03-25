#pragma once
#include<opencv2\core.hpp>
#include <iostream>
#include <vector>
#include<math.h>
#define PI 4.0*atan(1.0)

using namespace std;
enum type_edge {
	SOBEL,
	LAPLACE,
	CANNY
};
//edgedetect.cpp
void EdgeDetect(cv::Mat & src, cv::Mat& dst, type_edge type);
