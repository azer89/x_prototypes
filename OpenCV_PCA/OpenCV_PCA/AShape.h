#ifndef ASHAPE_H
#define ASHAPE_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <vector>
#include <limits>
#include <cmath>

//#include "MomentsCalculator.h"
#include "AVector.h"

struct AShape
{
	//cv::Moments shape_moments;
	//MyMoments shape_moments;
	//std::vector<cv::Point2f> shape_contours;
	std::vector<AVector> shape_contours;
	//double hu[7];	// hu moments
	std::string filepath = "";
	cv::Mat img;
};

#endif // ASHAPE_H