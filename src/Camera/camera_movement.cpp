#include "camera_movement.h"

#include <iostream>

const float CameraMovement::max_movement_intensity_ = 1.0f;

CameraMovement::CameraMovement(glm::mat4& view, glm::vec3& camera_position, glm::vec3& camera_front)
	: camera_position_(camera_position), camera_front_(camera_front), view_(view), is_arrow_key_pressed_(false), is_pad_pressed_(false),
	is_movement_from_joystick_(false), x_movement_intensity_(0.0f), y_movement_intensity_(0.0f)
{}

void CameraMovement::set_key_direction_active(Direction direction) //arrow keys + pad
{
	//Annuler la direction si on appuie à la fois sur le clavier et le pad de la manette
	if(is_pad_pressed_ && is_arrow_key_pressed_)
	{
		if(direction == Direction::UP || direction == Direction::DOWN)
		{
			y_movement_intensity_ = 0.0f;
		}
		if(direction == Direction::LEFT || direction == Direction::RIGHT)
		{
			x_movement_intensity_ = 0.0f;
		}
	}
	else
	{
		if(direction == Direction::UP)
		{
			if(is_movement_from_joystick_)
			{
				y_movement_intensity_ = std::min(y_movement_intensity_ + max_movement_intensity_, max_movement_intensity_);
			}
			else y_movement_intensity_ = max_movement_intensity_;
		}
		if(direction == Direction::DOWN)
		{
			if(is_movement_from_joystick_)
			{
				y_movement_intensity_ = std::max(y_movement_intensity_ - max_movement_intensity_, -max_movement_intensity_);
			}
			else y_movement_intensity_ = -max_movement_intensity_;
		}
		if(direction == Direction::LEFT)
		{
			if(is_movement_from_joystick_)
			{
				x_movement_intensity_ = std::max(x_movement_intensity_ - max_movement_intensity_, -max_movement_intensity_);
			}
			else x_movement_intensity_ = -max_movement_intensity_;
		}
		if(direction == Direction::RIGHT)
		{
			if(is_movement_from_joystick_)
			{
				x_movement_intensity_ = std::min(x_movement_intensity_ + max_movement_intensity_, max_movement_intensity_);
			}
			else x_movement_intensity_ = max_movement_intensity_;
		}
	}
	is_movement_from_joystick_ = false;
}

void CameraMovement::set_key_direction_inactive(Direction direction) //arrow keys + pad
{
	is_movement_from_joystick_ = false;
	if(direction == Direction::UP || direction == Direction::DOWN)
	{
		y_movement_intensity_ = 0.0f;
	}
	else if(direction == Direction::LEFT || direction == Direction::RIGHT)
	{
		x_movement_intensity_ = 0.0f;
	}
}

void CameraMovement::set_direction_joystick(Sint16 axis_value, sdl::Gamepad::JoystickAxis joystick_axis)
{
	is_movement_from_joystick_ = true;
	if(joystick_axis == sdl::Gamepad::JoystickAxis::X_AXIS)
	{
		x_movement_intensity_ = float(axis_value) / SDL_JOYSTICK_AXIS_MAX;
	}
	else if(joystick_axis == sdl::Gamepad::JoystickAxis::Y_AXIS)
	{
		y_movement_intensity_ = -(float(axis_value) / SDL_JOYSTICK_AXIS_MAX);
	}
}

void CameraMovement::handle_events(const SDL_Event& e)
{
	switch(e.type)
	{
		case SDL_EVENT_KEY_DOWN:
			//if(!is_arrow_key_pressed_) //pour avoir le même comportement que SDL_EVENT_GAMEPAD_BUTTON_DOWN qui n'est pas appelé en boucle
			//{
				is_arrow_key_pressed_ = true;
				if(e.key.key == SDLK_UP || e.key.scancode == SDL_SCANCODE_W)
				{
					set_key_direction_active(Direction::UP);
				}
				if(e.key.key == SDLK_DOWN || e.key.scancode == SDL_SCANCODE_S)
				{
					set_key_direction_active(Direction::DOWN);
				}
				if(e.key.key == SDLK_LEFT || e.key.scancode == SDL_SCANCODE_A)
				{
					set_key_direction_active(Direction::LEFT);
				}
				if(e.key.key == SDLK_RIGHT || e.key.scancode == SDL_SCANCODE_D)
				{
					set_key_direction_active(Direction::RIGHT);
				}
			//}
			break;

		case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
			is_pad_pressed_ = true;
			if(e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_UP)
			{
				set_key_direction_active(Direction::UP);
			}
			if(e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_DOWN)
			{
				set_key_direction_active(Direction::DOWN);
			}
			if(e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_LEFT)
			{
				set_key_direction_active(Direction::LEFT);
			}
			if(e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_RIGHT)
			{
				set_key_direction_active(Direction::RIGHT);
			}
			break;

		case SDL_EVENT_KEY_UP:
			is_arrow_key_pressed_ = false;
			if(e.key.key == SDLK_UP || e.key.scancode == SDL_SCANCODE_W)
			{
				set_key_direction_inactive(Direction::UP);
			}
			if(e.key.key == SDLK_DOWN || e.key.scancode == SDL_SCANCODE_S)
			{
				set_key_direction_inactive(Direction::DOWN);
			}
			if(e.key.key == SDLK_LEFT || e.key.scancode == SDL_SCANCODE_A)
			{
				set_key_direction_inactive(Direction::LEFT);
			}
			if(e.key.key == SDLK_RIGHT || e.key.scancode == SDL_SCANCODE_D)
			{
				set_key_direction_inactive(Direction::RIGHT);
			}
			break;

		case SDL_EVENT_GAMEPAD_BUTTON_UP:
			is_pad_pressed_ = false;
			if(e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_UP)
			{
				set_key_direction_inactive(Direction::UP);
			}
			if(e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_DOWN)
			{
				set_key_direction_inactive(Direction::DOWN);
			}
			if(e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_LEFT)
			{
				set_key_direction_inactive(Direction::LEFT);
			}
			if(e.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_RIGHT)
			{
				set_key_direction_inactive(Direction::RIGHT);
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
					y_movement_intensity_ = 0.0f;
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
					x_movement_intensity_ = 0.0f;
				}
			}
			break;

		default:
			break;
	}
}

void CameraMovement::update()
{
	//std::cout << x_movement_intensity_ << ", " << y_movement_intensity_ << std::endl;

	if(y_movement_intensity_ != 0.0f)
	{
		glm::vec3 camera_forward = glm::vec3(-view_[0][2], -view_[1][2], -view_[2][2]);
		camera_position_ += (y_movement_intensity_ * 0.05f) * camera_forward; //TODO : hardcodé
	}

	if(x_movement_intensity_ != 0.0f)
	{
		glm::vec3 camera_left = glm::vec3(-view_[0][0], -view_[1][0], -view_[2][0]);
		camera_position_ -= (x_movement_intensity_ * 0.05f) * camera_left; //TODO : hardcodé
	}
}