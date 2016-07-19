/*
================================================================================
Read blob file and calculate radius functions
================================================================================
*/
void ShapeRadiusMatching::ProcessBlobs1()
{
	std::cout << "ProcessBlobs\n";

	// ===== read .blob file =====
	std::string filepath = SystemParams::_image_folder + _artName + ".blob";
	_blobs = _pathIO.LoadBlobs(filepath);

	// ===== make clockwise and extend the streamlines =====
	for (int a = 0; a < _blobs.size(); a++)
	{ 
		_blobs[a].MakeClockwise();
		if (!_blobs[a]._isFocalElement)
			{ _blobs[a]._streamLine = ExtendStreamline(_blobs[a]); } 
	}

	// ===== uniform resample =====	
	// if you want the process faster, comment this
	/*for (int a = 0; a < _blobs.size(); a++)
	    { _blobs[a].ResampleBoundary(100); }*/		// hardcoded parameter

	// ===== create vectors of pair functions =====
	_matchedPFIndices = std::vector<int>(_blobs.size());
	_blobPFunctions = std::vector<PairFunctions>(_blobs.size());

	// ===== match file =====
	std::string matchFilename = SystemParams::_image_folder + _artName + ".match";

	//bool doesMatchingFileExist = _pathIO.DoesFileExist(matchFilename);
	//if (doesMatchingFileExist) { _matchedPFIndices = _pathIO.LoadIndices(matchFilename); }

	// ===== calculate pair functions =====
	for (int a = 0; a < _blobs.size(); a++)
	{
		std::cout << a << "/" << _blobs.size() << "\n";
		std::stringstream ss; ss << "blob_" << _artName << "_" << a;

		_blobPFunctions[a] = ProcessABlob(_blobs[a], a, _blobs, ss.str());;
	}

	// ===== save functions =====
	/*std::stringstream pFss;
	pFss << SystemParams::_image_folder << _artName << "_blobs.lr";
	_pathIO.SavePairFunctions(_blobPFunctions, pFss.str());*/
	//_blobPFunctions = _pathIO.LoadPairFunctions(pFss.str());

	// Snap streamline
	for (int a = 0; a < _blobs.size(); a++)
	{
		PairFunctions pFunc = _blobPFunctions[a];
		if (pFunc.GetLeftMean() < 0.1f || pFunc.GetRightMean() < 0.1f)
		{ 
			std::cout << "snap\n";
			_blobs[a]._streamLine = SnapStreamline(_blobs[a]);
		}
	}

	
	//if (this->_always_recalculate_matching)
	//{
		//// ===== distance grids and matching, note that this is a really slow process  =====
		//// uncomment if you want distance grid
		////_eFitting.CalculateBlobDistanceGrids(_blobPFunctions);

		//// ===== do matching =====
		std::cout << "Matching\n";
		for (int a = 0; a < _blobs.size(); a++)
		{
			std::cout << a << "/" << _blobs.size() << "\n";
			std::stringstream ss; ss << "blob_" << _artName << "_" << a;
			_matchedPFIndices[a] = DoMatching(_blobPFunctions[a], _blobs[a], a, ss.str());
			////SavePairFunctionsAnBlobContourToImage(_blobPFunctions[a], _blobs[a], ss.str());
		}
	//}
	//else
	//{
	//	_matchedPFIndices = _pathIO.LoadIndices(matchFilename);
	//}

	// ===== save indices =====
	//this->_pathIO.SaveIndices(_matchedPFIndices, matchFilename);

	// ===== proximity check =====
	//BlobProximityToRegionBoundary(testBlobs);

	// ===== deform ornaments =====
	std::cout << "DeformOrnaments\n";
	std::vector<ARegion> deformedOrnaments = DeformOrnaments(_matchedPFIndices, _blobPFunctions, _blobs);

	// add focal boundaries as ornaments
	for (int i = 0; i < _regions.size(); i++)
	{
		std::vector<std::vector<AVector>> focalBoundaries = _regions[i].GetFocalBoundaries();
		for (int a = 0; a < focalBoundaries.size(); a++)
		{
			std::vector<std::vector<AVector>> boundaries;
			boundaries.push_back(focalBoundaries[a]);

			ARegion oRegion;
			oRegion.SetBoundaries(boundaries);
			deformedOrnaments.push_back(oRegion);
		}
	}

	// ===== save to SVG =====
	MySVGRenderer::SaveOrnamentsToSVG(SystemParams::_image_folder + _artName + ".svg", deformedOrnaments);

	// ===== Display ornaments =====
	std::cout << "DisplayOrnaments\n";
	DisplayOrnaments(deformedOrnaments, _blobs);

	float blobScale = 2;
	_cvWrapper.CreateImage("Blobs", SystemParams::_upscaleFactor * blobScale, SystemParams::_upscaleFactor * blobScale, BGR_255);
	for (int a = 0; a < _blobs.size(); a++)
	{
		MyColor col = _cvWrapper.GetRandomColor();
		_cvWrapper.DrawPoly("Blobs", _blobs[a]._blobBoundary, col, true,  1, blobScale);
		_cvWrapper.DrawPoly("Blobs", _blobs[a]._streamLine,   col, false, 5, blobScale); // originally medial axis
	}
	_cvWrapper.ShowImage("Blobs");
	_cvWrapper.WaitKey();
}
