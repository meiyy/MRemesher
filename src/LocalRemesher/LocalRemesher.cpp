#include "LocalRemesher.h"
#include <iostream>
#include "builder.h"

using mesh_tools::Surface;
using mesh_tools::Builder;
using std::cout;
using std::endl;

int main() {
  Surface surface;
  Builder builder(surface);
  builder.AddPoint({0, 0, 0});
  builder.AddPoint({1, 1, 1});
  builder.AddPoint({1, 1, 0});
  builder.AddFacet(0, 1, 2);

  for (unsigned int i = 0; i < surface.num_facets(); i++) {
    std::cout << "Facet " << i << " :" << std::endl;
    for (unsigned int j = 0; j < 3; j++) {
      const auto pos = surface.FacetAt(i).Vertex(j);
      cout << surface.VertexAt(pos).Position() << endl;
    }
  }
  return 0;
}
