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
PointClass::PointClass() {

}
void PointClass::AddToPlaneList(int x)
{
	if (std::find(planeList.begin(), planeList.end(), x) == planeList.end()) {
		planeList.push_back(x);
	}
}
bool operator==(PointClass a, PointClass b) {
	return a.point == b.point;
}