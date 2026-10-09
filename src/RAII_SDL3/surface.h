#pragma once

#include <SDL3/SDL.h>
#include <string_view>

#include "font.h"

namespace sdl
{

class Surface
{
	public:
		Surface();
		Surface(const Surface&) = delete;
		Surface(Surface&& surface);
		Surface& operator=(const Surface&) = delete;
		Surface& operator=(Surface&& surface);
		~Surface();

		void load_png(std::string_view file);
		void render_text_solid(Font& font, std::string_view text, SDL_Color fg);
		void render_text_solid_wrapped(Font& font, std::string_view text, SDL_Color fg, int wrap_width);
		void render_text_lcd(Font& font, std::string_view text, SDL_Color fg);
		void render_text_lcd_wrapped(Font& font, std::string_view text, SDL_Color fg, int wrap_width);

		SDL_Surface* fetch() const;
		void clear();
		void convert_to_rgba8();
		int get_width() const;
		int get_height() const;
		void* get_pixels() const;

	private:
		SDL_Surface* surface_;
};

}

