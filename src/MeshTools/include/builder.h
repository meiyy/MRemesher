#pragma once
#include "point3.h"
#include "surface.h"
#include <cassert>

namespace mesh_tools {
class Surface;

class Builder {
  friend class Surface;
public:
  explicit Builder(Surface& surface): surface_(surface) {
  }

  void AddPoint(const Point3& point) const {
    surface_.vertices_.AddVertex(UINT_MAX, point);
  }

  void AddFacet(const unsigned int point0, const unsigned int point1,
                const unsigned int point2) const {
    auto const verteices_no = surface_.num_vertices();
    auto const halfedge_no = surface_.num_halfedges();
    auto const facet_no = surface_.num_facets();

    assert(point0<verteices_no
      &&point1<verteices_no
      &&point2<verteices_no);

    surface_.facets_.AddFacet(halfedge_no);

    surface_.halfedges_.AddHalfedge(UINT_MAX, halfedge_no + 2, halfedge_no + 1,
                                    point0, facet_no);
    surface_.halfedges_.AddHalfedge(UINT_MAX, halfedge_no, halfedge_no + 2,
                                    point1, facet_no);
    surface_.halfedges_.AddHalfedge(UINT_MAX, halfedge_no + 1, halfedge_no,
                                    point2, facet_no);

    surface_.vertices_.SetHalfedge(point0, halfedge_no);
    surface_.vertices_.SetHalfedge(point1, halfedge_no + 1);
    surface_.vertices_.SetHalfedge(point2, halfedge_no + 2);
  }

  void Build() const;

private:
  Surface& surface_;
};
}
