#pragma once

#include "RAII_SDL3/gamepad.h"
#include "Backend/backend.h"
#include "Render/model.h"
#include "Map/map.h"
#include "Camera/camera.h"
#include "Player/player.h"
#include "Input/input_manager.h"

class Game
{
	public:
		Game();

		void run();

	private:
		void handle_events();
		void draw();
		void update(float delta_time);
		void update_fps_count(Uint64& last_fps_refresh, unsigned int& frame_count_this_second) const;

		Backend backend_;
		input::InputManager input_manager_;
		Player player_;
		Camera camera_;
		bool running_;
		sdl::Gamepad gamepad_;
		Map test_map_;
		Model gizmo_;
};