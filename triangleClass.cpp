#include <vector>
#include <cmath>
#include <iostream>
#include "triangleClass.h"
#define _USE_MATH_DEFINES
using namespace std;
double pii = 3.14159265;


vector<vector<double>> points;
vector<double> normal;
int planeNo;
bool isPrimeTriangle;
int faceNum;


//triangles that are adjacent to the outermost layer of connected triangles
//but not apart of it
vector<triangleClass> borderTriangles;

triangleClass::triangleClass(vector<vector<double>> x, int faceNum) {
    planeNo = -1;
    points = x;
    this->faceNum = faceNum;
    vector<double> U = { points[0][0] - points[1][0]
                               , points[0][1] - points[1][1]
                               , points[0][2] - points[1][2] };

    vector<double> V = { points[0][0] - points[2][0]
                       , points[0][1] - points[2][1]
                       , points[0][2] - points[2][2] };

    normal = { 0,0,0 };

    normal[0] = (U[1] * V[2]) - (U[2] * V[1]);
    normal[1] = (U[2] * V[0]) - (U[0] * V[2]);
    normal[2] = (U[0] * V[1]) - (U[1] * V[0]);
    isPrimeTriangle = false;
}

triangleClass::triangleClass() {

}


vector<vector<double>> triangleClass::returnPoints() {
    return points;
}

vector<double> triangleClass::returnNormal() {
    return this->normal;
}


void triangleClass::makeTrianglePrime()
{
    this->isPrimeTriangle = true;
}


void triangleClass::updatePlaneNo(int x)
{
    this->planeNo = x;
}


double triangleClass::calcInteriorAngle(triangleClass triangle) {
    vector<double> normal2 = triangle.returnNormal();
    double numerator = abs((normal[0] * normal2[0]) + (normal[1] * normal2[1]) + (normal[2] * normal2[2]));
    double denominator = (sqrt(pow(normal[0], 2) + pow(normal[1], 2) + pow(normal[2], 2)) * sqrt(pow(normal2[0], 2) + pow(normal2[1], 2) + pow(normal2[2], 2)));
    double interiorAngle = acos(numerator / denominator);
    double interiorDegrees = interiorAngle * (180 / pii);
    return interiorDegrees;
}

bool operator!=(triangleClass t1, triangleClass t2)
{
    return t1.returnPoints() != t2.returnPoints();
}

bool operator==(triangleClass t1, triangleClass t2)
{
    return t1.returnPoints() == t2.returnPoints();
}

