#pragma once

#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <string>
#include <string_view>

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

std::string get_string_from_file(std::string_view path);
glm::vec3 get_camera_forward(glm::mat4 view_matrix);
glm::vec3 get_camera_left(glm::mat4 view_matrix);
GLfloat ieee754_to_float(uint64_t ieee754_number);

}