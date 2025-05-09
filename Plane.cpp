
#include "Plane.h"
#include <compare>
#include <vector>
#include <String>
#include <unordered_set>
#include <iostream>
using namespace std;


const triangleClass baseTriangle;
vector<vector<double>> Tpoints;
const vector<double> normal;
vector<Plane*> connectedPlanes;

   

Plane::Plane(triangleClass inputTriangle) {
    baseTriangle = inputTriangle;
    Tpoints = inputTriangle.returnPoints();

    vector<double> P1 = { Tpoints[0][0], Tpoints[0][1], Tpoints[0][2] };
    vector<double> P2 = { Tpoints[1][0], Tpoints[1][1], Tpoints[1][2] };
    vector<double> P3 = { Tpoints[2][0], Tpoints[2][1], Tpoints[2][2] };

    normal = inputTriangle.returnNormal();
}

Plane::Plane() {

}

void Plane::AddConnectedPlane(Plane* planeToAdd) {
    connectedPlanes.push_back(planeToAdd);
}
    
vector<vector<double>> Plane::getPoints() {
    return Tpoints;
}

vector<double> Plane::GetNormal() {
    return normal;
}

vector<Plane*> Plane::GetConnectedPlanes() {
    return connectedPlanes;
}


bool Plane::isPlaneConnected(Plane inputPlane) {

    return true;
}


bool operator==(Plane plane1, Plane plane2)
{
    return plane1.getRelatedTriangle() == plane2.getRelatedTriangle();
}

bool operator!=(Plane plane1, Plane plane2)
{
    return plane1.getRelatedTriangle() == plane2.getRelatedTriangle();
}

string Plane::toString() {
    string String = "\nPlane normal: " + to_string(normal[0]) + "j + " + to_string(normal[1]) + "k + " + to_string(normal[2]) + "l " + "\n" + "List size: " + to_string(connectedPlanes.size() ) + "\n" + to_string(relatedTriangle[0][0]) + " " + to_string(relatedTriangle[0][1]) + " " +  to_string(relatedTriangle[0][2]) + "\n";

    
    return String;
}


