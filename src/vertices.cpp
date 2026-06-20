#include "vertices.h"

#include <iostream>

Vertices::Vertices(uint64_t number)
	: vertices_(number), has_positions_(false), has_colors_(false), has_texture_coordinates_(false), has_normals_(false)
{}

void Vertices::add_position_attributes(std::vector<glm::vec3> position_vector)
{
	for(std::size_t i = 0; i < vertices_.size(); ++i)
	{
		vertices_[i].position_ = position_vector[i];
	}
	has_positions_ = true;
}

void Vertices::add_normal_attributes(std::vector<glm::vec3> normal_vector)
{
	for(std::size_t i = 0; i < vertices_.size(); ++i)
	{
		vertices_[i].normal_ = normal_vector[i];
	}
	has_normals_ = true;
}

void Vertices::add_texture_coordinates_attributes(std::vector<glm::vec2> texture_coordinates_vector)
{
	for(std::size_t i = 0; i < vertices_.size(); ++i)
	{
		vertices_[i].texture_coordinates_ = texture_coordinates_vector[i];
	}
	has_texture_coordinates_ = true;
}

void Vertices::add_color_attributes(std::vector<glm::vec4> color_vector)
{
	for(std::size_t i = 0; i < vertices_.size(); ++i)
	{
		vertices_[i].color_ = color_vector[i];
	}
	has_colors_ = true;
}

std::size_t Vertices::get_vertices_number() const
{
	return vertices_.size();
}

std::size_t Vertices::get_attribute_offset(AttributeIndex attribute_index) const
{
	std::size_t offset = 0;
	switch(attribute_index)
	{
		case AttributeIndex::Position:
			offset = offsetof(Vertex, position_);
			break;

		case AttributeIndex::Normal:
			offset = offsetof(Vertex, normal_);
			break;

		case AttributeIndex::TextureCoord:
			offset = offsetof(Vertex, texture_coordinates_);
			break;

		case AttributeIndex::Color:
			offset = offsetof(Vertex, color_);
			break;

		default:
			std::cout << "****ERROR****: Unknown attribute index!\n";
			break;
	}
	return offset;
}