#pragma once

#include "RAII_SDL3/sdl.h"
#include "RAII_SDL3/window.h"
#include "GLTF/model.h"
#include "Camera/camera.h"

class Game
{
	public:
		Game();

		void run();

	private:
		void handle_events();
		void draw();
		void update();
		void check_gamepad();

		sdl::SDL sdl_;
		sdl::Window window_;
		GLenum glew_;
		ShaderProgram shader_program_;
		glm::mat4 model_;
		glm::mat4 view_;
		Camera camera_;
		bool running_;
		sdl::Gamepad gamepad_;
		Model temp_model_;
};