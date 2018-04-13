#include "iterator.h"

namespace mesh_tools {
HalfedgeIterator FacetIterator::halfedge() const {
  return HalfedgeIterator(surface_, surface_.facets_[facet_no_].halfedge);
}

HalfedgeIterator VertexIterator::halfedge() const {
  return HalfedgeIterator(surface_, surface_.vertices_[vertex_no_].halfedge);
}

Point3 VertexIterator::position() const {
  return surface_.vertices_[vertex_no_].position;
}

HalfedgeIterator HalfedgeIterator::opposite() const {
  return HalfedgeIterator(surface_, surface_.halfedges_[halfedge_no_].opposite);
}

HalfedgeIterator HalfedgeIterator::prev() const {
  return HalfedgeIterator(surface_, surface_.halfedges_[halfedge_no_].prev);
}

HalfedgeIterator HalfedgeIterator::next() const {
  return HalfedgeIterator(surface_, surface_.halfedges_[halfedge_no_].next);
}

HalfedgeIterator HalfedgeIterator::next_around_vertex() const
{
  return opposite().prev();
}

HalfedgeIterator HalfedgeIterator::prev_around_vertex() const
{
  return next().opposite();
}

VertexIterator HalfedgeIterator::vertex() const {
  return VertexIterator(surface_, surface_.halfedges_[halfedge_no_].vertex);
}

FacetIterator HalfedgeIterator::facet() const {
  return FacetIterator(surface_, surface_.halfedges_[halfedge_no_].facet);
}

}
