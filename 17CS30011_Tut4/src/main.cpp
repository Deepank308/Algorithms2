/*******************************************
* Deepank Agrawal
* 17CS30011
* Algorithms II 
* Assignment no. 4
/******************************************/

#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

#include "VoronoiDiagram.hpp"
#include "VisualizeSVG.hpp"

inline double random_number() 
{
    return double(rand()) / double(RAND_MAX);
}

inline bool compare(const Point2D &p1, const Point2D &p2)
{
    return (fabs(p1.y - p2.y) < POINT_EPSILON && p1.x < p2.x) || (fabs(p1.y - p2.y) >= POINT_EPSILON && p1.y < p2.y);
}

// Generate random points
vector<Point2D> random_point(const int &number, const double &seed) 
{
    srand(seed);

    vector<Point2D> points;

    for (int i = 0; i < number; i++) 
    {
        double x = random_number(), y = random_number();
        points.push_back(Point2D(0.5*WIDTH*x + WIDTH/4, 0.5*HEIGHT*y + HEIGHT/4));
    }

    sort(points.begin(), points.end(), compare);
    for (int i = 1; i < number; i++) 
    {
        if ((points[i-1] - points[i]).norm() < 5.0)
        {
            points[i-1].x = 0.5*WIDTH*random_number() + WIDTH/4;
            i--;
        }
    }

    return points;
}

// Get the end-points corresponding to any half-edge
inline void edge_points(HalfEdgePtr h, vector<double> &x, vector<double> &y, const vector<Point2D> &points) 
{
    if (h->vertex != nullptr && h->twin->vertex != nullptr)
    {
        x[0] = h->vertex->point.x;
        x[1] = h->twin->vertex->point.x;
        
        y[0] = h->vertex->point.y;
        y[1] = h->twin->vertex->point.y;
    }
    else if (h->vertex != nullptr)
    {
        x[0] = h->vertex->point.x;
        y[0] = h->vertex->point.y;
        
        Point2D norm = (points[h->l_index] - points[h->r_index]).normalized().getRotated90CCW();
        x[1] = x[0] + norm.x*100;
        y[1] = y[0] + norm.y*100;
    }
    else if (h->twin->vertex != nullptr)
    {
        x[0] = h->twin->vertex->point.x;
        y[0] = h->twin->vertex->point.y;
        
        Point2D norm = (points[h->twin->l_index] - points[h->twin->r_index]).normalized().getRotated90CCW();
        x[1] = x[0] + norm.x*1000;
        y[1] = y[0] + norm.y*1000;
    }
    else
    {
        Point2D p1 = points[h->l_index], p2 = points[h->r_index];
        
        Point2D norm = (p1 - p2).normalized().getRotated90CCW();
        Point2D c = 0.5*(p1 + p2);
        
        x[0] = c.x + norm.x*1000;
        x[1] = c.x - norm.x*1000;
        
        y[0] = c.y + norm.y*1000;
        y[1] = c.y - norm.y*1000;
    }
}

inline double euclidean_distance(const Point2D &p1, const Point2D &p2)
{
    Point2D delta = p1 - p2;
    return delta.norm2();
}

double perpendicular_distance(const Point2D &p, HalfEdgePtr h, const vector<Point2D> &points)
{
    vector<double> _x(2, 0.0), _y(2, 0.0);
    edge_points(h, _x, _y, points);

    Point2D p1 = Point2D(_x[0], _y[0]);
    Point2D p2 = Point2D(_x[1], _y[1]);

    double a = euclidean_distance(p1, p);
    double b = euclidean_distance(p, p2);
    double t = euclidean_distance(p1, p2);

    double dist2 = a - pow(t + a - b, 2) / (4*t); // perpendicular distance from any line

    return sqrt(dist2);
}

int main(int argc, const char *argv[])
{
    time_t tic, toc;
    const string filepath = "t4.svg";
    
    // User input
    int n;
    cout << "\nEnter the number of sites: ";
    cin >> n;

    double seed;
    cout << "Enter the random seed: ";
    cin >> seed;

    // Generate random points
    vector<Point2D> points = random_point(n, seed);
    
    vector<HalfEdgePtr> halfedges, faces;
    vector<VertexPtr> vertices;

    VisualizeSVG visual = VisualizeSVG(filepath);

    tic = clock();

    // Construct Voronoi diagram
    build_voronoi(points, halfedges, vertices, faces);
    
    toc = clock();

    // Draw the edges in the Voronoi Diagram
    for (int i = 0; i < halfedges.size(); i++)
    {
        HalfEdgePtr h = halfedges[i];
        
        vector<double> x(2, 0.0), y(2, 0.0);
        edge_points(h, x, y, points);
        visual.draw_line(x, y);
    }

    // Find out closest edge to each site
    vector<double> x(2, 0.0), y(2, 0.0);
    for(int i = 0; i < faces.size(); i++)
    {
        HalfEdgePtr h = faces[i], h_next = h->next;
        Point2D p = points[i];

        int flag = 0;
        if(h_next == nullptr)
        {
            flag = 1;
            h_next = h->prev;
        }

        double min_dist = perpendicular_distance(p, h, points);
        HalfEdgePtr h_min = h;

        // Traverse the edges CCW or CW, bounding a particular face
        while(h_next != h && h_next != nullptr)
        {
            double dist = perpendicular_distance(p, h_next, points);
            if(dist < min_dist)
            {
                min_dist = dist;
                h_min = h_next;
            }
            if(flag) h_next = h_next->prev;
            else h_next = h_next->next;

            if(flag == 0 && h_next == nullptr) h_next = h->prev, flag = 1;
        }
        // Draw the largest possible circle
        visual.draw_circle(points[i], min_dist);
    }

    // Draw the sites on Voronoi Diagram
    visual.draw_points(points);
    visual.end_file();

    cout << "Execution Time: " << (toc - tic - 0.0) / CLOCKS_PER_SEC << "secs\n";
    cout << "Output SVG file: " << visual.filepath << "\n";
    return 0;
}
