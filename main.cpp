#include <igl/opengl/glfw/Viewer.h>
#include <igl/triangle/triangulate.h>
#include <unordered_set>

#include <igl/readOFF.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/read_triangle_mesh.h>
#include "Plane.h"
#include "triangleClass.h"
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
		
		//cout << endl << "== " << vec[y][0] << " ," << vec[y][1] << " ," << vec[y][2];
		
	}
	
}

triangleClass FindConnected(triangleClass startTriangle, vector<vector<vector<double>>> fullConnectedList, vector<vector<double>> curTriangle) {

	

	for (int x = 0; x < F.rows();x++) {

		//if the triangle has not already been added to the fullConnectedList (stops repeats and looping)
		if (F(x, 0) == 0 && F(x, 1) == 0 && F(x, 2) == 0) {
			continue;
		}

		//initialise the current triangle in loop
		vector<vector<double>> nextTriangle = {};
		vector<vector<double>> touchingPoints = {};

		for (int i = 0; i < 3; i++) {

			nextTriangle.push_back({ V(F(x, i), 0), V(F(x, i), 1), V(F(x, i), 2) });

		}

		//NEW

		triangleClass NEXT_TRIANGLE = triangleClass(nextTriangle);

		//NEW

			//check all points of the startTriangle that we pass in and the current triangle of 
			// the for loop and keep a count of the amount of corners that touch
			int count = 0;
			for (int j = 0;j < 3;j++) {
				for (int k = 0; k < 3; k++) {

					if (curTriangle[j] == nextTriangle[k]) {

						touchingPoints.push_back(curTriangle[j]);

						count++;
					}

				}
			}

			//if the amount of corners that touch is equal to 2 then remove the current triangle of the for loop from F
			//and recurse, using the current triangle as the start triangle for the function


			if (count == 2) {
				
				//NEW
				
				double intAng = startTriangle.calcInteriorAngle(NEXT_TRIANGLE);
				//NEW

				if (intAng < 30) {

						//adds row to matrix
					
						unsigned int numRows = F.rows() - 1;
						unsigned int numCols = F.cols();

						if (x < numRows) {
							F.block(x, 0, numRows - x, numCols) = F.block(x + 1, 0, numRows - x, numCols);
						}

						F.conservativeResize(numRows, numCols);

					
						fullConnectedList.push_back(nextTriangle);
						startTriangle.addToAdjacentTriangles(NEXT_TRIANGLE);
						if (startTriangle.isTrianglePresent(NEXT_TRIANGLE)) {
							//printf("TESTWORKING");
						}

						

						

						//changed startTriangle to temp + 1 to incrmement triangle each loop
						startTriangle = FindConnected(startTriangle, fullConnectedList, nextTriangle);
					
				}

				else {
					startTriangle.addToBorderTriangles(NEXT_TRIANGLE);
				}

			}




		}


	
	return startTriangle;
	

}

vector<double> threePlaneIntersectionPoint(Plane plane1, Plane* plane2, Plane* plane3) {
	vector<vector<double>> planeNormals;
	Eigen::MatrixXd A(3, 3);
	Eigen::MatrixXd D(3, 1);

	
	vector<double> Normal2 = plane1.GetNormal();
	
	vector<double> P1 = plane1.getPoints()[0];

	A(0, 0) = Normal2[0];
	A(0, 1) = Normal2[1];
	A(0, 2) = Normal2[2];
	
	D(0) = (Normal2[0] * P1[0] + Normal2[1] * P1[1] + Normal2[2] * P1[2]);

	Normal2 = plane2->GetNormal();
	P1 = plane2->getPoints()[0];

	A(1, 0) = Normal2[0];
	A(1, 1) = Normal2[1];
	A(1, 2) = Normal2[2];
	D(1) = (Normal2[0] * P1[0] + Normal2[1] * P1[1] + Normal2[2] * P1[2]);

	Normal2 = plane3->GetNormal();
	P1 = plane3->getPoints()[0];

	A(2, 0) = Normal2[0];
	A(2, 1) = Normal2[1];
	A(2, 2) = Normal2[2];
	D(2) = (Normal2[0] * P1[0] + Normal2[1] * P1[1] + Normal2[2] * P1[2]);

	
	Eigen::MatrixXd inter(3, 1);

	inter = A.inverse() * D;

	

	vector<double> interPoint = { inter(0),inter(1),inter(2) };

	

	return interPoint;
}
void weightingChecks(vector<vector<vector<double>>> faces) {
	//calculating surface area
	for (vector<vector<double>> triangle : faces) {
		
	}
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
				double area = sqrt(s * (s - a)*(s - b)*(s - c));
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
	Plane* lastPlane = adjacentPlanes[0];
	//initialise the frame
	vector<vector<double>> frame;
	vector<vector<vector<double>>> Loops;
	//used for when there is more than one loop
	vector<Plane*> usedPlanes;

	//initialise the first plane checked
	Plane* curPlane = adjacentPlanes[0];
	
	//used to prevent looping around the same frame
	Plane* start = adjacentPlanes[1];

	
	
	
	
	for (int i = 0; i < adjacentPlanes.size();i++) {
		
		for (int x = 0; x < adjacentPlanes.size(); x++) {
			Plane* nextPlane = adjacentPlanes[x];
			
			//checks to make sure the current plane is worth evaluating and also if its gone back to the start, that final condition stops the other conditions from mattering if its the first loop
			if ((nextPlane == curPlane || nextPlane == lastPlane || nextPlane == start) && lastPlane != adjacentPlanes[0]) {
				
				continue;
			}
			
			vector<Plane*> nextPlaneList = nextPlane->GetConnectedPlanes();
			
			for (int y = 0; y < nextPlaneList.size();y++) {
				
				if (nextPlaneList[y] == curPlane) {

					frame.push_back(threePlaneIntersectionPoint(planeX, curPlane, nextPlane));
					lastPlane = curPlane;
					curPlane = nextPlane;
					usedPlanes.push_back(curPlane);
					
				}
				
				
			}
		}
	}
	
	if (usedPlanes.size() == adjacentPlanes.size()) {
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

vector<double> calcD(Plane plane, double x, double y) {
	vector<double> pointOnPlane = plane.getPoints()[0];
	vector<double> normal = plane.GetNormal();
	double total = 0;
	total += normal[0] * pointOnPlane[0] + normal[1] * pointOnPlane[1] + normal[2] * pointOnPlane[2];
	
	total -= normal[0] * x + normal[1] * y;
	
	total /= normal[2];
	
	return { x, y, total };
	

}



/*

int main(int argc, char* argv[])
{
	// Load a mesh in OFF format
	//igl::read_triangle_mesh("C:/Users/jaywh/source/repos/ThirdYearDissertation4/models"  "/PlaneWithHoleTest.obj", V, F);
	//igl::read_triangle_mesh("C:/Uni Stuff/year3/3rd year project polyfit ver/ThirdYearDissertation/models"  "/PlaneWithHoleTest.obj", V, F);
	igl::read_triangle_mesh("C:/Uni Stuff/year3/3rd year project polyfit ver/ThirdYearDissertation/models"  "/Tower.obj", V, F);

	
	//Gets number of triangles from the faces matrix
	int numOfTriangles = F.rows();
	
	vector<vector<vector<double>>> fullConnectedList;

	//select random triangle
	//int random = rand() % numOfTrianlges;
	

	//assign point coordinates of a random traingle 
	vector<double> P1 = {V(F(0, 0), 0), V(F(0, 0), 1), V(F(0, 0), 2)};
	vector<double> P2 = { V(F(0, 1), 0), V(F(0, 1), 1), V(F(0, 1), 2) };
	vector<double> P3 = { V(F(0, 2), 0), V(F(0, 2), 1), V(F(0, 2), 2) };
	//initialise triangle with the points
	vector<vector<double>> randTriangle1 = { P1, P2, P3 };
	

	
	P1 = { V(F(1, 0), 0), V(F(1, 0), 1), V(F(1, 0), 2) };
	P2 = { V(F(1, 1), 0), V(F(1, 1), 1), V(F(1, 1), 2) };
	P3 = { V(F(1, 2), 0), V(F(1, 2), 1), V(F(1, 2), 2) };
	//initialise triangle with the points
	vector<vector<double>> randTriangle2 = { P1, P2, P3 };
	

	P1 = { V(F(2, 0), 0), V(F(2, 0), 1), V(F(2, 0), 2) };
	P2 = { V(F(2, 1), 0), V(F(2, 1), 1), V(F(2, 1), 2) };
	P3 = { V(F(2, 2), 0), V(F(2, 2), 1), V(F(2, 2), 2) };
	//initialise triangle with the points
	vector<vector<double>> randTriangle3 = { P1, P2, P3 };
	

	P1 = { V(F(3, 0), 0), V(F(3, 0), 1), V(F(3, 0), 2) };
	P2 = { V(F(3, 1), 0), V(F(3, 1), 1), V(F(3, 1), 2) };
	P3 = { V(F(3, 2), 0), V(F(3, 2), 1), V(F(3, 2), 2) };
	//initialise triangle with the points
	vector<vector<double>> randTriangle4 = { P1, P2, P3 };
	

	P1 = { V(F(4, 0), 0), V(F(4, 0), 1), V(F(4, 0), 2) };
	P2 = { V(F(4, 1), 0), V(F(4, 1), 1), V(F(4, 1), 2) };
	P3 = { V(F(4, 2), 0), V(F(4, 2), 1), V(F(4, 2), 2) };
	//initialise triangle with the points
	vector<vector<double>> randTriangle5 = { P1, P2, P3 };

	P1 = {V(F(5, 0), 0), V(F(5, 0), 1), V(F(5, 0), 2)};
	P2 = { V(F(5, 1), 0), V(F(5, 1), 1), V(F(5, 1), 2) };
	P3 = { V(F(5, 2), 0), V(F(5, 2), 1), V(F(5, 2), 2) };
	//initialise triangle with the points
	vector<vector<double>> randTriangle6 = { P1, P2, P3 };

	P1 = { V(F(6, 0), 0), V(F(6, 0), 1), V(F(6, 0), 2) };
	P2 = { V(F(6, 1), 0), V(F(6, 1), 1), V(F(6, 1), 2) };
	P3 = { V(F(6, 2), 0), V(F(6, 2), 1), V(F(6, 2), 2) };
	//initialise triangle with the points
	vector<vector<double>> randTriangle7 = { P1, P2, P3 };

	P1 = { V(F(7, 0), 0), V(F(7, 0), 1), V(F(7, 0), 2) };
	P2 = { V(F(7, 1), 0), V(F(7, 1), 1), V(F(7, 1), 2) };
	P3 = { V(F(7, 2), 0), V(F(7, 2), 1), V(F(7, 2), 2) };
	//initialise triangle with the point
	vector<vector<double>> randTriangle8 = { P1, P2, P3 };
	
	
	
	//list of connected faces to pass into FindConnected recursive function
	//Call function, currently just returns all triangles in the model but once we 
	//include the angle checks it will then print the correct triangles to make a plane out of
	//swap randTriangle for t
	// 
	//fullConnectedList.push_back(randTriangle);
	//fullConnectedList = FindConnected(randTriangle,fullConnectedList,randTriangle);
	





	Plane plane1(randTriangle1);
	Plane plane2(randTriangle2);
	Plane plane3(randTriangle3);
	Plane plane4(randTriangle4);
	Plane plane5(randTriangle5);
	Plane plane6(randTriangle6);
	Plane plane7(randTriangle7);
	Plane plane8(randTriangle8);
	

	

	plane3.AddConnectedPlane(&plane1);
	plane3.AddConnectedPlane(&plane2);
	plane3.AddConnectedPlane(&plane4);
	plane3.AddConnectedPlane(&plane5);
	plane3.AddConnectedPlane(&plane6);
	plane3.AddConnectedPlane(&plane7);
	plane3.AddConnectedPlane(&plane8);

	plane6.AddConnectedPlane(&plane7);
	plane6.AddConnectedPlane(&plane8);
	plane6.AddConnectedPlane(&plane3);

	plane7.AddConnectedPlane(&plane6);
	plane7.AddConnectedPlane(&plane8);
	plane7.AddConnectedPlane(&plane3);

	plane8.AddConnectedPlane(&plane7);
	plane8.AddConnectedPlane(&plane6);
	plane8.AddConnectedPlane(&plane3);

	plane1.AddConnectedPlane(&plane2);
	plane1.AddConnectedPlane(&plane3);
	plane1.AddConnectedPlane(&plane5);

	

	plane2.AddConnectedPlane(&plane1);
	plane2.AddConnectedPlane(&plane3);
	plane2.AddConnectedPlane(&plane4);

	

	plane4.AddConnectedPlane(&plane2);
	plane4.AddConnectedPlane(&plane3);
	plane4.AddConnectedPlane(&plane5);

	plane5.AddConnectedPlane(&plane1);
	plane5.AddConnectedPlane(&plane3);
	plane5.AddConnectedPlane(&plane4);


	

	


	
	

	
	
	


	


	
	vector<vector<vector<double>>> Loops = getNextPlane(plane3, plane3.GetConnectedPlanes());
	vector<vector<double>> holePoints = GetPointsInHoles(Loops);
	vector<vector<double>> frame;
	
	Eigen::MatrixXd V3;
	Eigen::MatrixXi E;
	Eigen::MatrixXd H;
	Eigen::MatrixXd V2;
	Eigen::MatrixXi F2;

	for (int x = 0; x < Loops.size();x++) {

		V3.resize(V3.rows() + Loops[x].size(), 2);
		E.resize(V3.rows() + Loops[x].size(), 2);

	}
	int z = 0;
	for (int x = 0; x < Loops.size();x++){
		
		
		for (int y = 0; y < Loops[x].size();y++) {
			
			V3(y + z, 0) = Loops[x][y][0];
			V3(y + z, 1) = Loops[x][y][1];
			
			
			
			E(y + z, 0) = y + z;
			if (y + z == z + Loops[x].size() - 1)
			{
				E(y + z, 1) = z;
			}
			else
			{
				E(y + z, 1) = y + z + 1;
					
			}
			cout << E(y + z, 0) << endl;
			cout << E(y + z, 1) << endl;
		
			
			

		}
		z += Loops[x].size();
	}
	
	
	cout << V3;
	

	H.resize(holePoints.size(), 2);

	for (int x = 0; x < holePoints.size();x++) {
		H(x, 0) = holePoints[x][0];
		H(x, 1) = holePoints[x][1];
	}

	igl::triangle::triangulate(V3, E, H, "1", V2, F2);

	Eigen::MatrixXd V4;
	Eigen::MatrixXi F4;

	V4.resize(V2.rows(), 3);
	
	for (int x = 0;x < V2.rows();x++) {
		V4(x, 0) = V2(x, 0);
		V4(x, 1) = V2(x, 1);
		V4(x, 2) = calcD(plane3, V2(x, 0), V2(x, 1))[ 2];

	}

	Eigen::MatrixXd D(V.rows() + V4.rows(), V.cols()); // <-- D(A.rows() + B.rows(), ...)
	D << V,V4;

	for (int x = 0;x < F2.rows();x++) {
		for (int y = 0;y < 3;y++) {
			F2(x, y) += V.rows();
		}
	}

	Eigen::MatrixXi P(F.rows() + F2.rows(), F.cols()); // <-- D(A.rows() + B.rows(), ...)
	P << F,F2;
	
	
	//Plot the mesh
	
	std::cout << endl << endl << endl;
	//open libigl viewer
	igl::opengl::glfw::Viewer viewer;
	viewer.data().set_mesh(D, P);
	viewer.launch();
	

	

}


*/

//Alex Testing MainVVVVV

int main(int argc, char* argv[])
{
	// Load a mesh in OFF format


	//igl::read_triangle_mesh("C:/Users/jaywh/source/repos/ThirdYearDissertation4/models"  "/Tower.obj", V, F);
	igl::read_triangle_mesh("C:/Uni Stuff/year3/3rd year project polyfit ver/ThirdYearDissertation/models"  "/Tower.obj", V, F);


	//Gets number of triangles from the faces matrix
	int numOfTriangles = F.rows();
	vector<vector<vector<double>>> fullConnectedList;

	//select random triangle
	//int random = rand() % numOfTrianlges;
	int random = 14;

	//assign point coordinates of a random traingle 
	vector<double> P1 = { V(F(random, 0), 0), V(F(random, 0), 1), V(F(random, 0), 2) };
	vector<double> P2 = { V(F(random, 1), 0), V(F(random, 1), 1), V(F(random, 1), 2) };
	vector<double> P3 = { V(F(random, 2), 0), V(F(random, 2), 1), V(F(random, 2), 2) };



	//initialise triangle with the points
	vector<vector<double>> randTriangle = { P1, P2, P3 };


	triangleClass START_TRIANGLE = triangleClass(randTriangle);
	//remove randTriangle from F
	unsigned int numRows = F.rows() - 1;
	unsigned int numCols = F.cols();

	if (random < numRows) {
		//F.block(random, 0, numRows - random, numCols) = F.block(random + 1, 0, numRows - random, numCols);
	}
	//F.conservativeResize(numRows, numCols);

	//list of connected faces to pass into FindConnected recursive function


	//Call function, currently just returns all triangles in the model but once we 
	//include the angle checks it will then print the correct triangles to make a plane out of

	//swap randTriangle for t
	fullConnectedList.push_back(randTriangle);
	//fullConnectedList = FindConnected(START_TRIANGLE, fullConnectedList, randTriangle);
	triangleClass finalTriangle = FindConnected(START_TRIANGLE, fullConnectedList, randTriangle);
	int z = finalTriangle.getNoOfConnectedTriangles();
	for (int x = 0; x < z; x++) {
		if (finalTriangle.adjacenttriangles[x].adjacenttriangles.empty()) {
			finalTriangle.adjacenttriangles[x].addToAdjacentTriangles(finalTriangle);
		}
		for (int y = 0; y < z; y++) {
			if (!finalTriangle.adjacenttriangles[x].isTrianglePresent(finalTriangle.adjacenttriangles[y])) {
				finalTriangle.adjacenttriangles[x].addToAdjacentTriangles(finalTriangle.adjacenttriangles[y]);
			}
		}
	}
	cout << "NUMBER OF CONNECTED TRIANGLES" << endl;
	int temp = finalTriangle.getNoOfConnectedTriangles();
	cout << to_string(temp)	 << endl;

	//vector<double> P1 = { V(F(random, 0), 0), V(F(random, 0), 1), V(F(random, 0), 2) };
	//vector<double> P2 = { V(F(random, 1), 0), V(F(random, 1), 1), V(F(random, 1), 2) };
	//vector<double> P3 = { V(F(random, 2), 0), V(F(random, 2), 1), V(F(random, 2), 2) };

	////initialise triangle with the points
	//vector<vector<double>> randTriangle = { P1, P2, P3 };


	V.conservativeResize(V.rows() + 3, V.cols());

	double x_c = (P1[0] + P2[0] + P3[0]) / 3;
	double y_c = (P1[1] + P2[1] + P3[1]) / 3;
	double z_c = (P1[2] + P2[2] + P3[2]) / 3;


	//test plane
	//plane made of 2 traingles
	// double plane1
	// plane1
	// double
	// double



	//cout << P1[0] << P1[1] << P1[2] << endl;
	//cout << P2[0] << P2[1] << P2[2] << endl;
	//cout << P3[0] << P3[1] << P3[2] << endl;
	//cout << x_c << endl;
	//cout << y_c << endl;
	//cout << z_c << endl;
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
	F(F.rows() - 1, 2) = V.rows() - 3;

	//cout << V << endl;

	//print all triangles in final list
	//for (vector<vector<double>> triangle : fullConnectedList) {
	//	std::cout << "Start of triangle" << endl;
	//	int count1 = 0;
	//	for (vector<double> point : triangle) {
	//		std::cout << "Point " << count1 << " coordinates:";
	//		count1++;
	//		//for (double coord : point) {
	//		//	std::cout << coord;
	//		//}
	//		std::cout << endl;
	//	}
	//	std::cout << "End of triangle" << endl;
	//}

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
