#include "color.h"
using namespace cv;

bool convertCorlor(cv::Mat & src, cv::Mat& dst, type_color type) {
	Size size = src.size();
	int imgtype = type ? src.type() : src.type() % 8;
	dst.create(size, imgtype);
	if (type == RGB_Gray) {

		for (int row = 0; row < size.height; row++) {
			for (int col = 0; col < size.width; col++)
			{
				Vec3b pixel_value = src.ptr<Vec3b>(row)[col];
				uchar p_value = (0.11*pixel_value[0] + 0.59*pixel_value[1] + 0.3*pixel_value[2]) / 1;

				dst.ptr<uchar>(row)[col] = p_value;

			}
		}
	}
	else if (type == RGB_Blue) {

		for (int row = 0; row < size.height; row++) {
			for (int col = 0; col < size.width; col++)
			{
				Vec3b pixel_value = src.ptr<Vec3b>(row)[col];
				Vec3b p_value = Vec3b(pixel_value[0], 0, 0);

				dst.ptr<Vec3b>(row)[col] = p_value;
			}
		}
	}
	else if (type == RGB_Green) {

		for (int row = 0; row < size.height; row++) {
			for (int col = 0; col < size.width; col++)
			{
				Vec3b pixel_value = src.ptr<Vec3b>(row)[col];
				Vec3b p_value = Vec3b(0, pixel_value[1], 0);

				dst.ptr<Vec3b>(row)[col] = p_value;
			}
		}
	}
	else if (type == RGB_Red) {

		for (int row = 0; row < size.height; row++) {
			for (int col = 0; col < size.width; col++)
			{
				Vec3b pixel_value = src.ptr<Vec3b>(row)[col];
				Vec3b p_value = Vec3b(0, 0, pixel_value[2]);

				dst.ptr<Vec3b>(row)[col] = p_value;
			}
		}
	}


	return true;
}

void RGB2HSL(Vec3b pixel_value, int &H, double &S, double &L) {
	double R = pixel_value[2] * 1.0 / 255;
	double G = pixel_value[1] * 1.0 / 255;
	double B = pixel_value[0] * 1.0 / 255;
	double Cmax, Cmin, d;

	Cmax = MAX(B, G);
	Cmax = MAX(Cmax, R);

	Cmin = MIN(B, G);
	Cmin = MIN(Cmin, R);

	d = Cmax - Cmin;

	//Vec3f HSL;
	L = (Cmax + Cmin) / 2;
	S = (d != 0) ? (d / (1 - abs(2 * L - 1))) : 0;

	if (d == 0) {
		H = 0;
	}
	else if (Cmax == R) {
		if (G > B) {
			H = ((G - B) / d + 0) * 60;
		}
		else if (G > B) {
			H = ((G - B) / d + 6) * 60;
		}

	}
	else if (Cmax == G) {
		H = ((B - R) / d + 2) * 60;
	}
	else if (Cmax == B) {
		H = ((B - R) / d + 4) * 60;
	};


}
void HSL2RGB(int &H, double &S, double &L, Vec3b& p_value) {
	double q;
	if (L >= 0.5) {
		q = L + S - (L*S);
	}
	else {
		q = L*(1 + S);
	}
	double p;
	p = 2 * L - q;
	//double hk = H / 360;
	double t[3];
	for (int i = 0; i < 3; i++) {
		t[i] = (H + 120 * (i - 1)) *1.0 / 360;
		t[i] = t[i] > 0 ? t[i] : (t[i] + 1);
	}


	for (int i = 0; i < 3; i++) {
		if (t[i] < 1.0 / 6) {
			p_value[i] = 255 * (p + ((q - p) * 6 * t[i]));
		}
		else if (t[i] < 1.0 / 2 && t[i] >= 1 / 6) {
			p_value[i] = 255 * q;
		}
		else if (t[i] >= 1.0 / 2 && t[i] < 2 / 3) {
			p_value[i] = 255 * (p + ((q - p) * 6 * (2 / 3 - t[i])));
		}
		else {
			p_value[i] = 255 * p;
		}
		//p_value[i] = p_value[i] * 255;

	}




}
void RGB2HSV(Vec3b pixel_value, int &H, double &S, double &V) {
	double R = pixel_value[2] * 1.0 / 255;
	double G = pixel_value[1] * 1.0 / 255;
	double B = pixel_value[0] * 1.0 / 255;
	double Cmax, Cmin, d;

	Cmax = MAX(B, G);
	Cmax = MAX(Cmax, R);

	Cmin = MIN(B, G);
	Cmin = MIN(Cmin, R);

	d = Cmax - Cmin;

	//Vec3f HSL;
	if (d == 0) {
		H = 0;
	}
	else if (Cmax == R) {
		if (G > B) {
			H = ((G - B) / d + 0) * 60;
		}
		else if (G > B) {
			H = ((G - B) / d + 6) * 60;
		}

	}
	else if (Cmax == G) {
		H = ((B - R) / d + 2) * 60;
	}
	else if (Cmax == B) {
		H = ((B - R) / d + 4) * 60;
	};

	V = Cmax;
	if (Cmax == 0) {
		S = 0;
	}
	else {
		S = (Cmax - Cmin) / Cmax;
	}

}
void HSV2RGB(int &H, double &S, double &V, Vec3b& p_value) {
	int hi = H / 60;
	hi = hi % 6;
	double f = H / 60.0 - hi;
	double min, q, t;
	min = V*(1 - S);
	q = V*(1 - f*S);
	t = V*(1 - (1 - f)*S);
	switch (hi) {
	case 0:
		p_value[2] = V * 255;
		p_value[1] = t * 255;
		p_value[0] = min * 255;
	case 1:
		p_value[2] = q * 255;
		p_value[1] = V * 255;
		p_value[0] = min * 255;
	case 2:
		p_value[2] = min * 255;
		p_value[1] = V * 255;
		p_value[0] = t * 255;
	case 3:
		p_value[2] = min * 255;
		p_value[1] = q * 255;
		p_value[0] = V * 255;
	case 4:
		p_value[2] = t * 255;
		p_value[1] = min * 255;
		p_value[0] = V * 255;
	case 5:
		p_value[2] = V * 255;
		p_value[1] = min * 255;
		p_value[0] = q * 255;

	}
}

bool setHSV(cv::Mat& src, cv::Mat& dst, int H_c, int S_c, int V_c) {
	cv::Mat temp(src.size(), src.type());
	dst.create(src.size(), src.type());


	for (int row = 0; row < src.size().height; row++) {
		for (int col = 0; col < src.size().width; col++)
		{
			Vec3b pixel_value = src.ptr<Vec3b>(row)[col];

			Vec3b p_value;
			int H;
			double  S, V, temp;
			RGB2HSV(pixel_value, H, S, V);

			H = (H + H_c) % 360;
			if (S_c != 0) {
				S = S*(S_c + 100) / 100.0;
				if (S >= 1)
					S = 1;
			}
			if (V_c != 0) {
				temp = (100 + V_c) / 100.0;
				V = (temp*V)>1 ? 1 : (temp*V);
			}

			HSV2RGB(H, S, V, p_value);
		
			dst.ptr<Vec3b>(row)[col] = p_value;

		}
	}
	return true;

}
bool setHSL(cv::Mat& src, cv::Mat& dst, int H_c, int S_c, int L_c) {
	//cv::Mat temp(src.size(), src.type());
	dst.create(src.size(), src.type());


	for (int row = 0; row < src.size().height; row++) {
		for (int col = 0; col < src.size().width; col++)
		{
			Vec3b pixel_value = src.ptr<Vec3b>(row)[col];

			Vec3b p_value;
			double S, L, temp=0;
			int H;
			RGB2HSL(pixel_value, H, S, L);
			if (H_c % 360 == 0) {
				H = (H + H_c) % 360;
			}
			if (S_c > 0) {
				
				temp =  S >(1 - (S_c/100.0))?S:(1- (S_c / 100.0));
				temp = 1 / temp - 1;
			}
			else if(S_c < 0) {
				temp = S_c / 100.0;
			}
			S = S*(1 + temp);
			if (L_c != 0) {
				temp = (100 + L_c) / 100.0;
				L = (temp*L)>1 ? 1 : (temp*L);
			}
			HSL2RGB(H, S, L, p_value);


			
			dst.ptr<Vec3b>(row)[col] = p_value;
		}
	}
	return true;

};
bool enhanceLinear(cv::Mat& src, cv::Mat& dst, type_contrast type, float contrast, std::vector<Vec2b>& points) {
	dst.create(src.size(), src.type());

	if (type == LINEAR1) {
		int threshold = 128;
		Vec3b thresholds = (128, 128, 128);
		if (src.type() == CV_8UC3) {
			for (int row = 0; row < src.size().height; row++) {
				for (int col = 0; col < src.size().width; col++)
				{

					Vec3b pixel_value = src.ptr<Vec3b>(row)[col];
					Vec3b pixel_new = (thresholds + (pixel_value - thresholds) * (1 + contrast));
					pixel_new[0] = saturate_cast<uchar>(pixel_new[0]);
					pixel_new[1] = saturate_cast<uchar>(pixel_new[1]);
					pixel_new[2] = saturate_cast<uchar>(pixel_new[2]);

					dst.ptr<Vec3b>(row)[col] = pixel_new;


				}
			}
		}
		else if (src.type() == CV_8UC1) {
			for (int row = 0; row < src.size().height; row++) {
				for (int col = 0; col < src.size().width; col++)
				{

					uchar pixel_value = src.ptr<uchar>(row)[col];
					uchar pixel_new = (pixel_value + (pixel_value - threshold) * (1 + contrast));

					dst.ptr<uchar>(row)[col] = pixel_new;


				}
			}
		}

	}
	else if (type == LINEARN) {
		vector<int> counts(256, 0);
		if (src.type() == CV_8UC3) {

			for (int row = 0; row < src.size().height; row++) {
				for (int col = 0; col < src.size().width; col++)
				{

					Vec3b pixel_value = src.ptr<Vec3b>(row)[col];

					int h;
					double s, l;
					RGB2HSL(pixel_value, h, s, l);
					int L = floor(l * 255 + 0.5);
					counts[L]++;

					for (int i = 0; i < points.size(); i++) {
						if (L < points[i + 1][0] && L >= points[i][0]) {
							L = points[i][1] + (L - points[i][0])*(points[i + 1][1] - points[i][1]) / (points[i + 1][0] - points[i][0]);
							double l = L*1.0 / 255;
							HSL2RGB(h, s, l, pixel_value);
						}
					}

					dst.ptr<Vec3b>(row)[col] = pixel_value;


				}
			}
		}
		else if (src.type() == CV_8UC1) {
			for (int row = 0; row < src.size().height; row++) {
				for (int col = 0; col < src.size().width; col++)
				{

					uchar pixel_value = src.ptr<uchar>(row)[col];
					uchar pixel_new;// = (pixel_value + (pixel_value - threshold) * (1 + contrast));
					for (int i = 0; i < points.size(); i++) {
						if (pixel_value < points[i + 1][0] && pixel_value >= points[i][0]) {
							pixel_new = points[i][1] + (pixel_value - points[i][0])*(points[i + 1][1] - points[i][1]) / (points[i + 1][0] - points[i][0]);

						}
					}
					dst.ptr<uchar>(row)[col] = pixel_new;


				}
			}
		}

	}
	
	else if (type == HISTO_EQUAL) {
		histoEqual(src, dst);
	}
	return true;

}
void enhanceLE(cv::Mat& src, cv::Mat& dst, type_contrast type, float contrast,float power) {
	if (type == LOG) {
		if (src.type() == CV_8UC3) {
			for (int row = 0; row < src.size().height; row++) {
				for (int col = 0; col < src.size().width; col++)
				{

					Vec3b pixel_value = src.ptr<Vec3b>(row)[col];
					Vec3b pixel_new;// = (thresholds + (pixel_value - thresholds) * (1 + contrast));
					pixel_new[0] = saturate_cast<uchar>(log(pixel_value[0] + 1));
					pixel_new[1] = saturate_cast<uchar>(log(pixel_value[1] + 1));
					pixel_new[2] = saturate_cast<uchar>(log(pixel_value[2] + 1));

					dst.ptr<Vec3b>(row)[col] = pixel_new;


				}
			}
		}
		else if (src.type() == CV_8UC1) {
			for (int row = 0; row < src.size().height; row++) {
				for (int col = 0; col < src.size().width; col++)
				{

					uchar pixel_value = src.ptr<uchar>(row)[col];
					uchar pixel_new = log(pixel_value + 1);//(pixel_value + (pixel_value - threshold) * (1 + contrast));

					dst.ptr<uchar>(row)[col] = pixel_new;


				}
			}
		}

	}

	else if (type == EXP) {
		if (src.type() == CV_8UC3) {
			for (int row = 0; row < src.size().height; row++) {
				for (int col = 0; col < src.size().width; col++)
				{

					Vec3b pixel_value = src.ptr<Vec3b>(row)[col];
					Vec3b pixel_new;// = (thresholds + (pixel_value - thresholds) * (1 + contrast));
					pixel_new[0] = saturate_cast<uchar>(pow(pixel_value[0], power));
					pixel_new[1] = saturate_cast<uchar>(pow(pixel_value[1], power));
					pixel_new[2] = saturate_cast<uchar>(pow(pixel_value[2], power));

					dst.ptr<Vec3b>(row)[col] = pixel_new;


				}
			}
		}
		else if (src.type() == CV_8UC1) {
			for (int row = 0; row < src.size().height; row++) {
				for (int col = 0; col < src.size().width; col++)
				{

					uchar pixel_value = src.ptr<uchar>(row)[col];
					uchar pixel_new = pow(pixel_value, power);// (pixel_value + (pixel_value - threshold) * (1 + contrast));

					dst.ptr<uchar>(row)[col] = pixel_new;


				}
			}
		}

	}
}
void histogramGray(cv::Mat &src, vector<int>&counts) {

	for (int row = 0; row < src.size().height; row++) {
		for (int col = 0; col < src.size().width; col++)
		{

			uchar pixel_value = src.ptr<uchar>(row)[col];
			counts[pixel_value]++;


		}
	}

}


void histogramRGB(cv::Mat&src, vector<int>&counts, vector<int>&countR, vector<int> &countG, vector<int> &countB) {
	if (src.type() == CV_8UC3) {
		countR.reserve(256);
		countG.reserve(256);
		countB.reserve(256);
		for (int row = 0; row < src.size().height; row++) {
			for (int col = 0; col < src.size().width; col++)
			{

				Vec3b pixel_value = src.ptr<Vec3b>(row)[col];
				countB[pixel_value[0]]++;
				countG[pixel_value[0]]++;
				countR[pixel_value[0]]++;
			}
		}
		for (int i = 0; i < 256; i++) {
			counts[i] = (countB[i] + countG[i] + countR[i]) / 3;
		}

	}

}

void histoEqual(cv::Mat&src, cv::Mat&dst) {
	vector<int> counts(256, 0);
	dst.create(src.size(), src.type());
	if (src.type() == CV_8UC1) {
		histogramGray(src, counts);
		vector<int> grays(256, 0);
		int sum = 0;
		for (int i = 0; i < 256; i++) {
			if (i == 0) {
				grays[i] = counts[i];
			}
			else {
				grays[i] = grays[i - 1] + counts[i];
			}
			sum += counts[i];

		}
		for (int row = 0; row < src.size().height; row++) {
			for (int col = 0; col < src.size().width; col++)
			{
				uchar pixel_value = src.ptr<uchar>(row)[col];
				dst.ptr<uchar>(row)[col] = floor((grays[pixel_value] * 1.0 / sum)* 255.0);//+0.5四舍五入
			}
		}

	}
	else if (src.type() == CV_8UC3) {
		vector<int> countR(256, 0);
		vector<int> countG(256, 0);
		vector<int> countB(256, 0);
		vector<int>R(256, 0), G(256, 0), B(256, 0);
		int sumR = 0, sumG = 0, sumB = 0;
		histogramRGB(src, counts, countR, countG, countB);

		for (int i = 0; i < 256; i++) {
			if (i == 0) {
				R[i] = countR[i];
				G[i] = countG[i];
				B[i] = countB[i];

			}
			else {
				R[i] = R[i - 1] + countR[i];
				G[i] = G[i - 1] + countG[i];
				B[i] = B[i - 1] + countB[i];
			}

			sumR += countR[i];
			sumG += countG[i];
			sumB += countB[i];


		}
		for (int row = 0; row < src.size().height; row++) {
			for (int col = 0; col < src.size().width; col++)
			{
				Vec3b pixel_value = src.ptr<Vec3b>(row)[col];
				int b = B[pixel_value[0]];
				pixel_value[0] = floor(b * 255.0 / sumB);
				int g = G[pixel_value[1]];
				pixel_value[1] = floor(g * 255.0 / sumG);
				int r = R[pixel_value[2]];
				pixel_value[2] = floor(r * 255.0 / sumR);
				dst.ptr<Vec3b>(row)[col] = pixel_value;//+0.5四舍五入
			}
		}

	}

}
