#include "aabb.h"

#include <glm/glm.hpp>

AABB::AABB(glm::vec3 min_values, glm::vec3 max_values)
	: min_values_(min_values), max_values_(max_values)
{}

AABB::AABB(const std::vector<glm::vec3>& points)
	: min_values_(std::numeric_limits<float>::max()), max_values_(std::numeric_limits<float>::lowest()) //pas std::numeric_limits<float>::min() qui ne retourne pas la valeur escomptée !
{
	for(const glm::vec3& point : points)
	{
		min_values_ = glm::min(point, min_values_);
		max_values_ = glm::max(point, max_values_);
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
	std::vector<glm::vec3> aabb_points
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
	return aabb_points;
}

glm::vec3 AABB::get_center() const
{
	return (min_values_ + max_values_) / 2.0f;
}

bool AABB::intersection_with_aabb(const AABB& aabb) const
{
	return (aabb.min_values_.x <= max_values_.x && aabb.max_values_.x >= min_values_.x
		 && aabb.min_values_.y <= max_values_.y && aabb.max_values_.y >= min_values_.y
		 && aabb.min_values_.z <= max_values_.z && aabb.max_values_.z >= min_values_.z);
}

glm::vec3 AABB::get_overlap_with_aabb(const AABB& aabb) const
{
	return glm::vec3(
		std::min(max_values_.x, aabb.max_values_.x) - std::max(min_values_.x, aabb.min_values_.x), //la valeur obtenue représente de combien en x le AABB en paramètre est entré dans ce AABB
		std::min(max_values_.y, aabb.max_values_.y) - std::max(min_values_.y, aabb.min_values_.y),
		std::min(max_values_.z, aabb.max_values_.z) - std::max(min_values_.z, aabb.min_values_.z)
	);
}