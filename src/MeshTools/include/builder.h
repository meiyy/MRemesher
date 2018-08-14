#pragma once
#include "point3.h"
#include "surface.h"
#include "iterator.h"

namespace mesh_tools {
class Builder {
public:
  explicit Builder(Surface& surface)
    : surface_(surface) {
  }

  void AddVertex(const Point3& point) const {
    surface_.vertex_manager_.AddVertex(UINT_MAX, point);
  }

  void AddFacet(unsigned int point0, unsigned int point1,
                unsigned int point2) const {
    auto const verteices_no = surface_.num_vertices();
    auto const halfedge_no = surface_.num_halfedges();
    auto const facet_no = surface_.num_facets();

    assert(point0<verteices_no
      &&point1<verteices_no
      &&point2<verteices_no);

    surface_.facet_manager_.AddFacet(halfedge_no);

    surface_.halfedge_manager_.AddHalfedge(UINT_MAX, halfedge_no + 2,
                                           halfedge_no + 1,
                                           point0, facet_no);
    surface_.halfedge_manager_.AddHalfedge(UINT_MAX, halfedge_no,
                                           halfedge_no + 2,
                                           point1, facet_no);
    surface_.halfedge_manager_.AddHalfedge(UINT_MAX, halfedge_no + 1,
                                           halfedge_no,
                                           point2, facet_no);

    surface_.vertex_manager_.SetHalfedge(point0, halfedge_no);
    surface_.vertex_manager_.SetHalfedge(point1, halfedge_no + 1);
    surface_.vertex_manager_.SetHalfedge(point2, halfedge_no + 2);
  }

  void Build() const {
    std::vector<std::vector<unsigned int>>
      vertex_start_halfedges(surface_.num_vertices());
    std::vector<std::vector<unsigned int>>
      vertex_end_halfedges(surface_.num_vertices());
    std::vector<unsigned int> halfedge_start(surface_.num_halfedges());

    for (HalfedgeIterator i(surface_, 0); !i.IsEnd(); ++i) {
      halfedge_start[i.next().halfedge_no()] = i.vertex().vertex_no();
      vertex_end_halfedges[i.vertex().vertex_no()].
        emplace_back(i.halfedge_no());
      vertex_start_halfedges[i.vertex().vertex_no()].
        emplace_back(i.next().halfedge_no());
    }

    for (unsigned int i = 0; i < surface_.num_vertices(); i++) {
      for (auto j : vertex_start_halfedges[i]) {
        bool found = false;
        for (auto k : vertex_end_halfedges[i]) {
          HalfedgeIterator it0(surface_, j), it1(surface_, k);
          if (halfedge_start[k] == it0.vertex().vertex_no()) {
            //assert(!it0.opposite().IsValid() && !it1.opposite().IsValid());
            surface_.halfedge_manager_.SetOpposite(j, k);
            surface_.halfedge_manager_.SetOpposite(k, j);
            found = true;
            break;
          }
        }
        if (!found) {
          surface_.halfedge_manager_.AddHalfedge(j, INVALID_ID, INVALID_ID,
                                                 i, INVALID_ID);
          surface_.halfedge_manager_.SetOpposite(j, surface_.num_halfedges());
        }
      }
    }
  }

private:
  Surface& surface_;
};
}
