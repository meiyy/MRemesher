#include "point2.h"
#include <algorithm>
#include <cmath>
#include <cassert>

namespace mesh_tools
{

	inline point2::point2()
	{
		data_[0] = data_[1] = 0;
	}

	inline point2::point2(const double x, const double y)
	{
		data_[0] = x;
		data_[1] = y;
	}

	inline point2::point2(const point2& from)
	{
		data_[0] = from[0];
		data_[1] = from[1];
	}

	inline point2::point2(point2&& from) noexcept
	{
		data_[0] = from[0];
		data_[1] = from[1];
	}

	inline point2& point2::operator=(const point2& from)
	{
		data_[0] = from[0];
		data_[1] = from[1];
		return *this;
	}

	inline point2& point2::operator=(point2&& from) noexcept
	{
		data_[0] = from[0];
		data_[1] = from[1];
		return *this;
	}

	inline double& point2::operator[](const int pos)
	{
		assert(pos >= 0 && pos < 2);
		return data_[pos];
	}

	inline const double& point2::operator[](const int pos) const
	{
		assert(pos >= 0 && pos < 2);
		return data_[pos];
	}

	inline point2 point2::operator+(const point2& another_point) const
	{
		return { data_[0] + another_point[0], data_[1] + another_point[1] };
	}

	inline point2 point2::operator-(const point2& another_point) const
	{
		return { data_[0] - another_point[0], data_[1] - another_point[1] };
	}

	inline point2 point2::operator*(const double number) const
	{
		return { data_[0] * number, data_[1] * number };
	}

	inline point2 point2::operator/(const double number) const
	{
		return { data_[0] / number, data_[1] / number };
	}

	inline double point2::inner_product(const point2& another_point) const
	{
		return data_[0] * another_point[0] + data_[1] * another_point[1];
	}

	inline double point2::length2() const
	{
		return data_[0] * data_[0] + data_[1] * data_[1];
	}

	inline double point2::length() const
	{
		return std::sqrt(length2());
	}

	inline double point2::distance_to(const point2& another_point) const
	{
		return (*this - another_point).length();
	}

	inline double point2::distance2_to(const point2& another_point) const
	{
		return (*this - another_point).length2();
	}
}