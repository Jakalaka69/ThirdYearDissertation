
#include "Plane.h"
#include <compare>
#include <vector>
#include <String>
#include <unordered_set>
#include <iostream>
using namespace std;


const triangleClass relatedTriangle;
vector<vector<double>> Tpoints;
const vector<double> normal;
vector<Plane*> connectedPlanes;
int num;
   

Plane::Plane(triangleClass inputTriangle, int x) {
    num = x;
    relatedTriangle = inputTriangle;
    Tpoints = inputTriangle.returnPoints();
    normal = inputTriangle.returnNormal();
}



Plane::Plane() {

}

void Plane::AddConnectedPlane(Plane* planeToAdd) {
    connectedPlanes.push_back(planeToAdd);
}
    
vector<vector<double>> Plane::getRelatedTriangle() {
    return Tpoints;
}

triangleClass Plane::getRelatedTriangleClass() {
    return relatedTriangle;
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
    //string String = "\nPlane normal: " + to_string(normal[0]) + "j + " + to_string(normal[1]) + "k + " + to_string(normal[2]) + "l " + "\n" + "List size: " + to_string(connectedPlanes.size() ) + "\n" + to_string(relatedTriangle.points[0][0]) + " " + to_string(relatedTriangle.points[0][1]) + " " +  to_string(relatedTriangle.points[0][2]) + "\n" + "Plane Number:" + to_string(num) + "\n";
    string String = "\n PlaneNumber:" + to_string(num) + "\n";

    
    return String;
}


