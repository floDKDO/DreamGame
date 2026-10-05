#include "font.h"

#include <iostream>

namespace sdl
{

Font::Font(std::string_view file, float ptsize) //TTF_OpenFont
{
	if((font_ = TTF_OpenFont(file.data(), ptsize)) == nullptr)
	{
		SDL_Log("(TTF_OpenFont) %s\n", SDL_GetError());
	}
}

Font::Font(Font&& font)
	: font_(font.font_)
{
	font.font_ = nullptr;
}

Font& Font::operator=(Font&& font)
{
	if(this == &font)
	{
		return *this;
	}

	if(font_ != nullptr)
	{
		TTF_CloseFont(font_);
	}

	font_ = font.font_;
	font.font_ = nullptr;
	return *this;
}

Font::~Font() //TTF_CloseFont
{
	if(font_ != nullptr)
	{
		TTF_CloseFont(font_);
	}
}

TTF_Font* Font::fetch() const
{
	return font_;
}

void Font::size_UTF8(std::string_view text, int* w, int* h) const
{
	if(!TTF_GetStringSize(font_, text.data(), text.size(), w, h))
	{
		SDL_Log("(TTF_GetStringSize) %s\n", SDL_GetError());
	}
}

void Font::set_style(int style) const
{
	TTF_SetFontStyle(font_, style);
}

void Font::set_size(float size) const
{
	if(!TTF_SetFontSize(font_, size))
	{
		SDL_Log("(TTF_SetFontSize) %s\n", SDL_GetError());
	}
}

}