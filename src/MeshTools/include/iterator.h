#pragma once
#include "surface.h"
// TODO: defines a copy assignment operator but does not define a destructor, a copy constructor, a move constructor or a move assignment operator 
  

namespace mesh_tools {
class HalfedgeIterator;

class FacetIterator {
public:
  HalfedgeIterator halfedge() const;

  explicit FacetIterator(Surface& surface, const unsigned int facet_no):
    surface_(surface),
    facet_no_(facet_no) {
  }

  bool IsValid() const {
    // TODO: Add a bool var to check if the element is moved or deleted.
    return facet_no_ < surface_.num_facets();
  }

  bool IsEnd() const {
    return facet_no_ == UINT_MAX;
  }

  FacetIterator& operator++() {
    facet_no_++;
    if (facet_no_ == surface_.num_facets())
      facet_no_ = UINT_MAX;
    return *this;
  }

  FacetIterator operator++(int) {
    FacetIterator tmp(*this);
    facet_no_++;
    if (facet_no_ == surface_.num_facets())
      facet_no_ = UINT_MAX;
    return tmp;
  }

  bool operator==(const FacetIterator& another_facet) const {
    return facet_no_ == another_facet.facet_no();
  }

  bool operator!=(const FacetIterator& another_facet) const {
    return !(*this == another_facet);
  }

  FacetIterator& operator=(const FacetIterator& another_facet) {
    assert((&surface_) == (&(another_facet.surface())));
    facet_no_ = another_facet.facet_no();
    return *this;
  }

  Surface& surface() const {
    return surface_;
  }

  unsigned int facet_no() const {
    return facet_no_;
  }

private:
  Surface& surface_;
  unsigned int facet_no_;
};

class VertexIterator {
public:
  HalfedgeIterator halfedge() const;
  Point3 position() const;

  explicit VertexIterator(Surface& surface, const unsigned int vertex_no):
    surface_(surface),
    vertex_no_(vertex_no) {
  }

  bool IsValid() const {
    // TODO: Add a bool var to check if the element is moved or deleted.
    return vertex_no_ < surface_.num_vertices();
  }

  bool IsEnd() const {
    return vertex_no_ == UINT_MAX;
  }

  VertexIterator& operator++() {
    vertex_no_++;
    if (vertex_no_ == surface_.num_vertices())
      vertex_no_ = UINT_MAX;
    return *this;
  }

  VertexIterator operator++(int) {
    VertexIterator tmp(*this);
    vertex_no_++;
    if (vertex_no_ == surface_.num_vertices())
      vertex_no_ = UINT_MAX;
    return tmp;
  }

  bool operator==(const VertexIterator& another_facet) const {
    return vertex_no_ == another_facet.vertex_no();
  }

  bool operator!=(const VertexIterator& another_facet) const {
    return !(*this == another_facet);
  }

  VertexIterator& operator=(const VertexIterator& another_facet) {
    assert((&surface_) == (&(another_facet.surface())));
    vertex_no_ = another_facet.vertex_no();
    return *this;
  }

  Surface& surface() const {
    return surface_;
  }

  unsigned int vertex_no() const {
    return vertex_no_;
  }

private:
  Surface& surface_;
  unsigned int vertex_no_;
};

class HalfedgeIterator {
public:
  HalfedgeIterator opposite() const;

  HalfedgeIterator prev() const;

  HalfedgeIterator next() const;

  VertexIterator vertex() const;

  FacetIterator facet() const;

  explicit HalfedgeIterator(Surface& surface, const unsigned int halfedge_no):
    surface_(surface),
    halfedge_no_(halfedge_no) {
  }

  bool IsValid() const {
    // TODO: Add a bool var to check if the element is moved or deleted.
    return halfedge_no_ < surface_.num_halfedges();
  }

  bool IsEnd() const {
    return halfedge_no_ == UINT_MAX;
  }

  HalfedgeIterator& operator++() {
    halfedge_no_++;
    if (halfedge_no_ == surface_.num_halfedges())
      halfedge_no_ = UINT_MAX;
    return *this;
  }

  HalfedgeIterator operator++(int) {
    HalfedgeIterator tmp(*this);
    halfedge_no_++;
    if (halfedge_no_ == surface_.num_halfedges())
      halfedge_no_ = UINT_MAX;
    return tmp;
  }

  bool operator==(const HalfedgeIterator& another_facet) const {
    return halfedge_no_ == another_facet.halfedge_no();
  }

  bool operator!=(const HalfedgeIterator& another_facet) const {
    return !(*this == another_facet);
  }

  HalfedgeIterator& operator=(const HalfedgeIterator& another_facet) {
    assert((&surface_) == (&(another_facet.surface())));
    halfedge_no_ = another_facet.halfedge_no();
    return *this;
  }

  Surface& surface() const {
    return surface_;
  }

  unsigned int halfedge_no() const {
    return halfedge_no_;
  }

private:
  Surface& surface_;
  unsigned int halfedge_no_;
};
}
