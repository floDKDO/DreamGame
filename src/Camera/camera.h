#pragma once

#include "Input/input_manager.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Camera
{
	public:
		Camera(InputManager& input_manager, const glm::vec3& target_position);

		void update(float delta_time);

		glm::vec3 get_camera_forward() const;
		glm::vec3 get_camera_left() const;
		glm::vec3 get_camera_up() const;
		glm::mat4 get_view_matrix() const;

		const glm::vec3& target_position_;
		glm::vec3 target_to_camera_offset_;
		glm::vec3 camera_position_;

	private:
		struct EulerAngles
		{
			EulerAngles()
				: pitch_(30.0f), yaw_(0.0f), roll_(0.0f)
			{}

			static const float min_pitch_;
			static const float max_pitch_;

			//valeur des angles en degrés
			float pitch_;
			float yaw_;
			float roll_; //inutilisé pour l'instant
		};

		glm::mat4 look_at(glm::vec3 camera_position, glm::vec3 camera_target_position, glm::vec3 up_vector) const;
		void compute_euler_angles(float delta_time);

		InputManager& input_manager_;
		glm::mat4 view_matrix_;
		EulerAngles euler_angles_;
};