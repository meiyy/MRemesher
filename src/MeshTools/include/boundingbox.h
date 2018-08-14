#pragma once
#include "surface.h"
#include "iterator.h"

namespace mesh_tools {
class BoundingBox {
public:
  BoundingBox()
    : x_min_(DBL_MAX),
      x_max_(DBL_MIN),
      y_min_(DBL_MAX),
      y_max_(DBL_MIN),
      z_min_(DBL_MAX),
      z_max_(DBL_MIN) {
  }

  explicit BoundingBox(Surface& surface)
    : x_min_(DBL_MAX),
      x_max_(DBL_MIN),
      y_min_(DBL_MAX),
      y_max_(DBL_MIN),
      z_min_(DBL_MAX),
      z_max_(DBL_MIN) {
    AddSurface(surface);
  }

  void Clear() {
    x_min_ = DBL_MAX;
    y_min_ = DBL_MAX;
    z_min_ = DBL_MAX;
    x_max_ = DBL_MIN;
    y_max_ = DBL_MIN;
    z_max_ = DBL_MIN;
  }

  void AddPoint3(const Point3& point3) {
    x_min_ = std::min(x_min_, point3[0]);
    y_min_ = std::min(y_min_, point3[1]);
    z_min_ = std::min(z_min_, point3[2]);
    x_max_ = std::max(x_max_, point3[0]);
    y_max_ = std::max(y_max_, point3[1]);
    z_max_ = std::max(z_max_, point3[2]);
  }

  void AddSurface(Surface& surface) {
    for (VertexIterator i(surface, 0); !i.IsEnd(); ++i) {
      AddPoint3(i.position());
    }
  }

  double x_min() const {
    return x_min_;
  }

  double x_max() const {
    return x_max_;
  }

  double y_min() const {
    return y_min_;
  }

  double y_max() const {
    return y_max_;
  }

  double z_min() const {
    return z_min_;
  }

  double z_max() const {
    return z_max_;
  }

  double length() const {
    return x_max_ - x_min_;
  }

  double width() const {
    return y_max_ - y_min_;
  }

  double height() const {
    return z_max_ - z_min_;
  }

  Point3 center() const {
    return Point3({
      (x_min_ + x_max_) / 2,
      (y_min_ + y_max_) / 2,
      (z_min_ + z_max_) / 2
    });
  }

private:
  double x_min_, x_max_, y_min_, y_max_, z_min_, z_max_;
};
}
