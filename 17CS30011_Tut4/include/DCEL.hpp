#ifndef _DCEL_HPP_
#define _DCEL_HPP_

#include "Point2D.hpp"

class Vertex;
class HalfEdge;

typedef std::shared_ptr<HalfEdge> HalfEdgePtr;
typedef std::shared_ptr<Vertex> VertexPtr;

class Vertex 
{
    public:
        
        Point2D point;
        HalfEdgePtr edge;
        
        Vertex(const Point2D &pos, HalfEdgePtr incident_edge = nullptr);
        
        double x() { return point.x; }
        double y() { return point.y; }
};

class HalfEdge 
{
    public:
        
        int l_index, r_index;
        
        VertexPtr vertex;
        HalfEdgePtr twin;
        HalfEdgePtr next;
        HalfEdgePtr prev;
        
        HalfEdge(int _l_index, int _r_index, VertexPtr _vertex = nullptr);
        
        VertexPtr vertex0() { return vertex; }
        VertexPtr vertex1() { return twin->vertex; }
        
        inline bool is_finite()
        {
            return vertex != nullptr && twin->vertex != nullptr;
        }
        
        HalfEdgePtr vertexNextCCW();
        HalfEdgePtr vertexNextCW();
};


pair<HalfEdgePtr, HalfEdgePtr> make_twins(int left_index, int right_index);

pair<HalfEdgePtr, HalfEdgePtr> make_twins(const std::pair<int,int> &indices);

void connect_halfedges(HalfEdgePtr p1, HalfEdgePtr p2);

#endif
