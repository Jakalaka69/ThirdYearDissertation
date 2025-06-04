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
#include "RegionGen.h"
#include "Reconstructor.h"

#define _USE_MATH_DEFINES
using namespace std;





double s1;
double pi = 3.14159265;

Reconstructor::Reconstructor() {}

vector<vector<double>> Reconstructor::GetPointsInHoles(vector<vector<PointClass>> Loops) {
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
vector<PointClass> Reconstructor::getRegionLoop(int PlaneX, vector<PointClass> unOrderedFrame) {
	vector<PointClass> frame;
	const PointClass firmStartPoint = unOrderedFrame[0];
	PointClass startPoint = unOrderedFrame[0];
	
	PointClass lastPoint = startPoint;
	for (int l = 0;l < unOrderedFrame.size();l++) {
		if (startPoint == firmStartPoint && l != 0) {
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
vector<double> Reconstructor::calcX(triangleClass plane, double y, double z) {
	vector<double> pointOnPlane = plane.points[0];
	vector<double> normal = plane.returnNormal();
	double total = 0;
	total += normal[0] * pointOnPlane[0] + normal[1] * pointOnPlane[1] + normal[2] * pointOnPlane[2];

	total -= normal[1] * y + normal[2] * z;

	total /= normal[0];

	return { y, z, total };


}
vector<double> Reconstructor::calcZ(triangleClass plane, double x, double y) {
	vector<double> pointOnPlane = plane.points[0];
	vector<double> normal = plane.returnNormal();
	double total = 0;
	total += normal[0] * pointOnPlane[0] + normal[1] * pointOnPlane[1] + normal[2] * pointOnPlane[2];

	total -= normal[0] * x + normal[1] * y;

	total /= normal[2];

	return { x, y, total };


}
vector<double> Reconstructor::calcY(triangleClass plane, double x, double z) {
	vector<double> pointOnPlane = plane.points[0];
	vector<double> normal = plane.returnNormal();
	double total = 0;
	total += normal[0] * pointOnPlane[0] + normal[1] * pointOnPlane[1] + normal[2] * pointOnPlane[2];

	

	total -= (normal[0] * x + normal[2] * z);



	total /= normal[1];



	return { x, z, total };


}
void Reconstructor::Reconstruct(OperatorClass::funcReturn returnValues,string outputPath)
{
	int RegionCount = returnValues.planeCount;
	vector<PointClass> mainFrame = returnValues.pointList;
	int startVertices = returnValues.vertices;
	int startFaces = returnValues.faces;
	


	

	Eigen::MatrixXd D;
	Eigen::MatrixXi P;

	//removing points with equal plane lists



	//Loops through each region
	for (int x = 0; x < RegionCount;x++){
		cout << "Starting region " << x << " out of " << RegionCount << endl;
		//Points connected to current region
		vector<PointClass> mainFrame2;

		//Used to remove duplicate points that may occur if a region is contained within two other planes
		vector<int> uniquePlanesInFrame;
		vector<int> planesInFrame;


		//finding points in the main Frame
		for (PointClass p : mainFrame) {
			if (p.planeList.size() > 2 && (find(p.planeList.begin(), p.planeList.end(), x) != p.planeList.end())) {
				mainFrame2.push_back(p);
			}
		}

		for (int p = 0; p < mainFrame2.size();p++) {
			bool pErase = false;
			for (int h = 0; h < mainFrame2.size();h++) {
				if (p == h) {
					continue;
				}
				auto v1 = mainFrame2[p].planeList;
				auto v2 = mainFrame2[h].planeList;
				sort(v1.begin(), v1.end());
				sort(v2.begin(), v2.end());
				if (v1 == v2) {
					if (p < h) {
						mainFrame2.erase(mainFrame2.begin() + p);
						h--;
						mainFrame2.erase(mainFrame2.begin() + h);
						pErase = true;
					}
					else if(p > h) {
						mainFrame2.erase(mainFrame2.begin() + h);
						p--;
						mainFrame2.erase(mainFrame2.begin() + p);
						h--;
					}
				}
			}
			if (pErase) {
				p--;
			}
		}
		if (mainFrame2.size() < 3) {
			continue;
		}
		for (PointClass p : mainFrame2) {
				for (int d = 0;d < p.planeList.size(); d++) {
					if (p.planeList[d] == x) {
						continue;
					}
					else if (find(uniquePlanesInFrame.begin(), uniquePlanesInFrame.end(), p.planeList[d]) == uniquePlanesInFrame.end()) {
						uniquePlanesInFrame.push_back(p.planeList[d]);
					}
					planesInFrame.push_back(p.planeList[d]);
				}
		}

		
		//seach through each unique plane in all of the points
		for (int i = 0; i < uniquePlanesInFrame.size();i++) {

			//counts how in how many points that plane appears
			int count = 0;
			for (int d = 0; d < planesInFrame.size();d++) {
				if (uniquePlanesInFrame[i] == planesInFrame[d]) {
					count++;
				}
			}
			

			//if it in at least two points, i.e. an edge can be made
			if (count < 2) {
				bool break2 = false;
				//for every point in the frame
				for (int d = 0; d < mainFrame2.size();d++) {

					//if this unique plane is in this point, and this point has 3 or less points, it removes the point from the frame
					if (find(mainFrame2[d].planeList.begin(), mainFrame2[d].planeList.end(), uniquePlanesInFrame[i]) != mainFrame2[d].planeList.end() && mainFrame2[d].planeList.size() < 4) {
						mainFrame2.erase(mainFrame2.begin() + d);
						d--;
					}
					else if (mainFrame2[d].planeList.size() >= 4) {
						for (int g = 0; g < mainFrame2[d].planeList.size();g++) {
							if (mainFrame2[d].planeList[g] == uniquePlanesInFrame[i]) {
								mainFrame2[d].planeList.erase(mainFrame2[d].planeList.begin() + g);
								g--;
							}

						}


					}
					
					
				}
			}
			else if (count > 2) {
				
				//for every point in the frame
				for (int d = 0; d < mainFrame2.size();d++) {

					//if this unique plane is in this point, and this point has 3 or less points, it removes the point from the frame
					if(mainFrame2[d].planeList.size() >= 4) {
						for (int g = 0; g < mainFrame2[d].planeList.size();g++) {
							if (mainFrame2[d].planeList[g] == uniquePlanesInFrame[i]) {
								mainFrame2[d].planeList.erase(mainFrame2[d].planeList.begin() + g);
								g--;
							}
							
						}

						
					}


				}
			}
		}
		
		int looping = true;
		vector<vector<PointClass>> Loops;



		while (looping) {

			//One loop
			vector<PointClass> mainFrame3;

			//regions in loop
			vector<int> planes;

			//adding first vertex
			mainFrame3.push_back(mainFrame2[0]);

			//adding first vertex's regions
			for (int h : mainFrame2[0].planeList) {
				if (h == x) {
					continue;
				}
				planes.push_back(h);
			}
			//remove first vertex from unordered vertices
			mainFrame2.erase(mainFrame2.begin());

			//repeat process checking if current vertex 
			//is touching regions already in vector
			//Remove from mainFrame2 as you go
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

			//for each loop, calculate their ordered frame
			Loops.push_back(getRegionLoop(x, mainFrame3));

			//Stop looping if you have reached the end of the unordered vertices
			if (mainFrame2.size() < 3) {
				looping = false;
			}

		}

		//Checking all loops are valid

		if (Loops.size() == 0) {continue;}
		
		for (int g = 0; g < Loops.size();g++) {
			if (Loops[g].size() < 3) {
				Loops.erase(Loops.begin() + g);
				g--;
			}
		}
		if (Loops.size() == 0) {continue;}

		//Getting holes in points if there is more than one loop
		vector<vector<double>> holePoints;
		if (Loops.size() > 1) {
			holePoints = GetPointsInHoles(Loops);
		}
		
		//vector<vector<double>> holePoints;
		vector<vector<double>> frame;


		
		triangleClass checks = triangleClass({ Loops[0][0].point, Loops[0][2].point, Loops[0][1].point },0);

		Eigen::MatrixXd V3;
		Eigen::MatrixXi E;
		Eigen::MatrixXd H;
		Eigen::MatrixXd V2;
		Eigen::MatrixXi F2;
		Eigen::MatrixXd pre;

		triangleClass normalCalc;
		double xDiff = 0;
		double yDiff = 0;
		double zDiff = 0;

		//Taking the average normal directions of some triangles that could make up the region
		for (int p = 0; p < Loops[0].size();p++) {
			normalCalc = triangleClass({ Loops[0][p% Loops[0].size()].point, Loops[0][(p + 1) % Loops[0].size()].point, Loops[0][(p + 2) % Loops[0].size()].point },0);
			xDiff = max(xDiff,abs(normalCalc.normal[0]));
			yDiff = max(yDiff, abs(normalCalc.normal[1]));
			zDiff = max(zDiff, abs(normalCalc.normal[2]));
		}
		s1 = 0;
		int s2 = 0;
		
		double biggest = 0;
		double second = 0;
		vector<double> axis = { xDiff,yDiff,zDiff };


		//comparing axis to see determine the top 2 most influential on the region
		for (int f = 0;f < 3;f++) {
			bool isBig = false;
			if (axis[f] > biggest) {
				s2 = s1;
				s1 = f;
				biggest = axis[f];
				isBig = true;
			}
			if (axis[f] > second && !isBig) {
				
				s2 = f;
				second = axis[f];
			}
		}

		
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


		
	
		
			z = 0;
			
			//For the amount of loops there are
			for (int g = 0; g < Loops.size();g++) {
				//loop through each vertex
				for (int y = 0; y < Loops[g].size();y++) {
					//Dependant on the axis check from before, choose the right 2 vertices to pass
					if (s1 == 0) {
						V3(y + z, 0) = Loops[g][y].point[1];
						V3(y + z, 1) = Loops[g][y].point[2];

					}
					else if (s1 == 1) {
						V3(y + z, 0) = Loops[g][y].point[0];
						V3(y + z, 1) = Loops[g][y].point[2];

					}
					else if (s1 == 2) {
						V3(y + z, 0) = Loops[g][y].point[0];
						V3(y + z, 1) = Loops[g][y].point[1];

					}

					//Creating edges between each vertex
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
					
					if (abs(b1(0) - b2(0)) == 0 && abs(b1(1) - b2(1)) == 0){
						s1 = s2;
						break1 = true;
						break;
					}
					

				}
				if (break1) {
					break;
				}

				
			}
	

		

		z = 0;
		for (int g = 0; g < Loops.size();g++) {
			for (int y = 0; y < Loops[g].size();y++) {
				if (s1 == 0) {
					V3(y + z, 0) = Loops[g][y].point[1];
					V3(y + z, 1) = Loops[g][y].point[2];

				}
				else if (s1 == 1) {
					V3(y + z, 0) = Loops[g][y].point[0];
					V3(y + z, 1) = Loops[g][y].point[2];

				}
				else if (s1 == 2) {
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
		break1 = false;
		for (int n = 0;n < V3.rows();n++) {
			for (int m = 0;m < V3.rows();m++) {
				if (n == m) {
					continue;
				}


				auto b1 = V3.block<1, 2>(n, 0);
				auto b2 = V3.block<1, 2>(m, 0);

				if (abs(b1(0) - b2(0)) == 0 && abs(b1(1) - b2(1)) == 0) {
					
					break1 = true;
					break;
				}


			}
			if (break1) {
				break;
			}


		}


		if (holePoints.size() > 0) {
			H.resize(holePoints.size(), 2);

			for (int x = 0; x < holePoints.size();x++) {
				if (s1 == 0) {
					H(x, 0) = holePoints[x][1];
					H(x, 1) = holePoints[x][2];
				}
				else if(s1 == 1) {
					H(x, 0) = holePoints[x][0];
					H(x, 1) = holePoints[x][2];
				}
				else if(s1 == 2) {
					H(x, 0) = holePoints[x][0];
					H(x, 1) = holePoints[x][1];
				}
				
			}
		}
		
		

		igl::triangle::triangulate(V3, E, H, "1", V2, F2);
		
		
	
		Eigen::MatrixXd V4;
		Eigen::MatrixXi F4;

		V4.resize(V2.rows(), 3);


		
		

		

		//Restoring original vertices using their copies
		for (int p = 0;p < pre.rows();p++) {
			if (s1 == 0) {
				V4(p, 0) = pre(p, 0);
				V4(p, 1) = V2(p, 0);
				V4(p, 2) = V2(p, 1);
			}
			else if (s1 == 1) {
				V4(p, 0) = V2(p, 0);
				V4(p, 1) = pre(p, 1);
				V4(p, 2) = V2(p, 1);
			}
			else if (s1 == 2) {
				V4(p, 0) = V2(p, 0);
				V4(p, 1) = V2(p, 1);
				V4(p, 2) = pre(p, 2);
			}
			
		}

		//Restoring new coordinates that may have been created 
		//by subbing their two known points into the planes equation
		for (int p = pre.rows();p < V2.rows();p++) {
			if (s1 == 0) {
				V4(p, 0) = V4(p, 0) = calcX(checks, V2(p, 0), V2(p, 1))[2];
				V4(p, 1) = V2(p, 0);
				V4(p, 2) = V2(p, 1);
			}
			else if (s1 == 1) {
				V4(p, 0) = V2(p, 0);
				V4(p, 1) = V4(p, 1) = calcY(checks, V2(p, 0), V2(p, 1))[2];
				V4(p, 2) = V2(p, 1);
			}
			else if (s1 == 2) {
				V4(p, 0) = V2(p, 0);
				V4(p, 1) = V2(p, 1);
				V4(p, 2) = calcZ(checks, V2(p, 0), V2(p, 1))[2];
			}

		}

			
		

		//Adding all vertices
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


		//adding face data
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
	igl::writeOBJ(outputPath, D, P);

	cout << endl << "Started with " << startVertices << " Vertices, ended with " << D.rows() << " Vertices" << endl ;

	cout << endl << "Started with " << startFaces << " Faces, ended with " << P.rows() << " Faces" << endl << endl << endl;
	igl::opengl::glfw::Viewer viewer;

	viewer.data().set_mesh(D, P);
	viewer.launch();



}






			