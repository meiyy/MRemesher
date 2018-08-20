#pragma once
#include "point3.h"

namespace mesh_tools {
#define M_PI (3.14159265358979323846)

template <class Type>
Type square(Type number) {
  return number * number;
}

inline Point3 TriangleNormal(const Point3 &point_a,const Point3 &point_b,const Point3 &point_c)
{
  const auto vec0 = point_b - point_a, vec1 = point_c - point_a;
  return vec0.outer_product(vec1).normalize();
}

template<class T>
T clamp(T number, T min, T max)
{
  return number<min?min:(number>max?max:number);
}
}
