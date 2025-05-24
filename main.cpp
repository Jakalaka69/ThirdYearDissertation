#include <igl/opengl/glfw/Viewer.h>
#include <igl/triangle/triangulate.h>
#include <unordered_set>
#include "PointClass.h"
#include <igl/readOFF.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/read_triangle_mesh.h>
#include "triangleClass.h"
#include "OperatorClass.h"
#include <vector>
#include <cmath>
#include <string>
#include <map> 
#define _USE_MATH_DEFINES
using namespace std;


int main(int argc, char* argv[])
{
	OperatorClass test = OperatorClass();


	auto fart = test.getVectors("C:/Uni Stuff/year3/3rd year project polyfit ver/ThirdYearDissertation/models/objBuilding.obj", 30);
	
	
	printf("hello");


}






			