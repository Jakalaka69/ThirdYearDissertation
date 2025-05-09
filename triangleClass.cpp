#include <vector>
#include <cmath>
#include <iostream>
#include "triangleClass.h"
#define _USE_MATH_DEFINES
using namespace std;
double pii = 3.14159265;


    vector<vector<double>> points;  
    vector<triangleClass> adjacenttriangles;
    vector<double> normal;
    int planeNo;
    bool isPrimeTriangle;
    vector<int> connectedPlanes;


    //triangles that are adjacent to the outermost layer of connected triangles
    //but not apart of it
    vector<triangleClass> borderTriangles;

    triangleClass::triangleClass(vector<vector<double>> x) {
        planeNo = -1;
        points = x;
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

    //void triangleClass::addToAdjacentTriangles(triangleClass* triangle) {
    //    adjacenttriangles.push_back(triangle);
    //}

    //void triangleClass::addToAdjacentTriangles(triangleClass triangle){
    //    adjacenttriangles.push_back(triangle);
    //}

    //void triangleClass::addToBorderTriangles(triangleClass triangle) {
    //    borderTriangles.push_back(triangle);
    //}

    //void triangleClass::removeAdjacentTriangle(triangleClass triangle) {
    //    adjacenttriangles.erase(std::remove(adjacenttriangles.begin(), adjacenttriangles.end(), triangle), adjacenttriangles.end());

    //}

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

    void triangleClass::addToConnectedPlanes(int x)
    {

        //cout << "HERE" << endl;
        connectedPlanes.push_back(x);
    }

    void triangleClass::updatePlaneNo(int x)
    {
        this->planeNo = x;
    }

    //int triangleClass::getNoOfConnectedTriangles()
    //{
    //    return adjacenttriangles.size();
    //}

    //vector<triangleClass> triangleClass::getConnectedTriangles()
    //{
    //    return adjacenttriangles;
    //}

    //bool triangleClass::isTrianglePresent(triangleClass t)
    //{

    //    for (triangleClass queried : adjacenttriangles) {
    //        if (queried == t) {
    //            return true;
    //        }
    //        else {
    //            return false;
    //        }
    //    }

    //    return false;
    //}

    string triangleClass::toString() {
        string String = "\n Triangle Point: " + to_string(points[0][0]) + " " + to_string(points[0][1]) + " Normal: " + to_string(points[0][2]) + " "
            + to_string(normal[0]) + " " + to_string(normal[1]) + " " + to_string(normal[2]) + "\n";


        return String;
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

