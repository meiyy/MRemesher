#pragma once

namespace mesh_tools
{
	class point2
	{
	public:

		// Constractors, destructor, assignment operator
		point2();

		point2(double x, double y);

		point2(const point2& from);

		point2(point2&& from) noexcept;

		~point2() = default;

		point2& operator=(const point2& from);

		point2& operator=(point2&& from) noexcept;

		// Operators
		double& operator[](int pos);

		const double& operator[](int pos) const;

		point2 operator+(const point2& another_point) const;

		point2 operator-(const point2& another_point) const;

		point2 operator*(double number) const;

		friend point2 operator*(const double number, const point2& point)
		{
			return { point[0] * number, point[1] * number };
		}

		point2 operator/(double number) const;

		double inner_product(const point2& another_point) const;

		double length2() const;

		double length() const;

		double distance_to(const point2& another_point) const;

		double distance2_to(const point2& another_point) const;

	private:
		double data_[2]{};
	};

	
}
