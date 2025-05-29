#include <vector>
#include <cmath>
#include <iostream>
#include "OperatorClass.h"
#include <igl/readOFF.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/read_triangle_mesh.h>
#include "triangleClass.h"
#include "PointClass.h"
#include <vector>
#include <cmath>
#include <string>
#include <map> 
#define _USE_MATH_DEFINES

Eigen::MatrixXd V;
Eigen::MatrixXi F;

vector<triangleClass> triangleList;
vector<PointClass> pointList;
double simplificationAngle;


OperatorClass::OperatorClass() {

}


void OperatorClass::FindConnected(triangleClass* startTriangle, triangleClass* curTriangle) {
	//for every triangle in object
	for (int x = 0; x < triangleList.size(); x++) {
		int count = 0;
		if (curTriangle->planeNo == triangleList[x].planeNo) {
			continue;
		}
		vector<vector<double>> twoPoints;
		for (int j = 0; j < 3; j++) {
			if (j == 2 && count < 1) {
				break;
			}
			for (int k = 0; k < 3; k++) {
				//check if the bounding box can be updated with any of the next_triangles points
				//boundingBoxUpdate(curTriangle->points[j]);
				//check if triangle 1 overlaps points with triangle 2
				if (curTriangle->points[j] == triangleList[x].points[k]) {
					//increment duplicate point count
					twoPoints.push_back(curTriangle->points[j]);
					count++;

				}
			}
		}
		//if the triangles have any overlapping points
		if (count == 2) {
			//checks if the next_triangles has been assigned a plane number
			if (triangleList[x].planeNo != -1) {
				for (int r = 0; r < 2; r++) {
					//if point is already in the pointList, add the two plane numbers to the point's plane list (if they arent already in it)
					bool found = false;
					for (int j = 0; j < pointList.size(); j++) {
						if (pointList[j].point == twoPoints[r]) {
							found = true;
							pointList[j].AddToPlaneList(curTriangle->planeNo);
							pointList[j].AddToPlaneList(triangleList[x].planeNo);
							break;
						}
					}
					//if point not in list
					if (found == false) {
						pointList.push_back(twoPoints[r]);
						pointList[pointList.size() - 1].AddToPlaneList(curTriangle->planeNo);
						pointList[pointList.size() - 1].AddToPlaneList(triangleList[x].planeNo);
					}
				}
			}
			else {
				double intAng = startTriangle->calcInteriorAngle(triangleList[x]);
				if (intAng <= this->simplificationAngle) {
					triangleList[x].updatePlaneNo(startTriangle->planeNo);
					FindConnected(startTriangle, &triangleList[x]);
				}
			}
		}
	}
}

OperatorClass::funcReturn OperatorClass::getVectors(string filePath, double simplificationAngle) {
	this->simplificationAngle = simplificationAngle;
	igl::read_triangle_mesh(filePath, V, F);
	cout << V.rows();
	for (int x = 0; x < F.rows(); x++) {
		//initialise the current triangle in loop
		vector<vector<double>> nextTriangle = {};
		for (int i = 0; i < 3; i++) {
			nextTriangle.push_back({ V(F(x, i), 0), V(F(x, i), 1), V(F(x, i), 2) });
		}
		triangleClass temp = triangleClass(nextTriangle,x);
		triangleList.push_back(temp);
	}
	int planeCount = 0;
	float percentCalcDenom = triangleList.size();
	for (int t = 0; t < triangleList.size(); t++) {
		if (triangleList[t].planeNo == -1) {
			triangleList[t].updatePlaneNo(planeCount);
			triangleList[t].makeTrianglePrime();
			FindConnected(&triangleList[t], &triangleList[t]);
			planeCount++;
		}
		cout << t / percentCalcDenom * 100 << "%" << endl;
	}

	OperatorClass::funcReturn returnValues;
	returnValues.triangleList = triangleList;
	returnValues.pointList = pointList;
	returnValues.V = V;
	returnValues.F = F;
	returnValues.planeCount = planeCount;
	return returnValues;

}