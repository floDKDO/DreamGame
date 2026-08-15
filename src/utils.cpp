#include "utils.h"

#include <fstream>
#include <sstream>
#include <iostream>

namespace utils
{

std::string get_string_from_file(std::string_view path)
{
	std::string path_str = std::string(path);
	std::ifstream file(path_str);
	if(!file.is_open())
	{
		std::cerr << "Error: unable to open the requested file (" << path << ")!\n";
	}

	std::stringstream stream;
	stream << file.rdbuf();
	return stream.str();
}

glm::vec3 get_camera_forward(glm::mat4 view_matrix)
{
	return glm::vec3(-view_matrix[0][2], -view_matrix[1][2], -view_matrix[2][2]);
}

glm::vec3 get_camera_left(glm::mat4 view_matrix)
{
	return glm::vec3(-view_matrix[0][0], -view_matrix[1][0], -view_matrix[2][0]);
}

GLfloat ieee754_to_float(uint64_t ieee754_number)
{
	GLfloat float_number;
	std::memcpy(&float_number, &ieee754_number, sizeof(float_number)); //obligé de faire cela pour convertir un nombre IEEE-754 en float (voir https://stackoverflow.com/questions/56710780/how-is-1-encoded-in-c-c-as-a-float-assuming-ieee-754-single-precision-represe)
	return float_number;
}

}