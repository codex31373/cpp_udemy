#pragma once

#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>

//------------ factory ---------

class Point {
    Point(const double& aX, const double& aY) : mX(aX), mY(aY) {};

    double mX;
    double mY;

public:
    void printCoordinates() {
        std::cout << "x: " << mX << " y: " << mY << std::endl;
    }

    //factory methods
    static Point newCartesian(const float& aX, const float& aY){
        return {aX, aY};
    }

    static Point newPolar(const double& aR, const double& aTheta){
        return {aR * cos(aTheta), aR * sin(aTheta)};
    }

    //can be moved here Pointfactory to be Inner factory method, it will have access to private member in that way
};

//concrete factory
struct PointFactory{
    static Point createCartesian(const double& aX, const double aY) {
        return Point::newCartesian(aX, aY); //ocp is preserved, constructor remains private
    }

    static Point createPolar(const double& aR, const double aTheta) {
        return Point::newPolar(aR, aTheta); //ocp is preserved, constructor remains private
    }
};

/* -- test task --
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int main()
{
    return 0;
}
*/

int main(const int argc,const char *argv[])
{
    auto p = Point::newPolar(3, M_PI_4);
    p.printCoordinates();

    auto p2 = PointFactory::createPolar(3, M_PI_4);
    p2.printCoordinates();

    std::cout << "6 July 2025\n";
    return 0;
}