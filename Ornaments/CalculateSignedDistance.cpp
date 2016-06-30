cv::Mat DistanceGrid::CalculateSignedDistance(std::vector<AVector> inputContour, std::string imageName)
{
	cv::Mat distImage(SystemParams::_upscaleFactor, SystemParams::_upscaleFactor, CV_32FC1);
	std::vector<cv::Point2f> cvContour = _cvWrapper.ConvertAVectorToCvPoint2f(inputContour);
	if (inputContour.size() > 0)
	{
		for (int y = 0; y < distImage.rows; y++)
		{
			for (int x = 0; x < distImage.cols; x++)
			{
				float d = cv::pointPolygonTest(cvContour, cv::Point2f(x, y), true);
				distImage.at<float>(y, x) = -d; // because I want positive (outside) and negative (inside)
			}
		}
	}
	cv::imwrite("C:\\Users\\saputra\\workspace\\Images\\element_fitting\\" + imageName + ".png", distImage);
	return distImage;
}
