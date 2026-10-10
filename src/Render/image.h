#pragma once

#include "Render/mesh.h"

#include <glm/mat4x4.hpp>
#include <string_view>

class Image
{
	public:
		explicit Image(std::string_view path);

		glm::vec2 get_position() const;
		float get_width() const;
		float get_height() const;

		void set_position(glm::vec2 position);
		void set_angle(float angle);
		void set_size(glm::vec2 size);
		void render() const;

	private:
		glm::mat4 get_model_matrix() const;
		
		int initial_texture_width_, initial_texture_height_;
		glm::vec2 position_, size_;
		float angle_;
		Mesh::Id mesh_id_;
};