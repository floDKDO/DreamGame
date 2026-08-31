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

glm::vec3 get_translation_from_model_matrix(glm::mat4 model_matrix)
{
	return glm::vec3(model_matrix[3]); //4ème colonne (les 3 premiers composants) contient le vecteur de translation
}

glm::vec3 get_scale_from_model_matrix(glm::mat4 model_matrix)
{
	glm::vec3 scale;
	scale.x = glm::length(glm::vec3(model_matrix[0])); //norme du vecteur composé des 3 composants de la première colonne
	scale.y = glm::length(glm::vec3(model_matrix[1])); //norme du vecteur composé des 3 composants de la deuxième colonne
	scale.z = glm::length(glm::vec3(model_matrix[2])); //norme du vecteur composé des 3 composants de la troisième colonne
	return scale;
}

glm::quat get_rotation_from_model_matrix(glm::mat4 model_matrix)
{
	glm::vec3 scale = get_scale_from_model_matrix(model_matrix);

	glm::mat4 rotation_matrix(1.0f);
	rotation_matrix[0] = glm::vec4(model_matrix[0] / scale.x);
	rotation_matrix[1] = glm::vec4(model_matrix[1] / scale.y);
	rotation_matrix[2] = glm::vec4(model_matrix[2] / scale.z);

	return glm::quat_cast(rotation_matrix);
}

}