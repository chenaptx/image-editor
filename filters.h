#pragma once
#include<opencv2\core.hpp>
#include <iostream>
#include <vector>
#include<math.h>
#define PI 4.0*atan(1.0)

using namespace std;


//filters.cpp
void GausFilter(cv::Mat& src, cv::Mat& dst, const double sigma, const int size = 3, cv::Vec2i anchor = cv::Vec2i(-1, -1));
void MedianFilter(cv::Mat& src, cv::Mat& dst, int size = 3, cv::Vec2i anchor = cv::Vec2i(-1, -1));
void MeanFilter(cv::Mat& src, cv::Mat& dst, int size = 3, cv::Vec2i anchor =cv::Vec2i(-1, -1));
