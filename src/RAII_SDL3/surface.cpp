#include "surface.h"

namespace sdl
{

Surface::Surface()
	: surface_(nullptr)
{}

void Surface::load_png(std::string_view file)
{
	clear();
	if((surface_ = SDL_LoadPNG(file.data())) == nullptr)
	{
		SDL_Log("(SDL_LoadPNG) %s\n", SDL_GetError());
	}
}

void Surface::render_text_solid(Font& font, std::string_view text, SDL_Color fg)
{
	clear();
	if((surface_ = TTF_RenderText_Solid(font.fetch(), text.data(), text.size(), fg)) == nullptr)
	{
		SDL_Log("(TTF_RenderText_Solid) %s\n", SDL_GetError());
	}
	convert_to_rgba8();
}

void Surface::render_text_solid_wrapped(Font& font, std::string_view text, SDL_Color fg, int wrap_width)
{
	clear();
	if((surface_ = TTF_RenderText_Solid_Wrapped(font.fetch(), text.data(), text.size(), fg, wrap_width)) == nullptr)
	{
		SDL_Log("(TTF_RenderText_Solid_Wrapped) %s\n", SDL_GetError());
	}
	convert_to_rgba8();
}

void Surface::render_text_blended(Font& font, std::string_view text, SDL_Color fg)
{
	clear();
	if((surface_ = TTF_RenderText_Blended(font.fetch(), text.data(), text.size(), fg)) == nullptr)
	{
		SDL_Log("(TTF_RenderText_Blended) %s\n", SDL_GetError());
	}
	convert_to_rgba8();
}

void Surface::render_text_blended_wrapped(Font& font, std::string_view text, SDL_Color fg, int wrap_width)
{
	clear();
	if((surface_ = TTF_RenderText_Blended_Wrapped(font.fetch(), text.data(), text.size(), fg, wrap_width)) == nullptr)
	{
		SDL_Log("(TTF_RenderText_Blended_Wrapped) %s\n", SDL_GetError());
	}
	convert_to_rgba8();
}

Surface::Surface(Surface&& surface)
	: surface_(surface.surface_)
{
	surface.surface_ = nullptr;
}

Surface& Surface::operator=(Surface&& surface)
{
	if(this == &surface)
	{
		return *this;
	}

	if(surface_ != nullptr)
	{
		SDL_DestroySurface(surface_);
	}

	surface_ = surface.surface_;
	surface.surface_ = nullptr;
	return *this;
}

Surface::~Surface()
{
	clear();
}

SDL_Surface* Surface::fetch() const
{
	return surface_;
}

void Surface::clear()
{
	if(surface_ != nullptr)
	{
		SDL_DestroySurface(surface_);
	}
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
