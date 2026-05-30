#pragma once

#include "RAII_SDL3/sdl.h"
#include "RAII_SDL3/window.h"
#include "RAII_SDL3/gamepad.h"
#include "shader_program.h"

#include <glm/gtc/matrix_transform.hpp>
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
		enum class Direction
		{
			UP,
			DOWN,
			LEFT,
			RIGHT
		};

		struct DirectionInfo
		{
			bool is_direction_;
			bool is_from_joystick_;
			float x_intensity_; //joystick value
			float y_intensity_; //joystick value
		};

		glm::mat4 look_at(glm::vec3 camera_position, glm::vec3 camera_target_position, glm::vec3 up_vector) const;
		void get_info_on_gltf_file(std::string_view path);

		sdl::SDL sdl_;
		sdl::Window window_;
		GLenum glew_;
		ShaderProgram shader_program_;
		glm::vec3 camera_position_;
		glm::mat4 model_;
		glm::mat4 view_;
		bool running_;
		sdl::Gamepad gamepad_;
		std::unordered_map<Direction, DirectionInfo> direction_state_;
};