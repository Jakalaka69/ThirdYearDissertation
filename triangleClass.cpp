#include <vector>
#include <cmath>
#include <iostream>
#include <C:\Uni Stuff\year3\3rd year project polyfit ver\ThirdYearDissertation\triangleClass.h>
#define _USE_MATH_DEFINES
using namespace std;
double pii = 3.14159265;


    vector<vector<double>> points;  
    vector<triangleClass> adjacenttriangles;
    vector<double> normal;

    triangleClass::triangleClass(vector<vector<double>> x) {
        points = x;
        vector<double> U = { points[0][0] - points[1][0]
                                   , points[0][1] - points[1][1]
                                   , points[0][2] - points[1][2] };

        vector<double> V = { points[0][0] - points[2][0]
                           , points[0][1] - points[2][1]
                           , points[0][2] - points[2][2] };

        vector<double> normal = { 0,0,0 };

        normal[0] = (U[1] * V[2]) - (U[2] * V[1]);
        normal[1] = (U[2] * V[0]) - (U[0] * V[2]);
        normal[2] = (U[0] * V[1]) - (U[1] * V[0]);
        //cout << normal[0] << endl;

    }

    void triangleClass::addToAdjacentTriangles(triangleClass triangle) {
                    adjacenttriangles.push_back(triangle);
    }

    vector<vector<double>> triangleClass::returnPoints() {
        return points;
    }

    vector<double> triangleClass::returnNormal() {
        return normal;
    }

    double triangleClass::calcInteriorAngle(triangleClass triangle) {
        vector<double> normal2 = triangle.returnNormal();
        normal2;

        double numerator = abs((normal[0] * normal2[0]) + (normal[1] * normal2[1]) + (normal[2] * normal2[2]));

        double denominator = (sqrt(pow(normal[0], 2) + pow(normal[1], 2) + pow(normal[2], 2)) * sqrt(pow(normal2[0], 2) + pow(normal2[1], 2) + pow(normal2[2], 2)));
        double interiorAngle = acos(numerator / denominator);
        double interiorDegrees = interiorAngle * (180 / pii);
        //cout << endl << interiorAngle * (180 / pi);

        return interiorDegrees;
    }

