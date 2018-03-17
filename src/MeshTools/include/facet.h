#pragma once
#include <cassert>

namespace mesh_tools {
class Builder;

class Facet {
  friend class Builder;
public:
  unsigned int Halfedfe() const
    {
      return halfedge_;
    }
private:
  Facet() = default;
  unsigned int halfedge_;
};
}
