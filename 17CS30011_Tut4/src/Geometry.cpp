#include "Geometry.hpp"

bool findCircleCenter(const Point2D &p1, const Point2D &p2, const Point2D &p3, Point2D &center)
{
    
    // get normalized vectors
    Point2D u1 = (p1 - p2).normalized(), u2 = (p3 - p2).normalized();
    
    double cross = u1.crossProduct(u2);
    
    // check if vectors are collinear
    if (fabs(cross) < CIRCLE_CENTER_EPSILON)
    {
        return false;
    }
    
    // get cental points
    Point2D pc1 = 0.5*(p1 + p2), pc2 = 0.5*(p2 + p3);
    
    // get free components
    double b1 = u1.dotProduct(pc1), b2 = u2.dotProduct(pc2);
    
    // calculate the center of a circle
    center.x = (b1*u2.y - b2*u1.y) / cross;
    center.y = (u1.x*b2 - u2.x*b1) / cross;
    
    return true;
}

int intersectionPointsNum(const Point2D &f1, const Point2D &f2, double directrix) {
    if (fabs(f1.x - f2.x) < POINT_EPSILON && fabs(f1.y - f2.y) < POINT_EPSILON) {
        return -1;
    }
    if (fabs(f1.y - f2.y) < POINT_EPSILON)
        return 1;
    return 2;
}

vector<Point2D> findIntersectionPoints(const Point2D &f1, const Point2D &f2, double d) {
    vector<Point2D> result;
    if (fabs(f1.x - f2.x) < POINT_EPSILON)
    {
        double y = 0.5*(f1.y + f2.y), D = sqrt(d*d - d*(f1.y + f2.y) + f1.y*f2.y);
        result.push_back(Point2D(f1.x - D, y));
        result.push_back(Point2D(f1.x + D, y));
    }
    else if (fabs(f1.y - f2.y) < POINT_EPSILON)
    { 
        double x = 0.5*(f1.x + f2.x);
        result.push_back(Point2D(x, 0.5*((x - f1.x)*(x - f1.x) + f1.y*f1.y  - d*d) / (f1.y - d)));
    }
    else
    {    
        double D = 2.0*sqrt(pow(f1.x - f2.x, 2)*(d - f1.y)*(d - f2.y)*(pow(f1.x - f2.x, 2) + pow(f1.y - f2.y, 2)));
        double T = -2.0*d*pow(f1.x - f2.x, 2) + (f1.y + f2.y)*(pow(f2.x - f1.x, 2) + pow(f2.y - f1.y, 2));
        double Q = 2.0*pow(f1.y - f2.y, 2);
        
        double y1 = (T - D) / Q, y2 = (T + D) / Q;
        double x1 = 0.5*(f1.x*f1.x - f2.x*f2.x + (2*y1 - f2.y - f1.y)*(f2.y - f1.y)) / (f1.x - f2.x);
        double x2 = 0.5*(f1.x*f1.x - f2.x*f2.x + (2*y2 - f2.y - f1.y)*(f2.y - f1.y)) / (f1.x - f2.x);
        
        if (x1 > x2) {
            swap(x1, x2);
            swap(y1, y2);
        }
        result.push_back(Point2D(x1, y1));
        result.push_back(Point2D(x2, y2));
    }
    return result;
}
