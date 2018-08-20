#pragma once
#include <utility>
#include "point3.h"

namespace mesh_tools {

class Plane
{
public:  
  enum Side
  {
    FRONT_OF_PLANE,
    ON_PLANE,
    BACK_OF_PLANE
  };
  Plane(Point3 point, Point3 direction):
    point_(std::move(point)),direction_(std::move(direction)) { }

  Side GetSideOfPoint(const Point3 &point) const
  {
    const auto vec = (point - point_);
    const auto angle_cos = direction_.inner_product(vec);
    const double angle_eps = 1e-8;
    if(angle_cos > angle_eps)
    {
      return FRONT_OF_PLANE;
    }
    else if(angle_cos < -angle_eps)
    {
      return BACK_OF_PLANE;
    }
    else
    {
      return ON_PLANE;
    }
  }

private:
  Point3 point_;
  Point3 direction_;
};

}
