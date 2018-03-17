#pragma once
#include "surface.h"

namespace mesh_tools {
class Surface;

class Builder {
public:
  explicit Builder(Surface& surface): surface_(surface) {
  }

  void AddPoint(const point3& point) const {
    Vertex tmp(point);
    surface_.vertices_.emplace_back(tmp);
  }

  void AddFacet(const unsigned int point0, const unsigned int point1,
                const unsigned int point2) const {
    Facet tmp;
    tmp.facet_vertex_[0] = point0;
    tmp.facet_vertex_[1] = point1;
    tmp.facet_vertex_[2] = point2;
    surface_.facets_.emplace_back(tmp);
  }

  void Build();

private:
  Surface& surface_;
};
}
