#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>

struct Texture
{
	//TODO : public/private
	Texture()
		: texture_id_(0), texture_unit_(0), image_path_(""),
		  mag_filter_(0), min_filter_(0), wrap_s_(GL_REPEAT), wrap_t_(GL_REPEAT)
	{}

	GLuint texture_id_; //id returned when calling glCreateTextures
	GLuint texture_unit_;
	std::string image_path_;
	std::vector<unsigned char> image_data_;
	GLint mag_filter_;
	GLint min_filter_;
	GLint wrap_s_;
	GLint wrap_t_;
};