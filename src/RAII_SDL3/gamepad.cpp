#include "RAII_SDL3/gamepad.h"

namespace sdl
{

Gamepad::Gamepad() //SDL_OpenGamepad
{
	if((gamepad_ = SDL_OpenGamepad(0)) == nullptr)
	{
		SDL_Log("(SDL_OpenGamepad) %s\n", SDL_GetError());
	}
}

Gamepad::~Gamepad() //SDL_CloseGamepad
{
	if(gamepad_ != nullptr)
	{
		SDL_CloseGamepad(gamepad_);
	}
}

SDL_Gamepad* Gamepad::fetch() const
{
	return gamepad_;
}

}