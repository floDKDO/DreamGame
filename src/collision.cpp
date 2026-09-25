#include "collision.h"

namespace collision
{

std::optional<std::pair<glm::vec3, AABB>> detection(const gltf::Node& map_node, const gltf::Node& player_node)
{
	if(!map_node.is_empty_node())
	{
		std::optional<AABB> map_node_aabb_optional = map_node.get_world_aabb();
		if(!map_node_aabb_optional.has_value())
		{
			return std::nullopt;
		}

		const AABB& map_node_aabb = map_node_aabb_optional.value();
		std::optional<AABB> player_node_aabb_optional = player_node.get_world_aabb();
		const AABB& player_node_aabb = player_node_aabb_optional.value(); //on part du principe que le joueur aura toujours un AABB

		if(player_node_aabb.intersection_with_aabb(map_node_aabb))
		{
			return std::make_pair(player_node_aabb.get_overlap_with_aabb(map_node_aabb), map_node_aabb);
		}
		else
		{
			return std::nullopt;
		}
	}

	for(const gltf::Node& child_node : map_node.get_children_nodes())
	{
		std::optional<std::pair<glm::vec3, AABB>> result = detection(child_node, player_node);
		if(result.has_value())
		{
			return result;
		}
	}
	return std::nullopt;
}

void response(const std::pair<glm::vec3, AABB>& collision_info, gltf::Node& player_node)
{
	const AABB& map_node_aabb = collision_info.second;
	std::optional<AABB> player_node_aabb_optional = player_node.get_world_aabb();
	const AABB& player_node_aabb = player_node_aabb_optional.value(); //on part du principe que le joueur aura toujours un AABB

	glm::vec3 player_center = player_node_aabb.get_center();
	glm::vec3 map_node_center = map_node_aabb.get_center();
	glm::vec3 overlap = collision_info.first;

	//on cherche le plus petit overlap car on veut déplacer le joueur de la plus petite distance possible pour qu'il ne soit plus en collision avec le modèle
	if(overlap.x < overlap.y && overlap.x < overlap.z)
	{
		player_node.add_translation_x((player_center.x < map_node_center.x) ? -overlap.x : overlap.x);
	}
	else if(overlap.y < overlap.z)
	{
		player_node.add_translation_y((player_center.y < map_node_center.y) ? -overlap.y : overlap.y);
	}
	else
	{
		player_node.add_translation_z((player_center.z < map_node_center.z) ? -overlap.z : overlap.z);
	}
}

}