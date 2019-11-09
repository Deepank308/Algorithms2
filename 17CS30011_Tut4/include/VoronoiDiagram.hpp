#ifndef _VoronoiDiagram_HPP_
#define _VoronoiDiagram_HPP_

#include <queue>

#include "Beachline.hpp"

void build_voronoi(const std::vector<Point2D> &points, std::vector<HalfEdgePtr> &halfedges, 
                    std::vector<VertexPtr> &vertices, std::vector<HalfEdgePtr> &faces);

#endif
