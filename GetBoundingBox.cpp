ARectangle GetBoundingBox(std::vector<std::vector<AVector>> boundaries)
{
	float xMax = std::numeric_limits<float>::min();
	float yMax = std::numeric_limits<float>::min();
	float xMin = std::numeric_limits<float>::max();
	float yMin = std::numeric_limits<float>::max();

	for (int a = 0; a < boundaries.size(); a++)
	{
		for (int b = 0; b < boundaries[a].size(); b++)
		{
			AVector pt = boundaries[a][b];

			if (pt.x > xMax) { xMax = pt.x; }
			if (pt.y > yMax) { yMax = pt.y; }
			if (pt.x < xMin) { xMin = pt.x; }
			if (pt.y < yMin) { yMin = pt.y; }
		}
	}

	return ARectangle(AVector(xMin, yMin), xMax - xMin, yMax - yMin);
}
