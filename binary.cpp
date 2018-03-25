#include "binary.h"
#include"color.h"

void ToBinary(cv::Mat & src, cv::Mat & dst, type_binary type, uchar valueLow , uchar valueHigh) {
	if (src.type() != CV_8UC1) {
		cv::Mat src1;
		cout << "error: do not convert a RGB imge to a binay image\n";
		convertCorlor(src, src1);
		src = src1;
	}
	
		dst.create(src.size(), src.type());
		if (type == OSTU) {
			vector<int> counts(256, 0);
			histogramGray(src, counts);
			float averageLow = 0, averageHigh = 0;;
			int sumLow = 0, numLow = 0, sumHigh = 0, numHigh = 0;
			vector<float> variance(256, 0);
			float minVariance = INT_MAX;
			int minVarPixel = 0;


			for (int j = 1; j < 255; j++) {
				numLow = 0;
				for (int i = 0; i < j; i++) {
					sumLow += counts[i] * i;
					numLow += counts[i];
				}
				for (int i = j; i <= 255; i++) {
					sumHigh += counts[i] * i;
					numHigh += counts[i];
				}
				averageLow = sumLow * 1.0 / numLow;
				averageHigh = sumHigh * 1.0 / numHigh;


				for (int i = 0; i < j; i++) {
					variance[j] += counts[i] * (i - averageLow)*(i - averageLow);
				}
				for (int i = j; i <=255; i++) {
					variance[j] += counts[i] * (i - averageHigh)*(i - averageHigh);
				}
				variance[j] = variance[j] / (numLow + numHigh);

				if (minVariance > variance[j]) {
					minVarPixel = j;
					minVariance = variance[j];
				}
			}
			for (int row = 0; row < src.size().height; row++) {
				for (int col = 0; col < src.size().width; col++)
				{
					if (src.ptr(row)[col] > minVarPixel) {
						dst.ptr(row)[col] = 255;
					}
					else {
						dst.ptr(row)[col] = 0;

					}
				}
			}

		}
		else if (type = DOUBLE) {
			for (int row = 0; row < src.size().height; row++) {
				for (int col = 0; col < src.size().width; col++) {
					uchar pixel = src.ptr(row)[col];
					if (pixel<=valueHigh &&pixel>valueLow) {
						dst.ptr(row)[col] = 255;
					}
					else {
						dst.ptr(row)[col] = 0;
					}

				}
			}
		}
	
}