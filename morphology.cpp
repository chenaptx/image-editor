#include "morphology.h"
#include "color.h"
#include "binary.h"
#include <opencv2\highgui.hpp>
using namespace cv;

vector< vector<vector<int>>> stdelems = { { { 0,0,0 },{ 0,1,0 },{ 1,1,1 } },
{ { 0,0,0 },{ 1,1,0 },{ 1,1,0 } },
{ { 1,0,0 },{ 1,1,0 },{ 1,0,0 } },
{ { 1,1,0 },{ 1,1,0 },{ 0,0,0 } },
{ { 1,1,1 },{ 0,1,0 },{ 0,0,0 } },
{ { 0,1,1 },{ 0,1,1 },{ 0,0,0 } },
{ { 0,0,1 },{ 0,1,1 },{ 0,0,1 } },
{ { 0,0,0 },{ 0,1,1 },{ 0,1,1 } }
};
vector< vector<vector<int>>> stdelemt = {
	{ { 0,0,0 },{ 1,1,0 },{ 1,1,0 } },

	{ { 1,1,0 },{ 1,1,0 },{ 0,0,0 } },

	{ { 0,1,1 },{ 0,1,1 },{ 0,0,0 } },

	{ { 0,0,0 },{ 0,1,1 },{ 0,1,1 } }
};
vector<vector<int>> stdwindow = { { 1,1,1 ,1 },{ 1,1,1 ,1 },{ 1,1,1 ,1 },{ 1,1,1 ,1 } };
vector<vector<int>> stdelem = { { 1,1,1 },{ 1,1,1 },{ 1,1,1 }, };
void Dilate(cv::Mat & src, cv::Mat& dst, vector<vector<int>>& elem, Vec2i anchor) {
	if (anchor[0] == -1 || anchor[1] == -1) {
		anchor = Vec2i(elem.size() / 2, elem[0].size() / 2);
	}

	if (src.type() == CV_8UC3) {
		convertCorlor(src, src);

	}
	cv::Mat temp(src.size(), CV_8UC1);
	ToBinary(src, temp, OSTU);

	dst = temp.clone();

	for (int row = 0; row < src.size().height; row++) {
		for (int col = 0; col < src.size().width; col++) {
			if (temp.ptr(row)[col] != 0) {

				for (int j = 0; j < elem.size(); j++) {
					for (int k = 0; k < elem[0].size(); k++) {
						if ((row + j - anchor[0]) < src.size().height
							&& (row + j - anchor[0]) >= 0
							&& (col + k - anchor[1]) >= 0
							&& (col + k - anchor[1]) < src.size().width) {
							int y = row + j - anchor[0];
							int x = col + k - anchor[1];
							if ((elem[j][k] != 0) || (dst.ptr<uchar>(y)[x] == 0)) {
								dst.ptr<uchar>(y)[x] = 255;
							}
							/*else {
							dst.ptr<uchar>(y)[x] = 0;
							}*/

							//num++;
						}


					}
				}
			}
		}
	}

}
void Erode(cv::Mat & src, cv::Mat& dst, vector<vector<int>>& elem, Vec2i anchor) {

	if (src.type() == CV_8UC3) {
		//convertCorlor(src, src);

	}
	if (anchor[0] == -1 || anchor[1] == -1) {
		anchor = Vec2i(elem.size() / 2, elem[0].size() / 2);
	}
	cv::Mat temp(src.size(), CV_8UC1);
	dst = src.clone();
	//ToBinary(src, temp, OSTU);
	temp = src.clone();

	for (int row = 0; row < src.size().height; row++) {
		for (int col = 0; col < src.size().width; col++) {
			if (temp.ptr(row)[col] != 0) {
				bool in = true;
				for (int j = 0; j < elem.size(); j++) {
					for (int k = 0; k < elem[0].size(); k++) {
						if ((row + j - anchor[0]) < src.size().height
							&& (row + j - anchor[0]) >= 0
							&& (col + k - anchor[1]) >= 0
							&& (col + k - anchor[1]) < src.size().width) {
							int y = row + j - anchor[0];
							int x = col + k - anchor[1];
							if (elem[j][k] != 0 && temp.ptr<uchar>(y)[x] == 0) {
								in = false;
							}
						}
					}
				}
				if (!in) {
					dst.ptr(row)[col] = 0;
				}

			}
		}
	}
}
void closing(cv::Mat& src, cv::Mat &dst, vector<vector<int>> &elem, Vec2i anchor) {
	cv::Mat temp;
	Dilate(src, temp, elem, anchor);
	Erode(temp, dst, elem, anchor);
}
void openning(cv::Mat& src, cv::Mat &dst, vector<vector<int>> &elem, Vec2i anchor) {
	cv::Mat temp;
	Erode(src, temp, elem, anchor);
	Dilate(temp, dst, elem, anchor);
}
void hitOrMiss(cv::Mat& src, cv::Mat& dst, vector<vector<int>> &elem, Vec2i anchor, vector<vector<int>>&window, Vec2i anchor1) {
	if (window.size() < elem.size() || window[0].size() < elem[0].size()) {
		cout << "erorr: window is smaller than element\n";
	}
	else {
		if (anchor[0] == -1 || anchor[1] == -1) {
			anchor = Vec2i(elem.size() / 2, elem[0].size() / 2);
		}
		if (anchor1[0] == -1 || anchor1[1] == -1) {
			anchor1 = Vec2i(window.size() / 2, window[0].size() / 2);
		}
		vector<vector<int>> winElem(window.size(), vector<int>(window[0].size(), 1));
		//计算 w-e
		//Mat WinElem = window;
		for (int y = 0; y <elem[0].size(); y++) {
			for (int x = 0; x <elem.size(); x++) {
				if (elem[y][x] != 0) {
					winElem[y + anchor1[1] - anchor[1]][x + anchor1[0] - anchor[0]] = 0;
				}
			}
		}

		//
		cv::Mat srcC(src.size(), src.type());
		for (int row = 0; row < src.size().height; row++) {
			for (int col = 0; col < src.size().width; col++) {
				if (src.ptr(row)[col] > 128) {
					srcC.ptr(row)[col] = 0;
				}
				else {
					srcC.ptr(row)[col] = 255;
				}
			}
		}
		/*namedWindow("???");
		imshow("???", srcC);
		waitKey(100);*/

		///
		cv::Mat temp(src.size(), src.type());
		Dilate(src, temp, elem, anchor);//A*B1
										/*namedWindow("????");
										imshow("????", temp);
										waitKey(100);*/

		Dilate(srcC, dst, winElem, anchor1);//A*B2
											/*namedWindow("?????");
											imshow("?????", dst);
											waitKey(100);*/
		for (int row = 0; row < src.size().height; row++) {
			for (int col = 0; col < src.size().width; col++) {
				if (temp.ptr(row)[col] == 0 && dst.ptr(row)[col] != 0) {
					dst.ptr(row)[col] = 0;
				}
			}
		}
		/*namedWindow("??????");
		imshow("??????", dst);
		waitKey(100);
		*/
	}
}


void Thinning(cv::Mat& src, cv::Mat& dst, vector< vector<vector<int>>> &elems, Vec2i anchor, vector<vector<int>> &window, Vec2i anchor1) {
	if (anchor[0] == -1 || anchor[1] == -1) {
		anchor = Vec2i(elems[0].size() / 2, elems[0][0].size() / 2);
	}
	if (anchor1[0] == -1 || anchor1[1] == -1) {
		anchor1 = Vec2i(window.size() / 2, window[0].size() / 2);
	}
	cv::Mat& src_t = src;
	dst.create(src.size(), src.type());
	for (int i = 0; i < elems.size(); i++) {

		hitOrMiss(src_t, dst, elems[i], anchor, window, anchor1);
		char ic = i + 48;

		for (int row = 0; row < src.size().height; row++) {
			for (int col = 0; col < src.size().width; col++) {
				if (src.ptr(row)[col]>128 && dst.ptr(row)[col]<128) {
					dst.ptr(row)[col] = 255;
				}
				else {
					dst.ptr(row)[col] = 0;
				}
			}
		}
		src_t = dst;
		/*namedWindow("???????");
		imshow("???????", dst);
		waitKey(100);*/
	}

}
void Thickening(cv::Mat& src, cv::Mat& dst, vector< vector<vector<int>>> &elems, Vec2i anchor, vector<vector<int>> &window, Vec2i anchor1) {
	cv::Mat& src_t = src;
	dst.create(src.size(), src.type());
	if (anchor[0] == -1 || anchor[1] == -1) {
		anchor = Vec2i(elems[0].size() / 2, elems[0][0].size() / 2);
	}
	if (anchor1[0] == -1 || anchor1[1] == -1) {
		anchor1 = Vec2i(window.size() / 2, window[0].size() / 2);
	}
	for (int i = 0; i < elems.size(); i++) {

		hitOrMiss(src_t, dst, elems[i], anchor, window, anchor1);
		for (int row = 0; row < src.size().height; row++) {
			for (int col = 0; col < src.size().width; col++) {
				if (src.ptr(row)[col] > 128 && dst.ptr(row)[col] <= 128) {
					dst.ptr(row)[col] = 255;
				}

			}
		}
		/*namedWindow("?????");
		imshow("?????", dst);
		waitKey(100);*/
		src_t = dst;
	}

}
void DistanceTransform(cv::Mat& src, cv::Mat &dst) {
	dst.create(src.size(), src.type());
	//Mat temp;temp.create(src.size().width + 2, src.size().height + 2);
	vector<Vec2i> inner, outer;
	Mat temp = src;
	copyMakeBorder(src, temp, 1, 1, 1, 1, 0, Scalar(255, 255, 255));
	for (int row = 1; row < src.size().height; row++) {
		for (int col = 1; col < src.size().width; col++) {

			if (temp.ptr(row)[col] != 0) {

				if (temp.ptr(row - 1)[col] == 0 || temp.ptr(row + 1)[col] == 0 || temp.ptr(row)[col + 1] == 0 || temp.ptr(row)[col - 1] == 0) {
					dst.ptr(row)[col] = 1;
				}
				else if (!(temp.ptr(row - 1)[col - 1] && temp.ptr(row - 1)[col + 1] && temp.ptr(row + 1)[col + 1] && temp.ptr(row + 1)[col - 1])) {
					dst.ptr(row)[col] = sqrt(2);
				}
				else {
					inner.push_back(Vec2i(row, col));
				}
			}
			else {
				outer.push_back(Vec2i(row, col));
			}
		}
	}
	for (int i = 0; i < inner.size(); i++) {
		Vec2i point = inner[i];
		float dis = INT_MAX;
		for (int j = 0; j < outer.size(); j++) {
			Vec2i opoint = outer[j];
			dis = MIN(dis, sqrt((point[0] - opoint[0])*(point[0] - opoint[0]) + (point[1] - opoint[1])*(point[1] - opoint[1])));
		}
		dst.ptr(point[0])[point[1]] = dis;
	}

}
void DistanceTransform2(cv::Mat& src, cv::Mat &dst) {
	dst.create(src.size(), CV_8SC1);
	Mat temp, temp0;
	copyMakeBorder(src, temp, 1, 1, 1, 1, 0, Scalar(255, 255, 255));
	temp0.create(temp.size(), CV_8SC1);
	if (src.type() != CV_8UC1) {
		cout << "请输入单通道二值图像\n";
		ToBinary(temp, temp0, OSTU);
		temp = temp0;
	}

	for (int row = 1; row < dst.size().height + 1; row++) {
		for (int col = 1; col < dst.size().width + 1; col++) {
			uchar pixel = temp.ptr(row)[col];
			pixel = MIN(pixel, (temp.ptr(row - 1)[col - 1] + 4));
			pixel = MIN(pixel, (temp.ptr(row - 1)[col] + 3));
			pixel = MIN(pixel, (temp.ptr(row)[col - 1] + 3));
			pixel = MIN(pixel, (temp.ptr(row - 1)[col + 1] + 4));
			temp.ptr(row)[col] = pixel;
		}
	}
	for (int row = 1; row < dst.size().height + 1; row++) {
		for (int col = 1; col < dst.size().width + 1; col++) {
			uchar pixel = temp.ptr(row)[col];
			pixel = MIN(pixel, (temp.ptr(row + 1)[col + 1] + 4));
			pixel = MIN(pixel, (temp.ptr(row + 1)[col] + 3));
			pixel = MIN(pixel, (temp.ptr(row)[col + 1] + 3));
			pixel = MIN(pixel, (temp.ptr(row + 1)[col - 1] + 4));
			dst.ptr(row - 1)[col - 1] = pixel;
		}
	}


}
vector<Mat> skeleton(cv::Mat& src, cv::Mat&dst) {
	vector<Mat> skeletons;

	if (src.type() != CV_8UC1) {
		cout << "skeleton:请输入单通道二值图像\n";
	}
	else {
		dst.create(src.size(), src.type());
		dst.setTo(0);
		/*namedWindow("skeleton");
		imshow("skeleton", dst);
		waitKey(100);*/
		bool flag = false;
		cv::Mat temp = src;
		cv::Mat temp0(src.size(), src.type());
		temp0.setTo(0);
		cv::Mat temp1(src.size(), src.type());
		temp1.setTo(0);
		cv::Mat temp2(src.size(), src.type());
		temp2.setTo(0);
		while (!flag) {
			Erode(temp, temp0);
			/*namedWindow("Erode");
			imshow("Erode", temp0);
			waitKey(100);*/
			flag = true;
			for (int row = 0; row < src.size().height; row++) {
				for (int col = 0; col < src.size().width; col++) {
					if (temp0.ptr(row)[col] != 0) {
						flag = false;
						goto Number0;
					}
				}
			}
		Number0:
			/*namedWindow("num");
			imshow("num", temp);
			waitKey(100);*/
			if (!flag) {
				//temp0开操作 temp1
				openning(temp0, temp1);


				//temp0 temp1作差->temp2
				for (int row = 0; row < src.size().height; row++) {
					for (int col = 0; col < src.size().width; col++) {
						if (temp0.ptr(row)[col] != 0 && temp1.ptr(row)[col] == 0) {
							temp2.ptr(row)[col] = 255;
						}
						else {
							temp2.ptr(row)[col] = 0;
						}
					}
				}

				/*namedWindow("sub");
				imshow("sub", temp2);
				waitKey(100);*/
				skeletons.push_back(temp2.clone());
				//temp2并上dst
				for (int row = 0; row < src.size().height; row++) {
					for (int col = 0; col < src.size().width; col++) {
						if (temp2.ptr(row)[col]>128 && dst.ptr(row)[col] == 0) {
							dst.ptr(row)[col] = 255;
						}
					}
				}
				/*namedWindow("dst");
				imshow("dst", dst);
				waitKey(100);*/
				temp = temp0;
			}

		}
		return skeletons;
	}

}
void skeletonRebuild(cv::Mat& src, cv::Mat&dst) {
	if (src.type() != CV_8UC1) {
		cout << "skeleton:请输入单通道二值图像\n";
	}
	else {
		bool flag = false;
		cv::Mat temp;
		cv::Mat temp0;
		cv::Mat temp1;
		cv::Mat temp2;
		temp2.create(src.size(), src.type());
		temp2.setTo(0);
		temp1.create(src.size(), src.type());
		temp1.setTo(0);
		dst.create(src.size(), src.type());
		dst.setTo(0);
		vector<Mat> skeletons = skeleton(src, temp);
		/*namedWindow("dst");
		imshow("dst", dst);
		waitKey(100);*/
		for (int i = 0; i < skeletons.size(); i++) {
			temp1 = skeletons[i];
			namedWindow("skeletons");
			imshow("skeletons", temp1);
			waitKey(100);
			for (int j = 0; j < i; j++) {
				Dilate(temp1, temp2);
				temp1 = temp2;
			}


			for (int row = 0; row < src.size().height; row++) {
				for (int col = 0; col < src.size().width; col++) {
					if (temp2.ptr(row)[col] != 0 && dst.ptr(row)[col] == 0) {
						dst.ptr(row)[col] = 255;
					}
				}
			}

		}
		/*namedWindow("dst");
		imshow("dst", dst);
		waitKey(100);*/
	}
}
void Dilate_cedi(cv::Mat &mask, cv::Mat&dst, cv::Mat&mould, vector<vector<int>> &elem) {
	Dilate(mask, dst, elem, Vec2i(1, 1));
	for (int row = 0; row < dst.size().height; row++) {
		for (int col = 0; col < dst.size().width; col++) {
			if (mould.ptr(row)[col] >128 && dst.ptr(row)[col]<128) {
				dst.ptr(row)[col] = 255;
			}
		}
	}

}
void rebuild(cv::Mat &mask, cv::Mat&dst, cv::Mat&mould, vector<vector<int>> &elem) {
	cv::Mat temp(mould.size(), mould.type());
	dst.create(mould.size(), mould.type());
	bool flag = true;
	while (flag) {
		Dilate_cedi(mask, dst, mould, elem);
		flag = false;
		for (int row = 0; row < dst.size().height; row++) {
			for (int col = 0; col < dst.size().width; col++) {
				if (dst.ptr(row)[col] != temp.ptr(row)[col]) {
					flag = true;
				}
			}
		}
		temp = dst;
	}


}

void GDilate(cv::Mat & src, cv::Mat& dst, vector<vector<int>>& elem, Vec2i anchor, type_elem type) {
	dst.create(src.size(), CV_8UC1);
	cv::Mat temp(src.size(), src.type());
	dst.setTo(0);
	temp = src.clone();
	if (anchor[0] == -1 || anchor[1] == -1) {
		anchor = Vec2i(elem.size() / 2, elem[0].size() / 2);
	}
	if (src.type() == CV_8UC3) {
		convertCorlor(src, temp);

	}
	for (int row = 0; row < src.size().height; row++) {
		for (int col = 0; col < src.size().width; col++) {
			if (temp.ptr(row)[col] != 0) {

				unsigned char maxpixel = 0;
				for (int j = 0; j < elem.size(); j++) {
					for (int k = 0; k < elem[0].size(); k++) {
						if ((row + j - anchor[0]) < src.size().height
							&& (row + j - anchor[0]) >= 0
							&& (col + k - anchor[1]) >= 0
							&& (col + k - anchor[1]) < src.size().width) {

							if (elem[j][k] != 0) {
								if (type == BINARY_ELEM) {
									maxpixel = MAX(temp.ptr<uchar>(row + j - anchor[0])[col + k - anchor[1]], maxpixel);
								}
								else {
									maxpixel = MAX(saturate_cast<uchar>(temp.ptr<uchar>(row + j - anchor[0])[col + k - anchor[1]] + elem[j][k]), maxpixel);

								}
							}

							//num++;
						}


					}
				}
				dst.ptr(row)[col] = maxpixel;
			}
		}
	}

}
void GErode(cv::Mat & src, cv::Mat& dst, vector<vector<int>>& elem, Vec2i anchor, type_elem type) {
	dst.create(src.size(), CV_8UC1);
	cv::Mat temp(src.size(), src.type());
	temp = src.clone();
	dst.setTo(0);

	if (anchor[0] == -1 || anchor[1] == -1) {
		anchor = Vec2i(elem.size() / 2, elem[0].size() / 2);
	}
	if (src.type() == CV_8UC3) {
		convertCorlor(src, temp);

	}

	for (int row = 0; row < src.size().height; row++) {
		for (int col = 0; col < src.size().width; col++) {
			if (temp.ptr<uchar>(row)[col] != 0) {
				unsigned char minpixel = temp.ptr<uchar>(row)[col];
				for (int j = 0; j < elem.size(); j++) {
					for (int k = 0; k < elem[0].size(); k++) {
						if ((row + j - anchor[0]) < src.size().height
							&& (row + j - anchor[0]) >= 0
							&& (col + k - anchor[1]) >= 0
							&& (col + k - anchor[1]) < src.size().width) {

							if (elem[j][k] != 0) {
								if (type == BINARY_ELEM) {
									int c = temp.ptr<uchar>(row + j - anchor[0])[col + k - anchor[1]];
									//if(c>1)cout << c;
									minpixel = MIN(c, minpixel);
								}
								else {
									minpixel = MIN(saturate_cast<uchar>(temp.ptr<uchar>(row + j - anchor[0])[col + k - anchor[1]] - elem[j][k]), minpixel);

								}
							}

							//num++;
						}


					}
				}

				dst.ptr(row)[col] = minpixel;
			}
		}
	}

}
void Gopening(cv::Mat & src, cv::Mat& dst, vector<vector<int>>& elem, Vec2i anchor) {
	cv::Mat temp;
	GErode(src, temp, elem, anchor);

	GDilate(temp, dst, elem, anchor);
}
void Gclosing(cv::Mat& src, cv::Mat &dst, vector<vector<int>> &elem, Vec2i anchor) {
	cv::Mat temp;
	GDilate(src, temp, elem, anchor);
	GErode(temp, dst, elem, anchor);
}
void GDilate_cedi(cv::Mat &mask, cv::Mat&dst, cv::Mat&mould, vector<vector<int>> &elem) {
	GDilate(mask, dst, elem, Vec2i(1, 1));
	for (int row = 0; row < dst.size().height; row++) {
		for (int col = 0; col < dst.size().width; col++) {
			if (mould.ptr(row)[col] != 0 || dst.ptr(row)[col] != 0) {
				dst.ptr(row)[col] = MIN(mould.ptr(row)[col], dst.ptr(row)[col]);
			}
		}
	}

}
void Grebuild(cv::Mat &mask, cv::Mat&dst, cv::Mat&mould, vector<vector<int>> &elem) {
	cv::Mat temp(mould.size(), mould.type());
	dst.create(mould.size(), mould.type());
	dst.setTo(0);
	bool flag = true;
	while (flag) {
		GDilate_cedi(mask, dst, mould, elem);
		flag = false;
		for (int row = 0; row < dst.size().height; row++) {
			for (int col = 0; col < dst.size().width; col++) {
				if (dst.ptr(row)[col] != temp.ptr(row)[col]) {
					flag = true;
				}
			}
		}
		temp = dst;
	}


}