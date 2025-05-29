#include <igl/opengl/glfw/Viewer.h>
#include <igl/triangle/triangulate.h>
#include <unordered_set>
#include "PointClass.h"
#include <igl/readOFF.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/read_triangle_mesh.h>
#include <igl/floor.h>
#include "triangleClass.h"
#include "OperatorClass.h"
#include <igl/find.h>
#include <vector>
#include <cmath>
#include <string>
#include <map>
#include "Reconstructor.h"
#include <cstdlib>

#define _USE_MATH_DEFINES
using namespace Eigen;
using namespace std;

vector<triangleClass> triangleListMain;
vector<PointClass> pointListMain;
vector<int> planeNums;
// Lower bound
int lb = 0;
// Upper bound
int ub = 100;

int main(int argc, char* argv[])
{
	OperatorClass test = OperatorClass();
	Reconstructor recon = Reconstructor();


	auto temp = test.getVectors(
		"C:/Uni Stuff/year3/3rd year project polyfit ver/ThirdYearDissertation/models/old_chair.obj",
		20);
	MatrixXi F = temp.F;
	MatrixXd V = temp.V;
	triangleListMain = temp.triangleList;
	pointListMain = temp.pointList;
	
	for (triangleClass x : triangleListMain) {
		int num = count(planeNums.begin(), planeNums.end(), x.planeNo);
		if (num <= 0) {
			planeNums.push_back(x.planeNo);
		}
	}

	MatrixXd C = RowVector3d(0.4, 0.8, 0.3).replicate(F.rows(), 1);

	for (int y : planeNums) {
		srand(y);
		double col1 = double(((rand() % (ub - lb + 1)) + lb)) / 100;
		double col2 = double(((rand() % (ub - lb + 1)) + lb)) / 100;
		double col3 = double(((rand() % (ub - lb + 1)) + lb)) / 100;
		for (triangleClass x : triangleListMain) {
			
			
			if (x.planeNo == y) {
				
				C.row(x.faceNum) = RowVector3d(col1,col2,col3);
			}
		}
	}

	auto out = recon.Reconstruct(temp, "C:/Uni Stuff/year3/3rd year project polyfit ver/ThirdYearDissertation/models/out.obj");


	cout << endl << "Started with " << V.rows() << " Vertices, ended with " << out.D.rows() << " Vertices" << endl;

	cout << endl << "Started with " << F.rows() << " Faces, ended with " << out.P.rows() << " Faces" << endl << endl << endl;

	igl::opengl::glfw::Viewer viewer;
	//viewer.data().set_mesh(out.D, out.P);
	viewer.data().set_mesh(V, F);
	//viewer.data().set_colors(C);
	viewer.launch();


}






			