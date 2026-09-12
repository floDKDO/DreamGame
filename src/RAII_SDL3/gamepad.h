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
		void check(Uint64 period);

	private:
		bool is_open() const;
		void open();

		//for the check method
		Uint64 last_check_time_;
		Uint64 current_check_time_;
		///////////////////////////

		SDL_Gamepad* gamepad_;
};

}