#pragma once

#include "RAII_SDL3/window.h"
#include "backend.h"
#include "glTF/scene.h"
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
		InputManager intput_manager_;

		//Player player_;
		glm::vec3 temp_; //TODO : à retirer
		Camera camera_;
		bool running_;
		sdl::Gamepad gamepad_;
		//gltf::Model temp_model_;
		//gltf::Model light_source_;
		gltf::Scene test_scene_;
};