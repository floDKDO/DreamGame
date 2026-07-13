#pragma once

#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace utils
{

//TODO : il existe glm::mix à la place de lerp
template<typename Arithmetic1, typename Arithmetic2, typename Arithmetic3>
double lerp(Arithmetic1 a, Arithmetic2 b, Arithmetic3 t) //TODO : à terme, remplacer par la fonction std::lerp de C++20
{
	double a_d = static_cast<double>(a);
	double b_d = static_cast<double>(b);
	double t_d = static_cast<double>(t);

	return a_d + t_d * (b_d - a_d);
}

glm::vec3 get_camera_forward(glm::mat4 view_matrix);
glm::vec3 get_camera_left(glm::mat4 view_matrix);
void GLAPIENTRY message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, [[maybe_unused]] const void* user_param);

}