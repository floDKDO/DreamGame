#include "RAII_SDL3/sdl.h"

#include <iostream>

namespace sdl
{

SDL::SDL()
{
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD) < 0)
	{
		SDL_Log("(SDL_Init) %s\n", SDL_GetError());
	}

	//TODO : SteamAPI_InitEx()
}

SDL::~SDL()
{
	SDL_Quit();
}

}