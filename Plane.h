#pragma once
#include <vector>
#include <string>
#include <compare>
#include <unordered_set>



using namespace std;
class Plane
{
private:
    vector<vector<double>> relatedTriangle;
    vector<double> normal;
    vector<Plane*> connectedPlanes;
    
public:
    Plane(vector<vector<double>> inputTriangle);
    void AddConnectedPlane(Plane* planeToAdd);
    vector<vector<double>> getRelatedTriangle();
    vector<double> GetNormal();
    vector<Plane*> GetConnectedPlanes();
    friend bool operator==(Plane plane1, Plane plane2);
    friend bool operator!=(Plane plane1, Plane plane2);
    string toString();
};
