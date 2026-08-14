#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

struct Transform
{
	glm::vec3 position_ = glm::vec3(0.0f);
	glm::vec4 rotation_ = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); //TODO : inutile pour l'instant (utiliser à terme glm::quat ??)
	glm::vec3 scale_ = glm::vec3(1.0f);
};