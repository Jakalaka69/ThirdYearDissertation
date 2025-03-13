#include <igl/readOFF.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/read_triangle_mesh.h>
#include <vector>

using namespace std;

Eigen::MatrixXd V;
Eigen::MatrixXi F;

vector<vector<vector<double>>> FindConnected(vector<vector<double>> startTriangle, vector<vector<vector<double>>> fullConnectedList) {
	//for every triangle in F
	for (int x = 0; x < F.rows();x++) {
		//if the triangle has not already been added to the fullConnectedList (stops repeats and looping)
		if (F(x, 0) == 0 && F(x, 1) == 0 && F(x, 2) == 0) {
			continue;
		}

		//initialise the current triangle in loop
		vector<vector<double>> nextTriangle = {};
		
		vector<vector<double>> touchingPoints = {};
		vector<vector<double>> allPoints = {};
		allPoints = startTriangle;

		for (int i = 0; i < 3; i++) {

			nextTriangle.push_back({ V(F(x, i), 0), V(F(x, i), 1), V(F(x, i), 2) });


			allPoints.push_back({ V(F(x, i), 0), V(F(x, i), 1), V(F(x, i), 2) });


			//std::cout << V(F(x, i), 0) << " " << V(F(x, i), 1) << " " << V(F(x, i), 2) << endl;
		}
		//check all points of the startTriangle that we pass in and the current triangle of the for loop and keep a count of the amount of corners that touch
		int count = 0;
		for (int j = 0;j < 3;j++) {
			for (int k = 0; k < 3; k++) {
				
				if (startTriangle[j] == nextTriangle[k]) {
					
					touchingPoints.push_back(startTriangle[j]);

					count++;
				}
				
			}
		}
		

		//below code sorts and removes duplicate points leaving 4 points in triangles that touch in 2 places
		//trying to come up with method of calculating angle between 2 traingles
		//may have to forgo the duplicate point removal

		//IDEA: look at unique points before removing the other points, that way we know these are the 2 peaks we have to calc the angles between

		/*sort(allPoints.begin(), allPoints.end());
		allPoints.erase(unique(allPoints.begin(), allPoints.end()), allPoints.end());*/

		//if the amount of corners that touch is equal to 2 then remove the current triangle of the for loop from F
		//and recurse, using the current triangle as the start triangle for the function
		if (count == 2) {
			//put angle checks here and only do the stuff below if they pass
			

			F(x, 0) = 0;
			F(x, 1) = 0;
			F(x, 2) = 0;
			fullConnectedList.push_back(nextTriangle);
			fullConnectedList = FindConnected(nextTriangle, fullConnectedList);
		}




	}
	return fullConnectedList;
}

int main(int argc, char* argv[])
{
	// Load a mesh in OFF format
	

	igl::read_triangle_mesh("C:/Uni Stuff/year3/3rd year project polyfit ver/ThirdYearDissertation/models"  "/planeTest2.obj", V, F);

	//Gets number of triangles from the faces matrix
	int numOfTrianlges = F.rows();

	//select random triangle
	//int random = rand() % numOfTrianlges;
	int random = 1;

	//assign point coordinates of a random traingle 
	vector<double> P1 = {V(F(random, 0), 0), V(F(random, 0), 1), V(F(random, 0), 2)};
	vector<double> P2 = { V(F(random, 1), 0), V(F(random, 1), 1), V(F(random, 1), 2) };
	vector<double> P3 = { V(F(random, 2), 0), V(F(random, 2), 1), V(F(random, 2), 2) };

	//initialise triangle with the points
	vector<vector<double>> randTriangle = { P1, P2, P3 };
	//remove randTriangle from F
	F(random) = 0;

	//list of connected faces to pass into FindConnected recursive function
	vector<vector<vector<double>>> fullConnectedList;

	//Call function, currently just returns all triangles in the model but once we 
	//include the angle checks it will then print the correct triangles to make a plane out of
	fullConnectedList = FindConnected(randTriangle,fullConnectedList);
	
	
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
