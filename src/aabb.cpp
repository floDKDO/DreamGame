#include "aabb.h"

AABB::AABB(glm::vec3 min_values, glm::vec3 max_values, std::vector<GLushort> ebo_values, Vertices vertices, std::vector<Texture> textures, GLenum draw_mode)
	: mesh_(ebo_values, vertices, textures, draw_mode), min_values_(min_values), max_values_(max_values)
{}

AABB::AABB(glm::vec3 min_values, glm::vec3 max_values, std::vector<GLushort> ebo_values, Vertices vertices, GLenum draw_mode)
	: AABB(min_values, max_values, ebo_values, vertices, {}, draw_mode)
{}

void AABB::draw(ShaderProgram& shader_program)
{
	mesh_.draw(shader_program);
}

glm::vec3 AABB::get_min_values() const
{
	return min_values_;
}

glm::vec3 AABB::get_max_values() const
{
	return max_values_;
}