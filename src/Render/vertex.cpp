#include "vertex.h"

#include <iostream>

VertexAttribute::Info Vertex::get_attribute_info(VertexAttribute::Name attribute_name)
{
	if(attribute_name == VertexAttribute::Name::POSITION)
	{
		return VertexAttribute::Info{0, 3, GL_FALSE, offsetof(Vertex, position_)};
	}
	else if(attribute_name == VertexAttribute::Name::NORMAL)
	{
		return VertexAttribute::Info{1, 3, GL_FALSE, offsetof(Vertex, normal_)};
	}
	else if(attribute_name == VertexAttribute::Name::TEXTURE_COORD)
	{
		return VertexAttribute::Info{2, 2, GL_FALSE, offsetof(Vertex, texture_coordinates_)};
	}
	else if(attribute_name == VertexAttribute::Name::COLOR)
	{
		return VertexAttribute::Info{3, 4, GL_FALSE, offsetof(Vertex, color_)};
	}
	else
	{
		std::cout << "****ERROR****: Unknown attribute!\n";
		return VertexAttribute::Info{0, 0, 0};
	}
}