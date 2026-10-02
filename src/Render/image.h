#pragma once

#include "Render/mesh.h"

#include <glm/mat4x4.hpp>
#include <string_view>

class Image
{
	public:
		explicit Image(std::string_view path, glm::vec2 position = glm::vec2(0.0f), glm::vec2 size = glm::vec2(100.0f), float angle = 0.0f);

		void set_position(glm::vec2 position);
		void set_angle(float angle);
		void set_size(glm::vec2 size);
		void render() const;

	private:
		glm::mat4 get_model_matrix() const;

		glm::vec2 position_, size_;
		float angle_;
		Mesh::Id mesh_id_;
};