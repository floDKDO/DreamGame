#pragma once

#include "RAII_SDL3/sdl.h"
#include "RAII_SDL3/window.h"

#include <GL/glew.h>

class Backend
{
	public:
		Backend();

		void get_window_size(int* w, int* h) const;
		void swap_window_buffers() const;

	private:
		void init_imgui() const;

		sdl::SDL sdl_;
		sdl::Window window_;
		GLenum glew_;
};

void GLAPIENTRY message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, [[maybe_unused]] const void* user_param);