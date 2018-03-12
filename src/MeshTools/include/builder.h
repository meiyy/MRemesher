#pragma once
#include <surface.h>
namespace mesh_tools
{
	class Surface;
	class Builder
	{
	public:
		explicit Builder(Surface &surface):surface_(surface){}
		void AddPoint(const point3 &point)
		{
			surface_.vertices_.emplace_back(point);
		}
		void add_facet(unsigned int point0, unsigned int point1, unsigned int point2)
		{
			surface
		}
	private:
		Surface &surface_;
	};
}