#pragma once
#include<opencv2\core.hpp>
#include <vector>
#include<math.h>
#define PI 4.0*atan(1.0)

using namespace std;
enum type_color {
	RGB_Gray,
	RGB_Green,
	RGB_Blue,
	RGB_Red,

};
enum type_contrast {
	LINEAR1,
	LINEARN,
	LOG,
	EXP,
	HISTO_EQUAL
};

//color.cpp OK
void RGB2HSL(cv::Vec3b pixel_value, int &H, double &S, double &L);
void HSL2RGB(int &H, double &S, double &L, cv::Vec3b& p_value);
void RGB2HSV(cv::Vec3b pixel_value, int &H, double &S, double &V);
void HSV2RGB(int &H, double &S, double &V, cv::Vec3b& p_value);
bool convertCorlor(cv::Mat & src, cv::Mat& dst, type_color type = RGB_Gray);// 转变成单通道图像或者是灰度图像
bool setHSV(cv::Mat& src, cv::Mat& dst, int H_c = 0, int S_c = 0, int V_c = 0); //调节色相 饱和度 明度
bool setHSL(cv::Mat& src, cv::Mat& dst, int H_c = 0, int S_c = 0, int L_c = 0); //调节色相，饱和度，亮度
void histogramGray(cv::Mat &src, vector<int>&counts);
void histogramRGB(cv::Mat&src, vector<int>&counts, vector<int>&countR, vector<int> &countG, vector<int> &countB);
void histoEqual(cv::Mat&src, cv::Mat&dst);
void enhanceLE(cv::Mat& src, cv::Mat& dst, type_contrast type, float contrast, float power);
bool enhanceLinear(cv::Mat& src, cv::Mat& dst, type_contrast type, float contrast, std::vector<cv::Vec2b>& points = std::vector<cv::Vec2b>());