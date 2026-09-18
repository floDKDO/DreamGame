#include "aabb.h"

AABB::AABB(glm::vec3 min_values, glm::vec3 max_values, resource::MeshKey mesh_key)
	: min_values_(min_values), max_values_(max_values), mesh_key_(mesh_key)
{}

void AABB::draw()
{
	if(const Mesh* mesh = resource::get_aabb_mesh(mesh_key_); mesh != nullptr)
	{
		mesh->draw();
	}
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
	std::vector<glm::vec3> world_position_attributes
	{
		glm::vec3(max_values_.x, min_values_.y, min_values_.z), //0 : bottom face, upper right
		glm::vec3(min_values_.x, min_values_.y, min_values_.z), //1 : bottom face, upper left
		glm::vec3(max_values_.x, min_values_.y, max_values_.z), //2 : bottom face, down right
		glm::vec3(min_values_.x, min_values_.y, max_values_.z), //3 : bottom face, down left
		glm::vec3(max_values_.x, max_values_.y, min_values_.z), //4 : top face, upper right
		glm::vec3(min_values_.x, max_values_.y, min_values_.z), //5 : top face, upper left
		glm::vec3(max_values_.x, max_values_.y, max_values_.z), //6 : top face, down right
		glm::vec3(min_values_.x, max_values_.y, max_values_.z)  //7 : top face, down left
	};
	return world_position_attributes;
}