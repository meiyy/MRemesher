#include "point3.h"
#include <algorithm>
#include <cmath>
#include <cassert>

namespace mesh_tools
{

	inline point3::point3()
	{
		data_[0] = data_[1] = data_[2] = 0;
	}

	inline point3::point3(const double x, const double y, const double z)
	{
		data_[0] = x;
		data_[1] = y;
		data_[2] = z;
	}

	inline point3::point3(const point3& from)
	{
		data_[0] = from[0];
		data_[1] = from[1];
		data_[2] = from[2];
	}

	inline point3::point3(point3&& from) noexcept
	{
		data_[0] = from[0];
		data_[1] = from[1];
		data_[2] = from[2];
	}

	inline point3& point3::operator=(const point3& from)
	{
		data_[0] = from[0];
		data_[1] = from[1];
		data_[2] = from[2];
		return *this;
	}

	inline point3& point3::operator=(point3&& from) noexcept
	{
		data_[0] = from[0];
		data_[1] = from[1];
		data_[2] = from[2];
		return *this;
	}

	inline double& point3::operator[](const int pos)
	{
		assert(pos >= 0 && pos < 3);
		return data_[pos];
	}

	inline const double& point3::operator[](const int pos) const
	{
		assert(pos >= 0 && pos < 3);
		return data_[pos];
	}

	inline point3 point3::operator+(const point3& another_point) const
	{
		return { data_[0] + another_point[0], data_[1] + another_point[1], data_[2] + another_point[2] };
	}

	inline point3 point3::operator-(const point3& another_point) const
	{
		return { data_[0] - another_point[0], data_[1] - another_point[1], data_[2] - another_point[2] };
	}

	inline point3 point3::operator*(const double number) const
	{
		return { data_[0] * number, data_[1] * number, data_[2] * number };
	}

	inline point3 point3::operator/(const double number) const
	{
		return { data_[0] / number, data_[1] / number, data_[2] / number };
	}

	inline double point3::inner_product(const point3& another_point) const
	{
		return data_[0] * another_point[0] + data_[1] * another_point[1] + data_[2] * another_point[2];
	}

	inline point3 point3::outer_product(const point3& another_point) const
	{
		return {
			data_[1] * another_point[2] - data_[2] * another_point[1], data_[2] * another_point[0] - data_[0] * another_point[2],
			data_[0] * another_point[1] - data_[1] * another_point[0]
		};
	}

	inline double point3::length2() const
	{
		return data_[0] * data_[0] + data_[1] * data_[1] + data_[2] * data_[2];
	}

	inline double point3::length() const
	{
		return std::sqrt(length2());
	}

	inline double point3::distance_to(const point3& another_point) const
	{
		return (*this - another_point).length();
	}

	inline double point3::distance2_to(const point3& another_point) const
	{
		return (*this - another_point).length2();
	}
}