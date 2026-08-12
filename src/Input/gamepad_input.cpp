#include "gamepad_input.h"

#include <iostream>

GamepadInput::GamepadInput()
	: is_movement_from_joystick_(false)
{}

void GamepadInput::set_pad_direction_active(input::Direction direction)
{
	if(direction == input::Direction::UP)
	{
		input_info_.y_movement_intensity_ = input::max_movement_intensity_;
	}
	if(direction == input::Direction::DOWN)
	{
		input_info_.y_movement_intensity_ = -input::max_movement_intensity_;
	}
	if(direction == input::Direction::LEFT)
	{
		input_info_.x_movement_intensity_ = -input::max_movement_intensity_;
	}
	if(direction == input::Direction::RIGHT)
	{
		input_info_.x_movement_intensity_ = input::max_movement_intensity_;
	}
	is_movement_from_joystick_ = false;
}

void GamepadInput::set_pad_direction_inactive(input::Direction direction)
{
	if(direction == input::Direction::UP || direction == input::Direction::DOWN)
	{
		input_info_.y_movement_intensity_ = 0.0f;
	}
	else if(direction == input::Direction::LEFT || direction == input::Direction::RIGHT)
	{
		input_info_.x_movement_intensity_ = 0.0f;
	}
	is_movement_from_joystick_ = false;
}

void GamepadInput::handle_events(const SDL_Event& e)
{
	switch(e.type)
	{
		case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
			if(e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_UP)
			{
				set_pad_direction_active(input::Direction::UP);
			}
			if(e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_DOWN)
			{
				set_pad_direction_active(input::Direction::DOWN);
			}
			if(e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_LEFT)
			{
				set_pad_direction_active(input::Direction::LEFT);
			}
			if(e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_RIGHT)
			{
				set_pad_direction_active(input::Direction::RIGHT);
			}
			break;

		case SDL_EVENT_GAMEPAD_BUTTON_UP:
			if(e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_UP)
			{
				set_pad_direction_inactive(input::Direction::UP);
			}
			if(e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_DOWN)
			{
				set_pad_direction_inactive(input::Direction::DOWN);
			}
			if(e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_LEFT)
			{
				set_pad_direction_inactive(input::Direction::LEFT);
			}
			if(e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_RIGHT)
			{
				set_pad_direction_inactive(input::Direction::RIGHT);
			}
			break;

		case SDL_EVENT_GAMEPAD_AXIS_MOTION:
			if(e.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTY)
			{
				if(std::abs(e.gaxis.value) > sdl::Gamepad::joystick_deadzone_)
				{
					set_direction_joystick(e.gaxis.value, sdl::Gamepad::JoystickAxis::Y_AXIS);
				}
				else if(is_movement_from_joystick_)
				{
					input_info_.y_movement_intensity_ = 0.0f;
				}
			}
			if(e.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTX)
			{
				if(std::abs(e.gaxis.value) > sdl::Gamepad::joystick_deadzone_)
				{
					set_direction_joystick(e.gaxis.value, sdl::Gamepad::JoystickAxis::X_AXIS);
				}
				else if(is_movement_from_joystick_)
				{
					input_info_.x_movement_intensity_ = 0.0f;
				}
			}
			if(e.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHTX)
			{
				if(std::abs(e.gaxis.value) > sdl::Gamepad::joystick_deadzone_)
				{
					set_rotation_joystick(e.gaxis.value, sdl::Gamepad::JoystickAxis::X_AXIS);
				}
				else
				{
					input_info_.x_rotation_intensity_ = 0.0f;
				}
			}
			if(e.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHTY)
			{
				if(std::abs(e.gaxis.value) > sdl::Gamepad::joystick_deadzone_)
				{
					set_rotation_joystick(e.gaxis.value, sdl::Gamepad::JoystickAxis::Y_AXIS);
				}
				else
				{
					input_info_.y_rotation_intensity_ = 0.0f;
				}
			}
			break;

		default:
			break;
	}
}

void GamepadInput::update(float delta_time)
{
	//std::cout << "(GAMEPAD) => x: " << input_info_.x_movement_intensity_ << ", y: " << input_info_.y_movement_intensity_ << ", rotation_x: " << input_info_.x_rotation_intensity_ << ", rotation_y: " << input_info_.y_rotation_intensity_ << std::endl;
}

input::Info GamepadInput::get_input_info() const
{
	return input_info_;
}

void GamepadInput::set_direction_joystick(Sint16 axis_value, sdl::Gamepad::JoystickAxis joystick_axis)
{
	if(joystick_axis == sdl::Gamepad::JoystickAxis::X_AXIS)
	{
		input_info_.x_movement_intensity_ = float(axis_value) / SDL_JOYSTICK_AXIS_MAX;
	}
	else if(joystick_axis == sdl::Gamepad::JoystickAxis::Y_AXIS)
	{
		input_info_.y_movement_intensity_ = -(float(axis_value) / SDL_JOYSTICK_AXIS_MAX);
	}
	is_movement_from_joystick_ = true;
}

void GamepadInput::set_rotation_joystick(Sint16 axis_value, sdl::Gamepad::JoystickAxis joystick_axis)
{
	if(joystick_axis == sdl::Gamepad::JoystickAxis::X_AXIS)
	{
		input_info_.x_rotation_intensity_ = float(axis_value) / SDL_JOYSTICK_AXIS_MAX;
	}
	else if(joystick_axis == sdl::Gamepad::JoystickAxis::Y_AXIS)
	{
		input_info_.y_rotation_intensity_ = float(axis_value) / SDL_JOYSTICK_AXIS_MAX;
	}
}