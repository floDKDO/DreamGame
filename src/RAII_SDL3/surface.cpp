#include "RAII_SDL3/surface.h"

namespace sdl
{

Surface::Surface(std::string_view file)
{
	if((surface_ = SDL_LoadPNG(file.data())) == nullptr)
	{
		SDL_Log("(SDL_LoadPNG) %s\n", SDL_GetError());
	}
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

}
