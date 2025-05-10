#include <igl/opengl/glfw/Viewer.h>
#include <igl/triangle/triangulate.h>
#include <unordered_set>
#include "triangleClass.h"
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
Eigen::MatrixXi L;
Eigen::MatrixXi F;
vector<triangleClass> objectTriangleArray;
double pi = 3.14159265;

void vecToString(vector<vector<double>> vec) {
	for (int y = 0; y < vec.size();y++) {

		cout << endl << "== " << vec[y][0] << " ," << vec[y][1] << " ," << vec[y][2];

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
				else {
					
					double intAng = startTriangle->calcInteriorAngle(objectTriangleArray[x]);
					
					if (intAng < 50) {
						objectTriangleArray[x].updatePlaneNo(startTriangle->planeNo);
						FindConnected(startTriangle, objectTriangleArray[x]);
			}
				}
				
			}
	}
}

//void combineSimilarPlanes(vector<Plane*> planeList) {
//	for (int x = 0; x < planeList.size();x++) {
//		Plane* curPlane = planeList[x];
//		for (int y = 0;y < planeList.size();y++) {
//
//		}
//	}
//}



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
		cout << i << "<-" << endl;
		for (int x = 0; x < adjacentPlanes.size(); x++) {
			
			Plane* nextPlane = adjacentPlanes[x];
			cout << nextPlane->toString();


			cout << curPlane->toString();
			//checks to make sure the current plane is worth evaluating and also if its gone back to the start, that final condition stops the other conditions from mattering if its the first loop
			if ((nextPlane == curPlane || nextPlane == lastPlane || usedPlanes.size() >= adjacentPlanes.size()) && lastPlane != NULL) {
				continue;
			}
			vector<Plane*> nextPlaneList = nextPlane->GetConnectedPlanes();
			
			cout << endl << endl;

			

			if (find(nextPlaneList.begin(), nextPlaneList.end(), curPlane) != nextPlaneList.end()) {
				frame.push_back(threePlaneIntersectionPoint(planeX, curPlane, nextPlane));
				lastPlane = curPlane;
				curPlane = nextPlane;
				usedPlanes.push_back(curPlane);
				break;
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
	igl::read_triangle_mesh("C:/Users/jaywh/source/repos/Dissertation/models"  "/old_chair.obj", V, F);
	
	
	


	

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
			
			planeCount++;
		}
	}
	
	vector<Plane> planeList;
	vector<triangleClass> primeTriangleList;
	for (triangleClass prime : objectTriangleArray) {
		if (prime.isPrimeTriangle) {
			
				cout << endl << endl;
				for (int p : prime.connectedPlanes) {
					cout << p << " ";
				}

			
			planeList.push_back(Plane(prime));
			primeTriangleList.push_back(prime);
		}
	}
	for (int planeNo = 0; planeNo < planeList.size();planeNo++) {
		for (int index : primeTriangleList[planeNo].connectedPlanes) {
			planeList[planeNo].AddConnectedPlane(&planeList[index]);
		}
	}
	



	

	Eigen::MatrixXd D;
	Eigen::MatrixXi P;



	



	for (int x = 0; x < planeList.size();x++) {
		
		vector<vector<vector<double>>> Loops = getNextPlane(planeList[x], planeList[x].GetConnectedPlanes());

		
		vector<vector<double>> holePoints = GetPointsInHoles(Loops);
		vector<vector<double>> frame;




		Eigen::MatrixXd V3;
		Eigen::MatrixXi E;
		Eigen::MatrixXd H;
		Eigen::MatrixXd V2;
		Eigen::MatrixXi F2;


		int axis = 0;


		if (planeList[x].GetNormal()[0] == 0 && planeList[x].GetNormal()[2] == 0) {
			axis = 1;
		}
		else if (planeList[x].GetNormal()[1] == 0 && planeList[x].GetNormal()[2] == 0) {
			axis = 0;
		}
		else if (planeList[x].GetNormal()[0] == 0 && planeList[x].GetNormal()[1] == 0) {
			axis = 2;
		}
		else if (abs(planeList[x].GetNormal()[0]) == abs(planeList[x].GetNormal()[2])) {
			axis = 0;
		}
		else if (abs(planeList[x].GetNormal()[1]) == abs(planeList[x].GetNormal()[2])) {
			axis = 1;
		}
		else if (abs(planeList[x].GetNormal()[0]) == abs(planeList[x].GetNormal()[1])) {
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

		

		igl::triangle::triangulate(V3, E, H, "1", V2, F2);

		Eigen::MatrixXd V4;
		Eigen::MatrixXi F4;

		V4.resize(V2.rows(), 3);

		for (int p = 0;p < V2.rows();p++) {
			if (axis == 0) {
				V4(p, 0) = calcX(planeList[x], V2(p, 0), V2(p, 1))[2];
				V4(p, 1) = V2(p, 0);
				V4(p, 2) = V2(p, 1);
			}
			else if (axis == 1) {
				V4(p, 0) = V2(p, 0);
				V4(p, 1) = calcY(planeList[x], V2(p, 0), V2(p, 1))[2];

				V4(p, 2) = V2(p, 1);
			}
			else if (axis == 2) {
				V4(p, 0) = V2(p, 0);;
				V4(p, 1) = V2(p, 1);
				V4(p, 2) = calcZ(planeList[x], V2(p, 0), V2(p, 1))[2];
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
		
		vector<vector<double>> checked;
		
		for (int i = 0; i < D.rows();i++) {
			
			vector<double> cur = { D(i,0),D(i,1),D(i,2) };
			if (cur[0] == test1[0]) {
				
			}
			if (find(checked.begin(), checked.end(), cur) == checked.end()) {
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

	
	
	



	//Plot the mesh

	std::cout << endl << endl << endl;
	//open libigl viewer

	igl::opengl::glfw::Viewer viewer;

	viewer.data().set_mesh(D, P);
	viewer.launch();

	


}






