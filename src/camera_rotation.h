#pragma once

#include "RAII_SDL3/gamepad.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <SDL3/SDL.h>

class CameraRotation
{
	public:
		CameraRotation(glm::mat4& view, glm::vec3& camera_position, glm::vec3& camera_front);

		void handle_events(const SDL_Event& e);
		void update();

		glm::vec3& camera_position_;
		glm::vec3& camera_front_;

	private:
		struct EulerAngles
		{
			EulerAngles()
				: pitch_(0.0f), yaw_(-90.0f), roll_(0.0f)
			{}

			float pitch_;
			float yaw_;
			float roll_; //inutilisé pour l'instant
		};

		void set_rotation_mouse(float xrel, float yrel);
		void set_rotation_joystick(Sint16 axis_value, sdl::Gamepad::JoystickAxis joystick_axis);
		void mouse_motion_event_end();
		void calculate_euler_angles();

		EulerAngles euler_angles_;
		glm::mat4& view_;
		Uint64 mouse_motion_last_time_;
		bool is_rotation_from_joystick_;
		float x_rotation_intensity_;
		float y_rotation_intensity_;
};