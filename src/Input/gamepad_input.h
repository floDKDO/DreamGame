#pragma once

#include "input_common.h"

#include <SDL3/SDL.h>

namespace input
{

class Gamepad //le fichier se nomme gamepad_input pour ne pas entrer en collision avec le fichier gamepad de RAII_SDL3/
{
	friend class InputManager;

	public:
		Gamepad();

		void handle_events(const SDL_Event& e);
		void update([[maybe_unused]] float delta_time);

	private:
		Info get_input_info() const;
		void set_pad_direction_active(Direction direction);
		void set_pad_direction_inactive(Direction direction);
		void set_direction_joystick(Sint16 axis_value, JoystickAxis joystick_axis);
		void set_rotation_joystick(Sint16 axis_value, JoystickAxis joystick_axis);

		Info input_info_;
		bool is_movement_from_joystick_;
};

}