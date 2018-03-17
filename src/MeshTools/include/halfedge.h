#pragma once

namespace mesh_tools
{
  class Vertex;
  class Facet;
  class Halfedge
  {
  public:
    Halfedge* opposite() const
    {
      return opposite_;
    }
    Vertex* vertex() const
    {
      return vertex_;
    }
    Facet* facet() const
    {
      return facet_;
    }
    Halfedge* next() const
    {
      return next_;
    }
    Halfedge* prev() const
    {
      return prev_;
    }
    Halfedge* next_around_vertex() const
    {
      return opposite_->prev();
    }
    Halfedge* prev_around_vertex() const
    {
      return next_->opposite();
    }

  private:
    Halfedge()=default;

    Halfedge* opposite_;
    Vertex* vertex_;
    Facet* facet_;
    Halfedge* next_;
    Halfedge* prev_;
  };
}
