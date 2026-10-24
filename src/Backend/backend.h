#pragma once

#include "RAII_SDL3/sdl.h"
#include "RAII_SDL3/window.h"
#include "RAII_SDL3/sdl_ttf.h"

#include <glm/vec2.hpp>
#include <AL/alc.h>
#include <GL/glew.h>

class Backend
{
	public:
		Backend();
		~Backend();
		Backend(const Backend&) = delete;
		Backend(Backend&&) = delete;
		Backend& operator=(const Backend&) = delete;
		Backend& operator=(Backend&&) = delete;

		void begin_frame() const;
		void end_frame() const;
		void stencil_test(bool enabled);
		void depth_test(bool enabled);
		void depth_write(bool enabled);
		glm::ivec2 get_window_size() const;
		void swap_window_buffers() const;
		void update_window_fps(unsigned int frame_count_this_second) const;
		void handle_events(const SDL_Event& e);
		void reset_opengl_states() const;

	private:
		struct PixelProcessingState
		{
			bool stencil_test_ = false;
			GLuint stencil_buffer_writing_ = 0xFFFFFFFF;
			GLenum stencil_func_ = GL_ALWAYS;
			GLint stencil_ref_ = 0;
			GLuint stencil_mask_ = 0xFFFFFFFF;
			GLenum stencil_sfail_ = GL_KEEP;
			GLenum stencil_dpfail_ = GL_KEEP;
			GLenum stencil_dppass_ = GL_KEEP;
			GLint stencil_clear_value_ = 0;

			bool depth_test_ = true;
			GLboolean depth_buffer_writing_ = GL_TRUE;
			GLenum depth_func_ = GL_LESS;
			GLclampd depth_clear_value_ = 1.0;
		};

		void init_pixel_processing_state() const;
		void init_viewport() const;
		void init_debug() const;
		void init_shaders() const;
		void init_imgui() const;
		void destroy_imgui() const;
		void init_openal();
		void destroy_openal();

		sdl::SDL sdl_;
		sdl::SDLTTF sdl_ttf_;
		sdl::Window window_;
		GLenum glew_;
		ALCdevice* device_;
		ALCcontext* context_;
		PixelProcessingState pixel_processing_state_;
};

void GLAPIENTRY message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, [[maybe_unused]] const void* user_param);
void print_opengl_stuff();