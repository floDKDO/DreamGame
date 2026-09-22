#include "mouse_keyboard_input.h"

#include <algorithm>
#include <iostream>

namespace input
{

const float MouseKeyboard::max_mouse_rel_value_ = 10.0f;

MouseKeyboard::MouseKeyboard()
	: mouse_motion_last_time_(0)
{}

void MouseKeyboard::handle_events(const SDL_Event& e)
{
	switch(e.type)
	{
		case SDL_EVENT_KEY_DOWN:
			//if(!is_arrow_key_pressed_) //pour avoir le même comportement que SDL_EVENT_GAMEPAD_BUTTON_DOWN qui n'est pas appelé en boucle
			//{

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

		case SDL_EVENT_KEY_UP:
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

		case SDL_EVENT_MOUSE_MOTION:
			set_rotation_mouse(e.motion.xrel, e.motion.yrel);
			break;

		default:
			break;
	}
}

void MouseKeyboard::update([[maybe_unused]] float delta_time)
{
	//std::cout << "(MOUSE KEYBOARD) => x: " << input_info_.x_movement_intensity_ << ", y: " << input_info_.y_movement_intensity_  << ", rotation_x: " << input_info_.x_rotation_intensity_ << ", rotation_y: " << input_info_.y_rotation_intensity_ << std::endl;

	//On est obligé de gérer nous-même car il n'y a pas d'événements qui détecte la fin du mouvement de souris
	mouse_motion_event_end();
}

Info MouseKeyboard::get_input_info() const
{
	return input_info_;
}

void MouseKeyboard::set_key_direction_active(Direction direction)
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
}

void MouseKeyboard::set_key_direction_inactive(Direction direction)
{
	if(direction == Direction::UP || direction == Direction::DOWN)
	{
		input_info_.y_movement_intensity_ = 0.0f;
	}
	else if(direction == Direction::LEFT || direction == Direction::RIGHT)
	{
		input_info_.x_movement_intensity_ = 0.0f;
	}
}

void MouseKeyboard::set_rotation_mouse(float xrel, float yrel)
{
	mouse_motion_last_time_ = SDL_GetTicks();
	input_info_.x_rotation_intensity_ = std::clamp(xrel / max_mouse_rel_value_, -1.0f, 1.0f);
	input_info_.y_rotation_intensity_ = std::clamp(yrel / max_mouse_rel_value_, -1.0f, 1.0f);
}

void MouseKeyboard::mouse_motion_event_end()
{
	if(SDL_GetTicks() > mouse_motion_last_time_ + 100) //100 ms
	{
		input_info_.x_rotation_intensity_ = 0.0f;
		input_info_.y_rotation_intensity_ = 0.0f;
	}
}

}