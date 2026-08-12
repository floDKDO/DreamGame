#pragma once

namespace input
{

enum class Direction
{
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

const float max_movement_intensity_ = 1.0f;

struct Info
{
	float x_movement_intensity_ = 0.0f;
	float y_movement_intensity_ = 0.0f;
	float x_rotation_intensity_ = 0.0f;
	float y_rotation_intensity_ = 0.0f;
};

}