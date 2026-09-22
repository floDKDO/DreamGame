#pragma once

#include "input_common.h"

#include <SDL3/SDL.h>

namespace input
{

class MouseKeyboard //le fichier se nomme mouse_keyboard_input pour garder la cohérence avec le fichier gamepad_input
{
	friend class InputManager;

	public:
		MouseKeyboard();

		void handle_events(const SDL_Event& e);
		void update([[maybe_unused]] float delta_time);

	private:
		Info get_input_info() const;
		void set_key_direction_active(Direction direction);
		void set_key_direction_inactive(Direction direction);
		void set_rotation_mouse(float xrel, float yrel);
		void mouse_motion_event_end();

		const static float max_mouse_rel_value_;
		Info input_info_;
		Uint64 mouse_motion_last_time_;
};

}