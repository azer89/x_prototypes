CVImg distanceMat = GetDistanceMap(numRegion, aRegion, artDataArray, boundary, focalBoundaries);

// save distance image (OpenCV code)
// http://answers.opencv.org/question/75608/imwrite-a-mat_float-keeping-the-image/
std::stringstream ss1;
ss1 << SystemParams::_image_folder << "\\segmentation\\region_" << numRegion << ".yml";
cv::FileStorage fs(ss1.str(), cv::FileStorage::WRITE);// image.yml file name
fs << "DistanceMat" << distanceMat.GetCVImage(); // text id of your data
