#include "builder.h"
#include "iterator.h"

namespace mesh_tools {
void Builder::Build() const {
  std::vector<std::vector<unsigned int>>
    vertex_start_halfedges(surface_.num_vertices());
  std::vector<std::vector<unsigned int>>
    vertex_end_halfedges(surface_.num_vertices());
  std::vector<unsigned int> halfedge_start(surface_.num_halfedges());

  for (HalfedgeIterator i(surface_, 0); i.IsEnd(); ++i) {
    halfedge_start[i.next().halfedge_no()] = i.vertex().vertex_no();
    vertex_end_halfedges[i.vertex().vertex_no()].emplace_back(i.halfedge_no());
    vertex_start_halfedges[i.vertex().vertex_no()].
      emplace_back(i.next().halfedge_no());
  }

  for (unsigned int i = 0; i < surface_.num_vertices(); i++) {
    for (auto j : vertex_start_halfedges[i]) {
      for (auto k : vertex_end_halfedges[i]) {
        HalfedgeIterator it0(surface_, j), it1(surface_, k);
        if (halfedge_start[k] == it0.vertex().vertex_no()) {
          assert(!it0.opposite().IsValid() && !it1.opposite().IsValid());
          surface_.halfedges_.SetOpposite(j, k);
          surface_.halfedges_.SetOpposite(k, j);
        }
      }
    }
  }
}
}
