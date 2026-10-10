#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>
#include <variant>

struct TextureInfo
{
	GLuint id_ = 0; //id returned when calling glCreateTextures
	GLuint texture_unit_ = 0;
	GLint mag_filter_ = GL_LINEAR;
	GLint min_filter_ = GL_NEAREST_MIPMAP_LINEAR;
	GLint wrap_s_ = GL_REPEAT;
	GLint wrap_t_ = GL_REPEAT;
};

using ImagePath = std::string; //std::string = chemin de l'image
using ImageData = std::vector<unsigned char>; //std::vector<unsigned char> = données de l'image si son fichier glTF ne contient pas de chemin pour l'image (ex : embedded glTF)
struct ImageTexture
{
	std::variant<ImagePath, ImageData> image_value_;
	int initial_width_, initial_height_;
	int channels_ = 4;
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
	bool should_have_mipmaps_ = true;
};