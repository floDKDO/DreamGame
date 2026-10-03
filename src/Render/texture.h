#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>
#include <variant>

enum class TextureType
{
	IMAGE,
	TEXT
};

struct TextureInfo
{
	TextureType type_;
	GLuint id_ = 0; //id returned when calling glCreateTextures
	GLuint texture_unit_ = 0;
	GLint mag_filter_ = GL_NEAREST;
	GLint min_filter_ = GL_NEAREST;
	GLint wrap_s_ = GL_REPEAT;
	GLint wrap_t_ = GL_REPEAT;
};

struct ImageTexture
{
	std::string image_path_;
	std::vector<unsigned char> image_data_; //contient les données de l'image si image_path_ est vide
};

struct TextTexture
{
	int width_ = 0;
	int height_ = 0;
	void* pixels_ = nullptr;
};

struct Texture
{
	TextureInfo info_;
	std::variant<ImageTexture, TextTexture> texture_;
};