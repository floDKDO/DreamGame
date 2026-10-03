#include "surface.h"

namespace sdl
{

Surface::Surface(std::string_view file)
{
	if((surface_ = SDL_LoadPNG(file.data())) == nullptr)
	{
		SDL_Log("(SDL_LoadPNG) %s\n", SDL_GetError());
	}
}

Surface::Surface(Font& font, std::string_view text, SDL_Color fg)
{
	if((surface_ = TTF_RenderText_Solid(font.fetch(), text.data(), text.size(), fg)) == nullptr)
	{
		SDL_Log("(TTF_RenderText_Solid) %s\n", SDL_GetError());
	}
	convert_to_rgba8();
}

Surface::~Surface() //SDL_DestroySurface
{
	if(surface_ != nullptr)
	{
		SDL_DestroySurface(surface_);
	}
}

SDL_Surface* Surface::fetch() const
{
	return surface_;
}

void Surface::convert_to_rgba8()
{
	if((surface_ = SDL_ConvertSurface(surface_, SDL_PIXELFORMAT_RGBA8888)) == nullptr)
	{
		SDL_Log("(SDL_ConvertSurface) %s\n", SDL_GetError());
	}
}

int Surface::get_width() const
{
	return surface_->w;
}

int Surface::get_height() const
{
	return surface_->h;
}

void* Surface::get_pixels() const
{
	return surface_->pixels;
}

}
