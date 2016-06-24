// MeshDeformation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "AVector.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

std::string			 _windowTitle;
std::vector<AVector> _strokes;
bool				 _isMouseDown = false;

void MouseEvent(int event, int x, int y, int flags, void* param)
{
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		_isMouseDown = true;
		_strokes.push_back(AVector(x, y));
	}
	else if (event == CV_EVENT_LBUTTONUP)
	{
		_isMouseDown = false;
	}
	else if (event == CV_EVENT_MOUSEMOVE)
	{
		if (_isMouseDown)
		{
			_strokes.push_back(AVector(x, y));
		}
	}

	cv::Mat &img = *((cv::Mat*)(param));

	if (_strokes.size() <= 1) { return; }

	img = 0; // clear
	for (int a = 0; a < _strokes.size() - 1; a++)
	{
		cv::line(img, 
				 cv::Point(_strokes[a].x, _strokes[a].y), 
				 cv::Point(_strokes[a + 1].x, _strokes[a + 1].y), 
				 cv::Scalar(255, 255, 255), 1);
	}

	cv::imshow(_windowTitle, img);
}



int _tmain(int argc, _TCHAR* argv[])
{
	_windowTitle = "drawing";
	// height width
	cv::Mat drawing = cv::Mat::zeros(768, 1024, CV_8UC3);

	cv::namedWindow(_windowTitle, cv::WINDOW_AUTOSIZE);
	cv::setMouseCallback(_windowTitle, MouseEvent, &drawing); // pass address of img here
	cv::imshow(_windowTitle, drawing);
	cv::waitKey(0);


	return 0;
}

