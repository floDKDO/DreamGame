#pragma once

#include "mouse_keyboard_input.h"
#include "gamepad_input.h"

namespace input
{

class InputManager
{
	public:
		InputManager();

		void handle_events(const SDL_Event& e);
		void update(float delta_time);
		Info get_input_info() const;

	private:
		bool is_input_active(Info input_info) const;

		ActiveDevice active_device_;
		Info input_info_;
		MouseKeyboard mouse_keyboard_input_;
		Gamepad gamepad_input_;
};

}