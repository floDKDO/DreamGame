#pragma once

#include "RAII_SDL3/surface.h"

#include <GL/glew.h>
#include <string>
#include <variant>

struct TextureInfo
{
	bool should_have_mipmaps_ = true;
	GLenum target_ = GL_TEXTURE_2D;
	GLuint id_ = 0; //id retourné par la fonction glCreateTextures()
	GLuint texture_unit_ = 0;
};

struct SamplingParameters
{
	GLint mag_filter_ = GL_LINEAR;
	GLint min_filter_ = GL_NEAREST_MIPMAP_LINEAR;
	GLint wrap_s_ = GL_REPEAT;
	GLint wrap_t_ = GL_REPEAT;
	GLint wrap_r_ = GL_REPEAT;
};

//TODO : à retirer
//using ImagePath = std::string; //std::string = chemin de l'image
//using ImageData = std::vector<unsigned char>; //std::vector<unsigned char> = données de l'image si son fichier glTF ne contient pas de chemin pour l'image (ex : embedded glTF)

struct ImageTexture
{
	//std::variant<ImagePath, ImageData> image_value_; //TODO : à retirer
	std::string path_; //si vide (= valeur initiale), alors c'est un "embedded glTF"
	int channels_ = 4;

	//TODO : RAII avec ImageLoader ?
	unsigned char* pixels_ = nullptr; //devra être libéré avec stbi_image_free()
};

struct CubemapTexture
{
	//Toutes les ImageTextures ont la même width et height
	ImageTexture back_;
	ImageTexture bottom_;
	ImageTexture front_;
	ImageTexture left_;
	ImageTexture right_;
	ImageTexture top_;

	//TODO : éventuellement créer un champ channels_ ?
};

struct TextTexture
{
	void* pixels_ = nullptr;

	//sdl::Surface surface_; //TODO
};

struct Texture
{
	TextureInfo info_;
	SamplingParameters sampling_params_;

	int initial_width_ = 0;
	int initial_height_ = 0;
	std::variant<ImageTexture, CubemapTexture, TextTexture> specific_info_;
};