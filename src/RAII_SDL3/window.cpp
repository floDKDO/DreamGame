#include "RAII_SDL3/window.h"

#include <iostream>

namespace sdl
{

Window::Window() 
	: window_icon_("resources/images/icon.png")
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

	if((window_ = SDL_CreateWindow("DreamGame", 1280, 720, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_OPENGL)) == nullptr)
	{
		SDL_Log("(SDL_CreateWindow) %s\n", SDL_GetError());
	}

	if((context_ = SDL_GL_CreateContext(window_)) == nullptr)
	{
		SDL_Log("(SDL_GL_CreateContext) %s\n", SDL_GetError());
	}

	if(!(SDL_GL_MakeCurrent(window_, context_)))
	{
		SDL_Log("(SDL_GL_MakeCurrent) %s\n", SDL_GetError());
	}

	if(!SDL_SetWindowIcon(window_, window_icon_.fetch())) 
	{
		SDL_Log("(SDL_SetWindowIcon) %s\n", SDL_GetError());
	}
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

void Window::swap_buffers() const
{
	if(!SDL_GL_SwapWindow(window_))
	{
		SDL_Log("(SDL_GL_SwapWindow) %s\n", SDL_GetError());
	}
}

}