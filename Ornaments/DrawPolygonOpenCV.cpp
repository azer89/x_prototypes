template <typename T>
void BlobEngine::DrawPolygon(cv::Mat& drawing, std::vector<T> shape_contours, cv::Scalar color, bool isClosed, float xOffset, float yOffset)
{
	std::vector<cv::Point2f> new_contours;
	for (size_t b = 0; b < shape_contours.size() - 1; b++)
	{
		new_contours.push_back(cv::Point2f((shape_contours[b].x + xOffset),
			(shape_contours[b].y + yOffset)));
	}
	for (size_t b = 0; b < new_contours.size() - 1; b++)
		{ cv::line(drawing, new_contours[b], new_contours[b + 1], color, 1); }
	if (isClosed)
		{ cv::line(drawing, new_contours[new_contours.size() - 1], new_contours[0], color, 1); }
}

// cv::Point2f
template void BlobEngine::DrawPolygon<cv::Point2f>(cv::Mat& drawing, std::vector<cv::Point2f> shape_contours, cv::Scalar color, bool isClosed, float xOffset, float yOffset);
// cv::Point
template void BlobEngine::DrawPolygon<cv::Point>(cv::Mat& drawing, std::vector<cv::Point> shape_contours, cv::Scalar color, bool isClosed, float xOffset, float yOffset);
// AVector
template void BlobEngine::DrawPolygon<AVector>(cv::Mat& drawing, std::vector<AVector> shape_contours, cv::Scalar color, bool isClosed, float xOffset, float yOffset);
