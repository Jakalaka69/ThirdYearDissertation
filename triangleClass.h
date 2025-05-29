#pragma once
#include <vector>
#include <cmath>
#define _USE_MATH_DEFINES

using namespace std;



class triangleClass {

public:
    vector<vector<double>> points;
    //vector<triangleClass> 
    // 
    // triangles;
    vector<double> normal;
    //vector<triangleClass> borderTriangles;
    int planeNo;
    bool isPrimeTriangle;
    vector<int> connectedPlanes;
    vector<int> countNum;
    int faceNum;
public:
    triangleClass();
    triangleClass(vector<vector<double>> x, int faceNum);
    //void addToAdjacentTriangles(triangleClass*);
    //void 
    // 
    // ToAdjacentTriangles(triangleClass);
    //void addToBorderTriangles(triangleClass);
    //void removeAdjacentTriangle(triangleClass triangle);
    vector<vector<double>> returnPoints();
    vector<double> returnNormal();
    void makeTrianglePrime();
    void addToConnectedPlanes(int x);
    void updatePlaneNo(int x);
    //int getNoOfConnectedTriangles();
    double calcInteriorAngle(triangleClass);
    void setCount(int x);
    friend bool operator!=(triangleClass t1, triangleClass t2);
    friend bool operator==(triangleClass t1, triangleClass t2);

};
