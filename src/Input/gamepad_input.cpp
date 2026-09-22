#include "gamepad_input.h"

#include <iostream>

namespace input
{

Gamepad::Gamepad()
	: is_movement_from_joystick_(false)
{}

void Gamepad::set_pad_direction_active(Direction direction)
{
	if(direction == Direction::UP)
	{
		input_info_.y_movement_intensity_ = max_movement_intensity_;
	}
	if(direction == Direction::DOWN)
	{
		input_info_.y_movement_intensity_ = -max_movement_intensity_;
	}
	if(direction == Direction::LEFT)
	{
		input_info_.x_movement_intensity_ = -max_movement_intensity_;
	}
	if(direction == Direction::RIGHT)
	{
		input_info_.x_movement_intensity_ = max_movement_intensity_;
	}
	is_movement_from_joystick_ = false;
}

void Gamepad::set_pad_direction_inactive(Direction direction)
{
	if(direction == Direction::UP || direction == Direction::DOWN)
	{
		input_info_.y_movement_intensity_ = 0.0f;
	}
	else if(direction == Direction::LEFT || direction == Direction::RIGHT)
	{
		input_info_.x_movement_intensity_ = 0.0f;
	}
	is_movement_from_joystick_ = false;
}

void Gamepad::handle_events(const SDL_Event& e)
{
	switch(e.type)
	{
		case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
			if(e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_UP)
			{
				set_pad_direction_active(Direction::UP);
			}
			if(e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_DOWN)
			{
				set_pad_direction_active(Direction::DOWN);
			}
			if(e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_LEFT)
			{
				set_pad_direction_active(Direction::LEFT);
			}
			if(e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_RIGHT)
			{
				set_pad_direction_active(Direction::RIGHT);
			}
			break;

		case SDL_EVENT_GAMEPAD_BUTTON_UP:
			if(e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_UP)
			{
				set_pad_direction_inactive(Direction::UP);
			}
			if(e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_DOWN)
			{
				set_pad_direction_inactive(Direction::DOWN);
			}
			if(e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_LEFT)
			{
				set_pad_direction_inactive(Direction::LEFT);
			}
			if(e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_RIGHT)
			{
				set_pad_direction_inactive(Direction::RIGHT);
			}
			break;

		case SDL_EVENT_GAMEPAD_AXIS_MOTION:
			if(e.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTY)
			{
				if(std::abs(e.gaxis.value) > joystick_deadzone_)
				{
					set_direction_joystick(e.gaxis.value, JoystickAxis::Y_AXIS);
				}
				else if(is_movement_from_joystick_)
				{
					input_info_.y_movement_intensity_ = 0.0f;
				}
			}
			if(e.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTX)
			{
				if(std::abs(e.gaxis.value) > joystick_deadzone_)
				{
					set_direction_joystick(e.gaxis.value, JoystickAxis::X_AXIS);
				}
				else if(is_movement_from_joystick_)
				{
					input_info_.x_movement_intensity_ = 0.0f;
				}
			}
			if(e.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHTX)
			{
				if(std::abs(e.gaxis.value) > joystick_deadzone_)
				{
					set_rotation_joystick(e.gaxis.value, JoystickAxis::X_AXIS);
				}
				else
				{
					input_info_.x_rotation_intensity_ = 0.0f;
				}
			}
			if(e.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHTY)
			{
				if(std::abs(e.gaxis.value) > joystick_deadzone_)
				{
					set_rotation_joystick(e.gaxis.value, JoystickAxis::Y_AXIS);
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

void Gamepad::update([[maybe_unused]] float delta_time)
{
	//std::cout << "(GAMEPAD) => x: " << input_info_.x_movement_intensity_ << ", y: " << input_info_.y_movement_intensity_ << ", rotation_x: " << input_info_.x_rotation_intensity_ << ", rotation_y: " << input_info_.y_rotation_intensity_ << std::endl;
}

Info Gamepad::get_input_info() const
{
	return input_info_;
}

void Gamepad::set_direction_joystick(Sint16 axis_value, JoystickAxis joystick_axis)
{
	if(joystick_axis == JoystickAxis::X_AXIS)
	{
		input_info_.x_movement_intensity_ = float(axis_value) / SDL_JOYSTICK_AXIS_MAX;
	}
	else if(joystick_axis == JoystickAxis::Y_AXIS)
	{
		input_info_.y_movement_intensity_ = -(float(axis_value) / SDL_JOYSTICK_AXIS_MAX);
	}
	is_movement_from_joystick_ = true;
}

void Gamepad::set_rotation_joystick(Sint16 axis_value, JoystickAxis joystick_axis)
{
	if(joystick_axis == JoystickAxis::X_AXIS)
	{
		input_info_.x_rotation_intensity_ = float(axis_value) / SDL_JOYSTICK_AXIS_MAX;
	}
	else if(joystick_axis == JoystickAxis::Y_AXIS)
	{
		input_info_.y_rotation_intensity_ = float(axis_value) / SDL_JOYSTICK_AXIS_MAX;
	}
}

}