#pragma once

#include "Render/model.h"
#include "Input/input_manager.h"

class Player
{
	public:
		explicit Player(const InputManager& input_manager);

		void draw();
		void update(float delta_time, glm::vec3 camera_forward, glm::vec3 camera_left);

		std::unique_ptr<Model> model_;

	private:
		const InputManager& input_manager_;
};