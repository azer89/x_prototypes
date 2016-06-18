AVector GetPointInterpolationFromStart(std::vector<AVector> curve, float val)
{
	float curveLength = CurveLength(curve);
	float interpLength = val * curveLength;

	std::cout << "=====" << curveLength << " " << interpLength << " " << std::abs(interpLength - curveLength) << " =====\n";
	float err = 1e-2; // I don't like this
	if (interpLength < err) { return curve[0]; }
	if (std::abs(interpLength - curveLength) < err) { return curve[curve.size() - 1]; }

	float sumLength = 0.0f;
	for (int a = 1; a < curve.size(); a++)
	{
		AVector prevPoint = curve[a - 1];
		AVector curPoint = curve[a];
		float dist = prevPoint.Distance(curPoint);
		AVector dir = (curPoint - prevPoint).Norm();

		// check whether it falls in between
		float diff = (sumLength + dist) - interpLength;
		//std::cout << diff << "\n";

		if (diff > 0)
		{
			return prevPoint + dir * (dist - diff);
		}
		else { sumLength += dist; }
	}
	std::cout << "can't find\n";
	return AVector(); // can't find
}
