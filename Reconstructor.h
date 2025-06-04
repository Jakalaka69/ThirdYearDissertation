#pragma once
#include <vector>
#include <cmath>
#define _USE_MATH_DEFINES

using namespace std;




class Reconstructor {
public:
	vector<vector<double>> GetPointsInHoles(vector<vector<PointClass>> Loops);
	vector<PointClass> getRegionLoop(int PlaneX, vector<PointClass> unOrderedFrame);
	vector<double> calcX(triangleClass plane, double y, double z);
	vector<double> calcZ(triangleClass plane, double x, double y);
	vector<double> calcY(triangleClass plane, double x, double z);
	void Reconstruct(OperatorClass::funcReturn returnValues, string outputPath);
	Reconstructor();


};