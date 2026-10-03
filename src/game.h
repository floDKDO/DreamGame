#pragma once

#include "Backend/backend.h"
#include "Resource/model.h"
#include "Map/map_file.h"
#include "Camera/camera.h"
#include "Player/player.h"
#include "Input/input_manager.h"
#include "Render/image.h"
#include "Render/text.h"

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
		Player player_;
		Camera camera_;
		bool running_;
		//Map test_map_;
		MapFile test_map_;
		Model* gizmo_;
		Image test_image_;
		Image test_image_2_;
		Text test_text_;
		Text test_text_2_;
};