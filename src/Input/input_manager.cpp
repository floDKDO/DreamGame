#include "input_manager.h"

#include <iostream>

InputManager::InputManager()
	: active_input_(ActiveInput::MOUSE_KEYBOARD)
{}

bool InputManager::is_input_active(input::Info input_info) const
{
	return input_info.x_movement_intensity_ != 0.0f || input_info.y_movement_intensity_ != 0.0f || input_info.x_rotation_intensity_ != 0.0f || input_info.y_rotation_intensity_ != 0.0f;
}

void InputManager::handle_events(const SDL_Event& e)
{
	mouse_keyboard_input_.handle_events(e);
	gamepad_input_.handle_events(e);
}

void InputManager::update(float delta_time)
{
	mouse_keyboard_input_.update(delta_time);
	gamepad_input_.update(delta_time);

	if(is_input_active(gamepad_input_.get_input_info()))
	{
		active_input_ = ActiveInput::GAMEPAD;
		//std::cout << "GAMEPAD IS ACTIVE!\n";
	}
	else if(is_input_active(mouse_keyboard_input_.get_input_info()))
	{
		active_input_ = ActiveInput::MOUSE_KEYBOARD;
		//std::cout << "MOUSE/KEYBOARD IS ACTIVE!\n";
	}

	if(active_input_ == ActiveInput::GAMEPAD)
	{
		//std::cout << "GAMEPAD IS ACTIVE! ";
		active_input_info_ = gamepad_input_.get_input_info();
	}
	else if(active_input_ == ActiveInput::MOUSE_KEYBOARD)
	{
		//std::cout << "MOUSE/KEYBOARD IS ACTIVE! ";
		active_input_info_ = mouse_keyboard_input_.get_input_info();
	}

	//std::cout << "=> x: " << active_input_info_.x_movement_intensity_ << ", y: " << active_input_info_.y_movement_intensity_ << ", rotation_x: " << active_input_info_.x_rotation_intensity_ << ", rotation_y: " << active_input_info_.y_rotation_intensity_ << std::endl;
}

input::Info InputManager::get_input_info() const
{
	return active_input_info_;
}