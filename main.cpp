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
#include "RegionGen.h"
#include "Reconstructor.h"
using namespace std;
using namespace Eigen;
vector<int> planeNums;

int lb = 0;

int ub = 100;

int main(int argc, char* argv[])
{
	string outPath = "";
	string inPath = "";
	double degrees;
	cout << "Please enter the path of the object you wish to simplify: ";
	cin >> inPath;
	cout << "Please enter the path of where the output should go: ";
	cin >> outPath;
	cout << "Please enter the angle threshold(0-80): ";
	cin >> degrees;

	bool break1 = false;
	std::ifstream test(inPath);
	if (!test)
	{
		std::cout << "That input file doesnt exist" << std::endl;
		break1 = true;
	}
	std::ifstream test1(outPath);
	if (!test1)
	{
		std::cout << "That output file doesnt exist" << std::endl;
		break1 = true;
	}
	if (degrees < 0 || degrees > 80) {
		cout << "Invalid angle threshold" << endl;
		break1 = true;
	}

	if (break1) {
		cout << "Invalid inputs detected, please try again";
		exit(0);
	}

	try {
		OperatorClass RegionGenerator = OperatorClass();
		auto returnValues = RegionGenerator.getVectors(inPath, degrees);

		for (triangleClass x : returnValues.triangleList) {
			int num = count(planeNums.begin(), planeNums.end(), x.planeNo);
			if (num <= 0) {
				planeNums.push_back(x.planeNo);
			}
		}

		MatrixXi F = returnValues.F1;
		MatrixXd V = returnValues.V1;


		MatrixXd C = RowVector3d(0.4, 0.8, 0.3).replicate(F.rows(), 1);

		for (int y : planeNums) {
		//cout << "colouring faces " << y << " out of " << planeNums.size() << endl;
		srand(y);
		double col1 = double(((rand() % (ub - lb + 1)) + lb)) / 100;
		double col2 = double(((rand() % (ub - lb + 1)) + lb)) / 100;
		double col3 = double(((rand() % (ub - lb + 1)) + lb)) / 100;
		for (triangleClass x : returnValues.triangleList) {
			
			
			if (x.planeNo == y) {
				
				C.row(x.faceNum) = RowVector3d(col1,col2,col3);
			}
		}
	}	
		//igl::opengl::glfw::Viewer viewer;
		//viewer.data().set_mesh(V, F);
		//viewer.data().set_colors(C);
		//viewer.launch();

		
		Reconstructor reconstructor = Reconstructor();

		reconstructor.Reconstruct(returnValues, outPath);
	}
	catch(...){
		cout << "An error occurred, please check the validity of the input paths and try again";
	}
	



}






			