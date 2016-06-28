// DON'T USE THIS FUNCTION
std::vector<AVector> LineTracer::TraceAStreamLineFromASeedSlow(AVector seed)
{
	AVector curPt = seed;
	float dist = std::numeric_limits<float>::max();

	// ===== forward tracing =====
	std::vector<AVector> forwardStreamLine;
	do
	{
		forwardStreamLine.push_back(curPt);
		AVector dir = _aRegion->GetDirectionInterpolation(curPt);
		curPt = curPt + dir * SystemParams::_tracer_step_length; // next point
		dist = UtilityFunctions::DistanceToPolylines(this->_streamLines, curPt);
	} while (UtilityFunctions::InsidePolygon(_shapeBoundary, curPt) && dist > SystemParams::_tracer_min_stop);

	// ===== backward tracing =====
	curPt = seed; // go back to seed point again
	dist = std::numeric_limits<float>::max(); // reset dist
	std::vector<AVector> backwardStreamLine;
	bool shouldContinue = true;
	do
	{
		AVector dir = _aRegion->GetDirectionInterpolation(curPt);
		curPt = curPt - dir * SystemParams::_tracer_step_length;
		dist = UtilityFunctions::DistanceToPolylines(this->_streamLines, curPt);

		shouldContinue = UtilityFunctions::InsidePolygon(_shapeBoundary, curPt) && dist > SystemParams::_tracer_min_stop;
		if (shouldContinue) { backwardStreamLine.push_back(curPt); /*insert a point*/ }
	} while (shouldContinue);

	std::reverse(backwardStreamLine.begin(), backwardStreamLine.end());

	std::vector<AVector> streamLine;
	streamLine.insert(streamLine.end(), backwardStreamLine.begin(), backwardStreamLine.end());
	streamLine.insert(streamLine.end(), forwardStreamLine.begin(), forwardStreamLine.end());

	return streamLine;
}
