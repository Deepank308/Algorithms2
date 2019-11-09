#include "Point2D.hpp"

const double Point2D::Inf = numeric_limits<double>::infinity();

Point2D::Point2D(double _x, double _y) : x(_x), y(_y) {}

Point2D::Point2D(const Point2D &point) : x(point.x), y(point.y) {}

Point2D operator+(const Point2D &p1, const Point2D &p2)
{
    return Point2D(p1.x + p2.x, p1.y + p2.y);
}

Point2D operator-(const Point2D &p1, const Point2D &p2)
{
    return Point2D(p1.x - p2.x, p1.y - p2.y);
}

Point2D operator/(const Point2D &p1, const Point2D &p2)
{
    return Point2D(p1.x / p2.x, p1.y / p2.y);
}

Point2D operator*(const Point2D &p, double value)
{
    return Point2D(p.x*value, p.y*value);
}

Point2D operator*(double value, const Point2D &p)
{
    return Point2D(p.x*value, p.y*value);
}

Point2D operator/(const Point2D &p, double value)
{
    return Point2D(p.x / value, p.y / value);
}

Point2D operator-(const Point2D &p)
{
	return Point2D(-p.x, -p.y);
}

Point2D Point2D::normalized()
{
    return (*this) / this->norm();
}

double Point2D::norm()
{
    return sqrt(x*x + y*y);
}

double Point2D::norm2()
{
    return x*x + y*y;
}

Point2D Point2D::getRotated90CW()
{
    return Point2D(y, -x);
}

Point2D Point2D::getRotated90CCW()
{
    return Point2D(-y, x);
}

bool Point2D::isLeftTurn(const Point2D &p1, const Point2D &p2, const Point2D &p3)
{
	return ((p2 - p1).crossProduct(p3 - p2) > 0.0);
}

bool Point2D::isRightTurn(const Point2D &p1, const Point2D &p2, const Point2D &p3)
{
	return ((p2 - p1).crossProduct(p3 - p2) < 0.0);
}
