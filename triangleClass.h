#include <vector>
#include <cmath>
#define _USE_MATH_DEFINES

using namespace std;



class triangleClass {

public:
    vector<vector<double>> points;
    vector<triangleClass> adjacenttriangles;
    vector<double> normal;
public:
    
    triangleClass(vector<vector<double>> x);
    //void addToAdjacentTriangles(triangleClass*);
    void addToAdjacentTriangles(triangleClass);
    void removeAdjacentTriangle(triangleClass triangle);
    vector<vector<double>> returnPoints();
    vector<double> returnNormal();
    int getNoOfConnectedTriangles();
    double calcInteriorAngle(triangleClass);
    vector<triangleClass> getConnectedTriangles();
    bool isTrianglePresent(triangleClass t);
    friend bool operator!=(triangleClass t1, triangleClass t2);
    friend bool operator==(triangleClass t1, triangleClass t2);
    string toString();
};