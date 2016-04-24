
#include "stdafx.h"
#include "UtilityFunctions.h"

#include "AVector.h"

/*
================================================================================
Even there is only one shape, cv::findContour() returns two shapes, which are
the inner shape and outer shape
================================================================================
*/
int UtilityFunctions::GetTheLargestIndex(std::vector<std::vector<cv::Point> > contours)
{
	size_t max_size = 0;
	int max_index = -1;
	for (size_t a = 0; a < contours.size(); a++)
	{
		if (contours[a].size() > max_size)
		{
			max_size = contours[a].size();
			max_index = a;
		}
	}
	return max_index;
}

/*
================================================================================
================================================================================
*/
std::vector<cv::Point2f> UtilityFunctions::CopyContourIntToFloat(std::vector<cv::Point> inContour)
{
	std::vector<cv::Point2f> outContour;
	for (size_t a = 0; a < inContour.size(); a++)
		{ outContour.push_back(cv::Point2f(inContour[a].x, inContour[a].y)); }
	return outContour;
}

/*
================================================================================
================================================================================
*/
std::vector<AVector> UtilityFunctions::CopyContourIntToAVector(std::vector<cv::Point> inContour)
{
	std::vector<AVector> outContour;
	for (size_t a = 0; a < inContour.size(); a++)
		{ outContour.push_back(AVector(inContour[a].x, inContour[a].y)); }
	return outContour;
}

/*
================================================================================
================================================================================
*/
// Todo: implement closed curves
double UtilityFunctions::CurveLength(std::vector<AVector> curves)
{
	double length = 0.0;
	for (size_t a = 1; a < curves.size(); a++) { length += curves[a].Distance(curves[a - 1]); }
	return length;
}


/*
================================================================================
================================================================================
*/
// https://github.com/azer89/OrganicLabyrinth/blob/8353f8b24e137c8ab24b52dc44f8f220d025944c/GLWidget.cpp
void UtilityFunctions::UniformResample(std::vector<AVector>& oriCurve, std::vector<AVector>& resampleCurve, int N /*bool isOpen*/)
{
	double pl_length = CurveLength(oriCurve);

	// fix me
	AVector lastPt = oriCurve[0];
	oriCurve.push_back(lastPt);

	resampleCurve.clear();

	double resample_size = pl_length / ((float)N);

	int iter = 0;
	double dist = 0.0;

	while (iter < oriCurve.size() - 1)
	{
		double last_dist = oriCurve[iter].Distance(oriCurve[iter + 1]);

		dist += last_dist;

		if (dist >= resample_size)
		{
			//put a point on line
			double _d = last_dist - (dist - resample_size);
			AVector cp(oriCurve[iter].x, oriCurve[iter].y);

			AVector  cp1 = AVector(oriCurve[iter + 1].x, oriCurve[iter + 1].y);

			AVector dirv = cp1 - cp;
			dirv = dirv * (1.0 / dirv.Length());

			AVector insertPt1 = cp + dirv * _d;
			resampleCurve.push_back(insertPt1);

			dist = last_dist - _d; //remaining dist

			//if remaining dist to next point needs more sampling... (within some epsilon)
			while (dist - resample_size > 1e-8)
			{
				AVector insertPt2 = resampleCurve[resampleCurve.size() - 1] + dirv * resample_size;
				resampleCurve.push_back(insertPt2);
				dist -= resample_size;
			}
		}
		iter++;
	}

	//oriCurve.pop_back();
}
