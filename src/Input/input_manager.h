#pragma once

#include "mouse_keyboard_input.h"
#include "gamepad_input.h"

class InputManager
{
	public:
		InputManager();

		void handle_events(const SDL_Event& e);
		void update(float delta_time);
		input::Info get_input_info() const;

	private:
		enum class ActiveInput
		{
			MOUSE_KEYBOARD,
			GAMEPAD
		};

		bool is_input_active(input::Info input_info) const;

		ActiveInput active_input_;
		input::Info active_input_info_;
		MouseKeyboardInput mouse_keyboard_input_;
		GamepadInput gamepad_input_;
};