#include "vertex.h"

#include <iostream>

attribute::Info Vertex::get_attribute_info(attribute::Name attribute_name)
{
	using attribute::Name;

	if(attribute_name == Name::POSITION)
	{
		return attribute::Info{static_cast<std::underlying_type_t<Name>>(Name::POSITION), 3, GL_FALSE, offsetof(Vertex, position_)};
	}
	else if(attribute_name == Name::NORMAL)
	{
		return attribute::Info{static_cast<std::underlying_type_t<Name>>(Name::NORMAL), 3, GL_FALSE, offsetof(Vertex, normal_)};
	}
	else if(attribute_name == attribute::Name::TEXCOORD)
	{
		return attribute::Info{static_cast<std::underlying_type_t<Name>>(Name::TEXCOORD), 2, GL_FALSE, offsetof(Vertex, texcoord_)};
	}
	else //if(attribute_name == attribute::Name::COLOR) //if commenté pour retirer le warning "not all control paths return a value enum values" (on est assuré que le else s'exécute pour la valeur COLOR)
	{
		return attribute::Info{static_cast<std::underlying_type_t<Name>>(Name::COLOR), 4, GL_FALSE, offsetof(Vertex, color_)};
	}
}