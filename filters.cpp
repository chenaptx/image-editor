#include"filters.h"

using namespace cv;
void MeanFilter(cv::Mat& src, cv::Mat& dst, int size, Vec2i anchor) {
	dst.create(src.size(), src.type());

	if (anchor == Vec2i(-1, -1)) {
		anchor = (size / 2, size / 2);
	}
	if (src.type() == CV_8UC1) {
		for (int row = 0; row < src.size().height; row++) {
			for (int col = 0; col < src.size().width; col++)
			{
				int num = 0;
				int pixel_new = 0;
				for (int j = 0; j < size; j++) {
					for (int k = 0; k < size; k++) {
						if ((row + j + anchor[0]) < src.size().height
							&& (row + j - anchor[0]) >= 0
							&& (col + k - anchor[1]) >= 0
							&& (col + k - anchor[1]) < src.size().width) {
							pixel_new += src.ptr<uchar>(row + j - anchor[0])[col + k - anchor[1]];
							num++;
						}

					}
				}
				if (num != 0) {
					dst.ptr<uchar>(row)[col] = pixel_new / num;
				}


			}

		}
	}
	else if (src.type() == CV_8UC3) {
		for (int row = 0; row < src.size().height; row++) {
			for (int col = 0; col < src.size().width; col++)
			{
				int num = 0;
				Vec3i pixel_new = 0;
				for (int j = 0; j < size; j++) {
					for (int k = 0; k < size; k++) {
						if ((row + j - anchor[0]) < src.size().height
							&& (row + j - anchor[0]) >= 0
							&& (col + k - anchor[1]) >= 0
							&& (col + k - anchor[1]) < src.size().width) {
							pixel_new += src.ptr<Vec3b>(row + j - anchor[0])[col + k - anchor[1]];
							num++;
						}

					}
				}
				dst.ptr<Vec3b>(row)[col] = pixel_new *1.0 / num;

			}

		}
	}
}

int findMedian(vector<int>& values) {
	int p = 0, low = 0, t = 0, oldnlow = 0, high = values.size();

	for (int j = 0; j <= values.size(); j++) {
		//oldnlow = low;
		p = low;
		for (int i = p; i < high; i++) {
			if (values[i] < values[low]) {
				p++;
				t = values[p];
				values[p] = values[i];
				values[i] = t;


			}

		}
		t = values[low];
		values[low] = values[p];
		values[p] = t;
		if (p < values.size() / 2)
			low = p + 1;
		else if (p > values.size() / 2) {
			high = p;
		}
		else {
			int m = values[p];
			return m;
		}
	}

}
void MedianFilter(cv::Mat& src, cv::Mat& dst, int size, Vec2i anchor) {
	dst.create(src.size(), src.type());

	if (anchor == Vec2i(-1, -1)) {
		anchor = (size / 2, size / 2);
	}
	if (src.type() == CV_8UC1) {
		for (int row = 0; row < src.size().height; row++) {
			for (int col = 0; col < src.size().width; col++)
			{
				int num = 0;
				int pixel_new = 0;
				int min = 255, max = 0;
				vector<int>pixels;
				for (int j = 0; j < size; j++) {
					for (int k = 0; k < size; k++) {
						if ((row + j - anchor[0]) < src.size().height
							&& (row + j - anchor[0]) >= 0
							&& (col + k - anchor[1]) >= 0
							&& (col + k - anchor[1]) < src.size().width) {
							int pixel = src.ptr<uchar>(row + j - anchor[0])[col + k - anchor[1]];
							if (min > pixel) min = pixel;
							if (max < pixel)max = pixel;
							pixels.push_back(pixel);
							//pixel_new += pixel;
							//num++;
						}


					}
				}
				pixel_new = findMedian(pixels);
				dst.ptr<uchar>(row)[col] = pixel_new;

			}

		}
	}
	else if (src.type() == CV_8UC3) {
		for (int row = 0; row < src.size().height; row++) {
			for (int col = 0; col < src.size().width; col++)
			{
				int num = 0;

				int min = 255, max = 0;
				vector<int>pixelsR, pixelsG, pixelsB;
				Vec3b pixel_new = 0;
				for (int j = 0; j < size; j++) {
					for (int k = 0; k < size; k++) {
						if ((row + j - anchor[0]) < src.size().height
							&& (row + j - anchor[0]) >= 0
							&& (col + k - anchor[1]) >= 0
							&& (col + k - anchor[1]) < src.size().width) {
							//pixel_new += src.ptr<Vec3b>(row + j - anchor[0])[col + k - anchor[1]];
							pixelsB.push_back(src.ptr<Vec3b>(row + j - anchor[0])[col + k - anchor[1]][0]);
							pixelsG.push_back(src.ptr<Vec3b>(row + j - anchor[0])[col + k - anchor[1]][1]);

							pixelsR.push_back(src.ptr<Vec3b>(row + j - anchor[0])[col + k - anchor[1]][2]);

							num++;
						}

					}
				}
				pixel_new[0] = findMedian(pixelsB);
				pixel_new[1] = findMedian(pixelsG);
				pixel_new[2] = findMedian(pixelsR);

				dst.ptr<Vec3b>(row)[col] = pixel_new;
			}

		}
	}
}





void GetGaussianKernel(/*vector<vector<double>> &*/double** gaus, const int size, const double sigma,Vec2b anchor)
{
	 
	
	double sum = 0;
	for (int i = 0; i<size; i++)
	{
		for (int j = 0; j<size; j++)
		{
			gaus[i][j] = (1 / (2 * PI*sigma*sigma))*exp(-((i - anchor[0])*(i - anchor[0]) + (j - anchor[1])*(j - anchor[1])) / (2 * sigma*sigma));
			sum += gaus[i][j];
		}
	}

	for (int i = 0; i<size; i++)
	{
		for (int j = 0; j<size; j++)
		{
			gaus[i][j] /= sum;
			cout <<"高斯卷积核"<< gaus[i][j] << "  ";
		}
		cout << endl;
	}
	return;
}
void GausFilter(cv::Mat& src, cv::Mat& dst, const double sigma, const int size, Vec2i anchor) {
	dst.create(src.size(), src.type());
	cout << sigma << " "<<size;
	if (anchor == Vec2i(-1, -1)) {
		anchor = (size / 2, size / 2);
	}
	//vector<vector<double>> gauss(size,vector<double>(size));
	double **gauss = new double *[size];
	for (int i = 0; i<size; i++)
	{
		gauss[i] = new double[size];  //动态生成矩阵  
	}
	GetGaussianKernel(gauss, size, sigma, anchor);

	if (src.type() == CV_8UC1) {
		for (int row = 0; row < src.size().height; row++) {
			for (int col = 0; col < src.size().width; col++)
			{

				int num = 0;
				uchar pixel_new = 0;

				for (int j = 0; j < size; j++) {
					for (int k = 0; k < size; k++) {
						if ((row + j - anchor[0]) < src.size().height
							&& (row + j - anchor[0]) >= 0
							&& (col + k - anchor[1]) >= 0
							&& (col + k - anchor[1]) < src.size().width) {
							int pixel = src.ptr<uchar>(row + j - anchor[0])[col + k - anchor[1]];

							pixel_new += pixel*gauss[j][k];
							//num++;
						}


					}
				}

				dst.ptr<uchar>(row)[col] = pixel_new;

			}

		}
	}
	else if (src.type() == CV_8UC3) {
		for (int row = 0; row < src.size().height; row++) {
			for (int col = 0; col < src.size().width; col++)
			{
				int num = 0;

				Vec3b pixel = 0;
				Vec3b pixel_new = 0;
				for (int j = 0; j < size; j++) {
					for (int k = 0; k < size; k++) {
						if ((row + j - anchor[0]) < src.size().height
							&& (row + j - anchor[0]) >= 0
							&& (col + k - anchor[1]) >= 0
							&& (col + k - anchor[1]) < src.size().width) {
							pixel = src.ptr<Vec3b>(row + j - anchor[0])[col + k - anchor[1]];
							pixel_new[0] += pixel[0] * gauss[j][k];
							pixel_new[1] += pixel[1] * gauss[j][k];
							pixel_new[2] += pixel[2] * gauss[j][k];

						}

					}
				}

				dst.ptr<Vec3b>(row)[col] = pixel_new;
			}

		}
	}
}



