#include "watershed.h"
#include "color.h"
#include "binary.h"

#include <queue>
using namespace cv;


	
	void  Watershed::orderedStructure(const cv::Mat &src) {
		int width = src.size().width;
		int height = src.size().height;
			 // 根据像素点总数预分配空间
													 /* 将每个像素点信息存入结构体 */
		for (int y = 0; y < height; ++y)
			for (int x = 0; x < width; ++x)
				watershedStructure.push_back(new WatershedPixel(x, y, src.ptr<uchar>(y)[x]));

		/* 计算各个像素点的8连通邻域像素 */
		for (int y = 0; y < height; ++y) {
			int offset = y * width;
			int topOffset = offset + width;
			int bottomOffset = offset - width;

			for (int x = 0; x < width; ++x) {
				int currentindex = x + y*width;	// 当前像素点索引
				WatershedPixel* currentPixel = watershedStructure.at(currentindex);

				if (x - 1 >= 0) {
					currentPixel->addNeighbour(watershedStructure.at(currentindex - 1)); // 左邻域
					if (y - 1 >= 0)	 // 左下角
						currentPixel->addNeighbour(watershedStructure.at(currentindex - 1 - width));
					if (y + 1 < height)	 // 左上角
						currentPixel->addNeighbour(watershedStructure.at(currentindex - 1 + width));
				}

				if (x + 1 < width) {
					currentPixel->addNeighbour(watershedStructure.at(currentindex + 1)); // 右邻域
					if (y - 1 >= 0)	 // 右下角
						currentPixel->addNeighbour(watershedStructure.at(currentindex + 1 - width));
					if (y + 1 < height) // 右上角
						currentPixel->addNeighbour(watershedStructure.at(currentindex + 1 + width));
				}

				if (y - 1 >= 0) // 下邻域
					currentPixel->addNeighbour(watershedStructure.at(currentindex - width));

				if (y + 1 < height)	 // 上邻域
					currentPixel->addNeighbour(watershedStructure.at(currentindex + width));
			}
		}

		/* 根据灰度值对结构体中的所有像素点从小到大进行排序 */
		std::sort(watershedStructure.begin(), watershedStructure.end(), [](WatershedPixel * pl, WatershedPixel * pr) { return pl->getIntHeight() < pr->getIntHeight(); });
	 // 构造结束
			// Apply watershed  
	}
	void Watershed::watershed( cv::Mat&src, cv::Mat&dst,Mat& graydst) {

		Mat gray(src.size(), CV_8UC1);
		dst.create(src.size(),CV_8UC1);
		convertCorlor(src, gray);
		ToBinary(gray, dst,OSTU);

		orderedStructure(gray);//排序像素点
		std::queue<WatershedPixel*> pque;	// 存储像素的临时队列
		int curlab = 0;
		int heightIndex1 = 0;
		int heightIndex2 = 0;

		for (int h = 0; h < 256; ++h) { // h-1 层的 Geodesic SKIZ

			for (int pixelIndex = heightIndex1; pixelIndex < watershedStructure.size(); ++pixelIndex) {
				WatershedPixel* p = watershedStructure[pixelIndex];

				/* 此像素点位于 h+1 层，暂不处理，跳出循环 */
				if (p->getIntHeight() != h) { 
					heightIndex1 = pixelIndex; 
					break; 
				}

				p->setLabelToMASK(); // 标记此像素将被处理

				std::vector<WatershedPixel*> neighbours = p->getNeighbours();
				for (unsigned i = 0; i < neighbours.size(); ++i) {
					WatershedPixel* q = neighbours[i];

					/* 将处于盆地或分水岭的h层的邻接像素点入队 */
					if (q->getLabel() >= 0) { p->setDistance(1); pque.push(p); break; }
				}
			}

			int curdist = 1;
			pque.push(new WatershedPixel());

			while (true) { // 扩展聚水盆地
				WatershedPixel* p = pque.front(); pque.pop();

				if (p->isFICTITIOUS()) {
					if (pque.empty()) { delete p; p = NULL; break; }
					else {
						pque.push(new WatershedPixel());
						curdist++;
						delete p; p = pque.front(); pque.pop();
					}
				}

					std::vector<WatershedPixel*> neighbours = p->getNeighbours();
					for (unsigned i = 0; i < neighbours.size(); ++i) { // 通过检查邻接像素来标记 p
						WatershedPixel* q = neighbours.at(i);

						/* q属于一个存在的盆地或分水线 */
						if ((q->getDistance() <= curdist) && (q->getLabel() >= 0)) {

							if (q->getLabel() > 0) {
								if (p->isLabelMASK())
									p->setLabel(q->getLabel());
								else if (p->getLabel() != q->getLabel())
									p->setLabelToWSHED();
							}
							else if (p->isLabelMASK())
								p->setLabelToWSHED();
						}
						else if (q->isLabelMASK() && (q->getDistance() == 0)) {
							q->setDistance(curdist + 1);
							pque.push(q);
						}
					} // 处理邻接像素的for循环
			} // 扩展盆地的while循环

			  /* 搜寻并处理h层中新的最小值 */
			for (int pixelIndex = heightIndex2; pixelIndex < watershedStructure.size(); pixelIndex++) {
				WatershedPixel* p = watershedStructure.at(pixelIndex);

				/* 此像素点位于 h+1 层，暂不处理，跳出循环 */
				if (p->getIntHeight() != h) { 
					heightIndex2 = pixelIndex;
					break; 
				}

				p->setDistance(0); // 重置距离为0

				if (p->isLabelMASK()) { // 该像素位于新最小值区域
					curlab++;
					p->setLabel(curlab);
					pque.push(p);

					while (!pque.empty()) {
						WatershedPixel* q = pque.front();
						pque.pop();

						std::vector<WatershedPixel*> neighbours = q->getNeighbours();

						for (unsigned i = 0; i < neighbours.size(); i++) { // 检查p2的邻域像素
							WatershedPixel* r = neighbours.at(i);

							if (r->isLabelMASK()) { r->setLabel(curlab); pque.push(r); }
						}
					} // end while
				} // end if
			} // end for
		}
		dst.create(src.size(), CV_8UC1);

		for (int pixelIndex = 0; pixelIndex < watershedStructure.size(); pixelIndex++) {
			WatershedPixel* p = watershedStructure.at(pixelIndex);

			if (p->isLabelWSHED() && !p->allNeighboursAreWSHED()) {
				dst.ptr<uchar>(p->getY())[p->getX()] = (char)255; // 在黑色背景中绘制白色分水线
				gray.ptr<uchar>(p->getY())[p->getX()] = (char)255;	// 在灰度图中绘制白色分水线
			}
		}

	}

	// Return result in the form of an image  
	cv::Mat Watershed::getmarkers() {

		cv::Mat tmp;
		// all segment with label higher than 255  
		// will be assigned value 255  
		markers.convertTo(tmp, CV_8U);

		return tmp;
	}

	// Return watershed in the form of an image以图像的形式返回分水岭  
	cv::Mat Watershed::getWatersheds() {

		cv::Mat tmp;
		//在变换前，把每个像素p转换为255p+255（在conertTo中实现）  
		markers.convertTo(tmp, CV_8U, 255, 255);

		return tmp;
	}

	bool watershed(Mat&src, Mat&dst, std::vector<Vec2i>seeds) {
		return true;
	}