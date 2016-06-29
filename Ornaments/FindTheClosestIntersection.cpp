/*
================================================================================
================================================================================
*/
AVector ClipperWrapper::FindTheClosestIntersection(ALine line, std::vector<AVector> shape) // from start
{
	AVector endPt = line.GetPointB(); // the endpoint, because clipper thinks a closed path is also a *filled* path
	AVector startPt = line.GetPointA();
	AVector intersectPt;

	float cScaling = 1e15; // because clipper only uses integer

	ClipperLib::Path cShape;
	ClipperLib::Path cLine;
	ClipperLib::PolyTree sol;

	for (int i = 0; i < shape.size(); i++)
		{ cShape << ClipperLib::IntPoint(shape[i].x * cScaling, shape[i].y * cScaling); }

	cLine << ClipperLib::IntPoint(line.XA * cScaling, line.YA * cScaling) << ClipperLib::IntPoint(line.XB * cScaling, line.YB * cScaling);

	ClipperLib::Clipper myClipper;
	myClipper.AddPath(cShape, ClipperLib::ptClip, true);    // closed shape should be the clipper
	myClipper.AddPath(cLine, ClipperLib::ptSubject, false); // line must be subject
	myClipper.Execute(ClipperLib::ctIntersection, sol, ClipperLib::pftNonZero, ClipperLib::pftNonZero);

	ClipperLib::Paths pSol;
	ClipperLib::PolyTreeToPaths(sol, pSol);

	float dist = std::numeric_limits<float>::min();
	for (int a = 0; a < pSol.size(); a++)
	{
		for (int b = 0; b < pSol[a].size(); b++)
		{
			AVector iPt(pSol[a][b].X, pSol[a][b].Y);
			iPt /= cScaling;

			float d1 = endPt.Distance(iPt);
			float d2 = startPt.Distance(iPt);

			if (d1 > dist && d2 > 1.5f)	// hardcoded, need to be fixed
			{
				dist = d1;
				intersectPt = iPt;
			}
		}
	}

	return intersectPt;
}
