#ifndef _Point2D_HPP_
#define _Point2D_HPP_

#include <memory>
#include <iostream>
#include <vector>
#include <limits>
#include <cmath>

using namespace std;

#define POINT_EPSILON 0.000001f

class Point2D 
{

public:
    double x, y;
    
    const static double Inf;
    
    Point2D(double x = 0.0, double y = 0.0);
    Point2D(const Point2D &point);
    
    double dotProduct(const Point2D &p1)
    {
        return p1.x*this->x + p1.y*this->y;
    }

    double crossProduct(const Point2D &p1)
    {
        return this->x*p1.y - this->y*p1.x;
    }

    // to support binary operations for any two points
    friend Point2D operator+(const Point2D &p1, const Point2D &p2);
    friend Point2D operator-(const Point2D &p1, const Point2D &p2);
    friend Point2D operator/(const Point2D &p1, const Point2D &p2);
    friend Point2D operator*(const Point2D &p, double value);
    friend Point2D operator*(double value, const Point2D &p);
    friend Point2D operator/(const Point2D &p, double value);
	friend Point2D operator-(const Point2D &p);
    
    Point2D normalized();
    double norm();
    double norm2();
    
    Point2D getRotated90CW();
    Point2D getRotated90CCW();

	static bool isLeftTurn(const Point2D &p1, const Point2D &p2, const Point2D &p3);
	static bool isRightTurn(const Point2D &p1, const Point2D &p2, const Point2D &p3);
};


#endif
