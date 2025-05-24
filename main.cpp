#include <igl/opengl/glfw/Viewer.h>
#include <igl/triangle/triangulate.h>
#include <unordered_set>
#include "PointClass.h"
#include <igl/readOFF.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/read_triangle_mesh.h>
#include "triangleClass.h"
#include <vector>
#include <cmath>
#include <string>
#include <map> 
#define _USE_MATH_DEFINES
using namespace std;


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

vector<triangleClass> objectTriangleArray;
vector<PointClass> mainFrame;

double pi = 3.14159265;



void FindConnected(triangleClass *startTriangle, triangleClass *curTriangle) {
	int count1 = 0;
	//for every triangle in object
	for (int x = 0; x < objectTriangleArray.size();x++) {
			int count = 0;
		
			
			if (curTriangle->planeNo == objectTriangleArray[x].planeNo) {
				continue;
			}
			vector<vector<double>> twoPoints;
			
			for (int j = 0;j < 3;j++) {

				


				if (j == 2 && count < 1) {
					break;
				}

				for (int k = 0; k < 3; k++) {
					//check if the bounding box can be updated with any of the next_triangles points
					//boundingBoxUpdate(curTriangle->points[j]);
					//check if triangle 1 overlaps points with triangle 2
					if (curTriangle->points[j] == objectTriangleArray[x].points[k]) {
						//increment duplicate point count
						twoPoints.push_back(curTriangle->points[j]);
						count++;
						
					}
				}
			}
			
			
			
			//if the triangles have any overlapping points
			if (count == 2 ) {

				//checks if the next_triangles has been assigned a plane number
				if (objectTriangleArray[x].planeNo != -1) {
					
					
					for (int r = 0; r < 2;r++) {

						//if point is already in the mainFrame, add the two plane numbers to the point's plane list (if they arent already in it)
						bool found = false;
						for (int j = 0; j < mainFrame.size();j++) {
							if (mainFrame[j].point == twoPoints[r]) {
								found = true;
								mainFrame[j].AddToPlaneList(curTriangle->planeNo);
								mainFrame[j].AddToPlaneList(objectTriangleArray[x].planeNo);
								
								break;
							}
						}
						//if point not in list
						if (found == false) {
							mainFrame.push_back(twoPoints[r]);
							mainFrame[mainFrame.size() - 1].planeList.push_back(curTriangle->planeNo);
							mainFrame[mainFrame.size() - 1].planeList.push_back(objectTriangleArray[x].planeNo);
						}
					}

				}
				else {

					double intAng = startTriangle->calcInteriorAngle(objectTriangleArray[x]);

					if (intAng <= 10) {
						objectTriangleArray[x].updatePlaneNo(startTriangle->planeNo);
						FindConnected(startTriangle, &objectTriangleArray[x]);

					}
				}
			}
	}
} 




vector<vector<double>> GetPointsInHoles(vector<vector<PointClass>> Loops) {
	//calculating areas of each loop to find the biggest
	vector<vector<double>> pointsInHoles;
	int pos = 0;
	double biggestArea = 0;
	for (int x = 0; x < Loops.size();x++) {
		vector<double> anchorPoint = Loops[x][0].point;
		double totalArea = 0;
		for (int y = 1; y < Loops[x].size(); y++) {
			if (y + 1 == Loops[x].size()) {
				continue;
			}
			else {
				vector<double> c2 = Loops[x][y].point;
				vector<double> c3 = Loops[x][y + 1].point;
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
			//three points in the loop
			vector<double> c1 = Loops[x][0].point;
			vector<double> c2 = Loops[x][1].point;
			vector<double> c3 = Loops[x][2].point;

			//get their edge distances
			double a = sqrt(pow(c1[0] - c2[0], 2) + pow(c1[1] - c2[1], 2) + pow(c1[2] - c2[2], 2));
			double b = sqrt(pow(c1[0] - c3[0], 2) + pow(c1[1] - c3[1], 2) + pow(c1[2] - c3[2], 2));
			double c = sqrt(pow(c3[0] - c2[0], 2) + pow(c3[1] - c2[1], 2) + pow(c3[2] - c2[2], 2));




			//find the incenter point
			double x = (a * c1[0] + b * c2[0] + c * c3[0]) / (a + b + c);
			double y = (a * c1[1] + b * c2[1] + c * c3[1]) / (a + b + c);
			double z = (a * c1[2] + b * c2[2] + c * c3[2]) / (a + b + c);

			pointsInHoles.push_back({ x, y,z });
		}
	}

	return pointsInHoles;
}
//function to get the effective "plane" boundary pointr
vector<PointClass> getRegionLoop(int PlaneX, vector<PointClass> unOrderedFrame) {
	vector<PointClass> frame;
	const PointClass firmStartPoint = unOrderedFrame[0];
	PointClass startPoint = unOrderedFrame[0];
	
	PointClass lastPoint = startPoint;
	for (int l = 0;l < unOrderedFrame.size();l++) {
		if (frame.size() == unOrderedFrame.size()) {
			break;
		}
		for (int plane : startPoint.planeList) {
			bool found = false;
			if (plane == PlaneX) {
				continue;
			}
			for (int d = 0; d < unOrderedFrame.size();d++) {
				if (unOrderedFrame[d].point == startPoint.point || unOrderedFrame[d].point == lastPoint.point || find(frame.begin(), frame.end(), unOrderedFrame[d]) != frame.end()) {
					continue;
				}
				if (find(unOrderedFrame[d].planeList.begin(), unOrderedFrame[d].planeList.end(), plane) != unOrderedFrame[d].planeList.end()) {
					frame.push_back(unOrderedFrame[d]);
					lastPoint = startPoint;
					startPoint = unOrderedFrame[d];
					found = true;
					break;
				}
			}
			if (found) {
				break;
			}
		}
	}
	
	return { frame };
}
vector<double> calcX(triangleClass plane, double y, double z) {
	vector<double> pointOnPlane = plane.points[0];
	vector<double> normal = plane.returnNormal();
	double total = 0;
	total += normal[0] * pointOnPlane[0] + normal[1] * pointOnPlane[1] + normal[2] * pointOnPlane[2];

	total -= normal[1] * y + normal[2] * z;

	total /= normal[0];

	return { y, z, total };


}
vector<double> calcZ(triangleClass plane, double x, double y) {
	vector<double> pointOnPlane = plane.points[0];
	vector<double> normal = plane.returnNormal();
	double total = 0;
	total += normal[0] * pointOnPlane[0] + normal[1] * pointOnPlane[1] + normal[2] * pointOnPlane[2];

	total -= normal[0] * x + normal[1] * y;

	total /= normal[2];

	return { x, y, total };


}
vector<double> calcY(triangleClass plane, double x, double z) {
	vector<double> pointOnPlane = plane.points[0];
	vector<double> normal = plane.returnNormal();
	double total = 0;
	total += normal[0] * pointOnPlane[0] + normal[1] * pointOnPlane[1] + normal[2] * pointOnPlane[2];

	

	total -= (normal[0] * x + normal[2] * z);



	total /= normal[1];



	return { x, z, total };


}

int main(int argc, char* argv[])
{
	// Load a mesh in OFF format
	

	//igl::read_triangle_mesh("C:/Users/karen/source/repos/year3diss/models"  "/Pin.obj", V, F);

	//igl::read_triangle_mesh("C:/Users/jaywh/source/repos/Dissertation/models"  "/handle.obj", V, F);
	igl::read_triangle_mesh("C:/Uni Stuff/year3/3rd year project polyfit ver/ThirdYearDissertation/models" "/Tower.obj", V, F);


	cout << V.rows();

	//igl::opengl::glfw::Viewer viewer;

	//viewer.data().set_mesh(V, F);
	//viewer.launch();


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
	float percentCalsDenom = objectTriangleArray.size();
	for (int t = 0; t < objectTriangleArray.size(); t++) {



		if (objectTriangleArray[t].planeNo == -1) {

			objectTriangleArray[t].updatePlaneNo(planeCount);
			objectTriangleArray[t].makeTrianglePrime();
			FindConnected(&objectTriangleArray[t], &objectTriangleArray[t]);

			planeCount++;
		}


		cout<< t/percentCalsDenom*100  <<"%"<<endl;
	}



	




	
	vector<triangleClass> primeTriangleList;
	int c = 0;
	int c2 = 0;

	for (int p = 0; p < objectTriangleArray.size() - 1;p++) {
		if (objectTriangleArray[p].planeNo == 1) {
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

	/*igl::opengl::glfw::Viewer viewer;

	viewer.data().set_mesh(V, F);
	viewer.launch();*/




	Eigen::MatrixXd D;
	Eigen::MatrixXi P;

	//removing points with equal plane lists
	for (int p = 0; p < mainFrame.size();p++) {
		bool pErase = false;
		for (int h = 0; h < mainFrame.size();h++) {
			if (p == h) {
				continue;
			}
			if (mainFrame[p].planeList == mainFrame[h].planeList) {
				mainFrame.erase(mainFrame.begin() + h);
				
				h--;
			}
		}
		
	}


	//Loops through each region
	for (int x = 0; x < planeCount;x++) {
		cout << x;
		








		//Points connected to current region
		vector<PointClass> mainFrame2;

		//Used to remove duplicate points that may occur if a region is contained within two other planes
		vector<int> uniquePlanesInFrame;
		vector<int> planesInFrame;


		//finding points in the main Frame
		for (PointClass p : mainFrame) {
			if (p.planeList.size() > 2 && (find(p.planeList.begin(), p.planeList.end(), x) != p.planeList.end())) {
				for (int d = 0;d < p.planeList.size(); d++) {
					if (find(uniquePlanesInFrame.begin(), uniquePlanesInFrame.end(), p.planeList[d]) == uniquePlanesInFrame.end()) {
						uniquePlanesInFrame.push_back(p.planeList[d]);
					}
					planesInFrame.push_back(p.planeList[d]);
				}
				mainFrame2.push_back(p);
			}
		}


		cout << endl << mainFrame2.size();

		for (int i = 0; i < uniquePlanesInFrame.size();i++) {

			int count = 0;
			for (int d = 0; d < planesInFrame.size();d++) {
				if (uniquePlanesInFrame[i] == planesInFrame[d]) {
					count++;
				}
			}

			if (count < 2) {
				for (int d = 0; d < mainFrame2.size();d++) {
					if (find(mainFrame2[d].planeList.begin(), mainFrame2[d].planeList.end(), uniquePlanesInFrame[i]) != mainFrame2[d].planeList.end() && mainFrame2[d].planeList.size() < 4) {
						mainFrame2.erase(mainFrame2.begin() + d);
					}
				}
			}
		}
		
		if (mainFrame2.size() < 3) {
			continue;
		}



		

		int looping = true;
		vector<vector<PointClass>> Loops;
		while (looping) {

			vector<PointClass> mainFrame3;
			vector<int> planes;
			mainFrame3.push_back(mainFrame2[0]);
			for (int h : mainFrame2[0].planeList) {
				if (h == x) {
					continue;
				}
				planes.push_back(h);
			}
			mainFrame2.erase(mainFrame2.begin());
			bool looping2 = true;
			while (looping2) {
				int size = mainFrame3.size();
				for (int h = 0; h < mainFrame2.size();h++) {
					for (int k : mainFrame2[h].planeList) {
						if (k == x) {
							continue;
						}
						if (find(planes.begin(), planes.end(), k) != planes.end()) {
							mainFrame3.push_back(mainFrame2[h]);
							for (int r : mainFrame2[h].planeList) {
								planes.push_back(r);
							}
							mainFrame2.erase(mainFrame2.begin() + h);
							h--;
							break;
						}
					}
				}
				if (mainFrame3.size() == size) {
					break;
				}
			}
		



			Loops.push_back(getRegionLoop(x, mainFrame3));
			if (mainFrame2.size() < 3) {
				looping = false;
			}

		}

		

		
		cout << Loops.size();
		for (int g = 0; g < Loops.size();g++) {
			if (Loops[g].size() < 3) {
				Loops.erase(Loops.begin() + g);
			}
		}

		if (Loops.size() == 0) {
			continue;
		}
		vector<vector<double>> holePoints;
		if (Loops.size() > 1) {
			holePoints = GetPointsInHoles(Loops);
		}
		
		//vector<vector<double>> holePoints;
		vector<vector<double>> frame;


		
		triangleClass checks = triangleClass({ Loops[0][0].point, Loops[0][2].point, Loops[0][1].point });

		Eigen::MatrixXd V3;
		Eigen::MatrixXi E;
		Eigen::MatrixXd H;
		Eigen::MatrixXd V2;
		Eigen::MatrixXi F2;
		Eigen::MatrixXd pre;


		
		for (int g = 0; g < Loops.size();g++) {
			pre.resize(pre.rows() + Loops[g].size(), 3);
		}

		int z = 0;
		for (int g = 0; g < Loops.size();g++) {
			for (int y = 0; y < Loops[g].size();y++) {

				pre(y + z, 0) = Loops[g][y].point[0];
				pre(y + z, 1) = Loops[g][y].point[1];
				pre(y + z, 2) = Loops[g][y].point[2];



			}
			z += Loops[g].size();
		}
		
		

		for (int g = 0; g < Loops.size();g++) {
			V3.resize(V3.rows() + Loops[g].size(), 2);
			E.resize(E.rows() + Loops[g].size(), 2);
		}
		int jay = 0;
		for (int j = 0;j < 3;j++) {

			

			jay = j;
			
			z = 0;
			for (int g = 0; g < Loops.size();g++) {
				for (int y = 0; y < Loops[g].size();y++) {
					if (j == 0) {
						V3(y + z, 0) = Loops[g][y].point[1];
						V3(y + z, 1) = Loops[g][y].point[2];

					}
					else if (j == 1) {
						V3(y + z, 0) = Loops[g][y].point[0];
						V3(y + z, 1) = Loops[g][y].point[2];

					}
					else if (j == 2) {
						V3(y + z, 0) = Loops[g][y].point[0];
						V3(y + z, 1) = Loops[g][y].point[1];

					}

					E(y + z, 0) = y + z;
					if (y + z == z + Loops[g].size() - 1)
					{
						E(y + z, 1) = z;
					}
					else
					{
						E(y + z, 1) = y + z + 1;
					}

				}
				z += Loops[g].size();
			}
			
			

			bool break1 = false;
			for (int n = 0;n < V3.rows();n++) {
				for (int m = 0;m < V3.rows();m++) {
					if (n == m) {
						continue;
					}
					auto b1 = V3.block<1, 2>(n, 0);
					auto b2 = V3.block<1, 2>(m, 0);
					
					if (abs(b1(0) - b2(0)) < 0.001 && abs(b1(1) - b2(1)) < 0.001){
						break1 = true;
					}
					

				}
				
			}
			if (break1) {
				continue;
			}
			break;

		
		}





		if (holePoints.size() > 0) {
			H.resize(holePoints.size(), 2);

			for (int x = 0; x < holePoints.size();x++) {
				if (jay == 0) {
					H(x, 0) = holePoints[x][1];
					H(x, 1) = holePoints[x][2];
				}
				else if(jay == 1) {
					H(x, 0) = holePoints[x][0];
					H(x, 1) = holePoints[x][2];
				}
				else if(jay == 2) {
					H(x, 0) = holePoints[x][0];
					H(x, 1) = holePoints[x][1];
				}
				
			}
		}
	
		cout << endl << endl << H;
		igl::triangle::triangulate(V3, E, H, "1", V2, F2);

	
	
	
		Eigen::MatrixXd V4;
		Eigen::MatrixXi F4;

		V4.resize(V2.rows(), 3);


		
		

		


		for (int p = 0;p < pre.rows();p++) {
			if (jay == 0) {

				
				V4(p, 0) = pre(p,0);
				//V4(p, 0) = calcX(checks, V2(p, 0), V2(p, 1))[2];
				V4(p, 1) = V2(p, 0);
				V4(p, 2) = V2(p, 1);
			}
			else if (jay == 1) {
				

				V4(p, 0) = V2(p, 0);
				V4(p, 1) = pre(p, 1);
				//V4(p, 1) = calcY(checks, V2(p, 0), V2(p, 1))[2];

				V4(p, 2) = V2(p, 1);
			}
			else if (jay == 2) {

				
				V4(p, 0) = V2(p, 0);
				V4(p, 1) = V2(p, 1);
				V4(p, 2) = pre(p, 2);
				//V4(p, 2) = calcZ(checks, V2(p, 0), V2(p, 1))[2];
			}
			
		}

		for (int p = pre.rows();p < V2.rows();p++) {
			if (jay == 0) {

				
				
				V4(p, 0) = calcX(checks, V2(p, 0), V2(p, 1))[2];
				V4(p, 1) = V2(p, 0);
				V4(p, 2) = V2(p, 1);
			}
			else if (jay == 1) {
			

				V4(p, 0) = V2(p, 0);
				
				V4(p, 1) = calcY(checks, V2(p, 0), V2(p, 1))[2];

				V4(p, 2) = V2(p, 1);
			}
			else if (jay == 2) {

				
				V4(p, 0) = V2(p, 0);
				V4(p, 1) = V2(p, 1);
				
				V4(p, 2) = calcZ(checks, V2(p, 0), V2(p, 1))[2];
			}

		}

			//}

	





			if (D.rows() == 0) {

				D.conservativeResize(V4.rows(), V4.cols());
				D << V4;
			}
			else {
				Eigen::MatrixXd temp1(D.rows() + V4.rows(), 3);

				temp1 << D, V4;
				D = temp1;
			}





			//Removing duplicates from the verticies list

			vector<vector<double>> checked;

			for (int i = 0; i < D.rows();i++) {

				vector<double> cur = { D(i,0),D(i,1),D(i,2) };

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

		//cout << D;


		cout << P.rows();

		igl::opengl::glfw::Viewer viewer;

		viewer.data().set_mesh(D, P);
		viewer.launch();



	}






			