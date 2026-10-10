#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include <string_view>

namespace sdl
{

class Font
{
	public:
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