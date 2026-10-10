#pragma once

#include <SDL3/SDL_joystick.h>

namespace input
{

enum class ActiveDevice
{
	GAMEPAD,
	MOUSE_KEYBOARD
};

enum class JoystickAxis
{
	X_AXIS,
	Y_AXIS
};

enum class Direction
{
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

const float max_movement_intensity_ = 1.0f;
const float joystick_deadzone_ = SDL_JOYSTICK_AXIS_MAX * 0.1f; //10% of the max value

struct Info
{
	//TODO : n'avoir qu'un seul bool qui serait "is_interacting_" par exemple (peut-être avoir un tableau de bool qui contient toutes les touches/actions de validation et si l'une des cases est à true, alors le bool "is_interacting_" est true)
	bool space_pressed_ = false;
	bool left_click_pressed_ = false;

	float x_movement_intensity_ = 0.0f;
	float y_movement_intensity_ = 0.0f;
	float x_rotation_intensity_ = 0.0f;
	float y_rotation_intensity_ = 0.0f;
};

}