#include "vertex.h"

#include <iostream>

attribute::Info Vertex::get_attribute_info(attribute::Name attribute_name)
{
	if(attribute_name == attribute::Name::POSITION)
	{
		return attribute::Info{0, 3, GL_FALSE, offsetof(Vertex, position_)};
	}
	else if(attribute_name == attribute::Name::NORMAL)
	{
		return attribute::Info{1, 3, GL_FALSE, offsetof(Vertex, normal_)};
	}
	else if(attribute_name == attribute::Name::TEXTURE_COORD)
	{
		return attribute::Info{2, 2, GL_FALSE, offsetof(Vertex, texture_coordinates_)};
	}
	else if(attribute_name == attribute::Name::COLOR)
	{
		return attribute::Info{3, 4, GL_FALSE, offsetof(Vertex, color_)};
	}
	else
	{
		std::cout << "****ERROR****: Unknown attribute!\n";
		return attribute::Info{0, 0, 0};
	}
}