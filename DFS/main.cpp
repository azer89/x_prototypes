
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <queue>
#include <algorithm>
#include <iostream>

//#include "DijkstraTest.h"
#include "DFSTest.h"

int main()
{
	// ===== inputs =====
	cv::Mat srcImg;
	srcImg = cv::imread("C:\\Users\\saputra\\workspace\\Images\\testy.png", CV_LOAD_IMAGE_GRAYSCALE);

	/*int dilation_type = cv::MORPH_RECT;
	int dilation_size = 3;
	cv::Mat element = cv::getStructuringElement(dilation_type, cv::Size(2 * dilation_size + 1, 2 * dilation_size + 1), cv::Point(dilation_size, dilation_size));
	dilate(srcImg, srcImg, element); // Apply the dilation operation*/
	
	cv::Point2i start(10, 41);
	cv::Point2i goal(83, 86);

	//cv::Point2i start(0, srcImg.rows - 1); // Start
	//cv::Point2i goal(srcImg.cols - 1, 0); // Goal	

	// ===== perform dijkstra =====
	//DijkstraTest dt;
	//dt.Dijkstra(srcImg, start, goal);

	DFSTest dfsTest;
	dfsTest.DFS(srcImg, start, goal);

	//std::getchar();

	return 0;
}
