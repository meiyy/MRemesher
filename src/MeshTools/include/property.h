#pragma once
#include <utility>
#include <vector>
#include "point3.h"

namespace mesh_tools {
template <class ValueType>
class Property {
public:
  ValueType operator[](const unsigned int pos) {
    return property_data_[pos];
  }

  unsigned int size() const {
    return static_cast<unsigned int>(property_data_.size());
  }

  void RemoveElement(const unsigned int element_no)
  {
    auto last = property_data_.size()-1;
    assert(element_no<=last);
    if(element_no!=last)
      std::swap(property_data_[element_no],property_data_[last]);
    property_data_.pop_back();
  }
protected:
  std::vector<ValueType> property_data_;
};

struct FacetElement {
  unsigned int halfedge;

  explicit FacetElement(const unsigned int halfedge): halfedge(halfedge) {
  }
};

class FacetStore : public Property<FacetElement> {
public:
  void AddFacet(const unsigned int halfedge) {
    property_data_.emplace_back(FacetElement(halfedge));
  }
};

struct VertexElement {
  unsigned int halfedge;
  Point3 position;

  VertexElement(const unsigned int halfedge, Point3 position):
    halfedge(halfedge),
    position(std::move(position)) {
  }
};

class VertexStore : public Property<VertexElement> {
public:
  void AddVertex(const unsigned int halfedge, const Point3& position) {
    property_data_.emplace_back(VertexElement(halfedge, position));
  }

  void SetHalfedge(const unsigned int vertex, const unsigned int halfedge) {
    property_data_[vertex].halfedge = halfedge;
  }
};

struct HalfedgeElement {
  unsigned int opposite;
  unsigned int prev;
  unsigned int next;
  unsigned int vertex;
  unsigned int facet;

  HalfedgeElement(const unsigned int opposite, const unsigned int prev,
                  const unsigned int next, const unsigned int vertex,
                  const unsigned int facet):
    opposite(opposite),
    prev(prev),
    next(next),
    vertex(vertex),
    facet(facet) {
  }
};

class HalfedgeStore : public Property<HalfedgeElement> {
public:
  void AddHalfedge(const unsigned int opposite, const unsigned int prev,
                   const unsigned int next, const unsigned int vertex,
                   const unsigned int facet) {
    property_data_.emplace_back(HalfedgeElement(opposite, prev, next, vertex,
                                                facet));
  }

  void SetOpposite(const unsigned int halfedge, const unsigned int opposite) {
    property_data_[halfedge].opposite = opposite;
  }
};
}
