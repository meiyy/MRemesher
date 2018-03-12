#pragma once
#include <cassert>

namespace mesh_tools
{
	class Facet
	{
	public:
		int Vertex(const unsigned int no) const
		{
			assert(no >= 0 && no <= 2);
			return facet_vertex_[no];
		}
	private:
		unsigned int facet_vertex_[3]{};
	};
}
