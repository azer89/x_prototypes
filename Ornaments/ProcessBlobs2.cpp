/*
================================================================================
DON'T USE THIS FUNCTION!
What is this function trying to do?
================================================================================
*/
void ShapeRadiusMatching::ProcessBlobs2()
{
	std::cout << "ProcessBlobs2\n";

	// ===== read .blob file =====
	_blobs = _pathIO.LoadBlobs(SystemParams::_image_folder + _artName + ".blob");

	// ===== make clockwise and extend the streamlines =====
	for (int a = 0; a < _blobs.size(); a++)
	{
		_blobs[a].MakeClockwise();
		if (!_blobs[a]._isFocalElement)
		{ 
			_blobs[a]._streamLine = _lineTracer.ExtendStreamline(&_ornamentRegions[_blobs[a]._whatRegion], _blobs[a]);
		}
	}
	// ===== calculate pair functions =====
	for (int a = 0; a < _blobs.size(); a++)
	{
		std::cout << (a + 1) << "/" << _blobs.size() << "\n";
		std::stringstream ss; ss << "blob_" << _artName << "_" << a;
		_blobs[a]._pFunctions = ProcessABlob(_blobs[a], a, _blobs, ss.str());
	}
	// ===== Snap streamline =====
	/*for (int a = 0; a < _blobs.size(); a++)
	{
		if ((_blobs[a]._pFunctions.GetLeftMean() < 0.1f || _blobs[a]._pFunctions.GetRightMean() < 0.1f) && !_blobs[a]._isFocalElement )
			{ std::cout << "snap\n"; _blobs[a]._streamLine = SnapStreamline(_blobs[a]); }
	}*/
	// ===== create masks =====
	std::cout << "Create masks\n";
	_blobEngine.CreateRegionMasks(_regions);
	// ===== do matching and deformation =====
	std::vector<VFDRegionWrapper> deformedOrnaments; // deformation
	std::vector<ArtData> artDataArray;               // deformation
	std::cout << "Matching and Deformation\n";
	for (int iter = 0; iter < _blobs.size(); iter++)
	{
		std::cout << (iter + 1) << "/" << _blobs.size() << "\n";
		std::stringstream ss; ss << "blob_" << _artName << "_" << iter;

		// recalculate regions
		if (iter > 0)
		{
			//int prevIdx = _blobs[iter - 1]._whatRegion;
			int curIdx = _blobs[iter]._whatRegion;

			std::vector<std::vector<AVector>> ornamentBoundaries;
			for (int a = 0; a < deformedOrnaments.size(); a++)
			{
				if (_blobs[a]._whatRegion == curIdx)
				{
					std::vector<std::vector<AVector>> oBoundaries = deformedOrnaments[a].GetBoundaries();
					ornamentBoundaries.insert(ornamentBoundaries.end(), oBoundaries.begin(), oBoundaries.end());
				}
			}

			if (ornamentBoundaries.size() > 0)
			{
				_blobEngine.CreateOrnamentMask(iter, ornamentBoundaries);
				_blobEngine.RecalculateBlobs(curIdx, iter, &_regions[curIdx], _blobs);
			}
			/*
			if (ornamentBoundaries.size() > 0)
			{
				_blobEngine.RecalculateBlobs(prevIdx, iter, &_regions[prevIdx], ornamentBoundaries, _blobs);
			}
			*/
		}

		// recalculate streamline length (NOT YET)
		/*for (int b = 0; b < _blobs.size(); b++)
			{ _blobs[b].GetStreamlineLength(); }*/

		// sorting
		std::sort(_blobs.begin() + iter, _blobs.end(), ACertainInfoSortPredicate);

		// matching
		_blobs[iter]._matchedPFIndex = DoMatching(_blobs[iter]._pFunctions, _blobs[iter], iter, ss.str());

		// deformation
		int idx = _blobs[iter]._matchedPFIndex;
		ArtData aData = DeformAnOrnament(iter,
										idx,
										_ornamentRegions[idx]._boundaryPaths,	 // boundaries of the ornament
										_ornamentRegions[idx]._skeletonLines[0], // the skeleton of the ornament
										_ornamentPFunctions[idx],				 // current ornament functions
										_blobs[iter]._pFunctions,				 // current blob functions
										_blobs[iter]);							 // current blob

		// store region
		VFDRegionWrapper aReg;
		aReg.SetBoundaries(aData._boundaries);
		deformedOrnaments.push_back(aReg);

		// store art data
		artDataArray.push_back(aData);
	}

	// ===== deform ornaments =====
	//std::cout << "DeformOrnaments\n";
	//std::vector<VFDRegionWrapper> deformedOrnaments;
	//std::vector<ArtData> artDataArray;
	/*for (int iter = 0; iter < _blobs.size(); iter++)
	{
		std::cout << (iter + 1) << "/" << _blobs.size() << "\n";
		// ===== deform =====
		int idx = _blobs[iter]._matchedPFIndex;
		ArtData aData = DeformAnOrnament(iter,
										idx,
										_ornamentRegions[idx]._boundaryPaths,	 // boundaries of the ornament
										_ornamentRegions[idx]._skeletonLines[0], // the skeleton of the ornament
										_ornamentPFunctions[idx],				 // current ornament functions
										_blobs[iter]._pFunctions,				 // current blob functions
										_blobs[iter]);							 // current blob

		// store region
		VFDRegionWrapper aReg;
		aReg.SetBoundaries(aData._boundaries);
		deformedOrnaments.push_back(aReg);

		// store art data
		artDataArray.push_back(aData);
	}*/

	// ===== save art data =====
	std::stringstream aDss;
	aDss << SystemParams::_image_folder << _artName << ".artdata";
	_pathIO.SaveArtData(artDataArray, aDss.str());

	// ===== add focal boundaries as ornaments =====
	for (int i = 0; i < _regions.size(); i++)
	{
		std::vector<std::vector<AVector>> focalBoundaries = _regions[i].GetFocalBoundaries();
		for (int a = 0; a < focalBoundaries.size(); a++)
		{
			std::vector<std::vector<AVector>> boundaries;
			boundaries.push_back(focalBoundaries[a]);

			VFDRegionWrapper oRegion;
			oRegion.SetBoundaries(boundaries);
			deformedOrnaments.push_back(oRegion);
		}
	}

	// ===== save to SVG =====
	//MySVGRenderer::SaveOrnamentsToSVG(SystemParams::_image_folder + _artName + ".svg", deformedOrnaments);
	MySVGRenderer::SaveOrnamentsToSVG(SystemParams::_image_folder + _artName + ".svg", artDataArray);

	// ===== Display ornaments and blobs =====	
	//DisplayOrnaments(deformedOrnaments, _blobs);
	DisplayOrnaments(artDataArray, _blobs);
}
