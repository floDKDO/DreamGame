#pragma once

#include <SDL3/SDL.h>
#include <string_view>

namespace sdl
{

class Surface
{
	public:
		explicit Surface(std::string_view file); //SDL_LoadPNG
		Surface(const Surface&) = delete;
		Surface(Surface&&) = delete;
		Surface& operator=(const Surface&) = delete;
		Surface& operator=(Surface&&) = delete;
		~Surface(); //SDL_DestroySurface

		SDL_Surface* fetch() const;

	private:
		SDL_Surface* surface_;
};

}

