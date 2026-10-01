#pragma once

#include "Render/mesh.h"

#include <string_view>

class Image
{
	public:
		Image(std::string_view path);

		void render() const;

	private:
		Mesh::Id mesh_id_;
};