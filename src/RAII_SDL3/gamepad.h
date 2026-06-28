#pragma once

#include <SDL3/SDL.h>

namespace sdl
{

class Gamepad
{
	public:
		Gamepad(); //SDL_OpenGamepad
		Gamepad(const Gamepad& gamepad) = delete;
		Gamepad(Gamepad&& gamepad) = delete;
		Gamepad& operator=(const Gamepad& gamepad) = delete;
		Gamepad& operator=(Gamepad&& gamepad) = delete;
		~Gamepad(); //SDL_CloseGamepad

		SDL_Gamepad* fetch() const;
		Sint16 get_axis(SDL_GamepadAxis axis) const;
		bool is_open() const;
		void open();

		static const float joystick_deadzone_;

	private:
		SDL_Gamepad* gamepad_;
};

}