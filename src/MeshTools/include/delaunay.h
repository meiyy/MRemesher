#pragma once
#include "sample.h"

namespace mesh_tools
{
  class Delaunay
  {
  public:
    explicit Delaunay(Sample sample):sample_(std::move(sample)){}
    void ComputeDelaunay()
    {
      neighbor.resize(sample_.Points().size());
      
    }
  private:
    std::vector<std::vector<unsigned int>> neighbor;
    Sample sample_;
  };
}