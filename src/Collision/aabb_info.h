#pragma once

#include "aabb.h"
#include "Common/constants.h"

#include <optional>

struct AABBInfo
{
	std::size_t mesh_id_ = constants::undefined_resource_id_;
	std::optional<AABB> aabb_ = std::nullopt;
};