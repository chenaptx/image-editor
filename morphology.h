#pragma once
#include<opencv2\core.hpp>
#include <iostream>
#include <vector>
#include<math.h>
#define PI 4.0*atan(1.0)

using namespace std;

extern  enum type_elem {
	BINARY_ELEM,
	GRAY_ELEM
};
extern vector< vector<vector<int>>> stdelems;
extern vector<vector<int>> stdwindow;
extern vector<vector<int>> stdelem;
extern vector< vector<vector<int>>> stdelemt;

//morphology.cpp
void Dilate(cv::Mat & src, cv::Mat& dst, vector<vector<int>>& elem = stdelem, cv::Vec2i anchor = cv::Vec2i(-1, -1));
void Erode(cv::Mat & src, cv::Mat& dst, vector<vector<int>>& elem = stdelem, cv::Vec2i anchor = cv::Vec2i(-1, -1));
void closing(cv::Mat& src, cv::Mat &dst, vector<vector<int>>& elem = stdelem, cv::Vec2i anchor = cv::Vec2i(-1, -1));
void openning(cv::Mat& src, cv::Mat &dst, vector<vector<int>>& elem = stdelem, cv::Vec2i anchor = cv::Vec2i(-1, -1));

void hitOrMiss(cv::Mat& src, cv::Mat& dst, vector<vector<int>>& elem = stdelem, cv::Vec2i anchor = cv::Vec2i(-1, -1), vector<vector<int>> &window = stdwindow, cv::Vec2i anchor1 = cv::Vec2i(-1, -1));
void Thinning(cv::Mat& src, cv::Mat& dst, vector< vector<vector<int>>> &elems = stdelemt, cv::Vec2i anchor = cv::Vec2i(-1, -1), vector<vector<int>> &window = stdwindow, cv::Vec2i anchor1 = cv::Vec2i(-1, -1));
void Thickening(cv::Mat& src, cv::Mat& dst, vector< vector<vector<int>>> &elems = stdelems, cv::Vec2i anchor = cv::Vec2i(-1, -1), vector<vector<int>> &window = stdwindow, cv::Vec2i anchor1 = cv::Vec2i(-1, -1));

void DistanceTransform(cv::Mat& src, cv::Mat &dst);
void DistanceTransform2(cv::Mat& src, cv::Mat &dst);

vector<cv::Mat> skeleton(cv::Mat& src, cv::Mat&dst);
void skeletonRebuild(cv::Mat& src, cv::Mat&dst);

void Dilate_cedi(cv::Mat &mask, cv::Mat&dst, cv::Mat&mould, vector<vector<int>> &elem = stdelem);
void rebuild(cv::Mat &mask, cv::Mat&dst, cv::Mat&mould, vector<vector<int>> &elem = stdelem);
void GDilate(cv::Mat & src, cv::Mat& dst, vector<vector<int>>& elem = stdelem, cv::Vec2i anchor = cv::Vec2i(-1, -1), type_elem type = BINARY_ELEM);
void GErode(cv::Mat & src, cv::Mat& dst, vector<vector<int>>& elem = stdelem, cv::Vec2i anchor = cv::Vec2i(-1, -1), type_elem type = BINARY_ELEM);
void Gopening(cv::Mat & src, cv::Mat& dst, vector<vector<int>>& elem = stdelem, cv::Vec2i anchor = cv::Vec2i(-1, -1));
void Gclosing(cv::Mat& src, cv::Mat &dst, vector<vector<int>>& elem = stdelem, cv::Vec2i anchor = cv::Vec2i(-1, -1));
void GDilate_cedi(cv::Mat &mask, cv::Mat&dst, cv::Mat&mould, vector<vector<int>> &elem = stdelem);
void Grebuild(cv::Mat &mask, cv::Mat&dst, cv::Mat&mould, vector<vector<int>> &elem = stdelem);