// Eigen_Tryout.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <Eigen/Dense>

int _tmain(int argc, _TCHAR* argv[])
{
	using namespace Eigen;

	// row - column
	Matrix2d m;
	m = Matrix2d::Random();
	//m << 1, 0,
	//	 0, 1;
	std::cout << "m =\n";
	std::cout << m << "\n\n";

	Vector2d u(2, 1);
	std::cout << "u =\n";
	std::cout << u << "\n\n";

	MatrixXd d = m * u;
	//std::cout << d.rows() << "-" << d.cols() << "\n";
	std::cout << "m * u =\n";
	std::cout << d << "\n\n";

	Eigen::JacobiSVD<Eigen::Matrix2d> svd(m, Eigen::ComputeFullU | Eigen::ComputeFullV);
	std::cout << "U = \n" << svd.matrixU() << "\n\n";
	std::cout << "V = \n" << svd.matrixV() << "\n\n";
	
	//std::cout << "Hello World\n";

	std::getchar();
	return 0;
}

