#pragma once
#include <climits>
#include "point3.h"

namespace mesh_tools
{
constexpr unsigned int INVALID_ID = UINT_MAX;
constexpr unsigned int FREE_MASK = 1u << ((sizeof(unsigned int)) * 8 - 1);
struct SurfaceElement
{
  unsigned int id;
  unsigned int next;
  unsigned int prev;
  SurfaceElement()
    : id(INVALID_ID), next(INVALID_ID), prev(INVALID_ID){
  }
};

/*
* FacetElement stores basic topo infomations of a face in halfedge-structure
*/
struct FacetElement 
{
  unsigned int halfedge;

  FacetElement()
    : halfedge(INVALID_ID) {
  }

  explicit FacetElement(const unsigned int halfedge)
    : halfedge(halfedge) {
  }
};

/*
* VertexElement stores basic topo infomations of a vertex in halfedge-structure
*/
struct VertexElement 
{
  unsigned int halfedge;
  Point3 position;

  VertexElement() :
    halfedge(INVALID_ID),
    position(Point3()) {
  }

  VertexElement(const unsigned int halfedge, Point3 position) :
    halfedge(halfedge),
    position(std::move(position)) {
  }
};

/*
* HalfedgeElement stores basic topo infomations of a halfedge in halfedge-structure
*/
struct HalfedgeElement 
{
  unsigned int opposite;
  unsigned int prev;
  unsigned int next;
  unsigned int vertex;
  unsigned int facet;

  HalfedgeElement() :
    opposite(INVALID_ID),
    prev(INVALID_ID),
    next(INVALID_ID),
    vertex(INVALID_ID),
    facet(INVALID_ID) {
  }

  HalfedgeElement(const unsigned int opposite, const unsigned int prev,
    const unsigned int next, const unsigned int vertex,
    const unsigned int facet) :
    opposite(opposite),
    prev(prev),
    next(next),
    vertex(vertex),
    facet(facet) {
  }
};
}
