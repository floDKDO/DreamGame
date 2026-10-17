#pragma once

#include "aabb.h"

#include <optional>

struct AABBInfo
{
	std::size_t mesh_id_ = std::numeric_limits<std::size_t>::max();
	std::optional<AABB> aabb_ = std::nullopt;
};