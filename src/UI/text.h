#pragma once

#include "Render/mesh.h"
#include "RAII_SDL3/font.h"
#include "RAII_SDL3/surface.h"

#include <glm/mat4x4.hpp>
#include <string_view>

class Text
{
	public:
		enum class Quality
		{
			SOLID,
			LCD
		};

		struct FontId
		{
			std::string font_path_;
			float font_size_;

			bool operator==(const FontId& other) const
			{
				return (font_path_ == other.font_path_ && font_size_ == other.font_size_);
			}
		};

		explicit Text(std::string_view text);
		Text(std::string_view text, int wrap_length);

		void set_wrapped(int wrap_length);
		void set_position(glm::vec2 position);
		void set_font_size(float font_size);
		void set_angle(float angle);
		void edit_text(std::string_view new_text);
		void clear();
		void add_char(char c);
		void render() const;

	private:
		glm::mat4 get_model_matrix() const;
		void init_surface_from_text();

		SDL_Color color_;

		bool is_wrapped_;
		int wrap_length_;

		Quality quality_;
		FontId font_id_;
		sdl::Surface surface_;
		std::string text_;
		glm::vec2 position_;
		float angle_;
		Mesh::Id mesh_id_;
};

template <>
struct std::hash<Text::FontId>
{
	std::size_t operator()(const Text::FontId& font_id) const
	{
		return (std::hash<std::string>()(font_id.font_path_) ^ (std::hash<float>()(font_id.font_size_) << 1));
	}
};