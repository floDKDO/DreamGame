#pragma once

#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace VertexAttribute
{

enum class Name
{
	POSITION,
	NORMAL,
	TEXTURE_COORD,
	COLOR
};

struct Info
{
	GLuint index_;
	GLint component_count_;
	GLboolean normalized_;
	std::size_t offset_;
};

}

struct Vertex
{
	Vertex()
		: position_(0.0f, 0.0f, 0.0f), normal_(0.0f, 0.0f, 0.0f), texture_coordinates_(0.0f, 0.0f), color_(0.0f, 0.0f, 0.0f, 1.0f)
	{}

	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texture_coordinates, glm::vec4 color)
		: position_(position), normal_(normal), texture_coordinates_(texture_coordinates), color_(color)
	{}

	static VertexAttribute::Info get_attribute_info(VertexAttribute::Name attribute_name);

	glm::vec3 position_;
	glm::vec3 normal_;
	glm::vec2 texture_coordinates_;
	glm::vec4 color_;
};