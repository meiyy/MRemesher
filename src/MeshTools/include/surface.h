#pragma once
#include "propertymanager.h"

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
    return static_cast<unsigned int>(facet_manager_.size());
  }

  unsigned int num_vertices() const {
    return static_cast<unsigned int>(vertex_manager_.size());
  }

  unsigned int num_halfedges() const {
    return static_cast<unsigned int>(halfedge_manager_.size());
  }

  auto& facet_manager() {
    return facet_manager_;
  }

  auto& vertex_manager() {
    return vertex_manager_;
  }

  auto& halfedge_manager() {
    return halfedge_manager_;
  }
private:
  FacetManager facet_manager_;
  VertexManager vertex_manager_;
  HalfedgeManager halfedge_manager_;
};
}
