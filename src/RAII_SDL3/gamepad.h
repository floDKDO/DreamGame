#pragma once

#include <SDL3/SDL.h>

namespace sdl
{

class Gamepad
{
	public:
		Gamepad(); //SDL_OpenGamepad
		Gamepad(const Gamepad&) = delete;
		Gamepad(Gamepad&&) = delete;
		Gamepad& operator=(const Gamepad&) = delete;
		Gamepad& operator=(Gamepad&&) = delete;
		~Gamepad(); //SDL_CloseGamepad

		SDL_Gamepad* fetch() const;
		Sint16 get_axis(SDL_GamepadAxis axis) const;
		void check(Uint64 period);

	private:
		bool is_open() const;
		void open();

		SDL_Gamepad* gamepad_;

		//for the check method
		Uint64 last_check_time_;
		Uint64 current_check_time_;
		///////////////////////////
};

}