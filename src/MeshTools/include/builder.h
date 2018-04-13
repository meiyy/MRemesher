#pragma once
#include "point3.h"
#include "surface.h"

namespace mesh_tools {
class Builder {
public:
  explicit Builder(Surface& surface): surface_(surface) {
  }

  void AddVertex(const Point3& point) const;

  void AddFacet(unsigned int point0, unsigned int point1,
    unsigned int point2) const;

  void Build() const;

private:
  Surface& surface_;
};
}
