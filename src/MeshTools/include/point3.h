#pragma once

namespace mesh_tools
{
	class point3
	{
	public:

		// Constractors, destructor, assignment operator
		point3();

		point3(double x, double y, double z);

		point3(const point3& from);

		point3(point3&& from) noexcept;

		~point3() = default;

		point3& operator=(const point3& from);

		point3& operator=(point3&& from) noexcept;

		// Operators
		double& operator[](int pos);

		const double& operator[](int pos) const;

		point3 operator+(const point3& another_point) const;

		point3 operator-(const point3& another_point) const;

		point3 operator*(double number) const;

		friend point3 operator*(const double number, const point3& point)
		{
			return { point[0] * number, point[1] * number, point[2] * number };
		}

		point3 operator/(double number) const;

		double inner_product(const point3& another_point) const;

		point3 outer_product(const point3& another_point) const;

		double length2() const;

		double length() const;

		double distance_to(const point3& another_point) const;

		double distance2_to(const point3& another_point) const;

	private:
		double data_[3]{};
	};
}
