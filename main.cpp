#include <igl/opengl/glfw/Viewer.h>
#include <igl/triangle/triangulate.h>
#include <unordered_set>

#include <igl/readOFF.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/read_triangle_mesh.h>
#include "Plane.h"
#include <vector>
#include <cmath>
#define _USE_MATH_DEFINES
using namespace std;

Eigen::MatrixXd V;
Eigen::MatrixXi F;
double pi = 3.14159265;



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
				//put angle checks here and only do the stuff below if they pass




				int temp = 0;
				for (vector<double> allPoint : allPoints) {
					if (allPoint == touchingPoints[0]) {
						final4Points.push_back(allPoint);
						aVertIndex = temp;
					}
					else if (allPoint == touchingPoints[1]) {
						final4Points.push_back(allPoint);
						dVertIndex = temp;
					}
					else if (allPoint != touchingPoints[0] && allPoint != touchingPoints[1]) {
						final4Points.push_back(allPoint);

					}
					temp++;
				}
				aVertIndex = aVertIndex % 4;
				dVertIndex = dVertIndex % 4;
				int offset = 1;
				int bVertIndex = 0;
				while (bVertIndex == aVertIndex || bVertIndex == dVertIndex) {

					bVertIndex = offset % 4;
					offset++;
				}
				offset = 1;
				int cVertIndex = 0;
				while (cVertIndex == aVertIndex || cVertIndex == dVertIndex || cVertIndex == bVertIndex) {

					cVertIndex = offset % 4;
					offset++;
				}



				vector<double> U = { startTriangle[0][0] - startTriangle[1][0]
								   , startTriangle[0][1] - startTriangle[1][1]
								   , startTriangle[0][2] - startTriangle[1][2] };

				vector<double> V = { startTriangle[0][0] - startTriangle[2][0]
								   , startTriangle[0][1] - startTriangle[2][1]
								   , startTriangle[0][2] - startTriangle[2][2] };

				vector<double> Normal = { 0,0,0 };



				Normal[0] = (U[1] * V[2]) - (U[2] * V[1]);
				Normal[1] = (U[2] * V[0]) - (U[0] * V[2]);
				Normal[2] = (U[0] * V[1]) - (U[1] * V[0]);







				vector<double> U2 = { final4Points[aVertIndex][0] - final4Points[cVertIndex][0]
								   , final4Points[aVertIndex][1] - final4Points[cVertIndex][1]
								   , final4Points[aVertIndex][2] - final4Points[cVertIndex][2] };

				vector<double> V2 = { final4Points[aVertIndex][0] - final4Points[dVertIndex][0]
								  , final4Points[aVertIndex][1] - final4Points[dVertIndex][1]
								  , final4Points[aVertIndex][2] - final4Points[dVertIndex][2] };

				vector<double> Normal2 = { 0,0,0 };

				Normal2[0] = (U2[1] * V2[2]) - (U2[2] * V2[1]);
				Normal2[1] = (U2[2] * V2[0]) - (U2[0] * V2[2]);
				Normal2[2] = (U2[0] * V2[1]) - (U2[1] * V2[0]);


				double numerator = abs((Normal[0] * Normal2[0]) + (Normal[1] * Normal2[1]) + (Normal[2] * Normal2[2]));
				double denominator = (sqrt(pow(Normal[0], 2) + pow(Normal[1], 2) + pow(Normal[2], 2)) * sqrt(pow(Normal2[0], 2) + pow(Normal2[1], 2) + pow(Normal2[2], 2)));
				double interiorAngle = acos(numerator / denominator);
				double interiorDegrees = interiorAngle * (180 / pi);
				cout << endl << interiorAngle * (180 / pi);



				if (interiorDegrees < 30) {

					
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

vector<double> threePlaneIntersectionPoint(Plane plane1, Plane plane2, Plane plane3) {
	vector<vector<double>> planeNormals;
	Eigen::MatrixXd A(3, 3);
	Eigen::MatrixXd D(3, 1);

	
	vector<double> Normal2 = plane1.GetNormal();
	
	vector<double> P1 = plane1.getRelatedTriangle()[0];

	A(0, 0) = Normal2[0];
	A(0, 1) = Normal2[1];
	A(0, 2) = Normal2[2];
	
	D(0) = (Normal2[0] * P1[0] + Normal2[1] * P1[1] + Normal2[2] * P1[2]);

	Normal2 = plane2.GetNormal();
	P1 = plane2.getRelatedTriangle()[0];

	A(1, 0) = Normal2[0];
	A(1, 1) = Normal2[1];
	A(1, 2) = Normal2[2];
	D(1) = (Normal2[0] * P1[0] + Normal2[1] * P1[1] + Normal2[2] * P1[2]);

	Normal2 = plane3.GetNormal();
	P1 = plane3.getRelatedTriangle()[0];

	A(2, 0) = Normal2[0];
	A(2, 1) = Normal2[1];
	A(2, 2) = Normal2[2];
	D(2) = (Normal2[0] * P1[0] + Normal2[1] * P1[1] + Normal2[2] * P1[2]);

	
	Eigen::MatrixXd inter(3, 1);

	inter = A.inverse() * D;

	cout << inter << endl;

	vector<double> interPoint = { inter(0),inter(1),inter(2) };

	

	return interPoint;
}



void weightingChecks(vector<vector<vector<double>>> faces) {
	//calculating surface area
	for (vector<vector<double>> triangle : faces) {
		double side1 = sqrt(pow(triangle[0][0] - triangle[1][0], 2) + pow(triangle[0][1] - triangle[1][1], 2) + pow(triangle[0][2] - triangle[1][2], 2));
		double side2 = sqrt(pow(triangle[0][0] - triangle[2][0], 2) + pow(triangle[0][1] - triangle[2][1], 2) + pow(triangle[0][2] - triangle[2][2], 2));
		double side3 = sqrt(pow(triangle[0][0] - triangle[1][0], 2) + pow(triangle[0][1] - triangle[1][1], 2) + pow(triangle[0][2] - triangle[1][2], 2));
	}
}

vector<vector<double>> getNextPlane(Plane planeX) {
	vector<Plane> adjacentPlanes = planeX.GetConnectedPlanes();
	
	Plane lastPlane = adjacentPlanes[0];
	cout << lastPlane.GetConnectedPlanes().size() << "<-----";
	vector<vector<double>> frame;
	Plane firstPlane = adjacentPlanes[0];
	Plane start = adjacentPlanes[1];
	bool first = true;
	

	for (int i = 0; i < adjacentPlanes.size();i++) {
		
		for (int x = 0; x < adjacentPlanes.size(); x++) {
			Plane currentPlane = adjacentPlanes[x];
			
			if ((firstPlane == currentPlane || currentPlane == lastPlane || currentPlane == start) && lastPlane != adjacentPlanes[0]) {
				
				continue;
			}
			
			vector<Plane> currentPlaneList = currentPlane.GetConnectedPlanes();
			
			for (int y = 0; y < currentPlaneList.size();y++) {
				
				if (currentPlaneList[y] == firstPlane) {

					frame.push_back(threePlaneIntersectionPoint(planeX, firstPlane, currentPlane));
					lastPlane = firstPlane;
					firstPlane = currentPlane;
					
				}
				
				
			}
		}
	}
	cout << frame.size();
	return frame;
	
}

vector<double> calcD(Plane plane, double x, double y) {
	vector<double> pointOnPlane = plane.getRelatedTriangle()[0];
	vector<double> normal = plane.GetNormal();
	double total = 0;
	total += normal[0] * pointOnPlane[0] + normal[1] * pointOnPlane[1] + normal[2] * pointOnPlane[2];
	cout << total << endl;
	total -= normal[0] * x + normal[1] * y;
	cout << total << endl;
	total /= normal[2];
	cout << total << endl;
	return { x, y, total };
	

}

int main(int argc, char* argv[])
{
	// Load a mesh in OFF format
	igl::read_triangle_mesh("C:/Users/jaywh/source/repos/ThirdYearDissertation4/models"  "/5PlaneTriangulator.obj", V, F);

	
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

	plane1.AddConnectedPlane(plane2);
	plane1.AddConnectedPlane(plane3);
	plane1.AddConnectedPlane(plane4);

	plane2.AddConnectedPlane(plane1);
	plane2.AddConnectedPlane(plane3);
	plane2.AddConnectedPlane(plane5);

	

	plane4.AddConnectedPlane(plane1);
	plane4.AddConnectedPlane(plane3);
	plane4.AddConnectedPlane(plane5);

	plane5.AddConnectedPlane(plane2);
	plane5.AddConnectedPlane(plane3);
	plane5.AddConnectedPlane(plane4);


	plane3.AddConnectedPlane(plane1);
	plane3.AddConnectedPlane(plane2);
	plane3.AddConnectedPlane(plane4);
	plane3.AddConnectedPlane(plane5);

	cout << plane4.toString();


	
	vector<Plane> adjacentPlanes = plane3.GetConnectedPlanes();

	Plane lastPlane = adjacentPlanes[2];
	cout << lastPlane.GetConnectedPlanes().size() << "<-----";
	


	



	vector<vector<double>> frame = getNextPlane(plane3);

	for (int x = 0; x < frame.size();x++) {
		frame[x].erase(frame[x].begin()+2);
	}
	

	Eigen::MatrixXd V3;
	Eigen::MatrixXi E;
	Eigen::MatrixXd H;
	Eigen::MatrixXd V2;
	Eigen::MatrixXi F2;

	V3.resize(frame.size(), 2);
	E.resize(frame.size(), 2);

	for (int x = 0; x < frame.size();x++) {
		V3(x, 0) = frame[x][0];
		V3(x, 1) = frame[x][1];
		//V(x, 2) = frame[x][2];
		
		E(x, 0) = x;
		if(x == frame.size()-1){ E(x, 1) = 0; }
		else{E(x, 1) = x + 1; }
		
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

	
	
	
	//Plot the mesh
	std::cout << endl << endl << endl;
	//open libigl viewer
	igl::opengl::glfw::Viewer viewer;
	viewer.data().set_mesh(V4, F2);
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


/*V.conservativeResize(V.rows() + 3, V.cols());

	double x_c = (P1[0] + P2[0] + P3[0]) / 3;
	double y_c = (P1[1] + P2[1] + P3[1]) / 3;
	double z_c = (P1[2] + P2[2] + P3[2]) / 3;


	//test plane
	//plane made of 2 traingles
	// double plane1
	// plane1
	// double
	// double




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