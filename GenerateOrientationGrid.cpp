/*
================================================================================
save orientation to RGB values
================================================================================
*/
void VFDCollector::GenerateOrientationGrid(std::string filename)
{
	// create some kd-tress
	/*std::vector<NANOFLANNWrapper> nnWrappers(_regions.size());
	for (int a = 0; a < _regions.size(); a++)
	{
		nnWrappers[a].InsertData(_regions[a]._insideFieldLines);
		nnWrappers[a].CreateLineKDTree();
	}*/

	int dim = (int)SystemParams::_upscaleFactor;
	cv::Mat drawing = cv::Mat::zeros(dim, dim, CV_8UC3);
	for (int x = 0; x < dim; x++)
	{
		for (int y = 0; y < dim; y++)
		{
			// get region
			int regionIdx = -1;
			for (int a = 0; a < _scaledBoundaryPaths.size(); a++)
			{
				bool isInside = UtilityFunctions::InsidePolygon(_boundaryPaths[a].points, AVector(x, y));
				if (isInside)
				{
					regionIdx = a;
					break;
				}
			} // get region

			// get color
			if (regionIdx >= 0)
			{
				float xf = ((float)x) * SystemParams::_downscaleFactor;
				float yf = ((float)y) * SystemParams::_downscaleFactor;
				/*int closestIdx = nnWrappers[regionIdx].GetClosestIndices(AVector(xf, yf), 1)[0];
				ALine closestLine = _regions[regionIdx]._insideFieldLines[closestIdx];
				AVector dir = closestLine.Direction().Norm();*/
				AVector dir = _regions[regionIdx].GetDirectionInterpolation(AVector(xf, yf)).Norm();

				float xCol = 0.5f * dir.x + 0.5f;
				float yCol = 0.5f * dir.y + 0.5f;

				// blue green red
				drawing.at<cv::Vec3b>(y, x)[0] = yCol * 255;
				drawing.at<cv::Vec3b>(y, x)[1] = 0;
				drawing.at<cv::Vec3b>(y, x)[2] = xCol * 255;
			} // get color
		} // y
	} // x

	cv::imwrite("C:\\Users\\saputra\\workspace\\Images\\" + filename + "_orientation.png", drawing);
}
