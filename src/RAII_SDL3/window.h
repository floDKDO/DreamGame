#pragma once

#include "RAII_SDL3/surface.h"

namespace sdl
{

class Window
{
	public:
		Window(); 
		Window(const Window& window) = delete;
		Window(Window&& window) = delete;
		Window& operator=(const Window& window) = delete;
		Window& operator=(Window&& window) = delete;
		~Window(); 

		SDL_Window* fetch() const;
		void swap_buffers() const;
		void get_size(int* w, int* h) const;

	private:
		SDL_Window* window_;
		SDL_GLContext context_;
		sdl::Surface window_icon_;
};

}

