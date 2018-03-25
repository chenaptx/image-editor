#pragma once
#include<opencv2\core.hpp>
#include <iostream>
#include <vector>
#include<math.h>
#include"algebra.h"
#include"binary.h"
#include"color.h"
#include"edgedetect.h"
#include"filters.h"
#include"morphology.h"
#include"watershed.h"

vector<vector<vector<int>>> getElems(vector<vector<int>> e) {
	return stdelems;
}
vector<vector<int>> getElem(string str, cv::Vec2i&anchor) {
	int n = 0, s = 0;
	int size = str.size() / 2;
	int col = 0;
	cout << str << "\n";
	while (n >= 0) {
		n = str.find_first_of(';', n + 1);

		if (n > 0) {
			str[n] = '\n';
			col++;
		}
	}
	cout << col;
	vector<vector<int>>r(col, vector<int>());
	stringstream ss(str);
	for (int i = 0; i < col; i++) {
		for (int j = 0; j < size / col; j++) {
			ss >> s;
			r[i].push_back(s);
			cout << s << " ";
			if (s == 2) {
				anchor = cv::Vec2i(i, j);
			}
		}
	}
	return r;
}