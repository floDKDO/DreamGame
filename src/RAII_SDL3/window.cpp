#include "RAII_SDL3/window.h"

#include <string>
#include <iostream>

namespace sdl
{

Window::Window() 
	: window_icon_("resources/images/icon.png")
{
	set_opengl_attributes();

	if((window_ = SDL_CreateWindow("DreamGame, FPS: ", 1280, 720, /*SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED |*/ SDL_WINDOW_OPENGL)) == nullptr)
	{
		SDL_Log("(SDL_CreateWindow) %s\n", SDL_GetError());
	}

	set_hints();
	set_opengl_context();
	set_icon();
}

Window::~Window()
{
	if(window_ != nullptr)
	{
		if(!SDL_GL_DestroyContext(context_))
		{
			SDL_Log("(SDL_GL_DestroyContext) %s\n", SDL_GetError());
		}
		SDL_DestroyWindow(window_);
	}
}

SDL_Window* Window::fetch() const
{
	return window_;
}

void Window::set_opengl_attributes() const
{
	if(!SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE))
	{
		SDL_Log("(SDL_GL_SetAttribute) %s\n", SDL_GetError());
	}
	if(!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4))
	{
		SDL_Log("(SDL_GL_SetAttribute) %s\n", SDL_GetError());
	}
	if(!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5))
	{
		SDL_Log("(SDL_GL_SetAttribute) %s\n", SDL_GetError());
	}
	if(!SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG))
	{
		SDL_Log("(SDL_GL_SetAttribute) %s\n", SDL_GetError());
	}
}

void Window::set_hints() const
{
	//TODO : voir si je garde
	/*if(!SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_CURSOR_VISIBLE, "1"))
	{
		SDL_Log("(SDL_SetHint) %s\n", SDL_GetError());
	}*/
	if(!SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_CENTER, "0"))
	{
		SDL_Log("(SDL_SetHint) %s\n", SDL_GetError());
	}
}

void Window::set_opengl_context()
{
	if((context_ = SDL_GL_CreateContext(window_)) == nullptr)
	{
		SDL_Log("(SDL_GL_CreateContext) %s\n", SDL_GetError());
	}

	if(!(SDL_GL_MakeCurrent(window_, context_)))
	{
		SDL_Log("(SDL_GL_MakeCurrent) %s\n", SDL_GetError());
	}
}

void Window::set_icon() const
{
	if(!SDL_SetWindowIcon(window_, window_icon_.fetch()))
	{
		SDL_Log("(SDL_SetWindowIcon) %s\n", SDL_GetError());
	}
}

void Window::swap_buffers() const
{
	if(!SDL_GL_SwapWindow(window_))
	{
		SDL_Log("(SDL_GL_SwapWindow) %s\n", SDL_GetError());
	}
}

void Window::get_size(int* w, int* h) const
{
	if(!SDL_GetWindowSize(window_, w, h))
	{
		SDL_Log("(SDL_GetWindowSize) %s\n", SDL_GetError());
	}
}

SDL_GLContext Window::get_context() const
{
	return context_;
}

void Window::update_fps(unsigned int fps) const
{
	set_title("DreamGame, FPS: " + std::to_string(fps));
}

void Window::set_title(std::string_view title) const
{
	if(!SDL_SetWindowTitle(window_, title.data()))
	{
		SDL_Log("(SDL_SetWindowTitle) %s\n", SDL_GetError());
	}
}

void Window::set_relative_mouse_mode(bool enabled) const
{
	if(!SDL_SetWindowRelativeMouseMode(window_, enabled))
	{
		SDL_Log("(SDL_SetWindowRelativeMouseMode) %s\n", SDL_GetError());
	}
}

}