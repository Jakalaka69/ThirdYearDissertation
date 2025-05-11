#pragma once
#include <vector>
#include <string>
#include <compare>
#include <unordered_set>
#include "triangleClass.h"


using namespace std;
class Plane
{
private:
    triangleClass relatedTriangle;
    vector<vector<double>> Tpoints;
    vector<double> normal;
    vector<Plane*> connectedPlanes;
    
    int num;
   
    
public:
    triangleClass Plane::getRelatedTriangleClass();
    Plane();
    Plane(triangleClass inputTriangle, int x);
    void AddConnectedPlane(Plane* planeToAdd);
    vector<vector<double>> getRelatedTriangle();
    vector<double> GetNormal();
    vector<Plane*> GetConnectedPlanes();
    bool isPlaneConnected(Plane inputPlane);
    friend bool operator==(Plane plane1, Plane plane2);
    friend bool operator!=(Plane plane1, Plane plane2);
    string toString();
};
