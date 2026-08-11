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
		ShaderProgram shader_program_;
		ShaderProgram shader_program_phong_;
		//glm::mat4 model_matrix_;
		glm::mat4 view_matrix_; //TODO : placée ici car partagée entre la caméra et le joueur (peut-être la placer à un autre endroit)
		Player player_;
		Camera camera_;
		bool running_;
		sdl::Gamepad gamepad_;
		gltf::Node temp_model_;
		gltf::Node light_source_;
};