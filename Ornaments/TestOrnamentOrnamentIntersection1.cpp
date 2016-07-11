/*
================================================================================
================================================================================
*/
void OrnamentResolver::TestOrnamentOrnamentIntersection()
{
	// ===== load neighbors ===== 
	ARegion neighbor = _pathIO.LoadRegions(SystemParams::_image_folder + "weird_blobs" + ".path")[0];
	std::cout << "neighbor size: " << neighbor._boundaryPaths.size() << "\n";

	// ===== load ornament ===== 
	ARegion ornament = _pathIO.LoadRegions(SystemParams::_image_folder + "weird_ornament" + ".path")[0];

	// ===== draw neighbors ===== 
	_cvWrapper.CreateImage("weird blobs", SystemParams::_upscaleFactor, SystemParams::_upscaleFactor, BGR_255);
	std::vector<std::vector<AVector>> neighborBoundaries = neighbor.GetBoundaries();
	std::vector<std::vector<AVector>> orientedBoundaries = ClipperWrapper::MakeClockwise(neighborBoundaries);
	for (int a = 0; a < orientedBoundaries.size(); a++)
	{
		if (ClipperWrapper::IsClockwise(orientedBoundaries[a])) { _cvWrapper.DrawPoly("weird blobs", orientedBoundaries[a], MyColor(255), true); }
		else{ _cvWrapper.DrawPoly("weird blobs", orientedBoundaries[a], MyColor(0, 255, 0), true); }
	}

	// ===== draw ornament ===== 
	/*std::vector<AVector> ornamentBoundary = ornament.GetBoundaries()[0];
	std::vector<AVector> ornamentSkeleton = ornament._skeletonLines[0];
	_cvWrapper.DrawPoly("weird blobs", ornamentBoundary, MyColor(255, 0, 0), true);
	_cvWrapper.DrawPoly("weird blobs", ornamentSkeleton, MyColor(0, 0, 255), false);*/

	// ===== test intersection =====
	/*for (int a = 0; a < 30; a++)
	{
		ALine line(_cvWrapper.GetRandom(0, 500), _cvWrapper.GetRandom(0, 500), _cvWrapper.GetRandom(0, 500), _cvWrapper.GetRandom(0, 500));
		AVector startPt = line.GetPointA();
		AVector endPt = line.GetPointB();

		_cvWrapper.DrawLine("weird blobs", startPt, endPt, MyColor(50), 1);
		_cvWrapper.DrawCircle("weird blobs", startPt, MyColor(50), 3);

		AVector pt = ClipperWrapper::FindTheClosestIntersection(line, orientedBoundaries);
		if (!pt.IsInvalid())
			_cvWrapper.DrawCircle("weird blobs", pt, MyColor(255, 0, 0), 3);
	}*/

	std::vector<AVector> ornamentBoundary = ornament.GetBoundaries()[0];
	std::vector<AVector> ornamentSkeleton = ornament._skeletonLines[0];
	_cvWrapper.DrawPoly("weird blobs", ornamentBoundary, MyColor(255, 0, 0), true);  // draw
	_cvWrapper.DrawPoly("weird blobs", ornamentSkeleton, MyColor(0, 0, 255), false); // draw
	std::vector<AVector> resampledSkeleton;
	//UtilityFunctions::UniformResample(ornamentSkeleton, resampledSkeleton, SystemParams::_function_length);
	UtilityFunctions::UniformResample(ornamentSkeleton, resampledSkeleton, 30);
	std::vector<AVector> lDirs;
	std::vector<AVector> rDirs;
	UtilityFunctions::GetListsOfLeftRightNormals(resampledSkeleton, lDirs, rDirs);
	float rayLength = SystemParams::_upscaleFactor * sqrt(2);
	for (int a = 0; a < resampledSkeleton.size(); a++)
	{
		AVector lPt = resampledSkeleton[a] + lDirs[a] * rayLength;
		AVector rPt = resampledSkeleton[a] + rDirs[a] * rayLength;
		AVector lIntersection = ClipperWrapper::FindTheClosestIntersection(ALine(resampledSkeleton[a], lPt), orientedBoundaries);
		AVector rIntersection = ClipperWrapper::FindTheClosestIntersection(ALine(resampledSkeleton[a], rPt), orientedBoundaries);

		_cvWrapper.DrawLine("weird blobs", resampledSkeleton[a], lPt, MyColor(50), 1);
		_cvWrapper.DrawLine("weird blobs", resampledSkeleton[a], rPt, MyColor(50), 1);

		if (!lIntersection.IsInvalid())
		{
			_cvWrapper.DrawCircle("weird blobs", lIntersection, MyColor(255, 0, 0), 3);
		}

		if (!rIntersection.IsInvalid())
		{
			_cvWrapper.DrawCircle("weird blobs", rIntersection, MyColor(255, 0, 0), 3);
		}
	}

	_cvWrapper.ShowImage("weird blobs");
	_cvWrapper.WaitKey();
}
