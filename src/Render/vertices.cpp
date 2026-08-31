#include "vertices.h"

#include <iostream>

Vertices::Vertices(uint64_t number)
	: vertices_(number), attributes_byte_(0)
{}

void Vertices::add_position_attributes(std::vector<glm::vec3> position_vector)
{
	for(std::size_t i = 0; i < vertices_.size(); ++i)
	{
		vertices_[i].position_ = position_vector[i];
	}
	attributes_byte_ |= AttributeByteValue::POSITION_BYTE;
}

void Vertices::add_normal_attributes(std::vector<glm::vec3> normal_vector)
{
	for(std::size_t i = 0; i < vertices_.size(); ++i)
	{
		vertices_[i].normal_ = normal_vector[i];
	}
	attributes_byte_ |= AttributeByteValue::NORMAL_BYTE;
}

void Vertices::add_texcoord_attributes(std::vector<glm::vec2> texcoord_vector)
{
	for(std::size_t i = 0; i < vertices_.size(); ++i)
	{
		vertices_[i].texcoord_ = texcoord_vector[i];
	}
	attributes_byte_ |= AttributeByteValue::TEXCOORD_BYTE;
}

void Vertices::add_color_attributes(std::vector<glm::vec4> color_vector)
{
	for(std::size_t i = 0; i < vertices_.size(); ++i)
	{
		vertices_[i].color_ = color_vector[i];
	}
	attributes_byte_ |= AttributeByteValue::COLOR_BYTE;
}

std::size_t Vertices::get_vertices_number() const
{
	return vertices_.size();
}

const Vertex* Vertices::get_vertices_data() const
{
	return vertices_.data();
}

void Vertices::print() const
{
	for(const Vertex& v : vertices_)
	{
		std::cout << "Position : (.x: " << v.position_.x << ", .y: " << v.position_.y << ", .z: " << v.position_.z << ")\n";
		std::cout << "Normal : (.x: " << v.normal_.x << ", .y: " << v.normal_.y << ", .z: " << v.normal_.z << ")\n";
		std::cout << "Texture coordinates : (.x: " << v.texcoord_.x << ", .y: " << v.texcoord_.y << ")\n";
		std::cout << "Color : (.x: " << v.color_.x << ", .y: " << v.color_.y << ", .z: " << v.color_.z << ")\n";
	}
	std::cout << std::endl;
}

bool Vertices::has_position_attribute() const
{
	return attributes_byte_ & AttributeByteValue::POSITION_BYTE;
}

bool Vertices::has_normal_attribute() const
{
	return attributes_byte_ & AttributeByteValue::NORMAL_BYTE;
}

bool Vertices::has_texcoord_attribute() const
{
	return attributes_byte_ & AttributeByteValue::TEXCOORD_BYTE;
}

bool Vertices::has_color_attribute() const
{
	return attributes_byte_ & AttributeByteValue::COLOR_BYTE;
}