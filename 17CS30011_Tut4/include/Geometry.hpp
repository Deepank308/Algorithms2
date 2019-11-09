#ifndef _Geometry_HPP_
#define _Geometry_HPP_

#include "Point2D.hpp"

#define CIRCLE_CENTER_EPSILON 1.0e-7

bool findCircleCenter(const Point2D &p1, const Point2D &p2, const Point2D &p3, Point2D &center);

int intersectionPointsNum(const Point2D &f1, const Point2D &f2, double directrix);

vector<Point2D> findIntersectionPoints(const Point2D &f1, const Point2D &f2, double directrix);

#endif
