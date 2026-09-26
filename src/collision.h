#pragma once

#include "glTF/node.h"

namespace collision
{

enum class TranslationAxis
{
	UNKNOWN,
	X,
	Y,
	Z
};

struct TranslationInfo
{
	TranslationAxis axis_ = TranslationAxis::UNKNOWN;
	float value_ = 0.0f;
};

std::optional<std::pair<glm::vec3, AABB>> detection(const gltf::Node* map_node, const gltf::Node* player_node);
TranslationInfo response(const std::pair<glm::vec3, AABB>& collision_info, const gltf::Node* player_node);

}