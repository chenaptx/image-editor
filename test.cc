#include <node.h>
#include <v8.h>
#include<opencv2\core\core.hpp>
#include <opencv2\highgui.hpp>
#include<opencv2\imgproc.hpp> 

#include "all.h"


using namespace v8;

vector<string> tmps = { "tmp0.jpg","tmp1.jpg" ,"tmp2.jpg" ,"tmp3.jpg" ,"tmp4.jpg" ,"tmp5.jpg" ,"tmp6.jpg" ,"tmp7.jpg" };

void Convert(const v8::FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);
	vector<string> argv;
	
	int size = args.Length();
	
	for (int i = 0; i < size; i++) {
		Local<String> str = Local<String>::Cast(args[i]);
		String::Utf8Value utfValue(str);
		argv.push_back(string(*utfValue));
	}
	if (argv.size() <=1) {
		args.GetReturnValue().Set((String::NewFromUtf8(isolate, "wrong too few argvs")));
	}
	else {
		cv::Mat src = cv::imread(argv[0]);
		cv::Mat res;		
		//int hue;

		int t=0;
		for (int i = 0; i < tmps.size(); i++) {
			if (tmps[i] == argv[0]) {
				t = (i+1)%8;
			}
		}
		string newaddr = argv[0].substr(0, argv[0].find_last_of("."))+'-' + argv[1][0]+ argv[1][1] + ".jpg";
		cout <<'\n'<< newaddr;
		stringstream ss;
		if (argv[1] == "Gray") {
			
			convertCorlor(src, res);

		
			
			
		}
		else if (argv[1] == "Green") {
			convertCorlor(src, res, RGB_Green);

		}
		else if (argv[1] == "Blue") {
			convertCorlor(src, res, RGB_Blue);

		}
		else if (argv[1] == "Red") {
			convertCorlor(src, res, RGB_Red);

		}
		else if(argv[1] == "hue"){
			int hue;
			ss << argv[2];
			ss >> hue;
			
			hue =( hue +180)%360;
			cout << "hue" << hue << "\n";
			setHSL(src, res, hue, 0, 0);

		}
		else if (argv[1] == "saturation") {
			int saturation;
			ss << argv[2];
			ss >> saturation; 
			
			saturation -= 100;
			cout << "saturation" << saturation << "\n";

			setHSL(src, res, 0, saturation, 0);

		}
		else if (argv[1] == "lightness") {
			int lightness;
			ss << argv[2];
			ss >> lightness;
			lightness-=100;
			cout << argv[1] <<lightness<< "\n";

			setHSL(src, res, 0, 0, lightness);

		}
		else if (argv[1] == "contrast") {
			double contrast;
			ss << argv[2];
			ss >> contrast;
			cout << argv[1] << "\n";

			contrast = contrast / 200.0+0.5;
			enhanceLinear(src,res,LINEAR1, contrast);

		}
		else if (argv[1] == "autocontrast") {
			cout << "\n" << argv[1] ;
			histoEqual(src, res);//??????????????

		}
		else if (argv[1] == "gauss") {
			int size;
			double sigma;
			int x, y;
			stringstream ss(argv[2]);
			ss >> size;
			stringstream s1(argv[3]);
			s1 >> sigma;
			ss.clear();
			ss.str("");
			ss<<argv[4];
			ss >> x;
			ss.clear();
			ss.str("");
			ss << argv[5];
			ss >> y;
			
			cout << argv[1]<<size << " " <<sigma << " " << x << " " << y;;
			GausFilter(src, res,  sigma, size);
			

		}
		else if (argv[1] == "median") {
			int size;
			
			int x, y;
			ss << argv[2];
			ss >> size;
			ss.str("");
			ss << argv[3];
			ss >> x;
			ss.str("");
			ss << argv[4];
			ss>> y;
			cout << "C++" << argv[1] << size << x << y;;

			MedianFilter(src, res, size);

		}
		else if (argv[1] == "mean") {
			int size;

			int x, y;
			ss << argv[2];
			ss >> size;
			ss.str("");
			ss << argv[3];
			ss >> x;
			ss.str("");
			ss << argv[4];
			ss >> y;
			cout <<"C++"<< argv[1] << size << x << y;;

			MeanFilter(src, res, size);

		}
		else if (argv[1] == "add") {
			string newone;
			ss << argv[2];
			ss >> newone ;
			cv::Mat one=cv::imread(newone);
			cout << "C++" << argv[1] << newone;

			res=Add(src, one);
		}
		else if (argv[1] == "substract") {
			string newone;
			ss << argv[2];
			ss >> newone;
			cv::Mat one = cv::imread(newone);
			cout << "C++" << argv[1] << newone;

			res = Subtract(src, one);
		}
		else if (argv[1] == "mutiply") {
			string newone;
			ss << argv[2];
			ss >> newone;
			cv::Mat one = cv::imread(newone);
			cout << "C++" << argv[1] << newone;

			res = Multiply(src, one);
		}
		else if (argv[1] == "divide") {
			string newone;
			ss << argv[2];
			ss >> newone;
			cout << "C++" << argv[1] << newone;

			cv::Mat one = cv::imread(newone);
			res = Divide(src, one);
		}
		else if (argv[1] == "scale") {
			float scale;
			ss << argv[2];
			ss >> scale;
			scale = scale / 100.0;
			cout << "C++" << argv[1] << scale;

			res=Scale(src, scale);
		}
		else if (argv[1] == "rotate") {
			float rotate;
			ss << argv[2];
			ss >> rotate;
			rotate = rotate / 180.0*PI;
			cout << "C++" << argv[1] << rotate;
			res = Rotate(src, rotate);
		}
		else if (argv[1] == "cut") {
			int x, y, w, h;
			stringstream ss(argv[2]);
			ss >> x;
			ss.str("");
			ss.clear();
			ss << argv[3];
			ss >> y;
			ss.str("");
			ss.clear();
			ss << argv[4];
			ss >> w;
			ss.str("");
			ss.clear();
			ss << argv[5];
			ss >> h;
			cout << "C++" << argv[1] << argv[2] << argv[3] << argv[4] << argv[5]<<x<<y<<w<<h;
			res = Cut(src, x,y,w,h);
		}
		else if (argv[1] == "Binaryostu") {
			cout << argv[1] << argv[0];
			ToBinary(src, res, OSTU);
		}
		else if (argv[1] == "Binary") {

			int low, high;
			ss << argv[2];
			ss >> low;
			stringstream ss1;
			ss1 << argv[3];
			ss1 >> high;
			cout << "C++" << argv[1] << low << high;

			ToBinary(src, res, type_binary::DOUBLE, low, high);
		}
		else if (argv[1] == "detectsobel") {
			EdgeDetect(src, res, SOBEL);
		}
		else if (argv[1] == "detectlaplace") {
			EdgeDetect(src, res, LAPLACE);
		}
		else if (argv[1] == "detectcanny") {
			EdgeDetect(src, res, CANNY);
		}
		else if (argv[1] == "watershed") {
			Watershed w;
			cv::Mat dst;
			w.watershed(src, res, dst);
		}
		else {
			src = cv::imread(argv[0], CV_LOAD_IMAGE_GRAYSCALE);
		if (argv[1] == "distancetransform") {
			cout << "C++" << argv[1];

			DistanceTransform2(src, res);// OK
		}
		else if (argv[1] == "skeleton") {
			cout << "C++" << argv[1];

			skeleton(src, res);
		}
		else if (argv[1] == "skeletonrebuild") {
			cout << "C++" << argv[1];

			skeletonRebuild(src, res);
		}
		else if (argv[1] == "dilate") {
			string elems = argv[2];
			cv::Vec2i anchor(-1, -1);
			vector<vector<int>> elem = getElem(elems, anchor);


			Dilate(src, res, elem, anchor);
		}
		else if (argv[1] == "erode") {
			string elems = argv[2];
			cv::Vec2i anchor(-1, -1);
			vector<vector<int>> elem = getElem(elems, anchor);
			Erode(src, res, elem, anchor);
		}
		else if (argv[1] == "openning") {
			string elems = argv[2];
			cv::Vec2i anchor(-1, -1);
			vector<vector<int>> elem = getElem(elems, anchor);
			openning(src, res, elem, anchor);
		}
		else if (argv[1] == "closing") {
			string elems = argv[2];
			cv::Vec2i anchor(-1, -1);
			vector<vector<int>> elem = getElem(elems, anchor);
			closing(src, res, elem, anchor);
		}
		else if (argv[1] == "Mrebuild") {
			string elems = argv[2];
			cv::Vec2i anchor(-1, -1);
			vector<vector<int>> elem = getElem(elems, anchor);
			rebuild(src, res, src, elem);
		}
		else if (argv[1] == "thick") {
			string str = argv[2], windows = argv[3];
			cv::Vec2i anchor(-1, -1), anchor1(-1, -1);
			cout <<"C++"<< str << "\n" << windows;
			vector<vector<int>> elem = getElem(str, anchor);
			vector<vector<int>> window = getElem(windows, anchor1);
			vector<vector<vector<int>>> elems = stdelemt;
			Thickening(src, res, elems, cv::Vec2i(-1, -1), window, cv::Vec2i(-1, -1));
		}
		else if (argv[1] == "thin") {
			string str = argv[2], windows = argv[3];
			cv::Vec2i anchor(-1, -1), anchor1(-1, -1);
			vector<vector<int>> elem = getElem(str, anchor);
			vector<vector<int>> window = getElem(windows, anchor1);
			
			vector<vector<vector<int>>> elems = stdelemt;
			Thinning(src, res, elems, cv::Vec2i(-1, -1), window, cv::Vec2i(-1, -1));

		}
		else if (argv[1] == "Gdilate") {
			string elems = argv[2];
			cv::Vec2i anchor(-1, -1);
			vector<vector<int>> elem = getElem(elems, anchor);


			GDilate(src, res, elem, anchor);
		}
		else if (argv[1] == "Gerode") {
			string elems = argv[2];
			cv::Vec2i anchor(-1, -1);
			vector<vector<int>> elem = getElem(elems, anchor);
			GErode(src, res, elem, anchor);
		}
		else if (argv[1] == "Gopenning") {
			string elems = argv[2];
			cv::Vec2i anchor(-1, -1);
			vector<vector<int>> elem = getElem(elems, anchor);
			Gopening(src, res, elem, anchor);
		}
		else if (argv[1] == "Gclosing") {
			string elems = argv[2];
			cv::Vec2i anchor(-1, -1);
			vector<vector<int>> elem = getElem(elems, anchor);
			Gclosing(src, res, elem, anchor);
		}
		else if (argv[1] == "GMrebuild") {
			string elems = argv[2];
			cv::Vec2i anchor(-1, -1);
			vector<vector<int>> elem = getElem(elems, anchor);
			Grebuild(src, res, src, elem);
		}
		}
		
		cout << newaddr;
		cv::imwrite(newaddr, res);
		//
		args.GetReturnValue().Set((String::NewFromUtf8(isolate, newaddr.c_str())));
	}

	
}

void Init(Handle<Object> exports) {
Isolate* isolate = Isolate::GetCurrent();
exports->Set(String::NewFromUtf8(isolate, "Convert"),
FunctionTemplate::New(isolate, Convert)->GetFunction());
}

NODE_MODULE(test, Init)