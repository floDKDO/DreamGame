#pragma once

#include "input_common.h"

#include <SDL3/SDL.h>

class MouseKeyboardInput
{
	public:
		MouseKeyboardInput();

		void handle_events(const SDL_Event& e);
		void update(float delta_time);
		input::Info get_input_info() const;

	private:
		void set_key_direction_active(input::Direction direction);
		void set_key_direction_inactive(input::Direction direction);
		void set_rotation_mouse(float xrel, float yrel);
		void mouse_motion_event_end();

		const static float max_mouse_rel_value_;
		input::Info input_info_;
		Uint64 mouse_motion_last_time_;
};