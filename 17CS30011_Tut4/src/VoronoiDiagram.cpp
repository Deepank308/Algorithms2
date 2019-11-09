#include "VoronoiDiagram.hpp"

#define BREAKPOINTS_EPSILON 0.00001

struct Event
{
    
    enum { SITE = 0, CIRCLE = 1, SKIP = 2, };
    
    int type;
    Point2D point;
    
    // Site event attributes:
    int index;
    
    // Circle event attributes:
    Point2D center;
    BLNodePtr arc;
    
    Event(int _index = -1, int _type = Event::SKIP, const Point2D &_point = Point2D(0.0, 0.0)) :
    index(_index), type(_type), point(_point), arc(nullptr) {}
    
};

typedef shared_ptr<Event> EventPtr;

struct Point2DComparator
{
    bool operator()(const Point2D &p1, const Point2D &p2)
    {
        return (fabs(p1.y - p2.y) < POINT_EPSILON && p1.x > p2.x) || p1.y > p2.y;
    }
};

struct EventPtrComparator
{
    Point2DComparator point_cmp;
    bool operator()(const EventPtr &e1, const EventPtr &e2)
    {
        return point_cmp(e1->point, e2->point);
    }
};

EventPtr checkCircleEvent(BLNodePtr n1, BLNodePtr n2, BLNodePtr n3, const vector<Point2D> &points, double sweepline)
{
    
    if (n1 == nullptr || n2 == nullptr || n3 == nullptr)
        return nullptr;
    
    Point2D p1 = points[n1->get_id()];
    Point2D p2 = points[n2->get_id()];
    Point2D p3 = points[n3->get_id()];
    Point2D center, bottom;
    
    if (p2.y > p1.y && p2.y > p3.y)
        return nullptr;
    
    if (!findCircleCenter(p1, p2, p3, center))
        return nullptr;
    
    bottom = center;
    bottom.y += (center - p2).norm();
    
    // check circle event
    if (fabs(bottom.y - sweepline) < POINT_EPSILON || sweepline < bottom.y)
    {
        // create a circle event structure
        EventPtr e = make_shared<Event>(-1, Event::CIRCLE, bottom);
        // initialize attributes
        e->center = center;
        e->arc = n2;
        // add reference in the corresponding node
        n2->circle_event = e;
        return e;
    }
    
    return nullptr;
}


void build_voronoi(const vector<Point2D> &points, vector<HalfEdgePtr> &halfedges, vector<VertexPtr> &vertices, vector<HalfEdgePtr> &faces) {
    
    // create a priority queue
    priority_queue<EventPtr, vector<EventPtr>, EventPtrComparator> pq;
    
    // initialize it with all site events
    for (int i = 0; i < points.size(); ++i)
    {
        pq.push(make_shared<Event>(i, Event::SITE, points[i]));
    }
    
    // initialize vector of halfedges for faces
    faces.resize(points.size(), nullptr);
    
    // create a beachline tree
    BLNodePtr root;
    double sweepline = 0; // current position of the sweepline
    
    // process events
    while (!pq.empty())
    {
        // extract new event from the queue
        EventPtr e = pq.top(); pq.pop();
        
        // set position of a sweepline
        sweepline = e->point.y;
        
        if (e->type == Event::SITE)
        { 
            // handle site event
            int point_i = e->index;
            if (root == nullptr)
            {   
                // init empty beachline tree
                root = make_shared<BLNode>(make_pair(point_i, point_i), &sweepline, &points);
            }
            else
            { 
                BLNodePtr arc = find(root, e->point.x);
                BLNodePtr subtree, left_leaf, right_leaf;
                
                if (arc->circle_event != nullptr)
                {
                    EventPtr circle_e = arc->circle_event;
                    circle_e->type = Event::SKIP; // ignore corresponding event
                }
                
                // check number of intersection points
                int isp_num = intersectionPointsNum(points[arc->get_id()], e->point, sweepline);
                
                // different subtrees depending on the number of intersection points
                if (isp_num == 1)
                {
                    subtree = make_simple_subtree(point_i, arc->get_id(), &sweepline, &points, halfedges);
                    left_leaf = subtree->left;
                    right_leaf = subtree->right;
                }
                else if (isp_num == 2)
                {
                    subtree = make_subtree(point_i, arc->get_id(), &sweepline, &points, halfedges);
                    left_leaf = subtree->left;
                    right_leaf = subtree->right->right;
                }
                else
                    continue;
                
                if (arc->prev != nullptr)
                    connect(arc->prev, left_leaf);
                
                if (arc->next != nullptr)
                    connect(right_leaf, arc->next);
                
                // Replace old leaf with a subtree and rebalance it
                root = replace(arc, subtree);
                
                // Check circle events
                EventPtr circle_event = checkCircleEvent(left_leaf->prev, left_leaf, left_leaf->next, points, sweepline);
                if (circle_event != nullptr)
                {
                    pq.push(circle_event);
                }
                circle_event = checkCircleEvent(right_leaf->prev, right_leaf, right_leaf->next, points, sweepline);
                if (circle_event != nullptr)
                {
                    pq.push(circle_event);
                }
            }
            
        }
        else if (e->type == Event::CIRCLE)
        { // handle circle event
            
            BLNodePtr arc = e->arc, prev_leaf, next_leaf;

            // get breakpoint nodes
            pair<BLNodePtr, BLNodePtr> breakpoints = find_breakpoints(arc);
            
            // recheck if it's a false alarm 1
            if (breakpoints.first == nullptr || breakpoints.second == nullptr)
            {
                continue;
            }
            
            // recheck if it's a false alarm 2
            double v1 = breakpoints.first->value(), v2 = breakpoints.second->value();
            
            if (fabs(v1 - v2) > BREAKPOINTS_EPSILON)
            {
                continue;
            }
            
            // create a new vertex and insert into doubly-connected edge list
            VertexPtr vertex = make_shared<Vertex>(e->center);
            HalfEdgePtr h_first = breakpoints.first->edge;
            HalfEdgePtr h_second = breakpoints.second->edge;
            
            // store vertex of Voronoi diagram
            vertices.push_back(vertex);
            
            // remove circle event corresponding to next leaf
            if (arc->prev != nullptr && arc->prev->circle_event != nullptr)
            {
                EventPtr circle_e = arc->prev->circle_event;
                circle_e->type = Event::SKIP; // ignore corresponding event
            }
            
            // remove circle event corresponding to prev leaf
            if (arc->next != nullptr && arc->next->circle_event != nullptr)
            {
                EventPtr circle_e = arc->next->circle_event;
                circle_e->type = Event::SKIP; // ignore corresponding event
            }
            
            // store pointers to the next and previous leaves
            prev_leaf = arc->prev;
            next_leaf = arc->next;
            
            // get node associated with a new edge
            BLNodePtr new_edge_node;
            if (arc->parent == breakpoints.first)
                new_edge_node = breakpoints.second;
            else
                new_edge_node = breakpoints.first;
            
            // remove arc from the beachline
            root = remove(arc);
            
            // make a new pair of halfedges
            pair<HalfEdgePtr, HalfEdgePtr> twin_nodes = make_twins(prev_leaf->get_id(), next_leaf->get_id());
            new_edge_node->edge = twin_nodes.first;

            // connect halfedges
            connect_halfedges(h_second, h_first->twin);
            connect_halfedges(h_first, twin_nodes.first);
            connect_halfedges(twin_nodes.second, h_second->twin);
            
            h_first->vertex = vertex;
            h_second->vertex = vertex;
            twin_nodes.second->vertex = vertex;
            vertex->edge = h_second;
            
            halfedges.push_back(twin_nodes.first);
            halfedges.push_back(twin_nodes.second);
            
            // check new circle events
            if (prev_leaf != nullptr && next_leaf != nullptr)
            {
                EventPtr circle_event = checkCircleEvent(prev_leaf->prev, prev_leaf, next_leaf, points, sweepline);
                if (circle_event != nullptr)
                {
                    pq.push(circle_event);
                }
                circle_event = checkCircleEvent(prev_leaf, next_leaf, next_leaf->next, points, sweepline);
                if (circle_event != nullptr)
                {
                    pq.push(circle_event);
                }
            }
        }
    }
    
    // Fill edges corresponding to faces
    for (size_t i = 0; i < halfedges.size(); ++i)
    {
        HalfEdgePtr he = halfedges[i];
        if (he->prev == nullptr || faces[he->l_index] == nullptr)
        {
            faces[he->l_index] = he;
        }
    }
    
}
