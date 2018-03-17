#pragma once
#include <utility>
#include "point3.h"

namespace mesh_tools {
class Builder;

class Vertex {
  friend class Builder;
public:
  const point3& Position() const {
    return position_;
  }
  unsigned int Halfedfe() const
  {
    return halfedge_;
  }
private:
  Vertex() = default;

  explicit Vertex(point3 position): position_(std::move(position)) {
  }

  point3 position_;
  unsigned int halfedge_;
};
}
