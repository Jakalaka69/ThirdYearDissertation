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
vector<triangleClass> objectTriangleArray;
double pi = 3.14159265; 

void vecToString(vector<vector<double>> vec) {
	for (int y = 0; y < vec.size();y++) {
		
		//cout << endl << "== " << vec[y][0] << " ," << vec[y][1] << " ," << vec[y][2];
		
	}
	
}

void FindConnected(triangleClass *startTriangle, triangleClass curTriangle) {
	for (int x = 0; x < objectTriangleArray.size();x++) {
		if (objectTriangleArray[x].planeNo == startTriangle->planeNo) {

			continue;
		}
			int count = 0;
			for (int j = 0;j < 3;j++) {
				for (int k = 0; k < 3; k++) {

					if (curTriangle.points[j] == objectTriangleArray[x].points[k]) {

						count++;
					}
				}
			}
			if (count == 2) {	
				//NEW	
				double intAng = startTriangle->calcInteriorAngle(objectTriangleArray[x]);
				//cout << intAng << endl;
				//NEW
				if (objectTriangleArray[x].planeNo != -1) {
					//searches if next_triangles plane no. is already in start triangles connected planes
					//doesnt add again if true
					if (find(startTriangle->connectedPlanes.begin(), startTriangle->connectedPlanes.end(),
						objectTriangleArray[x].planeNo) == startTriangle->connectedPlanes.end()) {
						//loops through triangle array
						for (int no = 0; no < objectTriangleArray.size();no++) {							
							//if prime is a prime and has the same planeNo as next_triangle
							if (objectTriangleArray[no].isPrimeTriangle && objectTriangleArray[no].planeNo == objectTriangleArray[x].planeNo) {
																startTriangle->addToConnectedPlanes(objectTriangleArray[x].planeNo);
								objectTriangleArray[no].addToConnectedPlanes(startTriangle->planeNo);
							}
						}
					}
				}
				else if (intAng < 30) {
						objectTriangleArray[x].updatePlaneNo(startTriangle->planeNo);
						FindConnected(startTriangle, objectTriangleArray[x]);					
				}
			}
	}
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
	//igl::read_triangle_mesh("C:/Uni Stuff/year3/3rd year project polyfit ver/ThirdYearDissertation/models"  "/Tower.obj", V, F);
	igl::read_triangle_mesh("C:/Users/Wooki/Downloads/"  "/cube.obj", V, F);
	for (int x = 0; x < F.rows(); x++) {		
		//initialise the current triangle in loop
		vector<vector<double>> nextTriangle = {};
		for (int i = 0; i < 3; i++) {
			nextTriangle.push_back({ V(F(x, i), 0), V(F(x, i), 1), V(F(x, i), 2) });
		}
		triangleClass temp = triangleClass(nextTriangle);
		objectTriangleArray.push_back(temp);
	}
	int random = 0;
	unsigned int numRows = F.rows() - 1;
	unsigned int numCols = F.cols();
	int planeCount = 0;
	for (int t = 0; t < objectTriangleArray.size(); t++) {
		if (objectTriangleArray[t].planeNo == -1) {
			objectTriangleArray[t].updatePlaneNo(planeCount);
			objectTriangleArray[t].makeTrianglePrime();
			FindConnected(&objectTriangleArray[t], objectTriangleArray[t]);
			cout << planeCount << endl;
			planeCount++;
		}
	}
	vector<Plane> planeList;
	vector<triangleClass> primeTriangleList;
	for (triangleClass prime : objectTriangleArray) {
		if (prime.isPrimeTriangle) {
			planeList.push_back(Plane(prime));
			primeTriangleList.push_back(prime);
		}
	}
	for (int planeNo = 0; planeNo < planeList.size();planeNo ++) {
		for (int index : primeTriangleList[planeNo].connectedPlanes) {
			planeList[planeNo].AddConnectedPlane(&planeList[index]);
		}
	}
	for (int test1 = 0; test1 < primeTriangleList.size(); test1++) {
		cout << test1 << endl;
		cout << primeTriangleList[test1].normal[0] <<" ";
		cout << primeTriangleList[test1].normal[1] << " ";
		cout << primeTriangleList[test1].normal[2] << endl;
		cout << planeList[test1].toString() << endl;
	}
	//Plot the mesh
	std::cout << endl << endl << endl;
	//open libigl viewer
	igl::opengl::glfw::Viewer viewer;
	viewer.data().set_mesh(V, F);
	viewer.launch();
}
