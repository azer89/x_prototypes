
// find end points of a skeleton
// input: a thinning image
// output: a list of points

/*
================================================================================
================================================================================
*/
std::vector<AVector> ShapeRadiusMatching::GetEndPoints(cv::Mat inputImage)
{
	std::vector<AVector> endPoints;

	// the input should have done this:
	// that every cell has a value 0 or 255
	//thinningImg *= 255;
	//cv::threshold(thinningImg, thinningImg, 127, 255, CV_THRESH_BINARY);

	int squareSz = 500;
	for (int curx = 0; curx < inputImage.cols; curx++)
	{
		for (int cury = 0; cury < inputImage.rows; cury++)
		{
			int val = inputImage.at<uchar>(cury, curx);

			if (val == 255)
			{
				int numNeighbor = 0;
				if (curx - 1 >= 0 && inputImage.at<uchar>(cury, curx - 1) == 255) { numNeighbor++; } // left
				if (curx + 1 < squareSz && inputImage.at<uchar>(cury, curx + 1) == 255) { numNeighbor++; } // right
				if (cury - 1 >= 0 && inputImage.at<uchar>(cury - 1, curx) == 255) { numNeighbor++; } // top
				if (cury + 1 < squareSz && inputImage.at<uchar>(cury + 1, curx) == 255) { numNeighbor++; } // bottom

				if (cury - 1 >= 0 && curx - 1 >= 0 && inputImage.at<uchar>(cury - 1, curx - 1) == 255) { numNeighbor++; } // upleft
				if (cury - 1 >= 0 && curx + 1 < squareSz && inputImage.at<uchar>(cury - 1, curx + 1) == 255) { numNeighbor++; } // upright
				if (cury + 1 < squareSz && curx - 1 >= 0 && inputImage.at<uchar>(cury + 1, curx - 1) == 255) { numNeighbor++; } // downleft
				if (cury + 1 < squareSz && curx + 1 < squareSz && inputImage.at<uchar>(cury + 1, curx + 1) == 255) { numNeighbor++; } // downright

				if (numNeighbor <= 1)
				{
					endPoints.push_back(AVector(curx, cury));
				}
			}
		} // cury
	} // curx
	return endPoints;
}
