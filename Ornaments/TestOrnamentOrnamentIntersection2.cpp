/*
================================================================================
================================================================================
*/
float OrnamentResolver::TestOrnamentOrnamentIntersection(int artIndex)
{
	// current ornament
	//int artIndex = 18;
	ArtData aData = _artDataArray[artIndex];
	std::vector<std::vector<AVector>> oBoundaries = aData._boundaries;

	// other ornaments
	std::vector<std::vector<AVector>> allBoundaries;
	for (int i = 0; i < _artDataArray.size(); i++)
	{
		if (i == artIndex) { continue; }
		std::vector<std::vector<AVector>> boundaries = _artDataArray[i]._boundaries;
		allBoundaries.insert(allBoundaries.end(), boundaries.begin(), boundaries.end());
	}

	float offset = 1.0f;
	float scaleFactor = 1.0f;
	//float maxDiff = std::numeric_limits<float>::min();
	float minDiff = std::numeric_limits<float>::max();
	float minVVDist;
	float minCtrToOthers;
	float minCtrToOrnmntPt;
	for (int a = 0; a < oBoundaries.size(); a++)
	{
		for (int b = 0; b < oBoundaries[a].size(); b++)
		{
			AVector curPt    = oBoundaries[a][b];
			AVector dir      = aData._vvDir[a][b];
			AVector centerPt = aData._vvsPt[a][b];

			ALine ln(centerPt, centerPt + dir * 200);

			AVector ctrToOrnmntPt = ClipperWrapper::FindTheFarthestIntersection(ln, oBoundaries);
			AVector ctrToOtherPt = ClipperWrapper::FindTheClosestIntersection(ln, allBoundaries);

			if (ctrToOrnmntPt.IsInvalid() || ctrToOtherPt.IsInvalid()) { continue; }

			float ctrToOrnmntDist = centerPt.Distance(ctrToOrnmntPt); // center to ornament's shape
			float ctrToOtherDist = centerPt.Distance(ctrToOtherPt);   // center to others
			float diff = ctrToOtherDist - ctrToOrnmntDist;            // no abs
			float val = (ctrToOtherDist - offset) / aData._vvDist[a][b]; // for scaleFactor

			//std::cout << diff << "\t" << val << "\n";
			//if (diff > maxDiff && val > 0)
			if (diff > 0 && diff < minDiff/* && val < 0.3*/)
			{
				//if(val > 1) std::cout << val << " = " << ctrToOtherDist << " / " << aData._vvDist[a][b] << "\n";
				scaleFactor = val;
				//maxDiff = diff;
				minDiff = diff;
				//isOverlap = true;

				minCtrToOthers = ctrToOtherDist;
				minCtrToOrnmntPt = ctrToOrnmntDist;
				minVVDist = aData._vvDist[a][b];
			}
		}
	}

	//if (scaleFactor > 0.5f)
	{
		std::cout << scaleFactor << " = " << minCtrToOthers << " / " << minVVDist << "\n";
		std::cout << minDiff << " = " << minCtrToOthers << " - " << minCtrToOrnmntPt << "\n\n";
	}

	return scaleFactor;
}
