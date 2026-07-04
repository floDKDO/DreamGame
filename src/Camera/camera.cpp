#include "camera.h"

#include <iostream>

Camera::Camera(glm::mat4& view)
	: view_(view), camera_position_(glm::vec3(0.0f, 0.5f, 5.0f)), camera_front_(glm::vec3(0.0f, 0.0f, -1.0f)), 
	camera_rotation_(view, camera_position_, camera_front_), camera_movement_(view, camera_position_, camera_front_)
{}

void Camera::handle_events(const SDL_Event& e)
{
	camera_rotation_.handle_events(e);
	camera_movement_.handle_events(e);
}

void Camera::update()
{
	camera_rotation_.update();
	camera_movement_.update();
	view_ = look_at(camera_position_, camera_position_ + camera_front_, glm::vec3(0.0f, 1.0f, 0.0f));
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

	glm::mat4 view = glm::mat4(1.0f);
	view = rotation * translation;

	return view;
}