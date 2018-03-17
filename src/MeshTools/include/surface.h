#pragma once
#include <vector>
#include "facet.h"
#include "vertex.h"

namespace mesh_tools {
class Builder;

class Surface {
  friend class Builder;
public:
  const Facet& FacetAt(const unsigned int facet_no) const {
    return facets_[facet_no];
  }

  const Vertex& VertexAt(const unsigned int vertex_no) const {
    return vertices_[vertex_no];
  }

  size_t num_facets() const {
    return facets_.size();
  }

  size_t num_vertices() const {
    return vertices_.size();
  }

  const std::vector<Facet>& facets_array() const {
    return static_cast<const std::vector<Facet>&>(facets_);
  }

  const std::vector<Vertex>& vertices_array() const {
    return static_cast<const std::vector<Vertex>&>(vertices_);
  }

private:

  std::vector<Facet> facets_;
  std::vector<Vertex> vertices_;
};
}
