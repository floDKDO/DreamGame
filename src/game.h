#pragma once

#include "RAII_SDL3/window.h"
#include "backend.h"
#include "model.h"
#include "map.h"
#include "Camera/camera.h"
#include "player.h"
#include "Input/input_manager.h"

class Game
{
	public:
		Game();

		void run();

	private:
		void handle_events();
		void draw();
		void update(float delta_time);
		void update_fps_count(Uint64& last_fps_refresh, unsigned int& frame_count_this_second) const;

		Backend backend_;

		//sdl::Window window_;
		std::unordered_map<std::string, ShaderProgram> shader_programs_;
		InputManager input_manager_;

		Player player_;
		Camera camera_;
		bool running_;
		sdl::Gamepad gamepad_;
		Map test_map_;
		Model gizmo_;

		glm::mat4 perspective_projection_matrix_; //TODO : voir si je garde ici
};