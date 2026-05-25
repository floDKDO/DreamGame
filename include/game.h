#pragma once

#include "RAII_SDL3/sdl.h"
#include "RAII_SDL3/window.h"
#include "shader_program.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <unordered_map>

class Game
{
	public:
		Game();

		void run();
		void handle_events();
		void draw();
		void update();

	private:
		enum class ArrowKeys
		{
			UP,
			DOWN,
			LEFT,
			RIGHT
		};

		glm::mat4 look_at(glm::vec3 camera_position, glm::vec3 camera_target_position, glm::vec3 up_vector) const;
		void update_arrow_keys_state();

		sdl::SDL sdl_;
		sdl::Window window_;
		GLenum glew_;
		ShaderProgram shader_program_;
		glm::vec3 camera_position_;
		glm::mat4 model_;
		glm::mat4 view_;
		bool running_;
		std::unordered_map<ArrowKeys, bool> arrow_keys_state_; 
};