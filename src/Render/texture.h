#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>
#include <variant>

struct TextureInfo
{
	GLuint id_ = 0; //id retourné par la fonction glCreateTextures()
	GLenum target_ = GL_TEXTURE_2D;
	GLuint texture_unit_ = 0;
	GLint mag_filter_ = GL_LINEAR;
	GLint min_filter_ = GL_NEAREST_MIPMAP_LINEAR;
	GLint wrap_s_ = GL_REPEAT;
	GLint wrap_t_ = GL_REPEAT;
	GLint wrap_r_ = GL_REPEAT;
};

using ImagePath = std::string; //std::string = chemin de l'image
using ImageData = std::vector<unsigned char>; //std::vector<unsigned char> = données de l'image si son fichier glTF ne contient pas de chemin pour l'image (ex : embedded glTF)
struct ImageTexture
{
	std::variant<ImagePath, ImageData> image_value_;
	int initial_width_ = 0, initial_height_ = 0;
	int channels_ = 4;
	unsigned char* pixels_ = nullptr;
};

struct CubeMapTexture
{
	//TODO : à retirer
	//std::vector<std::string> image_filenames_;
	//std::array<std::string, 6> image_filenames_; //TODO : contient 6 au lieu d'un vector 
	//int width_, height_;
	
	//TODO : à retirer
	//std::vector<ImageTexture> images_; //TODO : renommer image_textures_

	ImageTexture back_;
	ImageTexture bottom_;
	ImageTexture front_;
	ImageTexture left_;
	ImageTexture right_;
	ImageTexture top_;
};

struct TextTexture
{
	int width_ = 0;
	int height_ = 0;
	void* pixels_ = nullptr; //TODO : incohérence avec unsigned char* de ImageTexture ?
};

struct Texture
{
	TextureInfo info_;
	std::variant<ImageTexture, CubeMapTexture, TextTexture> texture_;
	bool should_have_mipmaps_ = true;
};