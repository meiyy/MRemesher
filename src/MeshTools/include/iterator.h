#pragma once
#include "surface.h"

namespace mesh_tools {
class HalfedgeIterator;

class FacetIterator {
public:
  HalfedgeIterator halfedge() const;

  explicit FacetIterator(Surface& surface, const unsigned int facet_no):
    surface_(surface),
    facet_no_(facet_no),
    unique_id_(facet_no_ != INVALID_ID ? surface_.facet_manager_.UniqueID(facet_no_) : INVALID_ID){
  }

  FacetIterator(const FacetIterator& facet_iterator):
    surface_(facet_iterator.surface()),
    facet_no_(facet_iterator.facet_no()),
    unique_id_(facet_iterator.unique_id()){
  }

  FacetIterator(FacetIterator&& facet_iterator) noexcept:
    surface_(facet_iterator.surface()),
    facet_no_(facet_iterator.facet_no()),
    unique_id_(facet_iterator.unique_id()){
  }

  ~FacetIterator() = default;

  bool IsEnd() const {
    return facet_no_ == INVALID_ID;
  }

  FacetIterator& operator++() {
    assert(IsValid());
    facet_no_ = surface_.facet_manager_.GetNextElementPosition(facet_no_);
    unique_id_ = facet_no_ != INVALID_ID ? surface_.facet_manager_.UniqueID(facet_no_) : INVALID_ID;
    return *this;
  }

  FacetIterator operator++(int) {
    assert(IsValid());
    FacetIterator tmp(*this);
    facet_no_ = surface_.facet_manager_.GetNextElementPosition(facet_no_);
    unique_id_ = facet_no_ != INVALID_ID ? surface_.facet_manager_.UniqueID(facet_no_) : INVALID_ID;
    return tmp;
  }

  bool operator==(const FacetIterator& another_facet) const {
    assert(IsValid());
    return facet_no_ == another_facet.facet_no();
  }

  bool operator!=(const FacetIterator& another_facet) const {
    assert(IsValid());
    return !(*this == another_facet);
  }

  FacetIterator& operator=(const FacetIterator& another_facet) {
    assert((&surface_) == (&(another_facet.surface())));
    facet_no_ = another_facet.facet_no();
    unique_id_ = another_facet.unique_id();
    return *this;
  }

  FacetIterator& operator=(FacetIterator&& another_facet) noexcept {
    assert((&surface_) == (&(another_facet.surface())));
    facet_no_ = another_facet.facet_no();
    unique_id_ = another_facet.unique_id();
    return *this;
  }

  Surface& surface() const {
    assert(IsValid());
    return surface_;
  }

  unsigned int facet_no() const {
    assert(IsValid());
    return facet_no_;
  }

  unsigned int unique_id() const {
    return unique_id_;
  }

  bool IsValid() const
  {
    return surface_.facet_manager_.UniqueID(facet_no_) == unique_id_;
  }

private:
  Surface& surface_;
  unsigned int facet_no_;
  unsigned int unique_id_;
};



class VertexIterator {
public:
  HalfedgeIterator halfedge() const;

  Point3 position() const {
    assert(IsValid());
    return surface_.vertex_manager_.VertexAt(vertex_no_).position;
  }

  explicit VertexIterator(Surface& surface, const unsigned int vertex_no):
    surface_(surface),
    vertex_no_(vertex_no),
    unique_id_(vertex_no_ != INVALID_ID ? surface_.vertex_manager_.UniqueID(vertex_no_) : INVALID_ID){
  }

  VertexIterator(const VertexIterator& vertex_iterator):
    surface_(vertex_iterator.surface()),
    vertex_no_(vertex_iterator.vertex_no()),
    unique_id_(vertex_iterator.unique_id()){
  }

  VertexIterator(VertexIterator&& vertex_iterator) noexcept:
    surface_(vertex_iterator.surface()),
    vertex_no_(vertex_iterator.vertex_no()),
    unique_id_(vertex_iterator.unique_id()){
  }

  ~VertexIterator() = default;

  bool IsEnd() const {
    return vertex_no_ == INVALID_ID;
  }

  VertexIterator& operator++() {
    assert(IsValid());
    vertex_no_ = surface_.vertex_manager_.GetNextElementPosition(vertex_no_);
    unique_id_ = vertex_no_!=INVALID_ID? surface_.vertex_manager_.UniqueID(vertex_no_):INVALID_ID;
    return *this;
  }

  VertexIterator operator++(int) {
    assert(IsValid());
    VertexIterator tmp(*this);
    vertex_no_ = surface_.vertex_manager_.GetNextElementPosition(vertex_no_);
    unique_id_ = vertex_no_ != INVALID_ID ? surface_.vertex_manager_.UniqueID(vertex_no_) : INVALID_ID;
    return tmp;
  }

  bool operator==(const VertexIterator& another_vertex) const {
    assert(IsValid());
    return vertex_no_ == another_vertex.vertex_no();
  }

  bool operator!=(const VertexIterator& another_vertex) const {
    assert(IsValid());
    return !(*this == another_vertex);
  }

  VertexIterator& operator=(const VertexIterator& another_vertex) {
    assert((&surface_) == (&(another_vertex.surface())));
    vertex_no_ = another_vertex.vertex_no();
    unique_id_ = another_vertex.unique_id();
    return *this;
  }

  VertexIterator& operator=(VertexIterator&& another_vertex) noexcept {
    assert((&surface_) == (&(another_vertex.surface())));
    vertex_no_ = another_vertex.vertex_no();
    unique_id_ = another_vertex.unique_id();
    return *this;
  }

  Surface& surface() const {
    assert(IsValid());
    return surface_;
  }

  unsigned int vertex_no() const {
    assert(IsValid());
    return vertex_no_;
  }


  unsigned int unique_id() const {
    return unique_id_;
  }

  bool IsValid() const
  {
    return surface_.vertex_manager_.UniqueID(vertex_no_) == unique_id_;
  }
private:
  Surface& surface_;
  unsigned int vertex_no_;
  unsigned int unique_id_;
};



class HalfedgeIterator {
public:
  HalfedgeIterator opposite() const {
    assert(IsValid());
    return HalfedgeIterator(surface_, surface_.halfedge_manager_.HalfedgeAt(halfedge_no_).opposite);
  }

  HalfedgeIterator prev() const {
    assert(IsValid());
    return HalfedgeIterator(surface_, surface_.halfedge_manager_.HalfedgeAt(halfedge_no_).prev);
  }

  HalfedgeIterator next() const {
    assert(IsValid());
    return HalfedgeIterator(surface_, surface_.halfedge_manager_.HalfedgeAt(halfedge_no_).next);
  }

  HalfedgeIterator next_around_vertex() const {
    assert(IsValid());
    return opposite().prev();
  }

  HalfedgeIterator prev_around_vertex() const {
    assert(IsValid());
    return next().opposite();
  }

  VertexIterator vertex() const {
    assert(IsValid());
    return VertexIterator(surface_, surface_.halfedge_manager_.HalfedgeAt(halfedge_no_).vertex);
  }

  FacetIterator facet() const {
    assert(IsValid());
    return FacetIterator(surface_, surface_.halfedge_manager_.HalfedgeAt(halfedge_no_).facet);
  }

  explicit HalfedgeIterator(Surface& surface, const unsigned int halfedge_no) :
    surface_(surface),
    halfedge_no_(halfedge_no),
    unique_id_(halfedge_no_ != INVALID_ID ? surface_.halfedge_manager_.UniqueID(halfedge_no_) : INVALID_ID) {
  }

  HalfedgeIterator(const HalfedgeIterator& halfedge_iterator):
    surface_(halfedge_iterator.surface()),
    halfedge_no_(halfedge_iterator.halfedge_no()),
    unique_id_(halfedge_iterator.unique_id()){
  }

  HalfedgeIterator(HalfedgeIterator&& halfedge_iterator) noexcept:
    surface_(halfedge_iterator.surface()),
    halfedge_no_(halfedge_iterator.halfedge_no()),
    unique_id_(halfedge_iterator.unique_id()) {
  }

  ~HalfedgeIterator() = default;

  bool IsEnd() const {
    return halfedge_no_ == INVALID_ID;
  }

  HalfedgeIterator& operator++() {
    assert(IsValid());
    halfedge_no_ = surface_.halfedge_manager_.GetNextElementPosition(halfedge_no_);
    unique_id_ = halfedge_no_ != INVALID_ID ? surface_.halfedge_manager_.UniqueID(halfedge_no_) : INVALID_ID;
    return *this;
  }

  HalfedgeIterator operator++(int) {
    assert(IsValid());
    HalfedgeIterator tmp(*this);
    halfedge_no_ = surface_.halfedge_manager_.GetNextElementPosition(halfedge_no_);
    unique_id_ = halfedge_no_!=INVALID_ID? surface_.halfedge_manager_.UniqueID(halfedge_no_):INVALID_ID;
    return tmp;
  }

  bool operator==(const HalfedgeIterator& another_halfedge) const {
    assert(IsValid());
    return halfedge_no_ == another_halfedge.halfedge_no();
  }

  bool operator!=(const HalfedgeIterator& another_halfedge) const {
    assert(IsValid());
    return !(*this == another_halfedge);
  }

  HalfedgeIterator& operator=(const HalfedgeIterator& another_halfedge) {
    assert((&surface_) == (&(another_halfedge.surface())));
    halfedge_no_ = another_halfedge.halfedge_no();
    unique_id_ = another_halfedge.unique_id();
    return *this;
  }

  HalfedgeIterator& operator=(HalfedgeIterator&& another_halfedge) noexcept {
    assert((&surface_) == (&(another_halfedge.surface())));
    halfedge_no_ = another_halfedge.halfedge_no();
    unique_id_ = another_halfedge.unique_id();
    return *this;
  }

  Surface& surface() const {
    assert(IsValid());
    return surface_;
  }

  unsigned int halfedge_no() const {
    assert(IsValid());
    return halfedge_no_;
  }

  unsigned int unique_id() const {
    return unique_id_;
  }

  bool IsValid() const
  {
    return surface_.halfedge_manager_.UniqueID(halfedge_no_) == unique_id_;
  }
private:
  Surface& surface_;
  unsigned int halfedge_no_;
  unsigned int unique_id_;
};

inline HalfedgeIterator VertexIterator::halfedge() const {
  assert(IsValid());
  return HalfedgeIterator(surface_,
    surface_
    .vertex_manager_.VertexAt(vertex_no_).halfedge);
}

inline HalfedgeIterator FacetIterator::halfedge() const {
  assert(IsValid());
  return HalfedgeIterator(surface_,
    surface_.facet_manager_.FacetAt(facet_no_).halfedge);
}
}
