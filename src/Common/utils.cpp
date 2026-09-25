#include "utils.h"
#include "Logging/logging.h"

#include <fstream>
#include <sstream>
#include <iostream>

namespace utils
{

std::string get_string_from_bool(bool b)
{
	return b ? "true" : "false";
}

std::string get_string_from_unsigned_char_ptr(const unsigned char* ptr)
{
	std::string string;
	for(std::size_t i = 0ULL; ptr[i] != '\0'; ++i)
	{
		string += ptr[i];
	}
	return string;
}

std::string get_string_from_file(std::string_view path)
{
	std::string path_str = std::string(path);
	std::ifstream file(path_str);
	if(!file.is_open())
	{
		logging::log("In get_string_from_file(), the requested file was not found!", logging::Severity::WARNING);
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

//Credit: https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c/13935718
std::string base64_decode(const void* data, const std::size_t len)
{
	static const int base64_index[256] = {
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 62, 63, 62, 62, 63, 52, 53, 54, 55,
	56, 57, 58, 59, 60, 61,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,
	7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  0,
	0,  0,  0, 63,  0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51};

	unsigned char* p = (unsigned char*)data;
	int pad = len > 0 && (len % 4 || p[len - 1] == '=');
	const std::size_t L = ((len + 3) / 4 - pad) * 4;
	std::string str(L / 4 * 3 + pad, '\0');

	for(std::size_t i = 0ULL, j = 0ULL; i < L; i += 4ULL)
	{
		int n = base64_index[p[i]] << 18 | base64_index[p[i + 1]] << 12 | base64_index[p[i + 2]] << 6 | base64_index[p[i + 3]];
		str[j++] = char(n >> 16);
		str[j++] = char(n >> 8 & 0xFF);
		str[j++] = char(n & 0xFF);
	}
	if(pad)
	{
		int n = base64_index[p[L]] << 18 | base64_index[p[L + 1]] << 12;
		str[str.size() - 1] = char(n >> 16);

		if(len > L + 2 && p[L + 2] != '=')
		{
			n |= base64_index[p[L + 2]] << 6;
			str.push_back(n >> 8 & 0xFF);
		}
	}
	return str;
}

}