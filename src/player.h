#pragma once

#include "glTF/node.h"
#include "RAII_SDL3/gamepad.h"
#include "Render/shader_program.h"

#include <SDL3/SDL.h>

class Player
{
	public:
		Player();

		void draw(ShaderProgram& shader_program);
		void handle_events(const SDL_Event& e);
		void update(float delta_time, glm::vec3 camera_forward, glm::vec3 camera_left);

		//glm::vec3 position_;
		gltf::Node model_;

	private:
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