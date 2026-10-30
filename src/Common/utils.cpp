#include "utils.h"
#include "Logging/logging.h"

#include <stb/stb_image.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

namespace utils
{

unsigned char* checked_stbi_load(std::string_view path, int& width, int& height, int& channels)
{
	std::string path_str = std::string(path);

	unsigned char* pixels;
	if((pixels = stbi_load(path_str.c_str(), &width, &height, &channels, 0)) == nullptr)
	{
		logging::log("stbi_load() returned nullptr: " + std::string(stbi_failure_reason()), logging::Severity::CRITICAL);
		exit(EXIT_FAILURE);
	}
	return pixels;
}

unsigned char* checked_stbi_load(std::string_view path)
{
	std::string path_str = std::string(path);
	int width, height, channels; //Variables inutilisées mais pas possible de mettre nullptr dans stbi_load()

	unsigned char* pixels;
	if((pixels = stbi_load(path_str.c_str(), &width, &height, &channels, 0)) == nullptr)
	{
		logging::log("stbi_load() returned nullptr: " + std::string(stbi_failure_reason()), logging::Severity::CRITICAL);
		exit(EXIT_FAILURE);
	}
	return pixels;
}

unsigned char* checked_stbi_load_from_memory(const unsigned char* buffer, std::size_t size, int& width, int& height, int& channels)
{
	unsigned char* pixels;
	if((pixels = stbi_load_from_memory(buffer, int(size), &width, &height, &channels, 0)) == nullptr)
	{
		logging::log("stbi_load_from_memory() returned nullptr: " + std::string(stbi_failure_reason()), logging::Severity::CRITICAL);
		exit(EXIT_FAILURE);
	}
	return pixels;
}

unsigned char* checked_stbi_load_from_memory(const unsigned char* buffer, std::size_t size)
{
	int width, height, channels; //Variables inutilisées mais pas possible de mettre nullptr dans stbi_load_from_memory()

	unsigned char* pixels;
	if((pixels = stbi_load_from_memory(buffer, int(size), &width, &height, &channels, 0)) == nullptr)
	{
		logging::log("stbi_load_from_memory() returned nullptr: " + std::string(stbi_failure_reason()), logging::Severity::CRITICAL);
		exit(EXIT_FAILURE);
	}
	return pixels;
}

std::string get_extension_from_cubemap_directory(std::string_view directory_path)
{
	return std::filesystem::directory_iterator(directory_path)->path().extension().string();
}

std::vector<std::string> get_files_from_directory(std::string_view directory_path)
{
	std::vector<std::string> files;
	for(const auto& file : std::filesystem::directory_iterator(directory_path))
	{
		files.push_back(file.path().string());
	}
	return files;
}

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
	const int base64_index[256] = {
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

int get_viewport_width()
{
	GLint viewport_data[4];
	glGetIntegerv(GL_VIEWPORT, viewport_data);
	return viewport_data[2];
}

int get_viewport_height()
{
	GLint viewport_data[4];
	glGetIntegerv(GL_VIEWPORT, viewport_data);
	return viewport_data[3];
}

std::vector<GLushort> get_2d_ebo_values()
{
	return std::vector<GLushort>{0, 1, 2, 0, 3, 1};
}

Vertices get_2d_vertices()
{
	std::vector<glm::vec3> position_vector //on simule un vec2 avec un vec3 => la composante z vaut donc 0.0f
	{
		glm::vec3(0.0f, 1.0f, 0.0f), //carré de coté 1 pixel => la multiplication par une size fonctionne (ex avec size = (300, 200) : (1, 1) * (300, 200) = (300, 200))
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f)
	};
	std::vector<glm::vec4> color_vector
	{
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
	};
	std::vector<glm::vec2> texcoord_vector
	{
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 1.0f)
	};

	Vertices vertices(position_vector.size());
	vertices.add_position_attributes(position_vector);
	vertices.add_color_attributes(color_vector);
	vertices.add_texcoord_attributes(texcoord_vector);

	return vertices;
}

std::vector<GLushort> get_3d_ebo_values()
{
	return std::vector<GLushort> //ex du rubix cube avec la face verte au-dessus et la face rouge face à nous
	{
		0, 1, 4, 4, 5, 1, //face rouge
		6, 2, 0, 6, 4, 0, //face jaune
		7, 3, 1, 7, 5, 1, //face blanche
		2, 3, 6, 6, 7, 3, //face orange
		6, 4, 5, 6, 7, 5, //face verte
		0, 1, 2, 2, 3, 1 //face bleue
	};
}

Vertices get_3d_vertices()
{
	std::vector<glm::vec3> position_vector //ex du rubix cube avec la face verte au-dessus et la face rouge face à nous
	{
		//face bleue
		glm::vec3(-0.5f, -0.5f,  0.5f), //0 : en bas à gauche
		glm::vec3( 0.5f, -0.5f,  0.5f), //1 : en bas à droite
		glm::vec3(-0.5f, -0.5f, -0.5f), //2 : en haut à gauche
		glm::vec3( 0.5f, -0.5f, -0.5f), //3 : en haut à droite
		
		//face verte
		glm::vec3(-0.5f, 0.5f,  0.5f), //4 : en bas à gauche
		glm::vec3( 0.5f, 0.5f,  0.5f), //5 : en bas à droite
		glm::vec3(-0.5f, 0.5f, -0.5f), //6 : en haut à gauche
		glm::vec3( 0.5f, 0.5f, -0.5f), //7 : en haut à droite
	};

	Vertices vertices(position_vector.size());
	vertices.add_position_attributes(position_vector);
	return vertices;
}

}