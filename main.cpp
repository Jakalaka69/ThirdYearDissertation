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
#include <map> 
#define _USE_MATH_DEFINES
using namespace std;

map<vector<double>, vector<Plane*>> planeDict;
Eigen::MatrixXd V;
Eigen::MatrixXi L;
Eigen::MatrixXi F;
int count1 = 0;
float minX = 0;
float maxX = 0;
float minY = 0;
float maxY = 0;
float minZ = 0;
float maxZ = 0;
vector<vector<double>> boundingBox;
vector<triangleClass> objectTriangleArray;
vector<vector<double>> corners;
double pi = 3.14159265;

void vecToString(vector<vector<double>> vec) {
	for (int y = 0; y < vec.size();y++) {

		cout << endl << "== " << vec[y][0] << " ," << vec[y][1] << " ," << vec[y][2];

	}

}

void boundingBoxUpdate(vector<double> coords) {
	for (int t = 0; t < coords.size(); t++) {
		if (t == 0) {
			if (coords[0] < minX) {
				minX = coords[0];
			}
			if (coords[0] > maxX) {
				maxX = coords[0];
			}
		}
		if (t == 1) {
			if (coords[1] < minY) {
				minY = coords[1];
			}
			if (coords[1] > maxY) {
				maxY = coords[1];
			}
		}
		if (t == 2) {
			if (coords[2] < minZ) {
				minZ = coords[2];
			}
			if (coords[2] > maxZ) {
				maxZ = coords[2];
			}
		}
	}
}

void boundingBoxCreation() {
	boundingBox.push_back({ minX,minY,minZ });
	boundingBox.push_back({ minX,minY,maxZ });
	boundingBox.push_back({ minX,maxY,minZ });
	boundingBox.push_back({ minX,maxY,maxZ });
	boundingBox.push_back({ maxX,minY,minZ });
	boundingBox.push_back({ maxX,minY,maxZ });
	boundingBox.push_back({ maxX,maxY,minZ });
	boundingBox.push_back({ maxX,maxY,maxZ });
}

void FindConnected(triangleClass *startTriangle, triangleClass *curTriangle) {

	/*for (triangleClass prime : objectTriangleArray) {
		if (prime.isPrimeTriangle) {

			cout << endl << endl;
			for (int p : prime.connectedPlanes) {
				cout << p << " ";
			}


			

		}
	}*/
	/*cout << endl << endl << curTriangle->points[0][0] << " " << curTriangle->points[0][1] << " " << curTriangle->points[0][2];
	cout << endl  << curTriangle->points[1][0] << " " << curTriangle->points[1][1] << " " << curTriangle->points[1][2];
	cout << endl  << curTriangle->points[2][0] << " " << curTriangle->points[2][1] << " " << curTriangle->points[2][2];
	cout << endl << curTriangle->planeNo;*/
	int count1 = 0;
	for (int x = 0; x < objectTriangleArray.size();x++) {
			int count = 0;
		
			if (objectTriangleArray[x].planeNo == startTriangle->planeNo) {
				continue;
			}
		
			
			
			for (int j = 0;j < 3;j++) {
				for (int k = 0; k < 3; k++) {
					boundingBoxUpdate(curTriangle->points[j]);
					
					if (curTriangle->points[j] == objectTriangleArray[x].points[k]) {
						
						count++;
						
					}
				}
			}
			
			
			

			if (count > 0 ) {
				
				if (startTriangle->planeNo == 5 && objectTriangleArray[x].planeNo == 4) {
					cout << "here";
					cout << count;
				}
				
				if (objectTriangleArray[x].planeNo != -1) {
					//searches if next_triangles plane no. is already in start triangles connected planes
					//doesnt add again if true
					
					bool inArray = false;
					int countArrayIndex = -1;
					int inArrayCount = -1;
					for (int k = 0; k < startTriangle->connectedPlanes.size();k++) {
						if (startTriangle->connectedPlanes[k] == objectTriangleArray[x].planeNo) {
							inArray = true;
							inArrayCount = startTriangle->countNum[k];
							countArrayIndex = k;
						}
					}

					if (!inArray) {
						for (int no = 0; no < objectTriangleArray.size();no++) {
							//if prime is a prime and has the same planeNo as next_triangle
							if (objectTriangleArray[no].isPrimeTriangle && objectTriangleArray[no].planeNo == objectTriangleArray[x].planeNo) {
								startTriangle->addToConnectedPlanes(objectTriangleArray[no].planeNo);
								startTriangle->setCount(count);
								objectTriangleArray[no].setCount(count);
								objectTriangleArray[no].addToConnectedPlanes(startTriangle->planeNo);
							}
						}
					}
					else if (inArray && inArrayCount == 1 && count == 2) {
						for (int no = 0; no < objectTriangleArray.size();no++) {
							//if prime is a prime and has the same planeNo as next_triangle
							if (objectTriangleArray[no].isPrimeTriangle && objectTriangleArray[no].planeNo == objectTriangleArray[x].planeNo) {
								
								startTriangle->countNum[countArrayIndex] = count;
								for (int k = 0; k < objectTriangleArray[no].connectedPlanes.size();k++) {
									if (objectTriangleArray[no].connectedPlanes[k] == startTriangle->planeNo) {
										objectTriangleArray[no].countNum[k] = count;
									}
								}
								
							}
						}
					}
					

				}
				else {

					double intAng = startTriangle->calcInteriorAngle(objectTriangleArray[x]);

					if (intAng <= 70) {
						objectTriangleArray[x].updatePlaneNo(startTriangle->planeNo);
						FindConnected(startTriangle, &objectTriangleArray[x]);

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



vector<double> threePlaneIntersectionPoint(Plane* plane1, Plane* plane2, Plane* plane3) {
	vector<vector<double>> planeNormals;
	Eigen::MatrixXd A(3, 3);
	Eigen::MatrixXd D(3, 1);

	//cout << "++++" << endl << plane1->toString() << plane2->toString() << plane3->toString();


	vector<double> Normal2 = plane1->GetNormal();

	vector<double> P1 = plane1->getRelatedTriangle()[0];


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


	

	//cout << endl << "->" << inter << "<-" << endl;

	vector<double> interPoint = { inter(0),inter(1),inter(2) };

	planeDict[interPoint].push_back(plane1);
	planeDict[interPoint].push_back(plane2);
	planeDict[interPoint].push_back(plane3);

	return interPoint;
}
double getDistanceToMain(vector<double> point, vector<double> midOfMain) {
	double dist = sqrt(pow(point[0] - midOfMain[0], 2) + pow(point[1] - midOfMain[1], 2) + pow(point[2] - midOfMain[2], 2));
	return dist;

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
	vector<double> lastPoint = { -100000,-100000,-100000 };
	//initialise the frame
	vector<vector<double>> frame;
	vector<vector<vector<double>>> Loops;

	//used for when there is more than one loop
	vector<Plane*> usedPlanes;
	Plane* curPlane = adjacentPlanes[0];
	Plane* startPlane = adjacentPlanes[0];
	int startIndex = 0;
	//initialise the first plane checked
	

	bool break1 = false;
	for(int y = 0; y < adjacentPlanes.size();y++){

		

		

		for (int x = 0; x < curPlane->GetConnectedPlanes().size(); x++) {
			
			
			Plane* nextPlane = curPlane->GetConnectedPlanes()[x];
			

			if (curPlane == startPlane && y != 0) {
				break1 = true;
				break;
			}

			

			//checks to make sure the current plane is worth evaluating and also if its gone back to the start, that final condition stops the other conditions from mattering if its the first loop
			if ((nextPlane == curPlane) || usedPlanes.size() >= adjacentPlanes.size() || nextPlane == lastPlane || nextPlane->getRelatedTriangle() == planeX.getRelatedTriangle()) {
				continue;
			}

			vector<Plane*> nextPlaneList = nextPlane->GetConnectedPlanes();
			vector<Plane*> curPlaneList = curPlane->GetConnectedPlanes();

			cout << endl << endl;
			cout << "--" << endl;
			int localCount = curPlane->getRelatedTriangleClass().countNum[x];
			if (localCount == 2 && find(adjacentPlanes.begin(),adjacentPlanes.end(), nextPlane) != adjacentPlanes.end()) {
				

					



				vector<double> point = threePlaneIntersectionPoint(&planeX, curPlane, nextPlane);

				bool found = false;

				for (int l = 0; l < frame.size();l++) {

					if (abs(frame[l][0] - point[0]) < 0.001 && abs(frame[l][1] - point[1]) < 0.001 && abs(frame[l][2] - point[2]) < 0.001) {
						found = true;
					}

				}
				if (!found) {
					frame.push_back(point);
				}
				cout << curPlane->toString();
				lastPlane = curPlane;
				curPlane = nextPlane;

				usedPlanes.push_back(curPlane);




			}
				
				
			
				
		}
			
			
		if (break1) {
			break;
		}
			

	}

	Loops.push_back(frame);

	return Loops;
		

}
	


			//vector<double> startPoint;
			//vector<vector<double>> points;
			//

			////loops until frame is finished
			//while(true){

			//	double closestDist = 10000000000000000;
			//	vector<double> closestPoint;
			//	Plane* closestPlane = NULL;

			//	//goes through every adjacent plane
			//	for (int i = 0; i < adjacentPlanes.size();i++) {
			//		Plane* nextPlane = adjacentPlanes[i];

			//		
			//		if (nextPlane == curPlane || nextPlane == lastPlane) {
			//			continue;
			//		}
			//		//get the next intersection point
			//		vector<double> point = threePlaneIntersectionPoint(curPlane, nextPlane, &planeX);

			//		//skips if its trying to go back on itself
			//		if (abs(lastPoint[0] - point[0]) < 0.01 && abs(lastPoint[1] - point[1]) < 0.01 && abs(lastPoint[2] - point[2]) < 0.01) {
			//			continue;
			//		};
			//		//get distance from intersection to (around) center of plane
			//		double dist = getDistanceToMain(point, curPlane->getRelatedTriangleClass().centerPoint);

			//		//if distance is smaller than last closest point, regardless of count switch them
			//		if (dist < closestDist) {
			//			closestDist = dist;
			//			closestPoint = point;
			//			closestPlane = nextPlane;
			//		}
			//		//if it is the same point as the closest but with the nextplane being a direct adjacent, switch them
			//		else if ((abs(closestDist - dist) < 0.01 ) && planeX.getRelatedTriangleClass().countNum[i] == 2) {
			//			closestDist = dist;
			//			closestPoint = point;
			//			closestPlane = nextPlane;
			//		}
			//		
			//	}
			//	cout << endl << closestPlane->toString();
			//	if (startPoint.empty()) {
			//		startPoint = closestPoint;
			//	}
			//	else if (closestPoint == startPoint) {
			//		
			//		break;
			//	}
			//	lastPlane = curPlane;
			//	curPlane = closestPlane;
			//	lastPoint = closestPoint;
			//	frame.push_back(closestPoint);
			//	usedPlanes.push_back(lastPlane);
			//	
			//}

			/*if (usedPlanes.size() == adjacentPlanes.size()) {
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


			}*/

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
	planeDict.clear();
	//igl::read_triangle_mesh("C:/Users/jaywh/source/repos/Dissertation/models"  "/old_chair.obj", V, F);
	igl::read_triangle_mesh("C:/Uni Stuff/year3/3rd year project polyfit ver/ThirdYearDissertation/models" "/Tower.obj", V, F);

	
	


	

	for (int x = 0; x < F.rows(); x++) {
		//initialise the current triangle in loop
		vector<vector<double>> nextTriangle = {};
		for (int i = 0; i < 3; i++) {
			nextTriangle.push_back({ V(F(x, i), 0), V(F(x, i), 1), V(F(x, i), 2) });
		}
		triangleClass temp = triangleClass(nextTriangle);
		objectTriangleArray.push_back(temp);
	}
	
	int planeCount = 0;
	for (int t = 0; t < objectTriangleArray.size(); t++) {
		
		if (objectTriangleArray[t].planeNo == -1) {
			
			objectTriangleArray[t].updatePlaneNo(planeCount);
			objectTriangleArray[t].makeTrianglePrime();
			FindConnected(&objectTriangleArray[t], &objectTriangleArray[t]);
			
			planeCount++;
		}
	}
	boundingBoxCreation();
	for (int num = 0; num < boundingBox.size(); num++) {
		cout << boundingBox[num][0]<<",";
		cout << boundingBox[num][1] << ",";
		cout << boundingBox[num][2] << endl;
	}
	
	vector<Plane> planeList;
	vector<triangleClass> primeTriangleList;
	int c = 0;
	int c2 = 0;
	for (triangleClass prime : objectTriangleArray) {
		
		
		if (prime.isPrimeTriangle) {
			cout << endl << endl;
				cout << c << " : ";
				cout << endl << endl;
				for (int p : prime.connectedPlanes) {
					cout << p << " ";
					
				}

				
			planeList.push_back(Plane(prime,c));
			primeTriangleList.push_back(prime);
			c++;
		}
		c2++;
	}
	for (int planeNo = 0; planeNo < planeList.size();planeNo++) {
		for (int index : primeTriangleList[planeNo].connectedPlanes) {
			planeList[planeNo].AddConnectedPlane(&planeList[index]);
		}
	}
	
	for (int p = 0; p < objectTriangleArray.size()-1;p++) {
		if (objectTriangleArray[p].planeNo == 2 || objectTriangleArray[p].planeNo == 0) {
			for (int x = 0; x < F.rows(); x++) {
				//initialise the current triangle in loop
				vector<vector<double>> nextTriangle = {};
				for (int i = 0; i < 3; i++) {
					nextTriangle.push_back({ V(F(x, i), 0), V(F(x, i), 1), V(F(x, i), 2) });
				}
				if (objectTriangleArray[p].points == nextTriangle) {
					unsigned int numRows = F.rows() - 1;
					unsigned int numCols = F.cols();

					if (x < numRows)
						F.block(x, 0, numRows - x, numCols) = F.block(x + 1, 0, numRows - x, numCols);

					F.conservativeResize(numRows, numCols);
				}
				
			}
			
		}
	}
	

	Eigen::MatrixXd D;
	Eigen::MatrixXi P;



	



	for (int x = 0; x < planeList.size();x++) {
		/*if (x == 9 || x == 2 ) {
			continue;
		}*/
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


		cout << axis;
		

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
					cout << endl << Loops[x][y][0];
					cout << Loops[x][y][1];
					cout << Loops[x][y][2];
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
		cout << endl << endl;
		cout << V3;

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

/*
vector<double> P1 =  prime.points[0] ;
			vector<double> P2 =  prime.points[1] ;
			vector<double> P3 = prime.points[2];

			V.conservativeResize(V.rows() + 3, V.cols());

			double x_c = (P1[0] + P2[0] + P3[0]) / 3;
			double y_c = (P1[1] + P2[1] + P3[1]) / 3;
			double z_c = (P1[2] + P2[2] + P3[2]) / 3;

			V(V.rows() - 1, 0) = x_c + (P1[0] - x_c) * 20;
			V(V.rows() - 1, 1) = y_c + (P1[1] - y_c) * 20;
			V(V.rows() - 1, 2) = z_c + (P1[2] - z_c) * 20;
			V(V.rows() - 2, 0) = x_c + (P2[0] - x_c) * 20;
			V(V.rows() - 2, 1) = y_c + (P2[1] - y_c) * 20;
			V(V.rows() - 2, 2) = z_c + (P2[2] - z_c) * 20;
			V(V.rows() - 3, 0) = x_c + (P3[0] - x_c) * 20;
			V(V.rows() - 3, 1) = y_c + (P3[1] - y_c) * 20;
			V(V.rows() - 3, 2) = z_c + (P3[2] - z_c) * 20;

			F.conservativeResize(F.rows() + 1, V.cols());

			F(F.rows() - 1, 0) = V.rows() - 1;
			F(F.rows() - 1, 1) = V.rows() - 2;
			F(F.rows() - 1, 2) = V.rows() - 3;*/




			//for (int i = 0; i < adjacentPlanes.size();i++) {
				//	cout << i << "<-" << endl;
				//	bool touchingCheck = false;
				//	for (int x = 0; x < adjacentPlanes.size(); x++) {
				//		
				//		Plane* nextPlane = adjacentPlanes[x];
				//		
				//		//checks to make sure the current plane is worth evaluating and also if its gone back to the start, that final condition stops the other conditions from mattering if its the first loop
				//		if ((nextPlane == curPlane) || usedPlanes.size() >= adjacentPlanes.size() || ((nextPlane == lastPlane) && lastPlane != NULL)) {
				//			continue;
				//		}
				//		vector<Plane*> nextPlaneList = nextPlane->GetConnectedPlanes();
				//		vector<Plane*> curPlaneList = curPlane->GetConnectedPlanes();
				//		cout << endl << endl;
				//		bool break1 = false;
				//		cout << "--" << endl;

				//		for (int d = 0; d < nextPlaneList.size();d++) {
				//			if (nextPlaneList[d]->getRelatedTriangle() == curPlane->getRelatedTriangle()) {
				//				vector<double> point = threePlaneIntersectionPoint(&planeX, curPlane, nextPlane);
				//				frame.push_back(point);
				//				corners.push_back(point);
				//				lastPlane = curPlane;
				//				curPlane = nextPlane;
				//				usedPlanes.push_back(curPlane);
				//				
				//				break1 = true;
				//				break;
				//			}
				//		}

				//		if (break1) {
				//			break1 = false;
				//			break;
				//		}			
						//else {
						//	 
						//	
						//	bool breakk = false;
						//	bool breakkk = false;
						//	int c2 = 0;
						//	for (int d = 0; d < nextPlaneList.size();d++) {
						//		
						//		
						//		if (nextPlaneList[d]->getRelatedTriangle() == planeX.getRelatedTriangle()) {
						//			
						//			continue;
						//		}

						//		vector<Plane*> temp4 = nextPlaneList[d]->GetConnectedPlanes();
						//		
						//		for (int l = 0; l < temp4.size();l++) {
						//			Plane* temp5 = temp4[l];
						//			if (temp5->getRelatedTriangle() == planeX.getRelatedTriangle()) {
						//				breakkk = true;
						//				break;
						//			}
						//			else if (temp5->getRelatedTriangle() == curPlane->getRelatedTriangle()) {
						//				c2++;
						//			}
						//			else if (temp5->getRelatedTriangle() == nextPlane->getRelatedTriangle()) {
						//				c2++;
						//			}
						//			else {
						//				for (int y = 0; y < adjacentPlanes.size();y++) {
						//					if (temp5->getRelatedTriangle() == adjacentPlanes[y]->getRelatedTriangle()) {
						//						c2--;
						//					}
						//				}
						//			}
						//		}

						//		if (breakkk == true || c2 != 2) {
						//			breakkk = false;
						//			c2 = 0;
						//			continue;
						//		}

						//		for (int l = 0; l < temp4.size();l++) {
						//			
						//			
						//			


						//			if (temp4[l] == curPlane) {
						//				
						//				
						//				//nextPlaneList[d]->AddConnectedPlane(&planeX);
						//				cout << nextPlaneList[d]->toString();
						//				cout << curPlane->toString();
						//				cout << planeX.toString();

						//				vector<double> first = threePlaneIntersectionPoint(&planeX, curPlane, nextPlane);
						//				vector<double> second = threePlaneIntersectionPoint(nextPlaneList[d], nextPlane, &planeX);
						//				vector<double> third = threePlaneIntersectionPoint(nextPlaneList[d], curPlane, &planeX);
						//				vector<double> fourth = threePlaneIntersectionPoint(nextPlaneList[d], curPlane, nextPlane);
						//				bool pushed = false;
						//				for (int m = 0; m < corners.size();m++) {
						//					if (corners[m] == first) {
						//						frame.push_back(first);
						//						pushed = true;
						//						break;
						//					}
						//					else if (corners[m] == second) {
						//						frame.push_back(second);
						//						pushed = true;
						//						break;
						//					}
						//					else if (corners[m] == third) {
						//						frame.push_back(third);
						//						pushed = true;
						//						break;
						//					}
						//					else if (corners[m] == fourth) {
						//						frame.push_back(fourth);
						//						pushed = true;
						//						break;
						//					}
						//				}

						//				if (!pushed) {
						//					frame.push_back(first);
						//					corners.push_back(first);
						//				}
						//				
						//				
						//				
						//					
						//				
						//				
						//				
						//				
						//				lastPlane = curPlane;
						//				curPlane = nextPlane;
						//				usedPlanes.push_back(curPlane);
						//				touchingCheck = true;
						//				breakk = true;
						//				break;
						//			}


						//			/*  1 2 3 9

						//				0 3 4 8

						//				0 4 5

						//				1 0 6 9

						//				2 1 7 8

						//				2 6 7 9

						//				5 3 7 8

						//				4 6 5

						//				6 1 4

						//				5 0 3*/


						//		}
						//		if (breakk) {
						//			break;
						//		}
						//	}
						//	if (breakk) {
						//		break;
						//	}
						//}



					//}



				//}