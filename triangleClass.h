#pragma once
#include <vector>
#include <cmath>
#define _USE_MATH_DEFINES

using namespace std;



class triangleClass {

public:
    vector<vector<double>> points;
    vector<double> normal;
    int planeNo;
    bool isPrimeTriangle;
    int faceNum;
public:
    triangleClass();
    triangleClass(vector<vector<double>> x, int faceNum);
    vector<vector<double>> returnPoints();
    vector<double> returnNormal();
    void makeTrianglePrime();
    void updatePlaneNo(int x);
    double calcInteriorAngle(triangleClass);
    friend bool operator!=(triangleClass t1, triangleClass t2);
    friend bool operator==(triangleClass t1, triangleClass t2);

};
