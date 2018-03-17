#pragma once
#include <ostream>
#include <cassert>

namespace mesh_tools {
class point3 {
public:

  // Constractors, destructor, assignment operator
  point3() {
    data_[0] = data_[1] = data_[2] = 0;
  }

  point3(const double x, const double y, const double z) {
    data_[0] = x;
    data_[1] = y;
    data_[2] = z;
  }

  point3(const point3& from) {
    data_[0] = from[0];
    data_[1] = from[1];
    data_[2] = from[2];
  }

  point3(point3&& from) noexcept {
    data_[0] = from[0];
    data_[1] = from[1];
    data_[2] = from[2];
  }

  ~point3() = default;


  friend point3 operator*(const double number, const point3& point) {
    return {point[0] * number, point[1] * number, point[2] * number};
  }


  friend std::ostream& operator<<(std::ostream& out, const point3& point) {
    out << "(" << point[0] << ", " << point[1] << ", " << point[2] << ")";
    return out;
  }

  point3& operator=(const point3& from) {
    data_[0] = from[0];
    data_[1] = from[1];
    data_[2] = from[2];
    return *this;
  }

  point3& operator=(point3&& from) noexcept {
    data_[0] = from[0];
    data_[1] = from[1];
    data_[2] = from[2];
    return *this;
  }

  double& operator[](const int pos) {
    assert(pos >= 0 && pos < 3);
    return data_[pos];
  }

  const double& operator[](const int pos) const {
    assert(pos >= 0 && pos < 3);
    return data_[pos];
  }

  point3 operator+(const point3& another_point) const {
    return {
      data_[0] + another_point[0], data_[1] + another_point[1],
      data_[2] + another_point[2]
    };
  }

  point3 operator-(const point3& another_point) const {
    return {
      data_[0] - another_point[0], data_[1] - another_point[1],
      data_[2] - another_point[2]
    };
  }

  point3 operator*(const double number) const {
    return {data_[0] * number, data_[1] * number, data_[2] * number};
  }

  point3 operator/(const double number) const {
    return {data_[0] / number, data_[1] / number, data_[2] / number};
  }

  double inner_product(const point3& another_point) const {
    return data_[0] * another_point[0] + data_[1] * another_point[1] + data_[2]
           *
           another_point[2];
  }

  point3 outer_product(const point3& another_point) const {
    return {
      data_[1] * another_point[2] - data_[2] * another_point[1],
      data_[2] * another_point[0] - data_[0] * another_point[2],
      data_[0] * another_point[1] - data_[1] * another_point[0]
    };
  }

  double length2() const {
    return data_[0] * data_[0] + data_[1] * data_[1] + data_[2] * data_[2];
  }

  double length() const {
    return std::sqrt(length2());
  }

  double distance_to(const point3& another_point) const {
    return (*this - another_point).length();
  }

  double distance2_to(const point3& another_point) const {
    return (*this - another_point).length2();
  }

private:
  double data_[3]{};
};
}
