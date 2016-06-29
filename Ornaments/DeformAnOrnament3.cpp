/*
================================================================================
================================================================================
*/
std::vector<std::vector<AVector>> ShapeRadiusMatching::DeformAnOrnament3(std::vector<APath> oPaths,
															             std::vector<AVector> oSkeleton, 
																	     PairFunctions oFunction,
															             PairFunctions bFunction, 
																	     ABlob blob)
{
	// deformed ornament
	std::vector<std::vector<AVector>> newBoundaries;
	// AVector newPt = sPt + dirs[idx] * shrinkFactor * dist * 0.5f;
	std::vector<std::vector<AVector>> vvsPt;
	std::vector<std::vector<AVector>> vvDir;
	std::vector<std::vector<float>>   vvDist;
	//std::vector<std::vector<bool>>    vvDist;

	// ===== get max, shrink factor, and copy =====
	float oMax = oFunction._maxVal;
	float shrinkFactor = bFunction._maxVal / oMax;
	std::vector<std::vector<AVector>> oBoundaries = UtilityFunctions::PathsToVectorAVector(oPaths);

	// ===== ornament skeleton =====
	AVector startSkelPt = oSkeleton[0];
	AVector endSkelPt = oSkeleton[oSkeleton.size() - 1];

	// ===== resample blob streamline =====
	// this should be adaptive
	std::vector<AVector> bResampledAxis = UtilityFunctions::GetCurveSubset(blob._streamLine, SystemParams::_function_length, 0.075, 0.075);

	// ===== get normals =====
	std::vector<AVector> lDirs; std::vector<AVector> rDirs;
	UtilityFunctions::GetListsOfLeftRightNormals(bResampledAxis, lDirs, rDirs);

	// ===== if both don't match we need to flip =====
	bool shouldLRFLip = false; bool shouldUDFLip = false;
	if ((bFunction.isLRFlipped && !oFunction.isLRFlipped) || (!bFunction.isLRFlipped &&  oFunction.isLRFlipped)) { shouldLRFLip = true; }
	if ((bFunction.isUDFlipped && !oFunction.isUDFlipped) || (!bFunction.isUDFlipped &&  oFunction.isUDFlipped)) { shouldUDFLip = true; }

	// ===== calculate =====
	for (int a = 0; a < oBoundaries.size(); a++)
	{
		std::vector<AVector> newBoundary;
		std::vector<AVector> vsPt;
		std::vector<AVector> vDir;
		std::vector<float>   vDist;
		for (int b = 0; b < oBoundaries[a].size(); b++)
		{
			AVector pt = oBoundaries[a][b]; // point of ornament
			AVector closestPt = UtilityFunctions::GetClosestPointOnFiniteLine(startSkelPt, endSkelPt, pt); // closest point om the straight skeleton

			float normDist = startSkelPt.Distance(closestPt) / startSkelPt.Distance(endSkelPt);			   // distance 0..1
			if (shouldUDFLip) { normDist = 1.0f - normDist; } // flip UD

			AVector sPt = UtilityFunctions::GetPointInterpolationFromStart(bResampledAxis, normDist);	   // this is interpolated point on the streamline

			// size = 2
			std::vector<AVector> dirs = GetInterPolatedLRDirs(normDist, lDirs, rDirs);

			int idx = 0;
			if (pt.x > closestPt.x) { idx = 1; }  // right
			if (shouldLRFLip) { idx = 1 - idx; }; // flip LR

			float dist = closestPt.Distance(pt);
			AVector newPt = sPt + dirs[idx] * shrinkFactor * dist * 0.2f;

			newBoundary.push_back(newPt); // push
			vsPt.push_back(sPt);
			vDir.push_back(dirs[idx]);
			vDist.push_back(dist);
		}
		newBoundaries.push_back(newBoundary); // push a new boundary
		vvsPt.push_back(vsPt);
		vvDir.push_back(vDir);
		vvDist.push_back(vDist);
	}

	float minDist = std::numeric_limits<float>::max();
	float extendRatio = 0.0f;
	//float minScale = 1.0f;
	for (int a = 0; a < newBoundaries.size(); a++)
	{
		std::vector<AVector> vsPt = vvsPt[a]; // points on the streamline
		std::vector<AVector> vDir = vvDir[a]; // direction
		for (int b = 0; b < newBoundaries[a].size(); b++)
		{
			// intersection
			AVector pt1 = vsPt[b];
			AVector pt2 = pt1 + vDir[b] * SystemParams::_max_lr_ray_length;
			AVector intersectPt = ClipperWrapper::FindTheFarthestIntersection(ALine(pt1, pt2), blob._blobBoundary);

			if (!intersectPt.IsInvalid())
			{
				AVector oriPt = newBoundaries[a][b];
				AVector dir = (intersectPt - oriPt).Norm();
				float dist = oriPt.Distance(intersectPt);

				//float dist2Center = vsPt[b].Distance(oriPt);
				float dist2Center = vsPt[b].Distance(intersectPt);

				if (dist < minDist)
				{
					minDist = dist;
					extendRatio = dist / dist2Center;
					//minScale = dist2 / dist1;
					/*if (minScale > 100)
					{
						std::cout << minScale << " " << dist2 << " " << dist1 << "\n";
					}*/
				}
			}
		}
	}
	//std::cout << minDist << "\n";
	extendRatio *= 0.9;

	for (int a = 0; a < newBoundaries.size(); a++)
	{
		std::vector<AVector> vsPt = vvsPt[a]; // points on the streamline
		std::vector<AVector> vDir = vvDir[a]; // direction
		for (int b = 0; b < newBoundaries[a].size(); b++)
		{
			AVector pt1 = vsPt[b];
			AVector pt2 = pt1 + vDir[b] * SystemParams::_max_lr_ray_length;
			AVector intersectPt = ClipperWrapper::FindTheFarthestIntersection(ALine(pt1, pt2), blob._blobBoundary);

			if (!intersectPt.IsInvalid())
			{
				AVector oriPt = newBoundaries[a][b]; // original point of an ornament
				AVector dir = vDir[b];				 // direction of the point			
				//newBoundaries[a][b] = vsPt[b] + dir * (vsPt[b].Distance(oriPt) + minDist);
				float extendVal = vsPt[b].Distance(intersectPt) * extendRatio;
				newBoundaries[a][b] = vsPt[b] + dir * (vsPt[b].Distance(oriPt) + minDist);
			}
		}
	}

	return newBoundaries;
}
