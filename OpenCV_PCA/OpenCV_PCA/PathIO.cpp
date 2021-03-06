
#include "stdafx.h"

#include "PathIO.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#include <sstream> // std::stringstream

// split string
std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems)
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}

// split string
std::vector<std::string> split(const std::string &s, char delim)
{
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}


PathIO::PathIO()
{
}

PathIO::~PathIO()
{
}

/* 
One path per line
format:
	x0 y0 x1 y1 x2 y2...
*/
void PathIO::SavePath(std::vector<AShape> paths, std::string filename)
{
	std::ofstream f;
	f.open(filename);

	for (size_t a = 0; a < paths.size(); a++)
	{
		AShape aPath = paths[a];
		for (size_t b = 0; b < aPath.shape_contours.size(); b++)
		{
			f << std::setprecision(20) << aPath.shape_contours[b].x << " " << std::setprecision(20) << aPath.shape_contours[b].y;
			if (b < aPath.shape_contours.size() - 1) { f << " "; }
		}
		f << "\n";
	}


	f.close();
}

/*
One path per line
format:
x0 y0 x1 y1 x2 y2...
*/
std::vector<AShape> PathIO::LoadPath(std::string filename)
{
	std::vector<AShape> paths;

	std::ifstream myfile(filename);
	while (!myfile.eof())
	{
		AShape aPath;

		std::string line;
		std::getline(myfile, line);

		std::vector<std::string> arrayStr = split(line, ' ');

		if (arrayStr.size() < 2) { continue;  }

		size_t halfLength = arrayStr.size() / 2;

		for (size_t a = 0; a < halfLength; a++)
		{
			int idx = a * 2;
			double x = std::stod(arrayStr[idx]);
			double y = std::stod(arrayStr[idx+1]);
			aPath.shape_contours.push_back(AVector(x, y));
		}

		//std::cout << aPath.points.size() << "\n";

		//aPath.isClosed = true;
		paths.push_back(aPath);
	}

	myfile.close();

	return paths;
}