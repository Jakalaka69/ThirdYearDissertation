#include <igl/readOFF.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/read_triangle_mesh.h>
#include <C:\Uni Stuff\year3\3rd year project polyfit ver\ThirdYearDissertation\triangleClass.h>
#include <vector>
#include <cmath>
#include <string>
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
				//put angle checks here and only do the stuff below if they pass




				//int temp = 0;
				//for (vector<double> allPoint : allPoints) {
				//	if (allPoint == touchingPoints[0]) {
				//		final4Points.push_back(allPoint);
				//		aVertIndex = temp;
				//	}
				//	else if (allPoint == touchingPoints[1]) {
				//		final4Points.push_back(allPoint);
				//		dVertIndex = temp;
				//	}
				//	else if (allPoint != touchingPoints[0] && allPoint != touchingPoints[1]) {
				//		final4Points.push_back(allPoint);

				//	}
				//	temp++;
				//}
				//aVertIndex = aVertIndex % 4;
				//dVertIndex = dVertIndex % 4;
				//int offset = 1;
				//int bVertIndex = 0;
				//while (bVertIndex == aVertIndex || bVertIndex == dVertIndex) {

				//	bVertIndex = offset % 4;
				//	offset++;
				//}
				//offset = 1;
				//int cVertIndex = 0;
				//while (cVertIndex == aVertIndex || cVertIndex == dVertIndex || cVertIndex == bVertIndex) {

				//	cVertIndex = offset % 4;
				//	offset++;
				//}



				//vector<double> U = { startTriangle[0][0] - startTriangle[1][0]
				//				   , startTriangle[0][1] - startTriangle[1][1]
				//				   , startTriangle[0][2] - startTriangle[1][2] };

				//vector<double> V = { startTriangle[0][0] - startTriangle[2][0]
				//				   , startTriangle[0][1] - startTriangle[2][1]
				//				   , startTriangle[0][2] - startTriangle[2][2] };

				//vector<double> Normal = { 0,0,0 };



				//Normal[0] = (U[1] * V[2]) - (U[2] * V[1]);
				//Normal[1] = (U[2] * V[0]) - (U[0] * V[2]);
				//Normal[2] = (U[0] * V[1]) - (U[1] * V[0]);

				//if (Normal == START_TRIANGLE.returnNormal()) {
				//	printf("Normal Match");
				//}







				//vector<double> U2 = { final4Points[aVertIndex][0] - final4Points[cVertIndex][0]
				//				   , final4Points[aVertIndex][1] - final4Points[cVertIndex][1]
				//				   , final4Points[aVertIndex][2] - final4Points[cVertIndex][2] };

				//vector<double> V2 = { final4Points[aVertIndex][0] - final4Points[dVertIndex][0]
				//				  , final4Points[aVertIndex][1] - final4Points[dVertIndex][1]
				//				  , final4Points[aVertIndex][2] - final4Points[dVertIndex][2] };

				//vector<double> Normal2 = { 0,0,0 };

				//Normal2[0] = (U2[1] * V2[2]) - (U2[2] * V2[1]);
				//Normal2[1] = (U2[2] * V2[0]) - (U2[0] * V2[2]);
				//Normal2[2] = (U2[0] * V2[1]) - (U2[1] * V2[0]);


				//double numerator = abs((Normal[0] * Normal2[0]) + (Normal[1] * Normal2[1]) + (Normal[2] * Normal2[2]));
				//double denominator = (sqrt(pow(Normal[0], 2) + pow(Normal[1], 2) + pow(Normal[2], 2)) * sqrt(pow(Normal2[0], 2) + pow(Normal2[1], 2) + pow(Normal2[2], 2)));
				//double interiorAngle = acos(numerator / denominator);
				//double interiorDegrees = interiorAngle * (180 / pi);
				////cout << endl << interiorAngle * (180 / pi);



				//Above code redundant

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
	vector<double> P1 = {V(F(random, 0), 0), V(F(random, 0), 1), V(F(random, 0), 2)};
	vector<double> P2 = { V(F(random, 1), 0), V(F(random, 1), 1), V(F(random, 1), 2) };
	vector<double> P3 = { V(F(random, 2), 0), V(F(random, 2), 1), V(F(random, 2), 2) };

	//initialise triangle with the points
	vector<vector<double>> randTriangle = { P1, P2, P3 };
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
	 fullConnectedList = FindConnected(randTriangle,fullConnectedList,randTriangle);
	
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



	 cout << P1[0] << P1[1] << P1[2] << endl;
	 cout << P2[0] << P2[1] << P2[2] << endl;
	 cout << P3[0] << P3[1] << P3[2] << endl;
	 cout << x_c << endl;
	 cout << y_c << endl;
	 cout << z_c << endl;
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

	 cout << V << endl;
	//print all triangles in final list
	for (vector<vector<double>> triangle : fullConnectedList) {
		std::cout << "Start of triangle" << endl;
		int count1 = 0;
		for (vector<double> point : triangle) {
			std::cout << "Point " << count1 << " coordinates:";
			count1++;
			for (double coord : point) {
				std::cout << coord;
			}
			std::cout << endl;
		}
		std::cout << "End of triangle" << endl;
	}
	
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




//Line AB 



			/*double distAtoB = sqrt(pow(final4Points[aVertIndex][0] - final4Points[bVertIndex][0], 2)
								 + pow(final4Points[aVertIndex][1] - final4Points[bVertIndex][1], 2)
								 + pow(final4Points[aVertIndex][2] - final4Points[bVertIndex][2], 2));
			cout << endl << "a-B = " << distAtoB;
			//line AC
			double distAtoC = sqrt(pow(final4Points[aVertIndex][0] - final4Points[cVertIndex][0], 2)
								 + pow(final4Points[aVertIndex][1] - final4Points[cVertIndex][1], 2)
								 + pow(final4Points[aVertIndex][2] - final4Points[cVertIndex][2], 2));
			cout << endl << "A-C = " << distAtoC;
			double distBtoC = sqrt(pow(final4Points[bVertIndex][0] - final4Points[cVertIndex][0], 2)
								 + pow(final4Points[bVertIndex][1] - final4Points[cVertIndex][1], 2)
								 + pow(final4Points[bVertIndex][2] - final4Points[cVertIndex][2], 2));
			cout << endl << "B-C = " << distBtoC;
			//Line DA
			double distDtoA = sqrt(pow(final4Points[dVertIndex][0] - final4Points[aVertIndex][0], 2)
								 + pow(final4Points[dVertIndex][1] - final4Points[aVertIndex][1], 2)
								 + pow(final4Points[dVertIndex][2] - final4Points[aVertIndex][2], 2));
			cout << endl << "D-A = " << distDtoA;
			//Line DB
			double distDtoB = sqrt(pow(final4Points[dVertIndex][0] - final4Points[bVertIndex][0], 2)
								 + pow(final4Points[dVertIndex][1] - final4Points[bVertIndex][1], 2)
								 + pow(final4Points[dVertIndex][2] - final4Points[bVertIndex][2], 2));
			cout << endl << "D-B = " << distDtoB;
			//Line DC
			double distDtoC = sqrt(pow(final4Points[dVertIndex][0] - final4Points[cVertIndex][0], 2)
								 + pow(final4Points[dVertIndex][1] - final4Points[cVertIndex][1], 2)
								 + pow(final4Points[dVertIndex][2] - final4Points[cVertIndex][2], 2));
			cout << endl << "D-C = " << distDtoC;



			//Shared verts are a and d
			// Triangle 1 = A-B B-D D-A
			// Triangle 2 = A-C C-D D-A
			//
			// assume both triangle scalene to cover all cases
			// s = (a+b+c)/2
			//
			// Heron's formula
			// sqrt(s(s-a)(s-b)(s-c)) to find area
			//
			// 2*area/base to find the height
			//
			//cos(A) = (b^2 + c^2 − a^2)/2bc For interior angle once all three values are found


			double s = (distAtoB + distDtoB + distDtoA) / 2;

			double h = sqrt(s*(s - distAtoB)*(s - distDtoB)*(s - distDtoA));

			double triangle1Height = 2 * h / distDtoA;

			s = (distAtoC + distDtoC + distDtoA) / 2;

			h = sqrt(s*(s - distAtoC)*(s - distDtoC)*(s - distDtoA));

			double triangle2Height = 2 * h / distDtoA;

			cout << triangle1Height << " " << triangle2Height << " " << distBtoC << endl;

			double interiorAngle = acos((pow(triangle2Height,2)+pow(triangle1Height,2)-pow(distBtoC,2))/(2*triangle2Height*triangle1Height));
			
			double interiorDegrees = interiorAngle * 180 / pi;
			cout << endl  << interiorDegrees << " <--";
			*/