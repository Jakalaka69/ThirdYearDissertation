#include <igl/readOFF.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/read_triangle_mesh.h>
#include <list>

using namespace std;

Eigen::MatrixXd V;
Eigen::MatrixXi F;

int main(int argc, char* argv[])
{
	// Load a mesh in OFF format

	igl::read_triangle_mesh("C:/Users/jaywh/source/repos/ThirdYearDissertation/Models" "/cylinder3.obj", V, F);

	//Gets number of triangles from the faces matrix
	int numOfTrianlges = F.rows();

	//select random triangle
	//int random = rand() % numOfTrianlges;
	int random = 315;
	//assign points of a random traingle
	auto point0 = V.row(F.row(random)[0]);
	auto point1 = V.row(F.row(random)[1]);
	auto point2 = V.row(F.row(random)[2]);


	
	//search through each triangle and find ones connected to the input
	for (int x = 0; x < F.rows();x++) {

		//skips currnnt loop iteration iftriangle is equal to input triangle
		if (F.row(x) == F.row(random)) {
			continue;
		}

		//gets the verticies for the current triangle in loop
		auto currPoint0 = V.row(F.row(x)[0]);
		auto currPoint1 = V.row(F.row(x)[1]);
		auto currPoint2 = V.row(F.row(x)[2]);


		//initialise how many verticies hit
		bool point0Hit = false;
		bool point1Hit = false;
		bool point2Hit = false;

		int count = 0;

		bool cuPo0 = false;
		bool cuPo1 = false;
		bool cuPo2 = false;
		//possibly come back to improve here with array based approch
		//checks if pairs of triangles have 2 matching points

		vector<Eigen::Block<Eigen::MatrixXd,1,-1,false>> origList = {point0,point1,point2};
		vector<Eigen::Block<Eigen::MatrixXd, 1, -1, false>> currentList = { currPoint0,currPoint1,currPoint2 };
		
		


		for(int p = 0; p < 3;p++){
			for (int y = 0; y < currentList.size()-1;y++) {
				if (origList[p] == currentList[y]) {
					point0Hit = true;
					currentList.erase(currentList.begin()+y);
					y--;
					count++;
				}
			}
		}
/*
		if (point0 == currPoint0  || point0 == currPoint1 || point0 == currPoint2) {
			count++;
			point0Hit = true;

		}
		if (point1 == currPoint0 || point1 == currPoint1 || point1 == currPoint2) {
			count++;
			point1Hit = true;
		}
		//if first two verticies dont match, skip the last comparison (saves time) and move onto the next triangle
		else if (count == 0) {
			continue;
		}
		//if first two verticies do match, skip the last comparison(saves time)
		if (count == 2) {}
		else if (point2 == currPoint0 || point2 == currPoint1 || point2 == currPoint2) {
			count++;
			point2Hit = true;
		}
		*/
		//if the triangles are touching, output the edge they touch on
		if (count == 2) {
			cout << "===";
			cout << currentList[0] << "    " << origList[0] << "    " << origList[1] << "    " << origList[2];
			cout << "===";



			
			
			
			cout << endl;
		}


	}
	//Plot the mesh
	cout << endl << endl << endl;
	//open libigl viewer
	igl::opengl::glfw::Viewer viewer;
	viewer.data().set_mesh(V, F);
	viewer.launch();
}