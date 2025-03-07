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

	igl::read_triangle_mesh("C:/Users/jaywh/source/repos/ThirdYearDissertation4/models"  "/planeTest2.obj", V, F);

	//Gets number of triangles from the faces matrix
	int numOfTrianlges = F.rows();

	//select random triangle
	//int random = rand() % numOfTrianlges;
	int random = 5;
	//assign points of a random traingle 

	//bug .data() is are not coordinates, its just some number, find way to extract coordinates from points, possibly store
	//using points[3][3] style list
	double *point0 = V.row(F.row(random)[0]).data();
	auto here = F;
	double *point1 = V.row(F.row(random)[1]).data();
	double *point2 = V.row(F.row(random)[2]).data();
	cout << "    " << here << "     " << endl;
	double* targetTri[3];

	//loop to select target triangle
	std::cout << "target Triangle" << endl;
	for (int i = 0; i < 3; i++) {
		targetTri[i] = V.row(F.row(random)[i]).data();
		std::cout << *targetTri[i] << endl;
	}

	
	//search through each triangle and find ones connected to the input
	for (int x = 0; x < F.rows();x++) {

		//skips currnnt loop iteration iftriangle is equal to input triangle
		if (F.row(x) == F.row(random)) {
			std::cout << "target Triangle Located" << endl;
			continue;
		}//asdasdsad

		//gets the verticies for the current triangle in loop
		double *currPoint0 = V.row(F.row(x)[0]).data();
		double *currPoint1 = V.row(F.row(x)[1]).data();

		double *currPoint2 = V.row(F.row(x)[2]).data();

		//initialises current points list
		double points[3];

		//prints all coords of current triangle
		std::cout << "========" << endl;
		std::cout << "triangle no." << x << endl;
		for (int i = 0; i < 3; i++) {
			
			std::cout << V(F(x, i),0) << " " << V(F(x, i), 1) << " " << V(F(x, i), 2) << endl;
		}
		std::cout << "========" << endl;


	}
	//Plot the mesh
	std::cout << endl << endl << endl;
	//open libigl viewer
	igl::opengl::glfw::Viewer viewer;
	viewer.data().set_mesh(V, F);
	viewer.launch();
}