#pragma once
#include <vector>
#include <cmath>
#define _USE_MATH_DEFINES

using namespace std;

class PointClass {

public:
	vector<double> point;
	vector<int> planeList;


	PointClass(vector<double> input);
	PointClass();
	void AddPlanesToPoint(int x, int y, int z);
};