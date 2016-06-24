//
// int ShapeRadiusMatching::DoMatching(PairFunctions pFunc, ABlob aBlob, int blobNumber, std::string blobName)
//

// draw normalized functions in 500x500 image
{
	std::stringstream imgName;
	int squareSize = 500;
	float functionGap = (float)(squareSize - 100) / (float)SystemParams::_function_length; // height = 400
	imgName << "Matching_Process_" << blobName;
	_cvWrapper.CreateImage(imgName.str(), squareSize, squareSize, BGR_255);

	// witdh = 400

	// functions of ornaments
	_cvWrapper.DrawFilledPoly(imgName.str(), _ornamentPFunctions[matchedIdx].GetBoundary(functionGap, 200), MyColor(255, 0, 0), 1.0f, 250, 50);
	// blob
	_cvWrapper.DrawRetranslatedPoly(imgName.str(), aBlob._blobBoundary, aBlob._streamLine, MyColor(255, 255, 255), 3, 3);

	// functions of blobs
	_cvWrapper.DrawPoly(imgName.str(), pFunc.GetRightBoundary(functionGap, 200), MyColor(0, 255, 0), false, 3, 1.0f, 250, 50);
	_cvWrapper.DrawPoly(imgName.str(), pFunc.GetLeftBoundary(functionGap, 200), MyColor(0, 255, 0), false, 3, 1.0f, 250, 50);

	_cvWrapper.DrawLine(imgName.str(), AVector(0, 250), AVector(500, 250), MyColor(255), 1);
	_cvWrapper.DrawLine(imgName.str(), AVector(250, 0), AVector(250, 500), MyColor(255), 1);
	
	_cvWrapper.SaveImage(imgName.str(), "C:\\Users\\saputra\\workspace\\Images\\debug\\" + blobName + ".png");
}
