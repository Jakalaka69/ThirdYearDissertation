#include <igl/opengl/glfw/Viewer.h>
#include <igl/triangle/triangulate.h>
#include <unordered_set>
#include "triangleClass.h"
#include <igl/readOFF.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/read_triangle_mesh.h>
#include "Plane.h"
#include <vector>
#include <cmath>
#include <string>

#define _USE_MATH_DEFINES
using namespace std;

Eigen::MatrixXd V;
Eigen::MatrixXi F;
double pi = 3.14159265;

void vecToString(vector<vector<double>> vec) {
	for (int y = 0; y < vec.size();y++) {

		cout << endl << "== " << vec[y][0] << " ," << vec[y][1] << " ," << vec[y][2];

	}

}

vector<vector<vector<double>>> FindConnected(vector<vector<double>> startTriangle, vector<vector<vector<double>>> fullConnectedList, vector<vector<double>> curTriangle) {



	for (int x = 0; x < F.rows();x++) {

		//if the triangle has not already been added to the fullConnectedList (stops repeats and looping)
		if (F(x, 0) == 0 && F(x, 1) == 0 && F(x, 2) == 0) {
			continue;
		}

		//initialise the current triangle in loop


		vector<vector<double>> nextTriangle = {};
		vector<vector<double>> touchingPoints = {};
		vector<vector<double>> allPoints = {};
		vector<vector<double>> final4Points = {};

		allPoints = curTriangle;

		for (int i = 0; i < 3; i++) {

			nextTriangle.push_back({ V(F(x, i), 0), V(F(x, i), 1), V(F(x, i), 2) });


			allPoints.push_back({ V(F(x, i), 0), V(F(x, i), 1), V(F(x, i), 2) });


			//std::cout << V(F(x, i), 0) << " " << V(F(x, i), 1) << " " << V(F(x, i), 2) << endl;
		}
		//cout << x << endl;

		//if (x == 14) {
		//	printf("BREAK");
		//}

		//NEW
		triangleClass NEXT_TRIANGLE = triangleClass(nextTriangle);
		triangleClass START_TRIANGLE = triangleClass(startTriangle);


		//cout << startTriangle[0][0] << endl;
		//cout << START_TRIANGLE.returnNormal().size() << endl;
		//NEW

			//check all points of the startTriangle that we pass in and the current triangle of the for loop and keep a count of the amount of corners that touch
		int count = 0;
		for (int j = 0;j < 3;j++) {
			for (int k = 0; k < 3; k++) {

				if (curTriangle[j] == nextTriangle[k]) {

					touchingPoints.push_back(curTriangle[j]);

					count++;
				}

			}
		}


		//below code sorts and removes duplicate points leaving 4 points in triangles that touch in 2 places
		//trying to come up with method of calculating angle between 2 traingles
		//may have to forgo the duplicate point removal

		//IDEA: look at unique points before removing the other points, that way we know these are the 2 peaks we have to calc the angles between

		sort(allPoints.begin(), allPoints.end());
		allPoints.erase(unique(allPoints.begin(), allPoints.end()), allPoints.end());


		int aVertIndex;
		int dVertIndex;
		//if the amount of corners that touch is equal to 2 then remove the current triangle of the for loop from F
		//and recurse, using the current triangle as the start triangle for the function
		if (count == 2) {
			//NEW

			double intAng = START_TRIANGLE.calcInteriorAngle(NEXT_TRIANGLE);
			//NEW

			if (intAng < 30) {


				unsigned int numRows = F.rows() - 1;
				unsigned int numCols = F.cols();

				if (x < numRows) {
					F.block(x, 0, numRows - x, numCols) = F.block(x + 1, 0, numRows - x, numCols);
				}

				F.conservativeResize(numRows, numCols);


				fullConnectedList.push_back(nextTriangle);

				//changed startTriangle to temp + 1 to incrmement triangle each loop
				fullConnectedList = FindConnected(startTriangle, fullConnectedList, nextTriangle);

			}

		}




	}
	return fullConnectedList;


}

void combineSimilarPlanes(vector<Plane*> planeList) {
	for (int x = 0; x < planeList.size();x++) {
		Plane* curPlane = planeList[x];
		for (int y = 0;y < planeList.size();y++) {

		}
	}
}



vector<double> threePlaneIntersectionPoint(Plane plane1, Plane* plane2, Plane* plane3) {
	vector<vector<double>> planeNormals;
	Eigen::MatrixXd A(3, 3);
	Eigen::MatrixXd D(3, 1);


	vector<double> Normal2 = plane1.GetNormal();

	vector<double> P1 = plane1.getRelatedTriangle()[0];


	A(0, 0) = Normal2[0];
	A(0, 1) = Normal2[1];
	A(0, 2) = Normal2[2];

	D(0) = (Normal2[0] * P1[0] + Normal2[1] * P1[1] + Normal2[2] * P1[2]);

	Normal2 = plane2->GetNormal();
	P1 = plane2->getRelatedTriangle()[0];

	A(1, 0) = Normal2[0];
	A(1, 1) = Normal2[1];
	A(1, 2) = Normal2[2];
	D(1) = (Normal2[0] * P1[0] + Normal2[1] * P1[1] + Normal2[2] * P1[2]);

	Normal2 = plane3->GetNormal();
	P1 = plane3->getRelatedTriangle()[0];

	A(2, 0) = Normal2[0];
	A(2, 1) = Normal2[1];
	A(2, 2) = Normal2[2];
	D(2) = (Normal2[0] * P1[0] + Normal2[1] * P1[1] + Normal2[2] * P1[2]);


	Eigen::MatrixXd inter(3, 1);

	inter = A.inverse() * D;

	cout << endl << "->" << inter << "<-" << endl;

	vector<double> interPoint = { inter(0),inter(1),inter(2) };



	return interPoint;
}

vector<vector<double>> GetPointsInHoles(vector<vector<vector<double>>> Loops) {
	//calculating areas of each loop to find the biggest
	vector<vector<double>> pointsInHoles;
	int pos = 0;
	double biggestArea = 0;
	for (int x = 0; x < Loops.size();x++) {
		vector<double> anchorPoint = Loops[x][0];
		double totalArea = 0;
		for (int y = 1; y < Loops[x].size(); y++) {
			if (y + 1 == Loops[x].size()) {
				continue;
			}
			else {
				vector<double> c2 = Loops[x][y];
				vector<double> c3 = Loops[x][y + 1];
				double a = sqrt(pow(anchorPoint[0] - c2[0], 2) + pow(anchorPoint[1] - c2[1], 2) + pow(anchorPoint[2] - c2[2], 2));
				double b = sqrt(pow(anchorPoint[0] - c3[0], 2) + pow(anchorPoint[1] - c3[1], 2) + pow(anchorPoint[2] - c3[2], 2));
				double c = sqrt(pow(c3[0] - c2[0], 2) + pow(c3[1] - c2[1], 2) + pow(c3[2] - c2[2], 2));
				double s = (a + b + c) / 2;
				double area = sqrt(s * (s - a) * (s - b) * (s - c));
				totalArea += area;
			}

		}
		if (totalArea > biggestArea) {
			biggestArea = totalArea;
			pos = x;

		}
	}
	for (int x = 0; x < Loops.size();x++) {

		if (x == pos) {
			continue;
		}
		else {
			vector<double> c1 = Loops[x][0];
			vector<double> c2 = Loops[x][1];
			vector<double> c3 = Loops[x][2];
			double a = sqrt(pow(c1[0] - c2[0], 2) + pow(c1[1] - c2[1], 2) + pow(c1[2] - c2[2], 2));
			double b = sqrt(pow(c1[0] - c3[0], 2) + pow(c1[1] - c3[1], 2) + pow(c1[2] - c3[2], 2));
			double c = sqrt(pow(c3[0] - c2[0], 2) + pow(c3[1] - c2[1], 2) + pow(c3[2] - c2[2], 2));





			double x = (a * c1[0] + b * c2[0] + c * c3[0]) / (a + b + c);
			double y = (a * c1[1] + b * c2[1] + c * c3[1]) / (a + b + c);
			double z = (a * c1[2] + b * c2[2] + c * c3[2]) / (a + b + c);

			pointsInHoles.push_back({ x, y });
		}
	}
	return pointsInHoles;
}
vector<vector<vector<double>>> getNextPlane(Plane planeX, vector<Plane*> remaining) {

	//Get connected planes to input plane
	vector<Plane*> adjacentPlanes = remaining;

	//initialise the last plane checked
	Plane* lastPlane = NULL;
	;
	//initialise the frame
	vector<vector<double>> frame;
	vector<vector<vector<double>>> Loops;
	//used for when there is more than one loop
	vector<Plane*> usedPlanes;

	//initialise the first plane checked
	Plane* curPlane = adjacentPlanes[0];

	//used to prevent looping around the same frame






	for (int i = 0; i < adjacentPlanes.size();i++) {
		for (int x = 0; x < adjacentPlanes.size(); x++) {
			Plane* nextPlane = adjacentPlanes[x];
			//checks to make sure the current plane is worth evaluating and also if its gone back to the start, that final condition stops the other conditions from mattering if its the first loop
			if ((nextPlane == curPlane || nextPlane == lastPlane || usedPlanes.size() >= adjacentPlanes.size()) && lastPlane != NULL) {
				continue;
			}
			vector<Plane*> nextPlaneList = nextPlane->GetConnectedPlanes();
			if (find(nextPlaneList.begin(), nextPlaneList.end(), curPlane) != nextPlaneList.end()) {
				frame.push_back(threePlaneIntersectionPoint(planeX, curPlane, nextPlane));
				lastPlane = curPlane;
				curPlane = nextPlane;
				usedPlanes.push_back(curPlane);
			}
		}
	}


	if (usedPlanes.size() == adjacentPlanes.size()) {
		for (int x = 0; x < usedPlanes.size();x++) {
			
		}
		Loops.push_back(frame);

		return Loops;
	}
	else {
		vector<Plane*> unusedPlanes = {};
		for (int x = 0; x < adjacentPlanes.size();x++) {
			if (find(usedPlanes.begin(), usedPlanes.end(), adjacentPlanes[x]) == usedPlanes.end()) {
				unusedPlanes.push_back(adjacentPlanes[x]);

			}

		}

		vector<vector<vector<double>>> L = getNextPlane(planeX, unusedPlanes);
		Loops.push_back(frame);
		Loops.insert(Loops.end(), L.begin(), L.end());


		return Loops;


	}


}

vector<double> calcX(Plane plane, double y, double z) {
	vector<double> pointOnPlane = plane.getRelatedTriangle()[0];
	vector<double> normal = plane.GetNormal();
	double total = 0;
	total += normal[0] * pointOnPlane[0] + normal[1] * pointOnPlane[1] + normal[2] * pointOnPlane[2];

	total -= normal[1] * y + normal[2] * z;

	total /= normal[0];

	return { y, z, total };


}

vector<double> calcZ(Plane plane, double x, double y) {
	vector<double> pointOnPlane = plane.getRelatedTriangle()[0];
	vector<double> normal = plane.GetNormal();
	double total = 0;
	total += normal[0] * pointOnPlane[0] + normal[1] * pointOnPlane[1] + normal[2] * pointOnPlane[2];

	total -= normal[0] * x + normal[1] * y;

	total /= normal[2];

	return { x, y, total };


}
vector<double> calcY(Plane plane, double x, double z) {
	vector<double> pointOnPlane = plane.getRelatedTriangle()[0];
	vector<double> normal = plane.GetNormal();
	double total = 0;
	total += normal[0] * pointOnPlane[0] + normal[1] * pointOnPlane[1] + normal[2] * pointOnPlane[2];

	

	total -= (normal[0] * x + normal[2] * z);



	total /= normal[1];



	return { x, z, total };


}

bool AxisCheck(vector<vector<double>> loop) {
	vector<double> checkedXs;
	bool x = false;
	bool y = false;
	bool z = false;

	for (int x = 0; x < loop.size();x++) {
		if (find(checkedXs.begin(), checkedXs.end(), loop[x][0]) != checkedXs.end()) {
			
			x = true;
		}
		else {
			checkedXs.push_back(loop[x][0]);
		}
	}

	vector<double> checkedYs;

	for (int x = 0; x < loop.size();x++) {
		if (find(checkedYs.begin(), checkedYs.end(), loop[x][1]) != checkedYs.end()) {
			y = true;
		}
		else {
			checkedYs.push_back(loop[x][1]);
		}
	}

	vector<double> checkedZs;

	for (int x = 0; x < loop.size();x++) {
		if (find(checkedZs.begin(), checkedZs.end(), loop[x][2]) != checkedZs.end()) {
			z = true;
		}
		else {
			checkedZs.push_back(loop[x][2]);
		}
	}
	return 1;
}


int main(int argc, char* argv[])
{
	// Load a mesh in OFF format
	igl::read_triangle_mesh("C:/Users/jaywh/source/repos/Dissertation/models"  "/8SidedTest.obj", V, F);
	
	for (int x = 0; x < V.size(); x++) {
		if (V(x) == 0) {
			V(x) = 0;
		}
	}

	for (int p = 0; p < V.size();p++) {
		
		int temp = (V(p) * 10000);
		double temp1 = temp;
		double temp2 = temp1 / 10000;
		cout << temp1 / 10000 << " ";

		V(p) = temp2;
	}

	//Gets number of triangles from the faces matrix
	int numOfTriangles = F.rows();
	cout << numOfTriangles;
	vector<vector<vector<double>>> fullConnectedList;

	//select random triangle
	//int random = rand() % numOfTrianlges;


	//assign point coordinates of a random traingle 
	vector<double> P1 = { V(F(0, 0), 0), V(F(0, 0), 1), V(F(0, 0), 2) };
	vector<double> P2 = { V(F(0, 1), 0), V(F(0, 1), 1), V(F(0, 1), 2) };
	vector<double> P3 = { V(F(0, 2), 0), V(F(0, 2), 1), V(F(0, 2), 2) };
	//initialise triangle with the points
	vector<vector<double>> randTriangle0 = { P1, P2, P3 };


	P1 = { V(F(10, 0), 0), V(F(10, 0), 1), V(F(10, 0), 2) };
	P2 = { V(F(10, 1), 0), V(F(10, 1), 1), V(F(10, 1), 2) };
	P3 = { V(F(10, 2), 0), V(F(10, 2), 1), V(F(10, 2), 2) };
	//initialise triangle with the points
	vector<vector<double>> randTriangle10 = { P1, P2, P3 };


	P1 = { V(F(14, 0), 0), V(F(14, 0), 1), V(F(14, 0), 2) };
	P2 = { V(F(14, 1), 0), V(F(14, 1), 1), V(F(14, 1), 2) };
	P3 = { V(F(14, 2), 0), V(F(14, 2), 1), V(F(14, 2), 2) };
	//initialise triangle with the points
	vector<vector<double>> randTriangle14 = { P1, P2, P3 };


	P1 = { V(F(18, 0), 0), V(F(18, 0), 1), V(F(18, 0), 2) };
	P2 = { V(F(18, 1), 0), V(F(18, 1), 1), V(F(18, 1), 2) };
	P3 = { V(F(18, 2), 0), V(F(18, 2), 1), V(F(18, 2), 2) };
	//initialise triangle with the points
	vector<vector<double>> randTriangle18 = { P1, P2, P3 };

	P1 = { V(F(12, 0), 0), V(F(12, 0), 1), V(F(12, 0), 2) };
	P2 = { V(F(12, 1), 0), V(F(12, 1), 1), V(F(12, 1), 2) };
	P3 = { V(F(12, 2), 0), V(F(12, 2), 1), V(F(12, 2), 2) };
	//initialise triangle with the points
	vector<vector<double>> randTriangle12 = { P1, P2, P3 };

	P1 = { V(F(16, 0), 0), V(F(16, 0), 1), V(F(16, 0), 2) };
	P2 = { V(F(16, 1), 0), V(F(16, 1), 1), V(F(16, 1), 2) };
	P3 = { V(F(16, 2), 0), V(F(16, 2), 1), V(F(16, 2), 2) };
	//initialise triangle with the points
	vector<vector<double>> randTriangle16 = { P1, P2, P3 };

	P1 = { V(F(8, 0), 0), V(F(8, 0), 1), V(F(8, 0), 2) };
	P2 = { V(F(8, 1), 0), V(F(8, 1), 1), V(F(8, 1), 2) };
	P3 = { V(F(8, 2), 0), V(F(8, 2), 1), V(F(8, 2), 2) };
	//initialise triangle with the points
	vector<vector<double>> randTriangle8 = { P1, P2, P3 };

	P1 = { V(F(6, 0), 0), V(F(6, 0), 1), V(F(6, 0), 2) };
	P2 = { V(F(6, 1), 0), V(F(6, 1), 1), V(F(6, 1), 2) };
	P3 = { V(F(6, 2), 0), V(F(6, 2), 1), V(F(6, 2), 2) };
	//initialise triangle with the points
	vector<vector<double>> randTriangle6 = { P1, P2, P3 };

	P1 = { V(F(2, 0), 0), V(F(2, 0), 1), V(F(2, 0), 2) };
	P2 = { V(F(2, 1), 0), V(F(2, 1), 1), V(F(2, 1), 2) };
	P3 = { V(F(2, 2), 0), V(F(2, 2), 1), V(F(2, 2), 2) };
	//initialise triangle with the points
	vector<vector<double>> randTriangle2 = { P1, P2, P3 };

	P1 = { V(F(4, 0), 0), V(F(4, 0), 1), V(F(4, 0), 2) };
	P2 = { V(F(4, 1), 0), V(F(4, 1), 1), V(F(4, 1), 2) };
	P3 = { V(F(4, 2), 0), V(F(4, 2), 1), V(F(4, 2), 2) };
	//initialise triangle with the points
	vector<vector<double>> randTriangle4 = { P1, P2, P3 };

	//list of connected faces to pass into FindConnected recursive function
	//Call function, currently just returns all triangles in the model but once we 
	//include the angle checks it will then print the correct triangles to make a plane out of
	//swap randTriangle for t
	// 
	//fullConnectedList.push_back(randTriangle);
	//fullConnectedList = FindConnected(randTriangle,fullConnectedList,randTriangle);



	if(angleCheck == failed && )



	Plane plane0(randTriangle0);
	Plane plane10(randTriangle10);
	Plane plane14(randTriangle14);
	Plane plane18(randTriangle18);
	Plane plane12(randTriangle12);
	Plane plane16(randTriangle16);
	Plane plane8(randTriangle8);
	Plane plane6(randTriangle6);
	Plane plane2(randTriangle2);
	Plane plane4(randTriangle4);

	plane0.AddConnectedPlane(&plane10);
	plane0.AddConnectedPlane(&plane6);
	plane0.AddConnectedPlane(&plane2);
	plane0.AddConnectedPlane(&plane4);

	plane10.AddConnectedPlane(&plane0);
	plane10.AddConnectedPlane(&plane14);
	plane10.AddConnectedPlane(&plane2);
	plane10.AddConnectedPlane(&plane4);

	plane14.AddConnectedPlane(&plane10);
	plane14.AddConnectedPlane(&plane18);
	plane14.AddConnectedPlane(&plane2);
	plane14.AddConnectedPlane(&plane4);

	plane18.AddConnectedPlane(&plane14);
	plane18.AddConnectedPlane(&plane12);
	plane18.AddConnectedPlane(&plane2);
	plane18.AddConnectedPlane(&plane4);

	plane12.AddConnectedPlane(&plane18);
	plane12.AddConnectedPlane(&plane16);
	plane12.AddConnectedPlane(&plane2);
	plane12.AddConnectedPlane(&plane4);

	plane16.AddConnectedPlane(&plane12);
	plane16.AddConnectedPlane(&plane8);
	plane16.AddConnectedPlane(&plane2);
	plane16.AddConnectedPlane(&plane4);

	plane8.AddConnectedPlane(&plane16);
	plane8.AddConnectedPlane(&plane6);
	plane8.AddConnectedPlane(&plane2);
	plane8.AddConnectedPlane(&plane4);

	plane6.AddConnectedPlane(&plane8);
	plane6.AddConnectedPlane(&plane0);
	plane6.AddConnectedPlane(&plane2);
	plane6.AddConnectedPlane(&plane4);

	plane2.AddConnectedPlane(&plane0);
	plane2.AddConnectedPlane(&plane10);
	plane2.AddConnectedPlane(&plane14);
	plane2.AddConnectedPlane(&plane18);
	plane2.AddConnectedPlane(&plane12);
	plane2.AddConnectedPlane(&plane16);
	plane2.AddConnectedPlane(&plane8);
	plane2.AddConnectedPlane(&plane6);

	plane4.AddConnectedPlane(&plane0);
	plane4.AddConnectedPlane(&plane10);
	plane4.AddConnectedPlane(&plane14);
	plane4.AddConnectedPlane(&plane18);
	plane4.AddConnectedPlane(&plane12);
	plane4.AddConnectedPlane(&plane16);
	plane4.AddConnectedPlane(&plane8);
	plane4.AddConnectedPlane(&plane6);

	vector<Plane> PlaneList;

	PlaneList.push_back(plane0);
	PlaneList.push_back(plane10);
	PlaneList.push_back(plane14);
	PlaneList.push_back(plane18);
	PlaneList.push_back(plane12);
	PlaneList.push_back(plane16);
	PlaneList.push_back(plane8);
	PlaneList.push_back(plane6);
	PlaneList.push_back(plane2);
	PlaneList.push_back(plane4);
	


	/*
		cout << "Plane 1: " << plane1.toString();
	cout << "Plane 3: " << plane3.toString();
	cout << "Plane 4: " << plane4.toString();
	cout << "Plane 7: " << plane7.toString();
	cout << "Plane 9: " << plane9.toString();
	cout << "Plane 10: " << plane10.toString();
	*/



	Eigen::MatrixXd D;
	Eigen::MatrixXi P;







	for (int x = 0; x < PlaneList.size();x++) {
		cout << PlaneList[x].toString();
		vector<vector<vector<double>>> Loops = getNextPlane(PlaneList[x], PlaneList[x].GetConnectedPlanes());

		vector<vector<double>> holePoints;
		//vector<vector<double>> holePoints = GetPointsInHoles(Loops);
		vector<vector<double>> frame;




		Eigen::MatrixXd V3;
		Eigen::MatrixXi E;
		Eigen::MatrixXd H;
		Eigen::MatrixXd V2;
		Eigen::MatrixXi F2;


		int axis = 0;


		if (PlaneList[x].GetNormal()[0] == 0 && PlaneList[x].GetNormal()[2] == 0) {
			axis = 1;
		}
		else if (PlaneList[x].GetNormal()[1] == 0 && PlaneList[x].GetNormal()[2] == 0) {
			axis = 0;
		}
		else if (PlaneList[x].GetNormal()[0] == 0 && PlaneList[x].GetNormal()[1] == 0) {
			axis = 2;
		}
		else if (abs(PlaneList[x].GetNormal()[0]) == abs(PlaneList[x].GetNormal()[2])) {
			axis = 0;
		}
		else if (abs(PlaneList[x].GetNormal()[1]) == abs(PlaneList[x].GetNormal()[2])) {
			axis = 1;
		}
		else if (abs(PlaneList[x].GetNormal()[0]) == abs(PlaneList[x].GetNormal()[1])) {
			axis = 0;
		}



		for (int x = 0; x < Loops.size();x++) {
			V3.resize(V3.rows() + Loops[x].size(), 2);
			E.resize(E.rows() + Loops[x].size(), 2);
		}
		int z = 0;
		for (int x = 0; x < Loops.size();x++) {
			for (int y = 0; y < Loops[x].size();y++) {
				if (axis == 0) {
					V3(y + z, 0) = Loops[x][y][1];
					V3(y + z, 1) = Loops[x][y][2];
				}
				else if (axis == 1) {
					V3(y + z, 0) = Loops[x][y][0];
					V3(y + z, 1) = Loops[x][y][2];
				}
				else if (axis == 2) {
					V3(y + z, 0) = Loops[x][y][0];
					V3(y + z, 1) = Loops[x][y][1];
				}

				E(y + z, 0) = y + z;
				if (y + z == z + Loops[x].size() - 1)
				{
					E(y + z, 1) = z;
				}
				else
				{
					E(y + z, 1) = y + z + 1;
				}

			}
			z += Loops[x].size();
		}





		H.resize(holePoints.size(), 2);

		for (int x = 0; x < holePoints.size();x++) {
			H(x, 0) = holePoints[x][0];
			H(x, 1) = holePoints[x][1];
		}

		cout << V3;

		igl::triangle::triangulate(V3, E, H, "1", V2, F2);

		Eigen::MatrixXd V4;
		Eigen::MatrixXi F4;

		V4.resize(V2.rows(), 3);

		for (int p = 0;p < V2.rows();p++) {
			if (axis == 0) {
				V4(p, 0) = calcX(PlaneList[x], V2(p, 0), V2(p, 1))[2];
				V4(p, 1) = V2(p, 0);
				V4(p, 2) = V2(p, 1);
			}
			else if (axis == 1) {
				V4(p, 0) = V2(p, 0);
				V4(p, 1) = calcY(PlaneList[x], V2(p, 0), V2(p, 1))[2];

				V4(p, 2) = V2(p, 1);
			}
			else if (axis == 2) {
				V4(p, 0) = V2(p, 0);;
				V4(p, 1) = V2(p, 1);
				V4(p, 2) = calcZ(PlaneList[x], V2(p, 0), V2(p, 1))[2];
			}



		}








		if (D.rows() == 0) {

			D.conservativeResize(V4.rows(), V4.cols());
			D << V4;
		}
		else {
			Eigen::MatrixXd temp1(D.rows() + V4.rows(), 3);

			temp1 << D, V4;
			D = temp1;
		}

		vector<double> test1 = {0.751771,1.9335,1.9433 };
		vector<double> test2 = { 0.751771,1.9335,1.9433 };

		
	
	
		//Removing duplicates from the verticies list
		cout << endl << D << endl;
		vector<vector<double>> checked;
		
		for (int i = 0; i < D.rows();i++) {
			cout << D(i, 0);
			vector<double> cur = { D(i,0),D(i,1),D(i,2) };
			if (cur[0] == test1[0]) {
				cout << "+++++++++++++++++++++++++++++++++++++++++++++++++";
			}
			if (find(checked.begin(), checked.end(), cur) == checked.end()) {
				cout << endl << cur[0] << " " << cur[1] << " " << cur[2] << endl;
				checked.push_back(cur);
			}
			else {
				int rows = D.rows() - 1;
				if (i < rows) {
					D.block(i, 0, rows - i, 3) = D.block(i + 1, 0, rows - i, 3);
				}
				D.conservativeResize(rows, 3);
				i--;
			}
		}
		
		
		cout << endl << D << endl;

		for (int n = 0;n < F2.rows();n++) {
			for (int m = 0;m < 3;m++) {
				bool found = false;
				for (int l = 0;l < D.rows();l++) {
					if (D.block<1, 3>(l, 0) == V4.block<1, 3>(F2(n, m), 0)) {
						F2(n, m) = l;
						found = true;
						
						break;
					}
				}
				if (found == false) { F2(n, m) += D.rows(); }
			}
		}
		

	

		
		if (P.rows() == 0) {
			P.conservativeResize(F2.rows(), F2.cols());
			
			P << F2;
		}
		else {
			Eigen::MatrixXi temp2(P.rows() + F2.rows(), 3);
			
			temp2 << P, F2;
			P = temp2;
		}
		

		

		

		

		
	}

	
	if (igl::writeOBJ("C:/Users/jaywh/source/repos/Dissertation/models/cube.obj", D, P)) {
		cout << "yay";
	}
	else {
		cout << "nay"
			;
	}
	

	cout<< endl << "->>>" <<  D;

	//Plot the mesh

	std::cout << endl << endl << endl;
	//open libigl viewer

	igl::opengl::glfw::Viewer viewer;
	
	viewer.data().set_mesh(V, F);
	viewer.launch();

	
		


}





/*output code for printing a triangle(weird because its a vector)

cout << "Start of startTriangle" << endl;


for (vector<double> point : startTriangle) {
	cout << "Point " << " coordinates:";

	for (double coord : point) {
		cout << coord << " ";
	}
	cout << endl;
}
cout << "End startTriangle" << endl;*/



//Code to create visual plane for testing

/*
V.conservativeResize(V.rows() + 3, V.cols());

	double x_c = (P1[0] + P2[0] + P3[0]) / 3;
	double y_c = (P1[1] + P2[1] + P3[1]) / 3;
	double z_c = (P1[2] + P2[2] + P3[2]) / 3;

	V(V.rows() - 1, 0) = x_c + (P1[0] - x_c) * 10;
	V(V.rows() - 1, 1) = y_c + (P1[1] - y_c) * 10;
	V(V.rows() - 1, 2) = z_c + (P1[2] - z_c) * 10;
	V(V.rows() - 2, 0) = x_c + (P2[0] - x_c) * 10;
	V(V.rows() - 2, 1) = y_c + (P2[1] - y_c) * 10;
	V(V.rows() - 2, 2) = z_c + (P2[2] - z_c) * 10;
	V(V.rows() - 3, 0) = x_c + (P3[0] - x_c) * 10;
	V(V.rows() - 3, 1) = y_c + (P3[1] - y_c) * 10;
	V(V.rows() - 3, 2) = z_c + (P3[2] - z_c) * 10;

	F.conservativeResize(F.rows() + 1, V.cols());

	F(F.rows() - 1, 0) = V.rows() - 1;
	F(F.rows() - 1, 1) = V.rows() - 2;
	F(F.rows() - 1, 2) = V.rows() - 3;*/
