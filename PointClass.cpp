#include <vector>
#include <cmath>
#include <iostream>
#include "PointClass.h"
#define _USE_MATH_DEFINES


vector<double> point;
vector<int> planeList;


PointClass::PointClass(vector<double> input) {
	this->point = input;
}

void PointClass::AddPlanesToPoint(int x, int y, int z) {
	planeList.push_back(x);
	planeList.push_back(y);
	planeList.push_back(z);
}