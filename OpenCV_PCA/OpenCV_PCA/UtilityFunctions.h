
#ifndef UTILITY_FUNCTIONS_H
#define UTILITY_FUNCTIONS_H

#include <vector>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

struct AVector;

class UtilityFunctions
{
public:
	/*
	================================================================================
	Even there is only one shape, cv::findContour() returns two shapes, which are
	the inner shape and outer shape
	================================================================================
	*/
	static int GetTheLargestIndex(std::vector<std::vector<cv::Point> > contours);

	/*
	================================================================================
	================================================================================
	*/
	static std::vector<cv::Point2f> CopyContourIntToFloat(std::vector<cv::Point> inContour);

	/*
	================================================================================
	================================================================================
	*/
	static std::vector<AVector> CopyContourIntToAVector(std::vector<cv::Point> inContour);


	// Todo: implement closed curves
	static double CurveLength(std::vector<AVector> curves);

	static void UniformResample(std::vector<AVector>& oriCurve, std::vector<AVector>& resampleCurve, int N /*bool isOpen*/);

};

#endif 
