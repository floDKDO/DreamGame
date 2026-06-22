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
		case AttributeIndex::POSITION:
			offset = offsetof(Vertex, position_);
			break;

		case AttributeIndex::NORMAL:
			offset = offsetof(Vertex, normal_);
			break;

		case AttributeIndex::TEXTURE_COORD:
			offset = offsetof(Vertex, texture_coordinates_);
			break;

		case AttributeIndex::COLOR:
			offset = offsetof(Vertex, color_);
			break;

		default:
			std::cout << "****ERROR****: Unknown attribute index!\n";
			break;
	}
	return offset;
}

GLint Vertices::get_number_of_attribute_values(Vertices::AttributeIndex attribute_index) const
{
	GLint number_of_attribute_values = 0;
	switch(attribute_index) 
	{
		case AttributeIndex::POSITION:
			number_of_attribute_values = 3;
			break;

		case AttributeIndex::NORMAL:
			number_of_attribute_values = 3;
			break;

		case AttributeIndex::TEXTURE_COORD:
			number_of_attribute_values = 2;
			break;

		case AttributeIndex::COLOR:
			number_of_attribute_values = 4;
			break;

		default:
			std::cout << "****ERROR****: Unknown attribute index!\n";
			break;
	}
	return number_of_attribute_values;
}

size_t Vertices::get_vertex_size() const
{
	return sizeof(Vertex);
}

size_t Vertices::get_length() const
{
	return vertices_.size();
}

const Vertices::Vertex* Vertices::get_data() const
{
	return vertices_.data();
}

void Vertices::print() const
{
	for(const Vertex& v : vertices_)
	{
		std::cout << "Position : (.x: " << v.position_.x << ", .y: " << v.position_.y << ", .z: " << v.position_.z << ")\n";
		std::cout << "Normal : (.x: " << v.normal_.x << ", .y: " << v.normal_.y << ", .z: " << v.normal_.z << ")\n";
		std::cout << "Texture coordinates : (.x: " << v.texture_coordinates_.x << ", .y: " << v.texture_coordinates_.y << ")\n";
		std::cout << "Color : (.x: " << v.color_.x << ", .y: " << v.color_.y << ", .z: " << v.color_.z << ")\n";
	}
	std::cout << std::endl;
}