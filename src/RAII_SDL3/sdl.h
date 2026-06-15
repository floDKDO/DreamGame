#pragma once

#include <SDL3/SDL.h>

namespace sdl
{

class SDL
{
	public:
		SDL();
		SDL(const SDL& sdl) = delete;
		SDL(SDL&& sdl) = delete;
		SDL& operator=(const SDL& sdl) = delete;
		SDL& operator=(SDL&& sdl) = delete;
		~SDL();
};

}
