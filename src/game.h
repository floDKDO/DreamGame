#pragma once

#include "RAII_SDL3/sdl.h"
#include "RAII_SDL3/window.h"
#include "GLTF/model.h"
#include "Camera/camera.h"
#include "player.h"

class Game
{
	public:
		Game();

		void run();

	private:
		void handle_events();
		void draw();
		void update(float delta_time);
		void check_gamepad();
		void update_fps_count(std::string_view fps) const;

		sdl::SDL sdl_;
		sdl::Window window_;
		GLenum glew_;
		ShaderProgram shader_program_;
		glm::mat4 model_matrix_;
		glm::mat4 view_matrix_;
		Player player_;
		Camera camera_;
		bool running_;
		sdl::Gamepad gamepad_;
		Model temp_model_;
};