#pragma once

#include "RAII_SDL3/gamepad.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Camera
{
	public:
		Camera(glm::mat4& view_matrix, glm::vec3& target_position);

		void handle_events(const SDL_Event& e);
		void update();

		glm::vec3& target_position_;
		glm::vec3 target_to_camera_offset_;
		glm::vec3 camera_position_;
		glm::vec3 camera_front_;

	private:
		glm::mat4 look_at(glm::vec3 camera_position, glm::vec3 camera_target_position, glm::vec3 up_vector) const;

		struct EulerAngles
		{
			EulerAngles()
				: pitch_(30.0f), yaw_(-90.0f), roll_(0.0f) //TODO : hardcodé
			{}

			float pitch_;
			float yaw_;
			float roll_; //inutilisé pour l'instant
		};

		void set_rotation_mouse(float xrel, float yrel);
		void set_rotation_joystick(Sint16 axis_value, sdl::Gamepad::JoystickAxis joystick_axis);
		void mouse_motion_event_end();
		void calculate_euler_angles();

		glm::mat4& view_matrix_;
		EulerAngles euler_angles_;
		Uint64 mouse_motion_last_time_;
		bool is_rotation_from_joystick_;
		float x_rotation_intensity_;
		float y_rotation_intensity_;
};