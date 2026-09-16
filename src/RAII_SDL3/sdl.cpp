#include "sdl.h"

#include <SDL3/SDL.h>
#include <iostream>

namespace sdl
{

SDL::SDL()
{
	if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
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