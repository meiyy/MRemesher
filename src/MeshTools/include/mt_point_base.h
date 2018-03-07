#pragma once
#include <algorithm>
#include <cassert>

namespace mesh_tools
{

	template <class NumberType, unsigned int Dim>
	class mt_point_base
	{
		typedef mt_point_base<NumberType, Dim> this_type;
	public:

		// Constractors and destructor
		mt_point_base() = default;
		~mt_point_base() = default;

		mt_point_base(const this_type& from)
		{
			from.copy_data_to(data_);
		}

		mt_point_base(const this_type&& from) noexcept
		{
			from.copy_data_to(data_);
		}

		// Operators
		NumberType& operator[](const int pos)
		{
			assert(pos >= 0 && pos < Dim);
			return data_[pos];
		}

		const NumberType& operator[](const int pos) const
		{
			assert(pos >= 0 && pos < Dim);
			return data_[pos];
		}

		mt_point_base& operator=(const this_type& from)
		{
			from.copy_data_to(data_);
			return *this;
		}

		mt_point_base& operator=(const this_type&& from)
		{
			from.copy_data_to(data_);
			return *this;
		}


		void copy_data_to(NumberType dst[Dim]) const
		{
			for (unsigned int i = 0; i < Dim; i++)
			{
				dst[i] = data_[i];
			}
		}

	private:
		NumberType data_[Dim];
	};
}