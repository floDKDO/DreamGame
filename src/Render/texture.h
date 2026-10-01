#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>

struct Texture
{
	GLuint texture_id_ = 0; //id returned when calling glCreateTextures
	GLuint texture_unit_ = 0;
	std::string image_path_;
	std::vector<unsigned char> image_data_; //contient les données de l'image si image_path_ est vide
	GLint mag_filter_ = GL_NEAREST;
	GLint min_filter_ = GL_NEAREST;
	GLint wrap_s_ = GL_REPEAT;
	GLint wrap_t_ = GL_REPEAT;
};