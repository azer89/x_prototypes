
#include "DFSTest.h"

DFSTest::DFSTest()
{
}

DFSTest::~DFSTest()
{
}

#define FREE_CELL 255
#define OCCUPIED_CELL 0
#define VISITED_CELL 127

bool IsInBound(cv::Point2i current, int rows, int cols)
	{ return (current.x >= 0 && current.y >= 0 && current.x < cols && current.y < rows); }

bool IsEqual(cv::Point2i pt1, cv::Point2i pt2)
	{ return (pt1.x == pt2.x && pt1.y == pt2.y); }

bool IsOccupiedCell(cv::Mat image, cv::Point2i pt)
	{ return (image.at<uchar>(pt.y, pt.x) == OCCUPIED_CELL); }

bool IsFreeCell(cv::Mat image, cv::Point2i pt)
	{ return (image.at<uchar>(pt.y, pt.x) == FREE_CELL); }

bool IsVisitedCell(cv::Mat image, cv::Point2i pt)
	{ return (image.at<uchar>(pt.y, pt.x) == VISITED_CELL); }

void DFSTest::DFS(cv::Mat srcImg, cv::Point2i startPoint, cv::Point2i goalPoint)
{
	// ===== input =====
	cv::Mat image(srcImg.size(), CV_8UC1);
	image = OCCUPIED_CELL;
	for (int x = 0; x < srcImg.cols; x++)
	{
		for (int y = 0; y < srcImg.rows; y++)
			{ if (srcImg.at<uchar>(y, x) == 255) { image.at<uchar>(y, x) = FREE_CELL; } }
	}

	// debugging
	cv::Mat resultImg;
	cv::cvtColor(image, resultImg, CV_GRAY2BGRA);

	// ===== queue =====
	std::vector<cv::Point2i> pathQ; // first is point, second is parent
	pathQ.push_back(goalPoint);

	bool isFound = false; // can we find the start ?
	while (!pathQ.empty() && !isFound)
	{
		//std::cout << pathQ.size() << "\n";
		cv::Point2i currentPt = pathQ[pathQ.size() - 1];

		// find a neighbour
		bool findOne = false; // can we find a neighbor ?
		cv::Point2i neighborPt;
		for (int x = currentPt.x - 1; x <= (currentPt.x + 1) && !findOne; x++)
		{
			for (int y = currentPt.y - 1; y <= (currentPt.y + 1) && !findOne; y++)
			{
				neighborPt = cv::Point2i(x, y);
				if (!IsInBound(neighborPt, image.rows, image.cols) || // within the image
					IsEqual(currentPt, neighborPt) ||    // neighbor is not current
					IsOccupiedCell(image, neighborPt) || // neighbor is not occupied
					IsVisitedCell(image, neighborPt))	 // neighbor is has not visited
					{ continue; }

				// it seems that the neighbor is free ceel and it hasn't been visited
				image.at<uchar>(neighborPt.y, neighborPt.x) = VISITED_CELL;
				findOne = true;
				pathQ.push_back(neighborPt);

			}
		}// find a neighbor

		if (findOne && IsEqual(neighborPt, startPoint)) // find start ?
			{ isFound = true; }
		else if (!findOne)
			{ pathQ.pop_back(); /* return back */ }
	}

	if (isFound)
	{
		std::cout << "find path\n";
		bool isGoal = false;
		for (int a = 0; a < pathQ.size(); a++)
		{
			cv::Point2i pt = pathQ[a];
			cv::circle(resultImg, pt, 1, cv::Scalar(0, 255, 0), -1);
		}
	}
	else { std::cout << "cannot find path\n"; }

	cv::imshow("DFS", resultImg);
	cv::waitKey();
}
