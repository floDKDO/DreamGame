#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include <string_view>

namespace sdl
{

class Font
{
	public:
		Font(std::string_view file, float ptsize); //TTF_OpenFont
		Font(const Font& font) = delete;
		Font(Font&& font);
		Font& operator=(const Font& font) = delete;
		Font& operator=(Font&& font);
		~Font(); //TTF_CloseFont

		TTF_Font* fetch() const;
		void size_UTF8(std::string_view text, int* w, int* h) const;
		void set_style(int style) const;
		void set_size(float size) const;

	private:
		TTF_Font* font_;
};

}