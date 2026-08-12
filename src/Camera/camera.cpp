#include "camera.h"

#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <iostream>

const float Camera::EulerAngles::min_pitch_ = -25.0f;
const float Camera::EulerAngles::max_pitch_ = 40.0f;

Camera::Camera(InputManager& input_manager, glm::vec3& target_position)
	: input_manager_(input_manager), view_matrix_(look_at(camera_position_, target_position_ + target_to_camera_offset_, glm::vec3(0.0f, 1.0f, 0.0f))),
	target_position_(target_position), target_to_camera_offset_(0.0f, 2.5f, 0.0f),
	camera_position_(target_position + target_to_camera_offset_), euler_angles_()
{}

void Camera::update(float delta_time)
{
	view_matrix_ = look_at(camera_position_, target_position_ + target_to_camera_offset_, glm::vec3(0.0f, 1.0f, 0.0f));
	compute_euler_angles(delta_time);
}

glm::vec3 Camera::get_camera_forward() const
{
	return glm::vec3(-view_matrix_[0][2], -view_matrix_[1][2], -view_matrix_[2][2]);
}

glm::vec3 Camera::get_camera_left() const
{
	return glm::vec3(-view_matrix_[0][0], -view_matrix_[1][0], -view_matrix_[2][0]);
}

glm::mat4 Camera::get_view_matrix() const
{
	return view_matrix_;
}

glm::mat4 Camera::look_at(glm::vec3 camera_position, glm::vec3 camera_target_position, glm::vec3 up_vector) const
{
	glm::vec3 forward = glm::normalize(camera_position - camera_target_position); //on obtient un vecteur qui part de la target vers la caméra
	glm::vec3 left = glm::normalize(glm::cross(up_vector, forward));
	glm::vec3 up = glm::normalize(glm::cross(forward, left));

	//operator[] est column major
	//=> mat[0][1] = colonne 0 ligne 1

	//***Translation matrix***
	//(1    0    0  -x_camera)
	//(0    1    0  -y_camera)
	//(0    0    1  -z_camera)
	//(0    0    0    1      )
	glm::mat4 translation(1.0f);
	translation[3][0] = -camera_position.x;
	translation[3][1] = -camera_position.y;
	translation[3][2] = -camera_position.z;

	//*********Rotation matrix**********
	//(leftx     lefty     leftz      0)
	//(upx        upy       upz       0)
	//(forwardx  forwardy  forwardz   0)
	//(   0         0         0       1)
	glm::mat4 rotation(1.0f);
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

	glm::mat4 view_matrix(1.0f);
	view_matrix = rotation * translation;

	return view_matrix;
}

void Camera::compute_euler_angles(float delta_time)
{
	//yaw et pitch n'ont pas de valeur max/min (grandissent à l'infini si on ne met pas les deux if avec valeur maximale) => pas un problème car on utilise cos et sin
	float sensitivity = 100.0f;
	input::Info input_info = input_manager_.get_input_info();

	euler_angles_.yaw_ += input_info.x_rotation_intensity_ * sensitivity * delta_time;
	euler_angles_.pitch_ = std::clamp(euler_angles_.pitch_ + -(input_info.y_rotation_intensity_) * sensitivity * delta_time, EulerAngles::min_pitch_, EulerAngles::max_pitch_); // "-" pour que le sens soit bon (mettre la souris vers le haut déplace la caméra vers le haut). Enlever "-" pour inverser la caméra

	float distance_camera_target = 10.0f;
	glm::vec3 euler_rotation = distance_camera_target * glm::vec3(
		cos(glm::radians(euler_angles_.yaw_)) * cos(glm::radians(euler_angles_.pitch_)), //x
		sin(glm::radians(euler_angles_.pitch_)),										 //y
		sin(glm::radians(euler_angles_.yaw_)) * cos(glm::radians(euler_angles_.pitch_))  //z
	);
	camera_position_ = euler_rotation /*+ target_to_camera_offset_*/ + target_position_;
}