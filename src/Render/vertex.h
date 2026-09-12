#pragma once

#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>

namespace attribute
{

enum class Name : GLuint
{
	POSITION = 0,
	NORMAL = 1,
	TEXCOORD = 2,
	COLOR = 3
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
	static attribute::Info get_attribute_info(attribute::Name attribute_name);

	glm::vec3 position_ = glm::vec3(0.0f);
	glm::vec3 normal_ = glm::vec3(0.0f);
	glm::vec2 texcoord_ = glm::vec2(0.0f);
	glm::vec4 color_ = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
};