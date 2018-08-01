#pragma once
#include "point3.h"
#include "surface.h"
#include "iterator.h"

namespace mesh_tools {
class Surface;

class Editor {
  friend class Surface;
public:
  explicit Editor(Surface& surface): surface_(surface) {
  }

  void AddVertex(const Point3& point) const {
    surface_.vertex_manager_.AddVertex(UINT_MAX, point);
  }

  void AddFacet(unsigned int vertex0, unsigned int vertex1,
    unsigned int vertex2) const {
    auto const verteices_no = surface_.num_vertices();
    auto const halfedge_no = surface_.num_halfedges();
    auto const facet_no = surface_.num_facets();

    assert(vertex0<verteices_no
      &&vertex1<verteices_no
      &&vertex2<verteices_no);

    surface_.facet_manager_.AddFacet(halfedge_no);

    surface_.halfedge_manager_.AddHalfedge(UINT_MAX, halfedge_no + 2, halfedge_no + 1,
      vertex0, facet_no);
    surface_.halfedge_manager_.AddHalfedge(UINT_MAX, halfedge_no, halfedge_no + 2,
      vertex1, facet_no);
    surface_.halfedge_manager_.AddHalfedge(UINT_MAX, halfedge_no + 1, halfedge_no,
      vertex2, facet_no);

    surface_.vertex_manager_.SetHalfedge(vertex0, halfedge_no);
    surface_.vertex_manager_.SetHalfedge(vertex1, halfedge_no + 1);
    surface_.vertex_manager_.SetHalfedge(vertex2, halfedge_no + 2);
  }

  void RemoveFacet(unsigned int facet_no)const {
    assert(facet_no<surface_.num_facets());
    FacetIterator facet(surface_, facet_no);
    unsigned int halfedge[3] = { facet.halfedge().halfedge_no(),facet.halfedge().next().halfedge_no(),facet.halfedge().prev().halfedge_no() };

    std::vector<unsigned int> vertex_to_remove;
    for (auto i : halfedge)
    {
      HalfedgeIterator hit(surface_, i);
      if (!hit.opposite().IsValid() && !hit.prev_around_vertex().IsValid())
      {
        vertex_to_remove.emplace_back(hit.vertex().vertex_no());
      }
      else if (hit == hit.vertex().halfedge())
      {
        if (hit.opposite().IsValid())
        {
          surface_.vertex_manager_.SetHalfedge(hit.vertex().vertex_no(), hit.next_around_vertex().halfedge_no());
        }
        else
        {
          surface_.vertex_manager_.SetHalfedge(hit.vertex().vertex_no(), hit.prev_around_vertex().halfedge_no());
        }
      }
    }


    for (auto i : halfedge)
    {
      HalfedgeIterator hit(surface_, i);
      auto right_boarder = hit;
      do
      {
        if (right_boarder.opposite().IsValid())
          right_boarder = right_boarder.next_around_vertex();
      } while (right_boarder != hit);
      if (hit.opposite().IsValid())
      {
        surface_.halfedge_manager_.SetOpposite(hit.opposite().halfedge_no(), UINT_MAX);
      }
      else
      {

      }
    }
    for (auto i : halfedge)
    {
      surface_.halfedge_manager_.RemoveElement(i);
    }
    for (auto i : vertex_to_remove)
    {
      surface_.vertex_manager_.RemoveElement(i);
    }
    surface_.facet_manager_.RemoveElement(facet_no);
  }

  void RemoveVertex(unsigned int vertex_no)const {
    assert(vertex_no<surface_.num_vertices());
    auto vertex_to_remove = VertexIterator(surface_, vertex_no);
    std::vector<unsigned int> facets_around;
    auto half_edge = vertex_to_remove.halfedge();
    do
    {
      facets_around.push_back(half_edge.halfedge_no());
      half_edge = half_edge.prev_around_vertex();
    } while (half_edge != vertex_to_remove.halfedge() && half_edge.IsValid());
    if (!half_edge.IsValid())
    {
      half_edge = vertex_to_remove.halfedge();
      while (half_edge.opposite().IsValid())
      {
        half_edge = half_edge.next_around_vertex();
        facets_around.push_back(half_edge.halfedge_no());
      }
    }
    for (auto i : facets_around)
    {
      RemoveFacet(i);
    }
  }

private:
  Surface& surface_;
};
}
