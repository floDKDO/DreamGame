#pragma once

#include "Render/model.h"
#include "Input/input_manager.h"

class Player
{
	public:
		explicit Player(const input::InputManager& input_manager);

		void draw();
		void update(float delta_time, glm::vec3 camera_forward, glm::vec3 camera_left);
		Model* get_model() const;

	private:
		const input::InputManager& input_manager_;
		std::unique_ptr<Model> model_;
};