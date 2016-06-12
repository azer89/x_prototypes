
#ifndef _DFS_TEST_H_
#define _DFS_TEST_H_

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <queue>
//#include <algorithm>

class DFSTest
{
public:
	DFSTest();
	~DFSTest();

	void DFS(cv::Mat srcImg, cv::Point2i startPoint, cv::Point2i goalPoint);
};


#endif
