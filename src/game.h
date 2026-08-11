#pragma once

#include "RAII_SDL3/sdl.h"
#include "RAII_SDL3/window.h"
#include "glTF/node.h"
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
		void update_fps_count(Uint64& last_fps_refresh, unsigned int& frame_count_this_second) const;

		sdl::SDL sdl_;
		sdl::Window window_;
		GLenum glew_;
		std::unordered_map<std::string, ShaderProgram> shader_programs_;

		Player player_;
		Camera camera_;
		bool running_;
		sdl::Gamepad gamepad_;
		gltf::Node temp_model_;
		gltf::Node light_source_;
};