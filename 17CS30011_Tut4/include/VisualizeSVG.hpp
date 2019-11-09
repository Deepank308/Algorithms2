#ifndef _VisualizeSVG_HPP_
#define _VisualizeSVG_HPP_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include<Point2D.hpp>

using namespace std;

#define RADIUS 0.5
#define WIDTH 1000
#define HEIGHT 1000
#define MARGIN 20

class VisualizeSVG
{
    static int i;

public:
    string filepath;
    VisualizeSVG(const string &filepath);
    VisualizeSVG();

    void draw_points(const vector<Point2D> &p);

    void draw_circle(const Point2D &c, const double &radius);

    void draw_line(const vector<double> &x, const vector<double> &y);

    void end_file();
};

#endif
