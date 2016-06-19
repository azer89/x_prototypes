/*
================================================================================
Testing shape radius function on ornaments

the input is .path files
================================================================================
*/
void ShapeRadiusMatching::TestAnOrnament1()
{
	// ===== read file =====
	ARegion reg = _pathIO.LoadRegions("C:\\Users\\saputra\\workspace\\Images\\swirl.path")[0];
	std::vector<AVector> myBoundary = reg._boundaryPaths[0].points;
	std::vector<AVector> mySkeleton = reg._skeletonLines[0];

	// ===== resample straight skeleton  =====
	AVector startSkelPt = mySkeleton[0];
	AVector endSkelPt = mySkeleton[mySkeleton.size() - 1];
	AVector skelDir = (endSkelPt - startSkelPt).Norm();
	float skelLength = startSkelPt.Distance(endSkelPt);
	std::vector<AVector> resampledSkeleton;
	float floatDelta = 20.0f;
	for (float a = 0; a < skelLength; a += floatDelta)
		{ resampledSkeleton.push_back(startSkelPt + skelDir * a); }

	// ===== image =====
	cv::Mat drawing = cv::Mat::zeros(500, 500, CV_8UC3);
	cv::Scalar color(255, 255, 255);
	DrawPoly(drawing, myBoundary, color, true);
	DrawPoly(drawing, mySkeleton, color, true);

	// ===== bounding box =====
	ARectangle bb = UtilityFunctions::GetBoundingBox(myBoundary);
	float vecWitdh = bb.witdh * 0.5f + 100;

	// ===== need to handle the last point =====
	for (int a = 0; a < resampledSkeleton.size() - 1; a++)
	{
		AVector curPt = resampledSkeleton[a];
		AVector nextPt = resampledSkeleton[a + 1];

		AVector lDir; AVector rDir;
		UtilityFunctions::GetLeftRightDirections(curPt, nextPt, &lDir, &rDir);
		
		// ALine
		AVector lPt = curPt + lDir * vecWitdh;
		AVector rPt = curPt + rDir * vecWitdh;

		AVector lIntersection = ClipperWrapper::FindTheClosestIntersection(ALine(curPt, lPt), myBoundary);
		AVector rIntersection = ClipperWrapper::FindTheClosestIntersection(ALine(curPt, rPt), myBoundary);
					
		if(!lIntersection.IsInvalid()) cv::line(drawing, cv::Point(curPt.x, curPt.y), cv::Point(lIntersection.x, lIntersection.y), cv::Scalar(255, 0, 0), 2);
		cv::circle(drawing, cv::Point(lIntersection.x, lIntersection.y), 3, cv::Scalar(0, 255, 0), -1);		
		if (!rIntersection.IsInvalid()) cv::line(drawing, cv::Point(curPt.x, curPt.y), cv::Point(rIntersection.x, rIntersection.y), cv::Scalar(255, 0, 0), 2);
		cv::circle(drawing, cv::Point(rIntersection.x, rIntersection.y), 3, cv::Scalar(0, 0, 255), -1);
	}
	// ===== show opencv window =====
	cv::namedWindow("ornament", cv::WINDOW_AUTOSIZE);
	cv::imshow("ornament", drawing);
	cv::waitKey();
}

/*
================================================================================
================================================================================
*/
void ShapeRadiusMatching::TestAnOrnament2()
{
	// ===== read file =====
	ARegion reg = _pathIO.LoadRegions("C:\\Users\\saputra\\workspace\\Images\\ornament_010.path")[0];
	std::vector<std::vector<AVector>> myBoundaries;	
	for (int a = 0; a < reg._boundaryPaths.size(); a++)
		{ myBoundaries.push_back(reg._boundaryPaths[a].points); }
	std::vector<AVector> mySkeleton = reg._skeletonLines[0];

	// ===== resample straight skeleton  =====
	AVector startSkelPt = mySkeleton[0];
	AVector endSkelPt = mySkeleton[mySkeleton.size() - 1];
	AVector skelDir = (endSkelPt - startSkelPt).Norm();
	float skelLength = startSkelPt.Distance(endSkelPt);
	std::vector<AVector> resampledSkeleton;
	float floatDelta = 20.0f;
	for (float a = 0; a < skelLength; a += floatDelta)
		{ resampledSkeleton.push_back(startSkelPt + skelDir * a); }

	// ===== bounding box =====
	ARectangle bb = UtilityFunctions::GetBoundingBox(myBoundaries);

	// ===== image =====
	cv::Mat drawing = cv::Mat::zeros(500, 500, CV_8UC3);
	cv::Scalar color(255, 255, 255);
	for (int a = 0; a < myBoundaries.size(); a++)
	{
		DrawPoly(drawing, myBoundaries[a], color, true);
	}	
	DrawPoly(drawing, mySkeleton, cv::Scalar(255,0,0), true);

	// ===== show opencv window =====
	cv::namedWindow("ornament", cv::WINDOW_AUTOSIZE);
	cv::imshow("ornament", drawing);
	cv::waitKey();
}
