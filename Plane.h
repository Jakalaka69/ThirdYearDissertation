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
    triangleClass baseTriangle;
    vector<vector<double>> Tpoints;
    vector<double> normal;
    vector<Plane*> connectedPlanes;
    
public:
    Plane(triangleClass inputTriangle);
    void AddConnectedPlane(Plane* planeToAdd);
    vector<vector<double>> getPoints();
    vector<double> GetNormal();
    vector<Plane*> GetConnectedPlanes();
    bool isPlaneConnected(Plane inputPlane);
    friend bool operator==(Plane plane1, Plane plane2);
    friend bool operator!=(Plane plane1, Plane plane2);
    string toString();
};
