#pragma once
#include "property.h"

namespace mesh_tools {
class Builder;
class Editor;
class FacetIterator;
class HalfedgeIterator;
class VertexIterator;

class Surface {
  friend class Builder;
  friend class Editor;
  friend class FacetIterator;
  friend class HalfedgeIterator;
  friend class VertexIterator;
public:

  unsigned int num_facets() const {
    return static_cast<unsigned int>(facets_.size());
  }

  unsigned int num_vertices() const {
    return static_cast<unsigned int>(vertices_.size());
  }

  unsigned int num_halfedges() const {
    return static_cast<unsigned int>(halfedges_.size());
  }

private:
  FacetStore facets_;
  VertexStore vertices_;
  HalfedgeStore halfedges_;
};
}
