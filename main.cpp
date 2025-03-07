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

	igl::read_triangle_mesh("C:/Uni Stuff/year3/3rd year project polyfit ver/ThirdYearDissertation/models" "/planeTest.obj", V, F);

	//Gets number of triangles from the faces matrix
	int numOfTrianlges = F.rows();

	//select random triangle
	//int random = rand() % numOfTrianlges;
	int random = 0;
	//assign points of a random traingle
	double *point0 = V.row(F.row(random)[0]).data();
	double *point1 = V.row(F.row(random)[1]).data();
	double *point2 = V.row(F.row(random)[2]).data();


	
	//search through each triangle and find ones connected to the input
	for (int x = 0; x < F.rows();x++) {

		//skips currnnt loop iteration iftriangle is equal to input triangle
		if (F.row(x) == F.row(random)) {
			continue;
		}

		//gets the verticies for the current triangle in loop
		double *currPoint0 = V.row(F.row(x)[0]).data();
	//	V.row(F.row(x)[0]).setZero();
		double *currPoint1 = V.row(F.row(x)[1]).data();
	//	V.row(F.row(x)[1]).setZero();
		double *currPoint2 = V.row(F.row(x)[2]).data();
	//	V.row(F.row(x)[2]).setZero();

		double* points[3];

		cout << "========" << endl;
		cout << "triangle no." << x << endl;
		for (int i = 0; i < 3; i++) {
			points[i] = V.row(F.row(x)[i]).data();
			cout << *points[i] << endl;
		}

		cout << "========" << endl;


	}
	//Plot the mesh
	cout << endl << endl << endl;
	//open libigl viewer
	igl::opengl::glfw::Viewer viewer;
	viewer.data().set_mesh(V, F);
	viewer.launch();
}