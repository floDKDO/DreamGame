#include "RAII_SDL3/gamepad.h"

#include <iostream>

namespace sdl
{

const float Gamepad::joystick_deadzone_ = SDL_JOYSTICK_AXIS_MAX * 0.1f; //10% of the max value

Gamepad::Gamepad() //SDL_OpenGamepad
	: gamepad_(nullptr), last_check_time_(0), current_check_time_(0)
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

void Gamepad::check(Uint64 period)
{
	current_check_time_ = SDL_GetTicks();
	if(!is_open() && current_check_time_ > last_check_time_ + period)
	{
		open();
		last_check_time_ = current_check_time_;
	}
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
	else
	{
		//std::cout << count << " controller(s) connected!\n"; //TODO : décommenter

		if((gamepad_ = SDL_OpenGamepad(joysticks[0])) == nullptr)
		{
			//SDL_Log("(SDL_OpenGamepad) %s\n", SDL_GetError()); //TODO : décommenter
		}
		SDL_free(joysticks);
	}
}

}