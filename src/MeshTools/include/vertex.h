#pragma once
#include <utility>
#include "point3.h"

namespace mesh_tools
{
	class Vertex
	{
	public:
		const point3& Position() const
		{
			return position_;
		}
	private:
		Vertex() = default;
		~Vertex() = default;

		explicit Vertex(point3 position):position_(std::move(position)){}
		point3 position_;
	};
}
