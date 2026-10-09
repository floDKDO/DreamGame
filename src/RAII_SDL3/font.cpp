#include "font.h"
#include "Logging/logging.h"

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

Font::Font(TTF_Font* copied_font, float ptsize)
{
	font_ = copied_font;
	set_size(ptsize);
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
		if(!TTF_WasInit())
		{
			logging::log("TTF is not initalized before a call to TTF_CloseFont()!", logging::Severity::CRITICAL);
		}
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
		if(!TTF_WasInit())
		{
			logging::log("TTF is not initalized before a call to TTF_CloseFont()!\n", logging::Severity::CRITICAL);
		}
		TTF_CloseFont(font_);
	}
}

TTF_Font* Font::fetch() const
{
	return font_;
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

TTF_Font* Font::copy() const
{
	TTF_Font* font = nullptr;
	if((font = TTF_CopyFont(font_)) == nullptr)
	{
		SDL_Log("(TTF_CopyFont) %s\n", SDL_GetError());
	}
	return font;
}

}