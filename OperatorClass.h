#pragma once
#include <vector>
#include <cmath>
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


class OperatorClass {

public:
	Eigen::MatrixXd V;
	Eigen::MatrixXi F;
	vector<triangleClass> triangleList;
	vector<PointClass> pointList;
	double simplificationAngle;
	struct funcReturn
	{
		vector<triangleClass> triangleList;
		vector<PointClass> pointList;
	};


	OperatorClass::OperatorClass();
	void OperatorClass::FindConnected(triangleClass* startTriangle, triangleClass* curTriangle);
	OperatorClass::funcReturn OperatorClass::getVectors(string filePath, double simplificationAngle);

};

