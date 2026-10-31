#pragma once

#include "Backend/backend.h"
#include "Input/input_manager.h"

class Game
{
	public:
		Game();

		void run();

	private:
		void handle_events();
		void render();
		void update(float delta_time);
		void update_fps_count(Uint64& last_fps_refresh, unsigned int& frame_count_this_second) const;

		Backend backend_;
		input::InputManager input_manager_;
		bool running_;
};