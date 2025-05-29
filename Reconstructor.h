#pragma once
#include <vector>
#include <cmath>
#define _USE_MATH_DEFINES

using namespace std;




class Reconstructor {
	struct matrixOut
	{
		Eigen::MatrixXd D;
		Eigen::MatrixXi P;

	};
public:
	vector<vector<double>> GetPointsInHoles(vector<vector<PointClass>> Loops);
	vector<PointClass> getRegionLoop(int PlaneX, vector<PointClass> unOrderedFrame);
	vector<double> calcX(triangleClass plane, double y, double z);
	vector<double> calcZ(triangleClass plane, double x, double y);
	vector<double> calcY(triangleClass plane, double x, double z);
	Reconstructor::matrixOut Reconstruct(OperatorClass::funcReturn returnValues, string outputPath);
	Reconstructor();


};