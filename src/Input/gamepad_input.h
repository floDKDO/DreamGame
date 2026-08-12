#pragma once

#include "input_common.h"
#include "RAII_SDL3/gamepad.h"

class GamepadInput
{
	public:
		GamepadInput();

		void handle_events(const SDL_Event& e);
		void update(float delta_time);
		input::Info get_input_info() const;

	private:
		void set_pad_direction_active(input::Direction direction);
		void set_pad_direction_inactive(input::Direction direction);
		void set_direction_joystick(Sint16 axis_value, sdl::Gamepad::JoystickAxis joystick_axis);
		void set_rotation_joystick(Sint16 axis_value, sdl::Gamepad::JoystickAxis joystick_axis);

		input::Info input_info_;
		bool is_movement_from_joystick_;
};