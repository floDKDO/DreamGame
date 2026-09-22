#pragma once

#include "RAII_SDL3/sdl.h"
#include "RAII_SDL3/window.h"

#include <glm/vec2.hpp>
#include <AL/alc.h>
#include <GL/glew.h>

class Backend
{
	public:
		Backend();
		~Backend();
		Backend(const Backend& backend) = delete;
		Backend(Backend&& backend) = delete;
		Backend& operator=(const Backend& backend) = delete;
		Backend& operator=(Backend&& backend) = delete;

		glm::ivec2 get_window_size() const;
		void swap_window_buffers() const;
		void handle_events(const SDL_Event& e);

	private:
		void init_imgui() const;
		void destroy_imgui() const;
		void init_openal();
		void destroy_openal();

		sdl::SDL sdl_;
		sdl::Window window_;
		GLenum glew_;
		ALCdevice* device_;
		ALCcontext* context_;
};

void GLAPIENTRY message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, [[maybe_unused]] const void* user_param);
void print_opengl_stuff();