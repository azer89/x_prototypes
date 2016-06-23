// originally used in VFDRegionWrapper.cpp


void VFDRegionWrapper::ClusterField()
{
	if (_fieldPaths.size() == 0) { return; }

	// ===== resampling =====
	_resampledFieldLines.clear();
	for (size_t a = 0; a < _insideFieldLines.size(); a++)
	{
		ALine ln = _insideFieldLines[a];
		AVector dir = ln.Direction().Norm();
		float radAngle = UtilityFunctions::Angle2D(1, 0, dir.x, dir.y);
		std::vector<AVector> points;

		AVector pt1 = ln.GetPointA();
		pt1.radAngle = radAngle;
		AVector pt2 = ln.GetPointB();
		pt1.radAngle = radAngle;
		points.push_back(pt1);
		points.push_back(pt2);
		_resampledFieldLines.push_back(points);
	}

	// num cluster
	float boundaryArea = UtilityFunctions::AreaOfPolygon(this->_scaledBoundaryPaths[0].points);
	std::cout << "area = " << boundaryArea << "\n";
	int numCluster = std::round(boundaryArea / SystemParams::_cluster_min_area);
	//std::cout << "round = " << std::round(boundaryArea / SystemParams::_cluster_min_area) << "\n";

	//std::vector<std::vector<AVector>>  clusters = _resampledFieldLines;
	_clusters = _resampledFieldLines;

	if (_clusters.size() <= numCluster)
	{
		// done!
		return;
	}
	
	while (_clusters.size() > numCluster)
	{
		float dist = std::numeric_limits<float>::max();
		int index1 = -1;
		int index2 = -1;

		for (size_t a = 0; a < _clusters.size() - 1; a++)
		{
			for (size_t b = a + 1; b < _clusters.size(); b++)
			{
				//float d = UtilityFunctions::SingleLinkageDistance(_clusters[a], _clusters[b]);
				float d = UtilityFunctions::AverageLinkageDistance(_clusters[a], _clusters[b]);
				if (d < dist)
				{
					dist = d;
					index1 = a;
					index2 = b;
				}
				//std::cout << a << " " << b << "\n";
			}
		}
		//std::cout << dist << "\n";

		if (index1 == -1 || index2 == -1)
		{
			std::cout << "a horrible thing happens\n";
		}

		std::vector<AVector> combinedCluster;
		combinedCluster.insert(combinedCluster.end(), _clusters[index1].begin(), _clusters[index1].end());
		combinedCluster.insert(combinedCluster.end(), _clusters[index2].begin(), _clusters[index2].end());

		std::vector<std::vector<AVector>>  newClusters;
		for (size_t a = 0; a < _clusters.size(); a++)
		{
			if (a != index1 && a != index2)
			{
				newClusters.push_back(_clusters[a]);
			}
		}
		newClusters.push_back(combinedCluster);

		_clusters = newClusters;

		//std::cout << _clusters.size() << "\n";
	}

	std::cout << "clustering done\n";

	// ===== knn =====
	//NANOFLANNWrapper knn;
}
