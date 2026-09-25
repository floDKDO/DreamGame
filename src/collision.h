#pragma once

#include "glTF/node.h"

namespace collision
{

std::optional<std::pair<glm::vec3, AABB>> detection(const gltf::Node& map_node, const gltf::Node& player_node);
void response(const std::pair<glm::vec3, AABB>& collision_info, gltf::Node& player_node);

}