#pragma once

#include "camera_rotation.h"
#include "camera_movement.h"

class Camera
{
	public:
		Camera(glm::mat4& view);

		void handle_events(const SDL_Event& e);
		void update();

		glm::vec3 camera_position_;
		glm::vec3 camera_front_;

	private:
		glm::mat4 look_at(glm::vec3 camera_position, glm::vec3 camera_target_position, glm::vec3 up_vector) const;

		glm::mat4& view_;
		CameraRotation camera_rotation_;
		CameraMovement camera_movement_;
};