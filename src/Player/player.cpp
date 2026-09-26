#include "player.h"
#include "Collision/collision.h"

#include <iostream>

Player::Player(const input::InputManager& input_manager)
	: input_manager_(input_manager), model_(nullptr)
{
	std::size_t player_model_id = resource::add_model("resources/models/player.glb");
	model_ = resource::get_model(player_model_id);
	model_->add_translation_y(5.0f);
}

void Player::handle_collisions()
{
	for(auto& [model_id, model] : resource::get_models())
	{
		if(std::optional<std::pair<glm::vec3, AABB>> collision_info = collision::detection(model.get_root_node(), get_model()->get_root_node()); &model != get_model() && collision_info.has_value())
		{
			collision::TranslationInfo translation_info = collision::response(collision_info.value(), get_model()->get_root_node());
			if(translation_info.axis_ == collision::TranslationAxis::X)
			{
				model_->add_translation_x(translation_info.value_);
			}
			else if(translation_info.axis_ == collision::TranslationAxis::Y)
			{
				model_->add_translation_y(translation_info.value_);
			}
			else if(translation_info.axis_ == collision::TranslationAxis::Z)
			{
				model_->add_translation_z(translation_info.value_);
			}
		}
	}
}

void Player::update(float delta_time, glm::vec3 camera_forward, glm::vec3 camera_left)
{
	float sensitivity = 7.5f;
	input::Info input_info = input_manager_.get_input_info();

	if(input_info.y_movement_intensity_ != 0.0f)
	{
		model_->add_translation((input_info.y_movement_intensity_ * sensitivity * delta_time) * camera_forward);
	}

	if(input_info.x_movement_intensity_ != 0.0f)
	{
		model_->add_translation(-((input_info.x_movement_intensity_ * sensitivity * delta_time) * camera_left));
	}

	model_->add_translation(glm::vec3(0.0f, -0.1f, 0.0f)); //gravité
	//model_->rotate(glm::angleAxis(-glm::degrees(atan2((input_info.x_movement_intensity_ * sensitivity * delta_time)/* * camera_left.x*/, (input_info.y_movement_intensity_ * sensitivity * delta_time)/* * camera_forward.z*/)), glm::vec3(0.0f, 1.0f, 0.0f)));

	handle_collisions();	
}

Model* Player::get_model() const
{
	return model_;
}