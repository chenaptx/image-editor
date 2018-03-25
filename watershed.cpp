#include "watershed.h"
#include "color.h"
#include "binary.h"

#include <queue>
using namespace cv;


	
	void  Watershed::orderedStructure(const cv::Mat &src) {
		int width = src.size().width;
		int height = src.size().height;
			 // �������ص�����Ԥ����ռ�
													 /* ��ÿ�����ص���Ϣ����ṹ�� */
		for (int y = 0; y < height; ++y)
			for (int x = 0; x < width; ++x)
				watershedStructure.push_back(new WatershedPixel(x, y, src.ptr<uchar>(y)[x]));

		/* ����������ص��8��ͨ�������� */
		for (int y = 0; y < height; ++y) {
			int offset = y * width;
			int topOffset = offset + width;
			int bottomOffset = offset - width;

			for (int x = 0; x < width; ++x) {
				int currentindex = x + y*width;	// ��ǰ���ص�����
				WatershedPixel* currentPixel = watershedStructure.at(currentindex);

				if (x - 1 >= 0) {
					currentPixel->addNeighbour(watershedStructure.at(currentindex - 1)); // ������
					if (y - 1 >= 0)	 // ���½�
						currentPixel->addNeighbour(watershedStructure.at(currentindex - 1 - width));
					if (y + 1 < height)	 // ���Ͻ�
						currentPixel->addNeighbour(watershedStructure.at(currentindex - 1 + width));
				}

				if (x + 1 < width) {
					currentPixel->addNeighbour(watershedStructure.at(currentindex + 1)); // ������
					if (y - 1 >= 0)	 // ���½�
						currentPixel->addNeighbour(watershedStructure.at(currentindex + 1 - width));
					if (y + 1 < height) // ���Ͻ�
						currentPixel->addNeighbour(watershedStructure.at(currentindex + 1 + width));
				}

				if (y - 1 >= 0) // ������
					currentPixel->addNeighbour(watershedStructure.at(currentindex - width));

				if (y + 1 < height)	 // ������
					currentPixel->addNeighbour(watershedStructure.at(currentindex + width));
			}
		}

		/* ���ݻҶ�ֵ�Խṹ���е��������ص��С����������� */
		std::sort(watershedStructure.begin(), watershedStructure.end(), [](WatershedPixel * pl, WatershedPixel * pr) { return pl->getIntHeight() < pr->getIntHeight(); });
	 // �������
			// Apply watershed  
	}
	void Watershed::watershed( cv::Mat&src, cv::Mat&dst,Mat& graydst) {

		Mat gray(src.size(), CV_8UC1);
		dst.create(src.size(),CV_8UC1);
		convertCorlor(src, gray);
		ToBinary(gray, dst,OSTU);

		orderedStructure(gray);//�������ص�
		std::queue<WatershedPixel*> pque;	// �洢���ص���ʱ����
		int curlab = 0;
		int heightIndex1 = 0;
		int heightIndex2 = 0;

		for (int h = 0; h < 256; ++h) { // h-1 ��� Geodesic SKIZ

			for (int pixelIndex = heightIndex1; pixelIndex < watershedStructure.size(); ++pixelIndex) {
				WatershedPixel* p = watershedStructure[pixelIndex];

				/* �����ص�λ�� h+1 �㣬�ݲ���������ѭ�� */
				if (p->getIntHeight() != h) { 
					heightIndex1 = pixelIndex; 
					break; 
				}

				p->setLabelToMASK(); // ��Ǵ����ؽ�������

				std::vector<WatershedPixel*> neighbours = p->getNeighbours();
				for (unsigned i = 0; i < neighbours.size(); ++i) {
					WatershedPixel* q = neighbours[i];

					/* ��������ػ��ˮ���h����ڽ����ص���� */
					if (q->getLabel() >= 0) { p->setDistance(1); pque.push(p); break; }
				}
			}

			int curdist = 1;
			pque.push(new WatershedPixel());

			while (true) { // ��չ��ˮ���
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
					for (unsigned i = 0; i < neighbours.size(); ++i) { // ͨ������ڽ���������� p
						WatershedPixel* q = neighbours.at(i);

						/* q����һ�����ڵ���ػ��ˮ�� */
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
					} // �����ڽ����ص�forѭ��
			} // ��չ��ص�whileѭ��

			  /* ��Ѱ������h�����µ���Сֵ */
			for (int pixelIndex = heightIndex2; pixelIndex < watershedStructure.size(); pixelIndex++) {
				WatershedPixel* p = watershedStructure.at(pixelIndex);

				/* �����ص�λ�� h+1 �㣬�ݲ���������ѭ�� */
				if (p->getIntHeight() != h) { 
					heightIndex2 = pixelIndex;
					break; 
				}

				p->setDistance(0); // ���þ���Ϊ0

				if (p->isLabelMASK()) { // ������λ������Сֵ����
					curlab++;
					p->setLabel(curlab);
					pque.push(p);

					while (!pque.empty()) {
						WatershedPixel* q = pque.front();
						pque.pop();

						std::vector<WatershedPixel*> neighbours = q->getNeighbours();

						for (unsigned i = 0; i < neighbours.size(); i++) { // ���p2����������
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
				dst.ptr<uchar>(p->getY())[p->getX()] = (char)255; // �ں�ɫ�����л��ư�ɫ��ˮ��
				gray.ptr<uchar>(p->getY())[p->getX()] = (char)255;	// �ڻҶ�ͼ�л��ư�ɫ��ˮ��
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

	// Return watershed in the form of an image��ͼ�����ʽ���ط�ˮ��  
	cv::Mat Watershed::getWatersheds() {

		cv::Mat tmp;
		//�ڱ任ǰ����ÿ������pת��Ϊ255p+255����conertTo��ʵ�֣�  
		markers.convertTo(tmp, CV_8U, 255, 255);

		return tmp;
	}

	bool watershed(Mat&src, Mat&dst, std::vector<Vec2i>seeds) {
		return true;
	}