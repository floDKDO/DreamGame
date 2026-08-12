#include "mouse_keyboard_input.h"

#include <algorithm>
#include <iostream>

const float MouseKeyboardInput::max_mouse_rel_value_ = 10.0f;

MouseKeyboardInput::MouseKeyboardInput()
	: mouse_motion_last_time_(0)
{}

void MouseKeyboardInput::handle_events(const SDL_Event& e)
{
	switch(e.type)
	{
		case SDL_EVENT_KEY_DOWN:
			//if(!is_arrow_key_pressed_) //pour avoir le même comportement que SDL_EVENT_GAMEPAD_BUTTON_DOWN qui n'est pas appelé en boucle
			//{

			if(e.key.key == SDLK_UP || e.key.scancode == SDL_SCANCODE_W)
			{
				set_key_direction_active(input::Direction::UP);
			}
			if(e.key.key == SDLK_DOWN || e.key.scancode == SDL_SCANCODE_S)
			{
				set_key_direction_active(input::Direction::DOWN);
			}
			if(e.key.key == SDLK_LEFT || e.key.scancode == SDL_SCANCODE_A)
			{
				set_key_direction_active(input::Direction::LEFT);
			}
			if(e.key.key == SDLK_RIGHT || e.key.scancode == SDL_SCANCODE_D)
			{
				set_key_direction_active(input::Direction::RIGHT);
			}
			//}
			break;

		case SDL_EVENT_KEY_UP:
			if(e.key.key == SDLK_UP || e.key.scancode == SDL_SCANCODE_W)
			{
				set_key_direction_inactive(input::Direction::UP);
			}
			if(e.key.key == SDLK_DOWN || e.key.scancode == SDL_SCANCODE_S)
			{
				set_key_direction_inactive(input::Direction::DOWN);
			}
			if(e.key.key == SDLK_LEFT || e.key.scancode == SDL_SCANCODE_A)
			{
				set_key_direction_inactive(input::Direction::LEFT);
			}
			if(e.key.key == SDLK_RIGHT || e.key.scancode == SDL_SCANCODE_D)
			{
				set_key_direction_inactive(input::Direction::RIGHT);
			}
			break;

		case SDL_EVENT_MOUSE_MOTION:
			set_rotation_mouse(e.motion.xrel, e.motion.yrel);
			break;

		default:
			break;
	}
}

void MouseKeyboardInput::update(float delta_time)
{
	//std::cout << "(MOUSE KEYBOARD) => x: " << input_info_.x_movement_intensity_ << ", y: " << input_info_.y_movement_intensity_  << ", rotation_x: " << input_info_.x_rotation_intensity_ << ", rotation_y: " << input_info_.y_rotation_intensity_ << std::endl;

	//On est obligé de gérer nous-même car il n'y a pas d'événements qui détecte la fin du mouvement de souris
	mouse_motion_event_end();
}

input::Info MouseKeyboardInput::get_input_info() const
{
	return input_info_;
}

void MouseKeyboardInput::set_key_direction_active(input::Direction direction)
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
}

void MouseKeyboardInput::set_key_direction_inactive(input::Direction direction)
{
	if(direction == input::Direction::UP || direction == input::Direction::DOWN)
	{
		input_info_.y_movement_intensity_ = 0.0f;
	}
	else if(direction == input::Direction::LEFT || direction == input::Direction::RIGHT)
	{
		input_info_.x_movement_intensity_ = 0.0f;
	}
}

void MouseKeyboardInput::set_rotation_mouse(float xrel, float yrel)
{
	mouse_motion_last_time_ = SDL_GetTicks();
	input_info_.x_rotation_intensity_ = std::clamp(xrel / max_mouse_rel_value_, -1.0f, 1.0f);
	input_info_.y_rotation_intensity_ = std::clamp(yrel / max_mouse_rel_value_, -1.0f, 1.0f);
}

void MouseKeyboardInput::mouse_motion_event_end()
{
	if(SDL_GetTicks() > mouse_motion_last_time_ + 100) //100 ms
	{
		input_info_.x_rotation_intensity_ = 0.0f;
		input_info_.y_rotation_intensity_ = 0.0f;
	}
}