#include "RAII_SDL3/gamepad.h"

#include <iostream>

namespace sdl
{

const float Gamepad::joystick_deadzone_ = SDL_JOYSTICK_AXIS_MAX * 0.1f; //10% of the max value

Gamepad::Gamepad() //SDL_OpenGamepad
	: gamepad_(nullptr)
{
	open();
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

Sint16 Gamepad::get_axis(SDL_GamepadAxis axis) const
{
	return SDL_GetGamepadAxis(gamepad_, axis);
}

bool Gamepad::is_open() const
{
	return gamepad_ != nullptr;
}

void Gamepad::open()
{
	int count = 0;
	SDL_JoystickID* joysticks = nullptr;
	if((joysticks = SDL_GetGamepads(&count)) == nullptr)
	{
		//SDL_Log("(SDL_GetGamepads) %s\n", SDL_GetError()); //TODO : décommenter
	}

	//std::cout << count << " controller(s) connected!\n"; //TODO : décommenter

	if(joysticks != nullptr)
	{
		if((gamepad_ = SDL_OpenGamepad(joysticks[0])) == nullptr)
		{
			//SDL_Log("(SDL_OpenGamepad) %s\n", SDL_GetError()); //TODO : décommenter
		}
		SDL_free(joysticks);
	}
}

}