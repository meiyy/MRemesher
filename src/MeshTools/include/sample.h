#pragma once
#include <vector>
#include "surfacealgorithm.h"


namespace mesh_tools
{

class Sample
{
public:
  Sample(Surface &surface): surface_(surface) {}
  Sample(Surface &surface, std::vector<Point3> points) : surface_(surface),points_(std::move(points)) {}

  void FixedNumberRandomSample(unsigned int sample_size)
  {
    assert(surface_.num_facets() > 0);
    double surfaceArea = 0;
    std::vector<double> prefix_sum_area;
    std::vector<FacetIterator> facet_no_to_iterator;
    for(FacetIterator i(surface_);!i.IsEnd();++i)
    {
      double area_cur = area(i);
      surfaceArea += area_cur;
      prefix_sum_area.push_back(surfaceArea);
      facet_no_to_iterator.push_back(i);
    }

    RandomDoubleUniform random(0, surfaceArea);
    std::vector<double> sample_position(sample_size);
    for(auto &i:sample_position)
    {
      i = random.Rand();
    }
    std::sort(sample_position.begin(), sample_position.end());
    
    int cur_triangle = 0;
    for(auto i:sample_position)
    {
      while (prefix_sum_area[cur_triangle] < i)
        cur_triangle++;
      const auto sample_point = RandomPointInTriangle(facet_no_to_iterator[cur_triangle]);
      points_.push_back(sample_point);
      sample_facet_.push_back(facet_no_to_iterator[cur_triangle]);
    }
  }

  const auto& Points()const
  {
    return points_;
  }
private:
  Surface & surface_;
  std::vector<Point3> points_;
  std::vector<FacetIterator> sample_facet_;
};

}
