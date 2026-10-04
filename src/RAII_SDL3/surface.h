#pragma once

#include <SDL3/SDL.h>
#include <string_view>

#include "font.h"

namespace sdl
{

class Surface
{
	public:
		explicit Surface(std::string_view file); //SDL_LoadPNG
		Surface(Font& font, std::string_view text, SDL_Color fg); //TTF_RenderText_Solid();
		Surface(const Surface&) = delete;
		Surface(Surface&& surface);
		Surface& operator=(const Surface&) = delete;
		Surface& operator=(Surface&& surface);
		~Surface(); //SDL_DestroySurface

		SDL_Surface* fetch() const;
		void convert_to_rgba8();
		int get_width() const;
		int get_height() const;
		void* get_pixels() const;

	private:
		SDL_Surface* surface_;
};

}

