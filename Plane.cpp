
#include "Plane.h"
#include <compare>
#include <vector>
#include <String>
#include <unordered_set>
#include <iostream>
using namespace std;



const vector<vector<double>> relatedTriangle;
const vector<double> normal;
vector<Plane*> connectedPlanes;

   

Plane::Plane(vector<vector<double>> inputTriangle) {
    relatedTriangle = inputTriangle;

    vector<double> P1 = { relatedTriangle[0][0], relatedTriangle[0][1], relatedTriangle[0][2] };
    vector<double> P2 = { relatedTriangle[1][0], relatedTriangle[1][1], relatedTriangle[1][2] };
    vector<double> P3 = { relatedTriangle[2][0], relatedTriangle[2][1], relatedTriangle[2][2] };
    vector<double> V1 = { P1[0] - P2[0]
                        , P1[1] - P2[1]
                        , P1[2] - P2[2] };

    vector<double> V2 = { P3[0] - P2[0]
                        , P3[1] - P2[1]
                        , P3[2] - P2[2] };

    vector<double> Normal2 = { 0,0,0 };

    Normal2[0] = (V1[1] * V2[2]) - (V1[2] * V2[1]);
    Normal2[1] = (V1[2] * V2[0]) - (V1[0] * V2[2]);
    Normal2[2] = (V1[0] * V2[1]) - (V1[1] * V2[0]);

    normal = Normal2;
}

void Plane::AddConnectedPlane(Plane* planeToAdd) {
    connectedPlanes.push_back(planeToAdd);
}
    
vector<vector<double>> Plane::getRelatedTriangle() {
    return relatedTriangle;
}

vector<double> Plane::GetNormal() {
    return normal;
}

vector<Plane*> Plane::GetConnectedPlanes() {
    return connectedPlanes;
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


