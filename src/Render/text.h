#pragma once

#include "Render/mesh.h"
#include "RAII_SDL3/font.h"
#include "RAII_SDL3/surface.h"

#include <glm/mat4x4.hpp>
#include <string_view>

class Text
{
	public:
	explicit Text(std::string_view text, SDL_Color color = {255, 255, 255, 255}, glm::vec2 position = glm::vec2(0.0f), float font_size = 24.0f, float angle = 0.0f);

		void set_position(glm::vec2 position);
		void set_font_size(float font_size);
		void set_angle(float angle);
		void edit_text(std::string_view new_text);
		void render() const;

	private:
		glm::mat4 get_model_matrix() const;

		SDL_Color color_;
		sdl::Font font_;
		sdl::Surface surface_;
		std::string text_;
		glm::vec2 position_;
		float font_size_, angle_;
		Mesh::Id mesh_id_;
};