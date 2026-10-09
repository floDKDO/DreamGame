#include "sdl_ttf.h"
#include "Logging/logging.h"

#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>

namespace sdl
{

SDLTTF::SDLTTF()
{
	if(!TTF_Init())
	{
		SDL_Log("(TTF_Init) %s\n", SDL_GetError());
	}
	logging::log("** Init TTF **", logging::Severity::DEBUG);
}

SDLTTF::~SDLTTF()
{
	TTF_Quit();
	logging::log("** Quit TTF **", logging::Severity::DEBUG);
}

}