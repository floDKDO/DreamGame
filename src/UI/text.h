#pragma once

#include "RAII_SDL3/surface.h"

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <string>
#include <string_view>

class Text
{
	public:
		enum class Quality
		{
			SOLID,
			LCD
		};

		explicit Text(std::string_view text, int wrap_length = 0);

		void set_position(glm::vec2 position);
		void set_font_size(float font_size);
		void set_angle(float angle);
		void edit_text(std::string_view new_text);
		void add_char(char c);
		void render() const;

	private:
		glm::mat4 get_model_matrix() const;
		void init_surface_from_text();

		SDL_Color color_;

		bool is_wrapped_;
		int wrap_length_;

		Quality quality_;
		std::string font_path_;
		std::size_t font_id_;
		sdl::Surface surface_;
		std::string text_;
		glm::vec2 position_;
		float angle_;
		std::size_t mesh_id_;
};