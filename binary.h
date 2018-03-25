#pragma once
#include<opencv2\core.hpp>
#include <iostream>
#include <vector>
#include<math.h>

using namespace std;
enum type_binary {
	OSTU,
	DOUBLE
};
//binary.cpp
void ToBinary(cv::Mat & src, cv::Mat & dst, type_binary type, uchar valueLow = 0, uchar valueHigh = 255);

