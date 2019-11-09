#include "Beachline.hpp"

BLNode::BLNode(const pair<int,int>& _indices, double* _sweepline, const vector<Point2D>* _points, BLNodePtr _left,
                BLNodePtr _right, BLNodePtr _parent, int _height) : indices(_indices), left(_left), right(_right),
                parent(_parent), height(_height), sweepline(_sweepline), points(_points), next(nullptr), prev(nullptr) {}


double BLNode::value()
{
    if (points == nullptr)
        return numeric_limits<double>::infinity();
    if (is_leaf())
    {
        return (*points)[indices.first].x;
    }
    else
    {
        Point2D p1 = (*points)[indices.first], p2 = (*points)[indices.second];
        
        vector<Point2D> ips = findIntersectionPoints(p1, p2, *sweepline);
        if (ips.size() == 2)
        {
            if (p1.y < p2.y)
            {
                return ips[0].x;
            }
            else
            {
                return ips[1].x;
            }
        }
        else
        {
            return ips[0].x;
        }
    }
}

void connect(BLNodePtr prev, BLNodePtr next)
{
    prev->next = next;
    next->prev = prev;
}

bool is_root(BLNodePtr node)
{
    return node->parent == nullptr;
}

int get_height(BLNodePtr node)
{
    if (node == nullptr) return 0;
    return node->height;
}

void update_height(BLNodePtr node)
{
    if (node == nullptr)
        return;
    node->height = max(get_height(node->left), get_height(node->right)) + 1;
}

int get_balance(BLNodePtr node)
{
    return get_height(node->left) - get_height(node->right);
}

BLNodePtr rotate_left(BLNodePtr node)
{
    
    if (node == nullptr)
        return nullptr;
    
    if (node->right == nullptr)
        return node;
    
    BLNodePtr rnode = node->right;
    
    if (!is_root(node))
    {
        if (node->parent->left == node)
        {
            node->parent->left = rnode;
        }
        else
        {
            node->parent->right = rnode;
        }
    }
    rnode->parent = node->parent;
    
    node->right = rnode->left;
    if (rnode->left != nullptr)
    {
        rnode->left->parent = node;
    }
    
    rnode->left = node;
    node->parent = rnode;
    
    update_height(node);
    update_height(rnode);
    update_height(rnode->parent);
    
    return rnode;
}

BLNodePtr rotate_right(BLNodePtr node)
{
    
    if (node == nullptr)
        return nullptr;
    
    if (node->left == nullptr)
        return node;
    
    BLNodePtr lnode = node->left;
    
    if (!is_root(node))
    {
        if (node->parent->left == node)
        {
            node->parent->left = lnode;
        }
        else
        {
            node->parent->right = lnode;
        }
    }
    lnode->parent = node->parent;
    
    node->left = lnode->right;
    if (lnode->right != nullptr)
    {
        lnode->right->parent = node;
    }
    
    lnode->right = node;
    node->parent = lnode;
    
    update_height(node);
    update_height(lnode);
    update_height(lnode->parent);
    
    return lnode;
}

BLNodePtr find(BLNodePtr root, double x)
{
    if (root == nullptr)
    {
        return nullptr;
    }
    BLNodePtr node = root;
    while (!node->is_leaf())
    {
        if (node->value() < x)
        {
            node = node->right;
        }
        else
        {
            node = node->left;
        }
    }
    return node;
}

BLNodePtr replace(BLNodePtr node, BLNodePtr new_node)
{
    
    if (node == nullptr)
    {
        return new_node;
    }
    
    double x = new_node->value();
    
    BLNodePtr parent_node = node->parent;
    
    new_node->parent = parent_node;
    if (parent_node != nullptr)
    {
        if (parent_node->value() < x)
        {
            parent_node->right = new_node;
        }
        else
        {
            parent_node->left = new_node;
        }
    }

    node = new_node;
    while (parent_node != nullptr)
    {
        update_height(parent_node);
        int balance = get_balance(parent_node);
        if (balance > 1)
        {
            if (parent_node->left != nullptr && !parent_node->left->is_leaf() && get_balance(parent_node->left) < 0)
            {
                parent_node->left = rotate_left(parent_node->left);
            }
            parent_node = rotate_right(parent_node);
        }
        else if (balance < -1)
        {
            if (parent_node->right != nullptr && !parent_node->right->is_leaf() && get_balance(parent_node->right) > 0)
            {
                parent_node->right = rotate_right(parent_node->right);
            }
            parent_node = rotate_left(parent_node);
        }
        
        node = parent_node;
        parent_node = parent_node->parent;
    }
    
    return node;
}

BLNodePtr remove(BLNodePtr leaf)
{
    
    if (leaf == nullptr)
        return nullptr;
    
    BLNodePtr parent = leaf->parent, grandparent = parent->parent;
    pair<int,int> bp1(leaf->prev->get_id(), leaf->get_id());
    pair<int,int> bp2(leaf->get_id(), leaf->next->get_id());
    pair<int,int> other_bp;
    
    if (parent->has_indices(bp1))
    {
        other_bp = bp2;
    }
    else if (parent->has_indices(bp2))
    {
        other_bp = bp1;
    }
    
    BLNodePtr other_subtree;
    if (parent->left == leaf)
        other_subtree = parent->right;
    else
        other_subtree = parent->left;
    
    other_subtree->parent = grandparent;
    if (grandparent->left == parent)
    {
        grandparent->left = other_subtree;
    }
    else
    {
        grandparent->right = other_subtree;
    }
    
    BLNodePtr new_root = grandparent;
    while (grandparent != nullptr)
    {
        if (grandparent->has_indices(other_bp))
            grandparent->indices = make_pair(leaf->prev->get_id(), leaf->next->get_id());

        update_height(grandparent);

        int balance = get_balance(grandparent);

        if (balance > 1)
        {
            if (grandparent->left != nullptr && !grandparent->left->is_leaf() && get_balance(grandparent->left) < 0)
            {
                grandparent->left = rotate_left(grandparent->left);
            }
            grandparent = rotate_right(grandparent);
        }
        else if (balance < -1)
        {
            if (grandparent->right != nullptr && !grandparent->right->is_leaf() && get_balance(grandparent->right) > 0)
            {
                grandparent->right = rotate_right(grandparent->right);
            }
            grandparent = rotate_left(grandparent);
        }
        
        new_root = grandparent;
        grandparent = grandparent->parent;
    }
    
    connect(leaf->prev, leaf->next);
    
    return new_root;
}

pair<BLNodePtr, BLNodePtr> find_breakpoints(BLNodePtr leaf)
{
    
    if (leaf == nullptr || leaf->next == nullptr || leaf->prev == nullptr)
        return make_pair<BLNodePtr>(nullptr, nullptr);
    
    BLNodePtr parent = leaf->parent, gparent = leaf->parent;
    pair<int,int> bp1(leaf->prev->get_id(), leaf->get_id());
    pair<int,int> bp2(leaf->get_id(), leaf->next->get_id());
    pair<int,int> other_bp;
    
    bool left_is_missing = true;
    
    if (parent->has_indices(bp1))
    {
        other_bp = bp2;
        left_is_missing = false;
    }
    else if (parent->has_indices(bp2))
    {
        other_bp = bp1;
        left_is_missing = true;
    }
    
    while (gparent != nullptr)
    {
        if (gparent->has_indices(other_bp))
        {
            break;
        }
        gparent = gparent->parent;
    }
    
    if (left_is_missing)
    {
        return make_pair(gparent, parent);
    }
    else
    {
        return make_pair(parent, gparent);
    }
    
}

BLNodePtr make_subtree(int index, int index_behind, double *sweepline, const vector<Point2D> *points, vector<HalfEdgePtr> &edges)
{
    
    BLNodePtr node1 = make_shared<BLNode>(make_pair(index_behind, index), sweepline, points);
    BLNodePtr node2 = make_shared<BLNode>(make_pair(index, index_behind), sweepline, points);
    
    BLNodePtr leaf1 = make_shared<BLNode>(make_pair(index_behind, index_behind), sweepline, points);
    BLNodePtr leaf2 = make_shared<BLNode>(make_pair(index, index), sweepline, points);
    BLNodePtr leaf3 = make_shared<BLNode>(make_pair(index_behind, index_behind), sweepline, points);
    
    node1->right = node2;
    node2->parent = node1;
    
    node1->left = leaf1;
    leaf1->parent = node1;
    
    node2->left = leaf2;
    leaf2->parent = node2;
    
    node2->right = leaf3;
    leaf3->parent = node2;
    
    pair<HalfEdgePtr, HalfEdgePtr> twin_edges = make_twins(index_behind, index);
    node1->edge = twin_edges.first;
    node2->edge = twin_edges.second;
    
    edges.push_back(twin_edges.first);
    edges.push_back(twin_edges.second);
    
    connect(leaf1, leaf2);
    connect(leaf2, leaf3);
    
    update_height(node2);
    update_height(node1);
    
    return node1;
}


BLNodePtr make_simple_subtree(int index, int index_behind, double *sweepline, const vector<Point2D> *points, vector<HalfEdgePtr> &edges)
{
    
    BLNodePtr node, leaf_l, leaf_r;
    
    pair<HalfEdgePtr, HalfEdgePtr> twin_edges = make_twins(index_behind, index);
    
    edges.push_back(twin_edges.first);
    edges.push_back(twin_edges.second);
    
    if ((*points)[index].x < (*points)[index_behind].x)
    {

        node = make_shared<BLNode>(make_pair(index, index_behind), sweepline, points);
        leaf_l = make_shared<BLNode>(make_pair(index, index), sweepline, points);
        leaf_r = make_shared<BLNode>(make_pair(index_behind, index_behind), sweepline, points);
        node->edge = twin_edges.second;
    }
    else
    {
        node = make_shared<BLNode>(make_pair(index_behind, index), sweepline, points);
        leaf_l = make_shared<BLNode>(make_pair(index_behind, index_behind), sweepline, points);
        leaf_r = make_shared<BLNode>(make_pair(index, index), sweepline, points);
        node->edge = twin_edges.first;
    }
    
    node->left = leaf_l;
    node->right = leaf_r;
    
    leaf_l->parent = node;
    leaf_r->parent = node;
    
    connect(leaf_l, leaf_r);
    update_height(node);
    
    return node;
}
