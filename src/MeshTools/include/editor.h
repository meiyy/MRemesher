#pragma once
#include "point3.h"
#include "surface.h"

namespace mesh_tools {
class Surface;

class Editor {
  friend class Surface;
public:
  explicit Editor(Surface& surface): surface_(surface) {
  }

  void AddVertex(const Point3& point) const;

  void AddFacet(unsigned int vertex0, unsigned int vertex1,
    unsigned int vertex2) const;

  void RemoveFacet(unsigned int facet_no)const;

  void RemoveVertex(unsigned int vertex_no)const;

private:
  Surface& surface_;
};
}
