#include "player.h"

#include <iostream>

Player::Player(InputManager& input_manager)
	: model_("resources/models/player.glb"), input_manager_(input_manager)
{}

void Player::draw(ShaderProgram& shader_program)
{
	model_.draw(shader_program);
}

void Player::update(float delta_time, glm::vec3 camera_forward, glm::vec3 camera_left)
{
	float sensitivity = 7.5f;
	input::Info input_info = input_manager_.get_input_info();

	if(input_info.y_movement_intensity_ != 0.0f)
	{
		model_.add_translation((input_info.y_movement_intensity_ * sensitivity * delta_time) * camera_forward);
	}

	if(input_info.x_movement_intensity_ != 0.0f)
	{
		model_.add_translation(-((input_info.x_movement_intensity_ * sensitivity * delta_time) * camera_left));
	}

	//model_.rotate(glm::angleAxis(-glm::degrees(atan2((input_info.x_movement_intensity_ * sensitivity * delta_time)/* * camera_left.x*/, (input_info.y_movement_intensity_ * sensitivity * delta_time)/* * camera_forward.z*/)), glm::vec3(0.0f, 1.0f, 0.0f)));
	//model_.transform_.position_.y = 0.75f; //TODO : hauteur du sol
}