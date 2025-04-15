#include <vector>
#include <cmath>
#define _USE_MATH_DEFINES

using namespace std;



class triangleClass {
public:
    //vector<vector<double>> points;  
    //vector<triangleClass> adjacenttriangles;
    //vector<double> normal;
    triangleClass(vector<vector<double>> x);
    void addToAdjacentTriangles(triangleClass);
    vector<vector<double>> returnPoints();
    vector<double> returnNormal();
    double calcInteriorAngle(triangleClass);
};