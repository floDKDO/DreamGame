#pragma once

#include "GLTF/model.h"
#include "RAII_SDL3/gamepad.h"

#include <SDL3/SDL.h>

class Player
{
	public:
		Player(glm::mat4& view_matrix);

		void draw(ShaderProgram& shader_program);
		void handle_events(const SDL_Event& e);
		void update();

		glm::mat4& view_matrix_;
		glm::vec3 position_;

	private:
		Model model_;

		///////////////////////////////////
		enum class Direction
		{
			UP,
			DOWN,
			LEFT,
			RIGHT
		};

		void set_key_direction_active(Direction direction);
		void set_key_direction_inactive(Direction direction);
		void set_direction_joystick(Sint16 axis_value, sdl::Gamepad::JoystickAxis joystick_axis);

		const static float max_movement_intensity_;
		bool is_arrow_key_pressed_;
		bool is_pad_pressed_;
		bool is_movement_from_joystick_;
		float x_movement_intensity_;
		float y_movement_intensity_;
		///////////////////////////////////
};