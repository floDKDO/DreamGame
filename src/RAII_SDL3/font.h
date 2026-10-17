#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <string_view>

namespace sdl
{

class Font
{
	public:
		struct Id
		{
			std::string font_path_;
			float font_size_;

			bool operator==(const Id& other) const
			{
				return (font_path_ == other.font_path_ && font_size_ == other.font_size_);
			}
		};

		Font(std::string_view file, float ptsize); //TTF_OpenFont
		Font(TTF_Font* copied_font, float ptsize);
		Font(const Font& font) = delete;
		Font(Font&& font);
		Font& operator=(const Font& font) = delete;
		Font& operator=(Font&& font);
		~Font(); //TTF_CloseFont

		TTF_Font* fetch() const;
		void set_style(int style) const;
		void set_size(float size) const;
		TTF_Font* copy() const;

	private:
		void close();

		TTF_Font* font_;
};

}

template <>
struct std::hash<sdl::Font::Id>
{
	std::size_t operator()(const sdl::Font::Id& font_id) const
	{
		return (std::hash<std::string>()(font_id.font_path_) ^ (std::hash<float>()(font_id.font_size_) << 1));
	}
};