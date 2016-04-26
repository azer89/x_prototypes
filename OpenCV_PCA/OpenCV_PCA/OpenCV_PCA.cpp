// OpenCV_PCA.cpp : Defines the entry point for the console application.
// http://docs.opencv.org/3.0-rc1/d1/dee/tutorial_introduction_to_pca.html

#include "stdafx.h"

#include <iostream>
#include <opencv2/opencv.hpp>

#include "PathIO.h"
#include "AVector.h"
#include "AShape.h"
#include "UtilityFunctions.h"
#include "dirent.h"

//using namespace std;
//using namespace cv;

/*
================================================================================
DATA
================================================================================
*/
//std::vector<AShape> _shapes;
std::vector<std::string> _fileStr;
std::string _directoryPath = "D:\\Code\\x_prototypes\\OpenCV_PCA\\OpenCV_PCA\\Bunnies";
float _line_thickness = 2.0f;

/*
================================================================================
Function declarations
================================================================================
*/
void drawAxis(cv::Mat&, cv::Point, cv::Point, cv::Scalar, const float);
double getOrientation(const std::vector<cv::Point> &, cv::Mat&);
void LoadShapes();
void DrawPolygon(cv::Mat& drawing, std::vector<AVector> shape_contours, cv::Scalar color, float xOffset = 0, float yOffset = 0);

/*
================================================================================
LoadShapes
================================================================================
*/
void LoadShapes()
{
	/*===== Stuff =====*/
	//MomentsCalculator* mCalculator = new MomentsCalculator();
	//PathIO* pathIO = new PathIO();

	/*===== clear =====*/
	//_shapes.clear();

	/*=====  dirent =====*/
	DIR *dp;
	struct dirent *ep;

	//std::vector<std::string> fileStr;
	//std::string directoryPath = "D:\\Code\\x_prototypes\\OpenCV_PCA\\OpenCV_PCA\\Bunnies";
	dp = opendir(_directoryPath.c_str());
	if (dp != NULL)
	{
		while (ep = readdir(dp)) { _fileStr.push_back(ep->d_name); }
		(void)closedir(dp);
	}
	else { perror("Couldn't open the directory"); }

	/*for (int a = 0; a < _fileStr.size(); a++)
	{
		if (_fileStr[a] == "." || _fileStr[a] == "..") { continue; }
		std::string fileName = directoryPath + "\\" + _fileStr[a];
		std::cout << "filepath: " << fileName << "\n";
	}*/
}

/*
================================================================================
Function drawAxis
================================================================================
*/
void drawAxis(cv::Mat& img, cv::Point p, cv::Point q, cv::Scalar colour, const float scale = 0.2)
{
	double angle;
	double hypotenuse;
	angle = atan2((double)p.y - q.y, (double)p.x - q.x); // angle in radians
	hypotenuse = sqrt((double)(p.y - q.y) * (p.y - q.y) + (p.x - q.x) * (p.x - q.x));
	// double degrees = angle * 180 / CV_PI; // convert radians to degrees (0-180 range)
	// cout << "Degrees: " << abs(degrees - 180) << endl; // angle in 0-360 degrees range

	// Here we lengthen the arrow by a factor of scale
	q.x = (int)(p.x - scale * hypotenuse * cos(angle));
	q.y = (int)(p.y - scale * hypotenuse * sin(angle));
	line(img, p, q, colour, 1, CV_AA);

	// create the arrow hooks
	p.x = (int)(q.x + 9 * cos(angle + CV_PI / 4));
	p.y = (int)(q.y + 9 * sin(angle + CV_PI / 4));
	line(img, p, q, colour, 1, CV_AA);

	p.x = (int)(q.x + 9 * cos(angle - CV_PI / 4));
	p.y = (int)(q.y + 9 * sin(angle - CV_PI / 4));
	line(img, p, q, colour, 1, CV_AA);
}


/*
================================================================================
Function getOrientation
================================================================================
*/
//double getOrientation(const std::vector<cv::Point> &pts, cv::Mat &img)
double getOrientation(const std::vector<AVector> &pts, cv::Mat &img)
{
	//Construct a buffer used by the pca analysis
	int sz = static_cast<int>(pts.size());
	cv::Mat data_pts = cv::Mat(sz, 2, CV_64FC1);
	for (int i = 0; i < data_pts.rows; ++i)
	{
		data_pts.at<double>(i, 0) = pts[i].x;
		data_pts.at<double>(i, 1) = pts[i].y;
	}

	//Perform PCA analysis
	cv::PCA pca_analysis(data_pts, cv::Mat(), CV_PCA_DATA_AS_ROW);

	//Store the center of the object
	cv::Point cntr = cv::Point(pca_analysis.mean.at<double>(0, 0), pca_analysis.mean.at<double>(0, 1));

	//Store the eigenvalues and eigenvectors
	std::vector<cv::Point2d> eigen_vecs(2);
	std::vector<double> eigen_val(2);
	for (int i = 0; i < 2; ++i)
	{
		eigen_vecs[i] = cv::Point2d(pca_analysis.eigenvectors.at<double>(i, 0), pca_analysis.eigenvectors.at<double>(i, 1));
		eigen_val[i] = pca_analysis.eigenvalues.at<double>(i);
	}

	// Draw the principal components
	circle(img, cntr, 3, cv::Scalar(255, 0, 255), 2);
	cv::Point p1 = cntr + 0.02 * cv::Point(eigen_vecs[0].x * eigen_val[0], eigen_vecs[0].y * eigen_val[0]);
	cv::Point p2 = cntr - 0.02 * cv::Point(eigen_vecs[1].x * eigen_val[1], eigen_vecs[1].y * eigen_val[1]);
	drawAxis(img, cntr, p1, cv::Scalar(0, 255, 0), 1);
	drawAxis(img, cntr, p2, cv::Scalar(255, 255, 0), 5);

	cv::Point p3 = cntr - 0.02 * cv::Point(eigen_vecs[0].x * eigen_val[0], eigen_vecs[0].y * eigen_val[0]);
	cv::Point p4 = cntr + 0.02 * cv::Point(eigen_vecs[1].x * eigen_val[1], eigen_vecs[1].y * eigen_val[1]);
	drawAxis(img, cntr, p3, cv::Scalar(0, 255, 0), 1);
	drawAxis(img, cntr, p4, cv::Scalar(255, 255, 0), 5);

	double angle = atan2(eigen_vecs[0].y, eigen_vecs[0].x); // orientation in radians

	return angle;
}

/*
================================================================================
Drawing function
================================================================================
*/
void DrawPolygon(cv::Mat& drawing, std::vector<AVector> shape_contours, cv::Scalar color, float xOffset, float yOffset)
{
	std::vector<cv::Point2f> new_contours;
	cv::RNG rng(12345);
	for (size_t b = 0; b < shape_contours.size() - 1; b++)
		//for (size_t b = 0; b < 990; b++)
	{

		//float randomX = ((float)rng.uniform(-500, 500)) / 500.0f;
		//float randomY = ((float)rng.uniform(-500, 500)) / 500.0f;
		//std::cout << randomX << " " << randomY << "\n";
		//new_contours.push_back(cv::Point2f((shape_contours[b].x + xOffset + randomX),
		//	                               (shape_contours[b].y + yOffset + randomY))); 
		new_contours.push_back(cv::Point2f((shape_contours[b].x + xOffset),
			(shape_contours[b].y + yOffset)));
	}


	for (size_t b = 0; b < new_contours.size() - 1; b++)
	{
		cv::line(drawing, new_contours[b], new_contours[b + 1], color, _line_thickness);
	}
	cv::line(drawing, new_contours[new_contours.size() - 1], new_contours[0], color, _line_thickness);
}


/*
================================================================================
================================================================================
*/
int _tmain(int argc, _TCHAR* argv[])
{
	PathIO* pathIO = new PathIO();
	//cv::Mat drawing = cv::Mat::zeros(cv::Size(1000, 1000), CV_8UC3);
	std::vector<cv::Mat> drawings;

	LoadShapes();

	cv::RNG rng(12345);
	for (int a = 0; a < _fileStr.size(); a++)
	{
		if (_fileStr[a] == "." || _fileStr[a] == "..") { continue; }
		std::string fileName = _directoryPath + "\\" + _fileStr[a];
		std::cout << "filepath: " << fileName << "\n";

		std::vector<AVector> ori_contour;
		std::vector<AShape> shapes = pathIO->LoadPath(fileName);
		ori_contour = shapes[0].shape_contours;
		std::vector<AVector> shape_contour;
		UtilityFunctions::UniformResample(ori_contour, shape_contour, 1000);


		//if (a == 4)
		//{
		cv::Mat drawing = cv::Mat::zeros(cv::Size(1000, 1000), CV_8UC3);
		getOrientation(shape_contour, drawing);

		cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		DrawPolygon(drawing, shape_contour, color);

		drawings.push_back(drawing);
		//}
	}

	/*
	// Load image
	cv::Mat src = cv::imread("pca_test4.jpg");
	//Mat src = imread(argv[1]);
	// Check if image is loaded successfully
	if (!src.data || src.empty())
	{
		std::cout << "Problem loading image!!!" << std::endl;
		return EXIT_FAILURE;
	}
	imshow("src", src);
	*/

	/*
	// Convert image to grayscale
	cv::Mat gray;
	cvtColor(src, gray, cv::COLOR_BGR2GRAY);
	
	// Convert image to binary
	cv::Mat bw;
	threshold(gray, bw, 50, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	//! [pre-process]

	//imshow("bw", bw);	
	//! [contours]
	// Find all the contours in the thresholded image
	std::vector<cv::Vec4i> hierarchy;
	std::vector<std::vector<cv::Point> > contours;
	findContours(bw, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	for (size_t i = 0; i < contours.size(); ++i)
	{		
		double area = contourArea(contours[i]);	// Calculate the area of each contour		
		if (area < 1e2 || 1e5 < area) { continue; }	// Ignore contours that are too small or too large

		// Draw each contour only for visualisation purposes
		drawContours(src, contours, static_cast<int>(i), cv::Scalar(0, 0, 255), 2, 8, hierarchy, 0);
		// Find the orientation of each shape
		getOrientation(contours[i], src);
	}
	cv::imshow("output", src);
	*/

	for (size_t a = 0; a < drawings.size(); a++)
	{
		std::string title = "drawing " + std::to_string(a);
		//cv::namedWindow(title.c_str(), CV_WINDOW_NORMAL); // CV_WINDOW_AUTOSIZE
		cv::namedWindow(title.c_str(), CV_WINDOW_AUTOSIZE);
		cv::imshow(title.c_str(), drawings[a]);
	}

	//cv::namedWindow("DRAWING", CV_WINDOW_NORMAL);
	//cv::imshow("DRAWING", drawing);

	cv::waitKey(0);

	delete pathIO;

	return 0;
}

