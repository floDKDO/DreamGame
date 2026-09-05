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

std::vector<glm::vec3> AABB::get_corners() const
{
	std::vector<glm::vec3> world_position_attributes;
	world_position_attributes.reserve(8);
	world_position_attributes.push_back(glm::vec3(max_values_.x, min_values_.y, min_values_.z)); //0 : bottom face, upper right
	world_position_attributes.push_back(glm::vec3(min_values_.x, min_values_.y, min_values_.z)); //1 : bottom face, upper left
	world_position_attributes.push_back(glm::vec3(max_values_.x, min_values_.y, max_values_.z)); //2 : bottom face, down right
	world_position_attributes.push_back(glm::vec3(min_values_.x, min_values_.y, max_values_.z)); //3 : bottom face, down left
	world_position_attributes.push_back(glm::vec3(max_values_.x, max_values_.y, min_values_.z)); //4 : top face, upper right
	world_position_attributes.push_back(glm::vec3(min_values_.x, max_values_.y, min_values_.z)); //5 : top face, upper left
	world_position_attributes.push_back(glm::vec3(max_values_.x, max_values_.y, max_values_.z)); //6 : top face, down right
	world_position_attributes.push_back(glm::vec3(min_values_.x, max_values_.y, max_values_.z)); //7 : top face, down left
	return world_position_attributes;
}