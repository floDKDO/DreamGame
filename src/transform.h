#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

struct Transform
{
	glm::vec3 position_ = glm::vec3(0.0f);
	glm::quat rotation_ = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 scale_ = glm::vec3(1.0f);
};