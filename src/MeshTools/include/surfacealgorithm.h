#pragma once

#include <random>
#include "surface.h"
#include "iterator.h"

namespace mesh_tools
{

inline double area(const Point3 &point_a, const Point3 &point_b, const Point3 &point_c)
{
  auto ba = point_b - point_a, ca = point_c - point_a;
  return ba.outer_product(ca).length() / 2.;
}

inline double area(const FacetIterator &facet)
{
  return area(
    facet.halfedge().vertex().position(),
    facet.halfedge().next().vertex().position(),
    facet.halfedge().prev().vertex().position()
  );
}

inline double area(Surface &surface)
{
  double total_area = 0;
  for(FacetIterator i(surface);!i.IsEnd();++i)
  {
    total_area += area(i);
  }
  return total_area;
}

class RandomDoubleUniform
{
public:
  RandomDoubleUniform(const double from, const double to):distribution_(from,to){}
  double Rand()
  {
    return distribution_(generator_);
  }
private:
  std::default_random_engine generator_;
  std::uniform_real_distribution<double> distribution_;
};

static RandomDoubleUniform standard_random_double(0.,1.);

inline Point3 RandomPointInTriangle(const Point3 &point_a, const Point3 &point_b, const Point3 &point_c)
{
  const double x_plus_y = std::sqrt(standard_random_double.Rand() / 4);
  const double x = standard_random_double.Rand() * x_plus_y;
  const double y = x_plus_y - x;
  return point_a + x * (point_b - point_a) + y * (point_c - point_a);
}

inline Point3 RandomPointInTriangle(FacetIterator &i)
{
  return   RandomPointInTriangle(
    i.halfedge().vertex().position(),
    i.halfedge().next().vertex().position(),
    i.halfedge().prev().vertex().position()
  );
}
  
}
