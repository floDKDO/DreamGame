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
		SDL_GLContext get_context() const;
		void update_fps(unsigned int fps) const;
		void set_relative_mouse_mode(bool enabled) const;

	private:
		void set_opengl_attributes() const;
		void set_hints() const;
		void set_opengl_context();
		void set_icon() const;
		void set_title(std::string_view title) const;

		SDL_Window* window_;
		SDL_GLContext context_;
		sdl::Surface window_icon_;
};

}

