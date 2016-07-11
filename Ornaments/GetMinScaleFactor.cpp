//float OrnamentResolver::GetMinIntersectionDistance(int artIndex)
//void OrnamentResolver::GetMinIntersectionDistance(int artIndex, float& minDist, float& ctrDist)
float OrnamentResolver::GetMinScaleFactor(int artIndex)
{
	float offset = 1.0f;

	// other ornaments
	std::vector<std::vector<AVector>> allBoundaries;
	for (int i = 0; i < _artDataArray.size(); i++)
	{
		if (i == artIndex) { continue; }
		ArtData curArt = _artDataArray[i];		
		std::vector<std::vector<AVector>> boundaries = curArt._boundaries;
		//std::vector<std::vector<AVector>> oBoundaries = ClipperWrapper::MakeClockwise(boundaries);
		allBoundaries.insert(allBoundaries.end(), boundaries.begin(), boundaries.end());
	}

	ArtData aData = _artDataArray[artIndex];
	std::vector<std::vector<AVector>> selfBoundaries = aData._boundaries;
	int blobIndex = aData._blobNumber;
	std::vector<AVector> ornamentSkeleton = _blobs[blobIndex]._streamLine;
	std::vector<AVector> resampledSkeleton;
	UtilityFunctions::UniformResample(ornamentSkeleton, resampledSkeleton, SystemParams::_function_length);
	std::vector<AVector> lDirs;
	std::vector<AVector> rDirs;
	UtilityFunctions::GetListsOfLeftRightNormals(resampledSkeleton, lDirs, rDirs);
	float rayLength = SystemParams::_upscaleFactor * sqrt(2);
	//float minDist = std::numeric_limits<float>::max();
	//minDist = std::numeric_limits<float>::max();
	float minFactor = std::numeric_limits<float>::max();
	float minD1;
	float minD2;
	for (int a = 0; a < resampledSkeleton.size(); a++)
	{
		AVector lPt1 = resampledSkeleton[a] + lDirs[a] * rayLength;
		AVector rPt1 = resampledSkeleton[a] + rDirs[a] * rayLength;
		AVector l1 = ClipperWrapper::FindTheClosestIntersection(ALine(resampledSkeleton[a], lPt1), allBoundaries);
		AVector r1 = ClipperWrapper::FindTheClosestIntersection(ALine(resampledSkeleton[a], rPt1), allBoundaries);

		AVector lPt2 = resampledSkeleton[a] + lDirs[a] * 200;
		AVector rPt2 = resampledSkeleton[a] + rDirs[a] * 200;
		AVector l2 = ClipperWrapper::FindTheFarthestIntersection(ALine(resampledSkeleton[a], lPt2), selfBoundaries);
		AVector r2 = ClipperWrapper::FindTheFarthestIntersection(ALine(resampledSkeleton[a], rPt2), selfBoundaries);

		if (!l1.IsInvalid() && !l2.IsInvalid())
		{
			// note: what if d1 < offset !!!
			float d1 = l1.Distance(resampledSkeleton[a]);
			float d2 = l2.Distance(resampledSkeleton[a]);
			float val = (d1 - offset) / d2;

			if (val < minFactor && val < 10.0)
			{
				minFactor = val;
				minD1 = d1;
				minD2 = d2;
			}
		}

		if (!r1.IsInvalid() && !r2.IsInvalid())
		{
			float d1 = r1.Distance(resampledSkeleton[a]);
			float d2 = r2.Distance(resampledSkeleton[a]);
			float val = (d1 - offset) / d2;
			if (val < minFactor && val < 10.0)
			{
				minFactor = val;
				minD1 = d1;
				minD2 = d2;
			}
		}
	}
	std::cout << minFactor << "\t" << minD1 << "\t" << minD2 << "\n";

	//return minDist;
	return minFactor;
}
