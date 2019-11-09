#ifndef _Beachline_HPP_
#define _Beachline_HPP_

#include <time.h>
#include <iostream>
#include <limits>
#include <iomanip>
#include <vector>
#include <random>

#include "Geometry.hpp"
#include "DCEL.hpp"

class Event;

class BLNode;
typedef shared_ptr<BLNode> BLNodePtr;

class BLNode {
    public:
        
        int height;
        
        double *sweepline;
        
        const vector<Point2D> *points;
        
        pair<int, int> indices;
        
        BLNodePtr left, right, parent;
        
        shared_ptr<Event> circle_event;
        shared_ptr<HalfEdge> edge;
        
        BLNode(const pair<int,int>& _indices,
                double* _sweepline = nullptr,
                const vector<Point2D>* _points = nullptr,
                BLNodePtr _left = nullptr,
                BLNodePtr _right = nullptr,
                BLNodePtr _parent = nullptr,
                int _height = 1);
        
        BLNodePtr next, prev;
        
        inline bool is_leaf() {
            return indices.first == indices.second;
        }
        
        inline int get_id() {
            return indices.first;
        }
        
        inline bool has_indices(int a, int b) {
            return indices.first == a && indices.second == b;
        }
        
        inline bool has_indices(const pair<int,int> &p) {
            return indices.first == p.first && indices.second == p.second;
        }
        
        double value();
    
};

void connect(BLNodePtr prev, BLNodePtr next);

bool is_root(BLNodePtr node);

int get_height(BLNodePtr node);

void update_height(BLNodePtr node);

int get_balance(BLNodePtr node);

BLNodePtr rotate_left(BLNodePtr node);

BLNodePtr rotate_right(BLNodePtr node);

BLNodePtr find(BLNodePtr root, double x);

BLNodePtr replace(BLNodePtr node, BLNodePtr new_node);

BLNodePtr remove(BLNodePtr leaf);;

pair<BLNodePtr, BLNodePtr> find_breakpoints(BLNodePtr leaf);

BLNodePtr make_subtree(int index, int index_behind, double *sweepline, const vector<Point2D> *points, vector<HalfEdgePtr> &edges);

BLNodePtr make_simple_subtree(int index, int index_behind, double *sweepline, const vector<Point2D> *points, vector<HalfEdgePtr> &edges);

#endif
