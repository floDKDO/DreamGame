#pragma once

#include "RAII_SDL3/gamepad.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <SDL3/SDL.h>

class CameraMovement
{
	public:
		CameraMovement(glm::mat4& view, glm::vec3& camera_position_, glm::vec3& camera_front_);

		void handle_events(const SDL_Event& e);
		void update();

		glm::vec3& camera_position_;
		glm::vec3& camera_front_;

	private:
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
		glm::mat4& view_;
		bool is_arrow_key_pressed_;
		bool is_pad_pressed_;
		bool is_movement_from_joystick_;
		float x_movement_intensity_;
		float y_movement_intensity_;
};