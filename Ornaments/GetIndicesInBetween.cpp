void GetIndicesInBetween(std::vector<AVector> curve, float val, int& index1, int& index2)
{
	float curveLength = GetCurveLength(curve);
	float interpLength = val * curveLength;

	float err = 1e-2; // I don't like this
	if (interpLength < err) 
	{ 
		index1 = 0;
		index2 = 1;
		return; 
	}
	if (std::abs(interpLength - curveLength) < err) 
	{ 
		index1 = curve.size() - 2;
		index2 = curve.size() - 1;
		return; 
	}

	float sumLength = 0.0f;
	for (int a = 1; a < curve.size(); a++)
	{
		AVector prevPoint = curve[a - 1];
		AVector curPoint = curve[a];
		float dist = prevPoint.Distance(curPoint);
		AVector dir = (curPoint - prevPoint).Norm();

		// check whether it falls in between
		float diff = (sumLength + dist) - interpLength;
		if (diff > 0)
		{
			index1 = a - 1;
			index2 = a;
			return;
		}
		else { sumLength += dist; }
	}
	std::cout << "can't find\n";
}
