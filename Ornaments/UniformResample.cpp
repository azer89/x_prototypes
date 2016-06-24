void UniformResample(std::vector<AVector> oriCurve, std::vector<AVector>& resampleCurve, int N)
{
	resampleCurve.clear();
	float curveLength = CurveLength(oriCurve);
	float resampleLength = curveLength / (float)(N - 1);

	//int i = 0;
	int iter = 0;
	double sumDist = 0.0;
	resampleCurve.push_back(oriCurve[0]);
	while (resampleCurve.size() < (N - 1))
	{
		double currentDist = oriCurve[iter].Distance(oriCurve[iter + 1]);
		sumDist += currentDist;

		if (sumDist > resampleLength)
		{
			double vectorLength = currentDist - (sumDist - resampleLength);
			AVector pt1 = oriCurve[iter];
			AVector pt2 = oriCurve[iter + 1];
			AVector directionVector = (pt2 - pt1).Norm();

			AVector newPoint1 = pt1 + directionVector * vectorLength;
			resampleCurve.push_back(newPoint1);

			sumDist = currentDist - vectorLength;

			while (sumDist - resampleLength > 1e-8)
			{
				AVector insertPt2 = resampleCurve[resampleCurve.size() - 1] + directionVector * resampleLength;
				resampleCurve.push_back(insertPt2);
				sumDist -= resampleLength;
			}
		}
		iter++;
	}
	resampleCurve.push_back(oriCurve[oriCurve.size() - 1]);

	// bug 
	if (resampleCurve.size() > N)
		{ resampleCurve.pop_back(); }
}
