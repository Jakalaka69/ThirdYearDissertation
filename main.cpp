#include <igl/readOFF.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/read_triangle_mesh.h>

using namespace std;

Eigen::MatrixXd V;
Eigen::MatrixXi F;

int main(int argc, char* argv[])
{
	// Load a mesh in OFF format

	igl::read_triangle_mesh("C:/Users/karen/source/repos/libigl/out/build/x64-Debug/_deps/libigl_tutorial_data-src" "/cylinder.obj", V, F);

	//Gets number of triangles from the faces matrix
	int numOfTrianlges = F.rows();

	int random = rand() % numOfTrianlges;

	auto point0 = V.row(F.row(random)[0]);
	auto point1 = V.row(F.row(random)[1]);
	auto point2 = V.row(F.row(random)[2]);


	for (int x = 0; x < F.rows();x++) {

		if (F.row(x) == F.row(random)) {
			continue;
		}

		auto currPoint0 = V.row(F.row(x)[0]);
		auto currPoint1 = V.row(F.row(x)[1]);
		auto currPoint2 = V.row(F.row(x)[2]);

		bool point0Hit = false;
		bool point1Hit = false;
		bool point2Hit = false;

		int count = 0;

		if (point0 == currPoint0 || point0 == currPoint1 || point0 == currPoint2) {
			count++;
			point0Hit = true;
		}
		if (point1 == currPoint0 || point1 == currPoint1 || point1 == currPoint2) {
			count++;
			point1Hit = true;
		}
		else if (count == 0) {
			continue;
		}
		if (point2 == currPoint0 || point2 == currPoint1 || point2 == currPoint2) {
			count++;
			point2Hit = true;
		}

		if (count == 2) {

			cout << "These triangles touch on the edge:" << endl;
			if (point0Hit) {
				cout << point0;
			}
			if (point1Hit) {
				cout << point1;
			}
			if (point2Hit) {
				cout << point2;
			}
			cout << endl;
		}


	}
	// Plot the mesh
	igl::opengl::glfw::Viewer viewer;
	viewer.data().set_mesh(V, F);
	viewer.launch();
}