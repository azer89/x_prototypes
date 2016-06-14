// UniformResample.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <iostream>

#include "AVector.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

cv::Mat _drawing;
std::string _windowTitle;

std::vector<AVector> _strokes;

void DrawPoly(cv::Mat& drawing, std::vector<AVector> shape_contours, cv::Scalar color, bool isClosed, float xOffset = 0, float yOffset = 0);
float CurveLength(std::vector<AVector> curves);
void UniformResample(std::vector<AVector> oriCurve, std::vector<AVector>& resampleCurve, int N);

void onmouse(int event, int x, int y, int flags, void* param)
{
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		_strokes.push_back(AVector(x, y));
		if (_strokes.size() <= 1) { return; }

		cv::Mat &img = *((cv::Mat*)(param));

		img = 0; // clear
		std::vector<AVector> resampleStroke;
		UniformResample(_strokes, resampleStroke, 104);
		DrawPoly(img, resampleStroke, cv::Scalar(0, 255, 0), false);
		for (int a = 0; a < resampleStroke.size(); a++)
			{ cv::circle(img, cv::Point(resampleStroke[a].x, resampleStroke[a].y), 3, cv::Scalar(0, 0, 255), -1); }
		std::cout << resampleStroke.size() << "\n";
		
		cv::imshow("drawing", img);
	}
}

int main(int argc, char **argv)
{
	_windowTitle = "drawing";
	_drawing = cv::Mat::zeros(500, 500, CV_8UC3);

	cv::namedWindow(_windowTitle, cv::WINDOW_AUTOSIZE);
	cv::setMouseCallback(_windowTitle, onmouse, &_drawing); // pass address of img here
	cv::imshow(_windowTitle, _drawing);
	cv::waitKey(0);

	return 0;
}

void UniformResample(std::vector<AVector> oriCurve, std::vector<AVector>& resampleCurve, int N)
{
	resampleCurve.clear();
	float curveLength = CurveLength(oriCurve);
	float resampleLength = curveLength / (float)(N - 1);

	//int i = 0;
	int iter = 0;
	double sumDist = 0.0;
	resampleCurve.push_back(oriCurve[0]);
	while (resampleCurve.size() < (N - 1))
	{
		double currentDist = oriCurve[iter].Distance(oriCurve[iter + 1]);
		sumDist += currentDist;

		if (sumDist > resampleLength)
		{
			double vectorLength = currentDist - (sumDist - resampleLength);
			AVector pt1 = oriCurve[iter];
			AVector pt2 = oriCurve[iter + 1];
			AVector directionVector = (pt2 - pt1).Norm();

			AVector newPoint1 = pt1 + directionVector * vectorLength;
			resampleCurve.push_back(newPoint1);

			sumDist = currentDist - vectorLength;

			while (sumDist - resampleLength > 1e-8)
			{
				AVector insertPt2 = resampleCurve[resampleCurve.size() - 1] + directionVector * resampleLength;
				resampleCurve.push_back(insertPt2);
				sumDist -= resampleLength;
			}
		}
		iter++;
	}
	resampleCurve.push_back(oriCurve[oriCurve.size() - 1]);

	// bug 
	if (resampleCurve.size() > N)
	{
		resampleCurve.pop_back();
	}
}

void DrawPoly(cv::Mat& drawing, std::vector<AVector> shape_contours, cv::Scalar color, bool isClosed, float xOffset, float yOffset)
{
	std::vector<cv::Point2f> new_contours;
	for (size_t b = 0; b < shape_contours.size(); b++)
		{ new_contours.push_back(cv::Point2f((shape_contours[b].x + xOffset), (shape_contours[b].y + yOffset))); }

	for (size_t b = 0; b < new_contours.size() - 1; b++)
		{ cv::line(drawing, new_contours[b], new_contours[b + 1], color, 1); }

	if (isClosed)
		{ cv::line(drawing, new_contours[new_contours.size() - 1], new_contours[0], color, 1); }
}

float CurveLength(std::vector<AVector> curves)
{
	float length = 0.0;
	for (size_t a = 1; a < curves.size(); a++) 
		{ length += curves[a].Distance(curves[a - 1]); }
	return length;
}



