#include "camera.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Camera::Camera(glm::mat4& view_matrix, glm::vec3& target_position)
	: view_matrix_(view_matrix), target_position_(target_position), target_to_camera_offset_(glm::vec3(0.0f, 2.5f, 0.0f)), camera_position_(target_position + target_to_camera_offset_),
	camera_front_(glm::vec3(0.0f, 0.0f, -1.0f)), euler_angles_(), mouse_motion_last_time_(0), is_rotation_from_joystick_(false), 
	x_rotation_intensity_(0.0f), y_rotation_intensity_(0.0f)
{}

void Camera::handle_events(const SDL_Event& e)
{
	switch(e.type)
	{
		case SDL_EVENT_GAMEPAD_AXIS_MOTION:
			if(e.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHTX)
			{
				if(std::abs(e.gaxis.value) > sdl::Gamepad::joystick_deadzone_)
				{
					set_rotation_joystick(e.gaxis.value, sdl::Gamepad::JoystickAxis::X_AXIS);
				}
				else if(is_rotation_from_joystick_)
				{
					x_rotation_intensity_ = 0.0f;
				}
			}
			if(e.gaxis.axis == SDL_GAMEPAD_AXIS_RIGHTY)
			{
				if(std::abs(e.gaxis.value) > sdl::Gamepad::joystick_deadzone_)
				{
					set_rotation_joystick(e.gaxis.value, sdl::Gamepad::JoystickAxis::Y_AXIS);
				}
				else if(is_rotation_from_joystick_)
				{
					y_rotation_intensity_ = 0.0f;
				}
			}
			break;

		case SDL_EVENT_MOUSE_MOTION:
			set_rotation_mouse(e.motion.xrel, e.motion.yrel);
			break;

		default:
			break;
	}
}

void Camera::update()
{
	view_matrix_ = look_at(camera_position_, target_position_ + target_to_camera_offset_, glm::vec3(0.0f, 1.0f, 0.0f));

	//On est obligé de gérer nous-même car il n'y a pas d'événements qui détecte la fin du mouvement de souris
	mouse_motion_event_end();
	calculate_euler_angles();
}

glm::mat4 Camera::look_at(glm::vec3 camera_position, glm::vec3 camera_target_position, glm::vec3 up_vector) const
{
	glm::vec3 forward = glm::normalize(camera_position - camera_target_position);
	glm::vec3 left = glm::normalize(glm::cross(up_vector, forward));
	glm::vec3 up = glm::normalize(glm::cross(forward, left));

	glm::mat4 translation = glm::mat4(1.0f);
	translation[3][0] = -camera_position.x;
	translation[3][1] = -camera_position.y;
	translation[3][2] = -camera_position.z;

	glm::mat4 rotation = glm::mat4(1.0f);
	rotation[0][0] = left.x;
	rotation[0][1] = left.y;
	rotation[0][2] = left.z;
	rotation[1][0] = up.x;
	rotation[1][1] = up.y;
	rotation[1][2] = up.z;
	rotation[2][0] = forward.x;
	rotation[2][1] = forward.y;
	rotation[2][2] = forward.z;
	rotation = glm::transpose(rotation);

	glm::mat4 view_matrix = glm::mat4(1.0f);
	view_matrix = rotation * translation;

	return view_matrix;
}

void Camera::set_rotation_mouse(float xrel, float yrel)
{
	is_rotation_from_joystick_ = false;
	mouse_motion_last_time_ = SDL_GetTicks();
	x_rotation_intensity_ = xrel * 2.0f; //TODO : hardcodé
	y_rotation_intensity_ = yrel * 2.0f; //TODO : hardcodé
}

void Camera::set_rotation_joystick(Sint16 axis_value, sdl::Gamepad::JoystickAxis joystick_axis)
{
	is_rotation_from_joystick_ = true;
	if(joystick_axis == sdl::Gamepad::JoystickAxis::X_AXIS)
	{
		x_rotation_intensity_ = float(axis_value) / SDL_JOYSTICK_AXIS_MAX;
	}
	else if(joystick_axis == sdl::Gamepad::JoystickAxis::Y_AXIS)
	{
		y_rotation_intensity_ = float(axis_value) / SDL_JOYSTICK_AXIS_MAX;
	}
}

void Camera::mouse_motion_event_end()
{
	if(!is_rotation_from_joystick_ && SDL_GetTicks() > mouse_motion_last_time_ + 100) //TODO : hardcodé
	{
		x_rotation_intensity_ = 0.0f;
		y_rotation_intensity_ = 0.0f;
	}
}

void Camera::calculate_euler_angles()
{
	if(!is_rotation_from_joystick_)
	{
		euler_angles_.yaw_ += (std::min(x_rotation_intensity_, 15.0f) / 15.0f); //TODO : hardcodé
		euler_angles_.pitch_ += (std::min(-y_rotation_intensity_, 15.0f) / 15.0f); // "-" pour que le sens soit bon (mettre la souris vers le haut déplace la caméra vers le haut). Enlever "-" pour inverser la caméra
	}
	else
	{
		float sensitivity = 0.5f;
		euler_angles_.yaw_ += x_rotation_intensity_ * sensitivity;
		euler_angles_.pitch_ += -y_rotation_intensity_ * sensitivity; // "-" pour que le sens soit bon (mettre la souris vers le haut déplace la caméra vers le haut). Enlever "-" pour inverser la caméra
	}

	//TODO : std::max/std::min/std::clamp ?
	if(euler_angles_.pitch_ > 40.0f) //TODO : hardcodé
	{
		euler_angles_.pitch_ = 40.0f; //TODO : hardcodé
	}
	else if(euler_angles_.pitch_ < -25.0f) //TODO : hardcodé
	{
		euler_angles_.pitch_ = -25.0f; //TODO : hardcodé
	}

	float distance_camera_target = 10.0f; //TODO : hardcodé

	glm::vec3 front = distance_camera_target * glm::vec3(
		cos(glm::radians(euler_angles_.yaw_)) * cos(glm::radians(euler_angles_.pitch_)),
		sin(glm::radians(euler_angles_.pitch_)),
		sin(glm::radians(euler_angles_.yaw_)) * cos(glm::radians(euler_angles_.pitch_))
	);
	camera_position_ = front + target_position_;
	camera_front_ = glm::normalize(target_position_ - camera_position_);
}