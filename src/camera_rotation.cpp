#include "camera_rotation.h"
#include "utils.h"

#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <iostream>

CameraRotation::CameraRotation(glm::mat4& view, glm::vec3& camera_position, glm::vec3& camera_front)
	: camera_position_(camera_position), camera_front_(camera_front), euler_angles_(), view_(view),
	mouse_motion_last_time_(0), is_rotation_from_joystick_(false), x_rotation_intensity_(0.0f), y_rotation_intensity_(0.0f)
{}

void CameraRotation::set_rotation_mouse(float xrel, float yrel)
{
	is_rotation_from_joystick_ = false;
	mouse_motion_last_time_ = SDL_GetTicks();
	x_rotation_intensity_ = xrel * 1.6f; //TODO : hardcodé
	y_rotation_intensity_ = yrel * 1.6f; //TODO : hardcodé
}

void CameraRotation::set_rotation_joystick(Sint16 axis_value, sdl::Gamepad::JoystickAxis joystick_axis)
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

void CameraRotation::mouse_motion_event_end()
{
	if(!is_rotation_from_joystick_ && SDL_GetTicks() > mouse_motion_last_time_ + 100) //TODO : hardcodé
	{
		x_rotation_intensity_ = 0.0f;
		y_rotation_intensity_ = 0.0f;
	}
}

void CameraRotation::handle_events(const SDL_Event& e)
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

void CameraRotation::update()
{
	//std::cout << std::boolalpha << is_rotation_from_joystick_ << ", " << x_rotation_intensity_ << ", " << y_rotation_intensity_ << std::endl;

	//On est obligé de gérer nous-même car il n'y a pas d'événements qui détecte la fin du mouvement de souris
	mouse_motion_event_end();

	//TODO : ne pas utiliser de variables statiques, créer une classe à la place
	static bool should_reset = false;
	static float saved_yaw = euler_angles_.yaw_;
	static float saved_pitch = euler_angles_.pitch_;
	static float temp = 0.0f;
	if(x_rotation_intensity_ != 0.0f || y_rotation_intensity_ != 0.0f)
	{
		calculate_euler_angles();
		should_reset = true;
		saved_yaw = euler_angles_.yaw_;
		saved_pitch = euler_angles_.pitch_;
		temp = 0.0f;
	}
	else
	{
		//std::cout << "YAW: " << euler_angles_.yaw_ << ", PITCH: " << euler_angles_.pitch_ << std::endl;
		if(should_reset)
		{
			//std::cout << lerp(euler_angles_.yaw_, -90.0f, temp) << ", " << lerp(euler_angles_.pitch_, 0.0f, temp) << std::endl;

			float yaw = float(utils::lerp(saved_yaw, -90.0f, temp));
			float pitch = float(utils::lerp(saved_pitch, 0.0f, temp));

			euler_angles_.yaw_ = yaw;
			euler_angles_.pitch_ = pitch;

			glm::vec3 front = glm::vec3(
				cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
				sin(glm::radians(pitch)),
				sin(glm::radians(yaw)) * cos(glm::radians(pitch))
			);
			camera_front_ = glm::normalize(front);
			//view_ = look_at(camera_position_, camera_position_ + camera_front_, glm::vec3(0.0f, 1.0f, 0.0f));
			//shader_program_.set_uniform_matrix_4fv("view", glm::value_ptr(view_));

			//std::cout << "YAW: " << euler_angles_.yaw_ << ", PITCH: " << euler_angles_.pitch_ << std::endl;

			temp += 0.01f;
			//std::cout << "TEMP: " << temp << ", YAW: " << yaw << ", PITCH: " << pitch << std::endl;
			if(temp > 1.0f)
			{
				temp = 0.0f;
				should_reset = false;

				//euler_angles_.yaw_ = yaw;
				//euler_angles_.pitch_ = pitch;

				//std::cout << right_joystick_info_.x_axis_ << ", " << right_joystick_info_.y_axis_ << std::endl;
				//std::cout << "YAW: " << euler_angles_.yaw_ << ", PITCH: " << euler_angles_.pitch_ << std::endl;
			}
		}
	}
	camera_position_.y = 0.5f; //TODO : doit être égal à la hauteur du sol
}

void CameraRotation::calculate_euler_angles()
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
	if(euler_angles_.pitch_ > 25.0f) //TODO : hardcodé
	{
		euler_angles_.pitch_ = 25.0f; //TODO : hardcodé
	}
	else if(euler_angles_.pitch_ < -25.0f) //TODO : hardcodé
	{
		euler_angles_.pitch_ = -25.0f; //TODO : hardcodé
	}

	//TODO : std::max/std::min/std::clamp ?
	if(euler_angles_.yaw_ > -50.0f) //TODO : hardcodé
	{
		euler_angles_.yaw_ = -50.0f; //TODO : hardcodé
	}
	else if(euler_angles_.yaw_ < -130.0f) //TODO : hardcodé
	{
		euler_angles_.yaw_ = -130.0f; //TODO : hardcodé
	}

	glm::vec3 front = glm::vec3(
		cos(glm::radians(euler_angles_.yaw_)) * cos(glm::radians(euler_angles_.pitch_)),
		sin(glm::radians(euler_angles_.pitch_)),
		sin(glm::radians(euler_angles_.yaw_)) * cos(glm::radians(euler_angles_.pitch_))
	);
	camera_front_ = glm::normalize(front);
}