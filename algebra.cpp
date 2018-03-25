#include "algebra.h"
using namespace cv;

cv::Mat Add(cv::Mat &src1, cv::Mat &src2) {
	cv::Mat temp(src1.size(),src1.type());
	if (src1.size() != src2.size()) {
		cout << "add  two diffrent size imgs\n";
		return temp;
	}
	for (int row = 0; row < temp.size().height; row++) {
		for (int col = 0; col < temp.size().width; col++)
		{
			if (src1.type() == CV_8UC3) {
				Vec3b pixel_value1 = src1.ptr<Vec3b>(row)[col];
				Vec3b pixel_value2 = src2.ptr<Vec3b>(row)[col];
				Vec3b temp_p;
				temp_p[0] = saturate_cast<uchar>(pixel_value1[0] + pixel_value2[0]);
				temp_p[1] = saturate_cast<uchar>(pixel_value1[1] + pixel_value2[1]);

				temp_p[2] = saturate_cast<uchar>(pixel_value1[2] + pixel_value2[2]);

				temp.ptr<Vec3b>(row)[col] = temp_p;
			}
			else if (src1.type() == CV_8UC1) {
				uchar pixel_value1 = src1.ptr<uchar>(row)[col];
				uchar pixel_value2 = src2.ptr<uchar>(row)[col];
				uchar temp_p = saturate_cast<uchar>(pixel_value1 + pixel_value2);
				temp.ptr<uchar>(row)[col] = temp_p;
	}

		}
	}
	return temp;

}
cv::Mat Subtract(cv::Mat &src1, cv::Mat &src2) {
	cv::Mat temp(src1.size(),src1.type());
	if (src1.size() != src2.size()) {
		cout << "add  two diffrent size imgs\n";
		return temp;
	}
	
	for (int row = 0; row < temp.size().height; row++) {
		for (int col = 0; col < temp.size().width; col++)
		{
			if (src1.type() == CV_8UC3) {
				Vec3b pixel_value1 = src1.ptr<Vec3b>(row)[col];
				Vec3b pixel_value2 = src2.ptr<Vec3b>(row)[col];
				Vec3b temp_p;
				temp_p[0] = saturate_cast<uchar>(pixel_value1[0] - pixel_value2[0]);
				temp_p[1] = saturate_cast<uchar>(pixel_value1[1] - pixel_value2[1]);

				temp_p[2] = saturate_cast<uchar>(pixel_value1[2] - pixel_value2[2]);

				temp.ptr<Vec3b>(row)[col] = temp_p;
			}
			else if (src1.type() == CV_8UC1) {
				uchar pixel_value1= src1.ptr<uchar>(row)[col];
				uchar pixel_value2 = src2.ptr<uchar>(row)[col];
				uchar temp_p = saturate_cast<uchar>(pixel_value1 - pixel_value2);
				temp.ptr<uchar>(row)[col] = temp_p;
			}

		}
	}
	return temp;
}
cv::Mat Multiply(cv::Mat &src1, cv::Mat &src2) {
	cv::Mat temp(src1.size(), src1.type());
	if (src1.size() != src2.size()) {
		cout << "add  two diffrent size imgs\n";
		return temp;
	}

	for (int row = 0; row < temp.size().height; row++) {
		for (int col = 0; col < temp.size().width; col++)
		{
			if (src1.type() == CV_8UC3) {
				Vec3b pixel_value1 = src1.ptr<Vec3b>(row)[col];
				Vec3b pixel_value2 = src2.ptr<Vec3b>(row)[col];
				Vec3b temp_p;
				temp_p[0] = saturate_cast<uchar>(pixel_value1[0] * pixel_value2[0]);
				temp_p[1] = saturate_cast<uchar>(pixel_value1[1] * pixel_value2[1]);

				temp_p[2] = saturate_cast<uchar>(pixel_value1[2] * pixel_value2[2]);

				temp.ptr<Vec3b>(row)[col] = temp_p;
			}
			else if (src1.type() == CV_8UC1) {
				uchar pixel_value1 = src1.ptr<uchar>(row)[col];
				uchar pixel_value2 = src2.ptr<uchar>(row)[col];
				uchar temp_p = saturate_cast<uchar>(pixel_value1 * pixel_value2);
				temp.ptr<uchar>(row)[col] = temp_p;
			}

		}
	}
	return temp;
}
cv::Mat Divide(cv::Mat &src1, cv::Mat &src2) {
	cv::Mat temp(src1.size(), src1.type());
	if (src1.size() != src2.size()) {
		cout << "add  two diffrent size imgs\n";
		return temp;
	}

	for (int row = 0; row < temp.size().height; row++) {
		for (int col = 0; col < temp.size().width; col++)
		{
			if (src1.type() == CV_8UC3) {
				Vec3b pixel_value1 = src1.ptr<Vec3b>(row)[col];
				Vec3b pixel_value2 = src2.ptr<Vec3b>(row)[col];
				Vec3b temp_p;
				temp_p[0] = saturate_cast<uchar>((pixel_value1[0]*1.0) / pixel_value2[0]);
				temp_p[1] = saturate_cast<uchar>((pixel_value1[1]* 1.0)/ pixel_value2[1]);
				temp_p[2] = saturate_cast<uchar>((pixel_value1[2] *1.0)/ pixel_value2[2]);

				temp.ptr<Vec3b>(row)[col] = temp_p;
			}
			else if (src1.type() == CV_8UC1) {
				uchar pixel_value1 = src1.ptr<uchar>(row)[col];
				uchar pixel_value2 = src2.ptr<uchar>(row)[col];
				uchar temp_p = saturate_cast<uchar>((pixel_value1*1.0)/ pixel_value2);
				temp.ptr<uchar>(row)[col] = temp_p;
			}

		}
	}
	return temp;

}

cv::Mat Cut(cv::Mat& src, int x, int y, int height, int width) {// get a new smaller img (x,y) (x+width,y)(x,y+height) (x+width,y+height)
	cv::Mat temp(height, width, src.type());
	if ((x + width) > src.size().width || (y + height) > src.size().height) {
		cout << "error: out of scale of src\n";
		return temp;
	}
	
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++)
		{
			
				Vec3b pixel_value = src.ptr<Vec3b>(y+row)[x+col];
			
				temp.ptr<Vec3b>(row)[col] = pixel_value;
			
		}
	}
	return temp;
}
cv::Mat Scale(cv::Mat& src, float scale, type_scale_rotate type) {
	return(Scalexy(src, scale, scale,type));

}
void Resize(cv::Mat&src, cv::Mat&dst, type_scale_rotate type) {
	int w0 = src.size().width;
	int h0 = src.size().height;

	int w1 = dst.size().width;
	int h1 = dst.size().height;

	
	if (type == NN) {
		float fw = float(w0) / w1;
		float fh = float(h0) / h1;
		for (int row = 0; row < dst.size().height; row++) {
			int y =floor( row*fh + 0.5);
			for (int col = 0; col < dst.size().width; col++) {
				int x =floor( col*fw + 0.5);

				dst.ptr<Vec3b>(row)[col] = src.ptr<Vec3b>(y)[x];
			}
		}
	}
	else if (type == LINEAR) {
		float fw = float(w0-1) / w1;//避免越界或者新的图像比原图像大
		float fh = float(h0-1) / h1;
		for (int row = 0; row < dst.size().height; row++) {
			float y = row*fh;
			int y0 = floor(y);
			for (int col = 0; col < dst.size().width; col++) {
				int x = col*fw;
				int x0 = floor(x);
				
				Vec3b pixel0 = src.ptr<Vec3b>(y0)[x0];
				Vec3b pixel1 = src.ptr<Vec3b>(y0+1)[x0];
				Vec3b pixel2 = src.ptr<Vec3b>(y0)[x0+1];
				Vec3b pixel3 = src.ptr<Vec3b>(y0+1)[x0+1];

				float u = x - x0;
				float v = y - y0;
				float pm0 = (1 - u)*(1 - v);
				float pm1 = v*(1 - u);
				float pm2 = (1 - v)*u;
				float pm3 = v*u;

				dst.ptr<Vec3b>(row)[col] = pixel0*pm0 + pixel1*pm1 + pixel2*pm2 + pixel3*pm3;
			}
		}
	}
	
	

};
cv::Mat Scalexy(cv::Mat&src, float xscale, float yscale, type_scale_rotate type) {
	cv::Mat temp(src.size().width*xscale, src.size().height*yscale, src.type());
	Resize(src, temp, LINEAR);
	return temp;

}

cv::Mat Rotate(cv::Mat& src,float a, type_scale_rotate type){
	int newwidth = src.size().width*cos(a) + src.size().height*sin(a);
	int newheight= src.size().width*sin(a) + src.size().height*cos(a);

	cv::Mat temp(newheight, newwidth, src.type());
	if (type == NN) {
		for (int row = 0; row < newheight; row++) {
			for (int col = 0; col < newwidth; col++) {
				float srcy = col*sin(a) + row*cos(a) - src.size().width*sin(a)*cos(a);
				float srcx = col*cos(a) - row*sin(a) + src.size().width*sin(a)*sin(a);

				int x0 = floor(srcx);
				int y0 = floor(srcy);
				if (src.type() == CV_8UC3) {
					if (x0 >= 0 && x0 < src.size().width && y0>0 && y0 < src.size().height) {

						temp.ptr<Vec3b>(row)[col] = src.ptr<Vec3b>(y0)[x0];
					}
					else {
						temp.ptr<Vec3b>(row)[col] = Vec3b(255, 255, 255);
					}
				}
				else if (src.type() == CV_8UC1) {
					if (y0 < src.size().height && x0 < src.size().width) {
						temp.ptr<uchar>(row)[col] = src.ptr<uchar>(y0)[x0];
					}
					else {
						temp.ptr<uchar>(row)[col] = 255;
					}
				}
			}
		}
	}
	else if (type == LINEAR) {
		for (int row = 0; row < newheight; row++) {
			for (int col = 0; col < newwidth; col++) {
				float srcy = col*sin(a) + row*cos(a) - src.size().width*sin(a)*cos(a);
				float srcx = col*cos(a) - row*sin(a) + src.size().width*sin(a)*sin(a);

				int x0 = floor(srcx);
				int y0 = floor(srcy);
				
				
				if (src.type() == CV_8UC3) {
					if (x0>=0 && x0 < src.size().width && y0>0 && y0 < src.size().height) {
						if (x0 == (src.size().width - 1 )|| y0 ==(src.size().height - 1)) {
							temp.ptr<Vec3b>(row)[col] = src.ptr<Vec3b>(y0)[x0];
						}
						else {
							Vec3b pixel0 = src.ptr<Vec3b>(y0)[x0];
							Vec3b pixel1 = src.ptr<Vec3b>(y0 + 1)[x0];
							Vec3b pixel2 = src.ptr<Vec3b>(y0)[x0 + 1];
							Vec3b pixel3 = src.ptr<Vec3b>(y0 + 1)[x0 + 1];
							float u = srcx - x0;
							float v = srcy - y0;
							float pm0 = (1 - u)*(1 - v);
							float pm1 = v*(1 - u);
							float pm2 = (1 - v)*u;
							float pm3 = v*u;

							temp.ptr<Vec3b>(row)[col] = pixel0*pm0 + pixel1*pm1 + pixel2*pm2 + pixel3*pm3;
						}
						
					}
					else {
						temp.ptr<Vec3b>(row)[col] = Vec3b(255, 255, 255);
					}
				}
				else if (src.type() == CV_8UC1) {
					if (x0 >= 0 && x0 < src.size().width && y0>0 && y0 < src.size().height) {
						if (x0 == (src.size().width - 1) || x0 == (src.size().height - 1)) {
							temp.ptr<uchar>(row)[col] = src.ptr<uchar>(x0)[x0];
						}
						else {
							uchar pixel0 = src.ptr<uchar>(y0)[x0];
							uchar pixel1 = src.ptr<uchar>(y0 + 1)[x0];
							uchar pixel2 = src.ptr<uchar>(y0)[x0 + 1];
							uchar pixel3 = src.ptr<uchar>(y0 + 1)[x0 + 1];
							float u = srcx - x0;
							float v = srcy - y0;
							float pm0 = (1 - u)*(1 - v);
							float pm1 = v*(1 - u);
							float pm2 = (1 - v)*u;
							float pm3 = v*u;

							temp.ptr<uchar>(row)[col] = pixel0*pm0 + pixel1*pm1 + pixel2*pm2 + pixel3*pm3;
						}
						temp.ptr<uchar>(row)[col] = src.ptr<uchar>(y0)[x0];
					}
					else {
						temp.ptr<uchar>(row)[col] = 255;
					}
				}
			}
		}
	}
	return temp;
}