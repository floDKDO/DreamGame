#pragma once

#include "Render/model.h"
#include "Render/shader_program.h"
#include "Input/input_manager.h"

class Player
{
	public:
		Player(InputManager& input_manager);

		void draw(ShaderProgram& shader_program);
		void update(float delta_time, glm::vec3 camera_forward, glm::vec3 camera_left);

		Model model_;

	private:
		InputManager& input_manager_;
};