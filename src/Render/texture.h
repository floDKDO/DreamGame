#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>

struct Texture
{
	GLuint texture_id_ = 0; //id returned when calling glCreateTextures
	GLuint texture_unit_ = 0;
	std::string image_path_;
	std::vector<unsigned char> image_data_;
	GLint mag_filter_ = 0;
	GLint min_filter_ = 0;
	GLint wrap_s_ = GL_REPEAT;
	GLint wrap_t_ = GL_REPEAT;
};