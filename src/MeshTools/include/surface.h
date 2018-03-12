#pragma once
#include <vector>
#include <facet.h>
#include <vertex.h>

namespace mesh_tools
{
	class Builder;
	class Surface 
	{
		friend class Builder;
	public:
		const Facet& FacetAt(const unsigned int facet_no) const
		{
			return facets_[facet_no];
		}
		const Vertex& VertexAt(const unsigned int vertex_no) const
		{
			return vertices_[vertex_no];
		}
	private:
		
		std::vector<Facet> facets_;
		std::vector<Vertex> vertices_;
	};
}
