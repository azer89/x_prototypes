/*
================================================================================
================================================================================
*/
cv::Mat BlobEngine::CalculateSignedDistance(std::vector<cv::Point2f> inputContour, std::string windowName)
{
	// ===== convert the input to a "computable" one (closed contour) =====
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Point> contour;
	for (int i = 0; i < inputContour.size(); i++)
		{ contour.push_back(inputContour[i]); }
	std::vector<cv::Point> revContour = contour;
	std::reverse(revContour.begin(), revContour.end()); // simulate open curve
	contour.insert(contour.end(), revContour.begin(), revContour.end());
	contours.push_back(contour);

	// ===== drawing matrix =====
	cv::Mat drawing = cv::Mat::zeros(500, 500, CV_8UC3);

	// ===== calculate distance =====
	cv::Mat rawDist(drawing.size(), CV_32FC1);
	for (int y = 0; y < drawing.rows; y++)
	{
		for (int x = 0; x < drawing.cols; x++)
		{
			float d = cv::pointPolygonTest(contours[0], cv::Point2f(x, y), true);
			rawDist.at<float>(y, x) = -d; // because I want positive value 
		}
	}

	return rawDist;
}
