#include "gamepad.h"

#include <SDL3/SDL_timer.h>
#include <iostream>

namespace sdl
{

Gamepad::Gamepad() //SDL_OpenGamepad
	: gamepad_(nullptr), last_check_time_(0), current_check_time_(0)
{
	open();
}

Gamepad::Gamepad(Gamepad&& gamepad)
	: gamepad_(gamepad.gamepad_), last_check_time_(gamepad.last_check_time_), current_check_time_(gamepad.current_check_time_)
{
	gamepad.gamepad_ = nullptr;
}

Gamepad& Gamepad::operator=(Gamepad&& gamepad)
{
	if(this == &gamepad)
	{
		return *this;
	}

	destroy();

	gamepad_ = gamepad.gamepad_;
	last_check_time_ = gamepad.last_check_time_;
	current_check_time_ = gamepad.current_check_time_;
	gamepad.gamepad_ = nullptr;
	return *this;
}

Gamepad::~Gamepad() //SDL_CloseGamepad
{
	destroy();
}

SDL_Gamepad* Gamepad::fetch() const
{
	return gamepad_;
}

void Gamepad::destroy()
{
	if(gamepad_ != nullptr)
	{
		SDL_CloseGamepad(gamepad_);
		gamepad_ = nullptr;
	}
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
	if(SDL_JoystickID* joysticks = SDL_GetGamepads(&count); joysticks == nullptr)
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