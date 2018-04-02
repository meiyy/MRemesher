#include "LocalRemesher.h"
#include <iostream>
#include "builder.h"
#include "iterator.h"

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
  builder.Build();

  for (mesh_tools::FacetIterator i(surface, 0); !i.IsEnd(); ++i) {
    std::cout << "facet:" << std::endl;
    auto j = i.halfedge();
    do {
      std::cout << "--" << j.vertex().position() << std::endl;
      j = j.next();
    }
    while (j != i.halfedge());
  }
  return 0;
}
