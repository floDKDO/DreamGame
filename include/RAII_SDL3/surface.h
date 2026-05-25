#pragma once

#include <SDL3/SDL.h>
#include <string_view>

namespace sdl
{

class Surface
{
	public:
		explicit Surface(std::string_view file); //SDL_LoadPNG
		Surface(const Surface& surface) = delete;
		Surface(Surface&& surface) = delete;
		Surface& operator=(const Surface& surface) = delete;
		Surface& operator=(Surface&& surface) = delete;
		~Surface(); //SDL_DestroySurface

		SDL_Surface* fetch() const;

	private:
		SDL_Surface* surface_;
};

}

