#include "sdl_ttf.h"

#include <SDL3_ttf/SDL_ttf.h>

namespace sdl
{

SDLTTF::SDLTTF()
{
	if(!TTF_Init())
	{
		SDL_Log("(TTF_Init) %s\n", SDL_GetError());
	}
}

SDLTTF::~SDLTTF()
{
	TTF_Quit();
}

}