#include "image_loader.h"
#include "Logging/logging.h"

#include <stb/stb_image.h>
#include <string>

ImageLoader::ImageLoader(std::string_view path)
	: path_(path)
{
	if((pixels_ = stbi_load(path.data(), &initial_width_, &initial_height_, &channels_, 0)) == nullptr)
	{
		logging::log("stbi_load() returned nullptr: " + std::string(stbi_failure_reason()), logging::Severity::CRITICAL);
		exit(EXIT_FAILURE);
	}
}

/*ImageLoader::ImageLoader(unsigned char* data, int size)
{
	if((pixels_ = stbi_load_from_memory(data, size, &w_, &h_, &channels_, 0)) == nullptr)
	{
		logging::log("stbi_load_from_memory() returned nullptr: " + std::string(stbi_failure_reason()), logging::Severity::CRITICAL);
		exit(EXIT_FAILURE);
	}
}*/

//TODO : à retirer ?
ImageLoader::ImageLoader(const ImageTexture& image_texture)
{
	if(std::holds_alternative<ImagePath>(image_texture.image_value_))
	{
		const ImagePath& image_path = std::get<ImagePath>(image_texture.image_value_);
		if((pixels_ = stbi_load(image_path.c_str(), &initial_width_, &initial_height_, &channels_, 0)) == nullptr)
		{
			logging::log("stbi_load() returned nullptr for the file " + image_path + ": " + std::string(stbi_failure_reason()), logging::Severity::CRITICAL);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		const ImageData& image_data = std::get<ImageData>(image_texture.image_value_);
		if((pixels_ = stbi_load_from_memory(image_data.data(), int(image_data.size()), &initial_width_, &initial_height_, &channels_, 0)) == nullptr)
		{
			logging::log("stbi_load_from_memory() returned nullptr: " + std::string(stbi_failure_reason()), logging::Severity::CRITICAL);
			exit(EXIT_FAILURE);
		}
	}
}

ImageLoader::ImageLoader(ImageLoader&& image_loader)
	: path_(image_loader.path_), pixels_(image_loader.pixels_), initial_width_(image_loader.initial_width_), initial_height_(image_loader.initial_height_), channels_(image_loader.channels_)
{
	image_loader.pixels_ = nullptr;
}

ImageLoader& ImageLoader::operator=(ImageLoader&& image_loader)
{
	if(this == &image_loader)
	{
		return *this;
	}

	stbi_image_free(pixels_);

	path_ = image_loader.path_;
	pixels_ = image_loader.pixels_;
	initial_width_ = image_loader.initial_width_;
	initial_height_ = image_loader.initial_height_;
	channels_ = image_loader.channels_;
	image_loader.pixels_ = nullptr;

	return *this;
}

ImageLoader::~ImageLoader()
{
	stbi_image_free(pixels_);
}

std::string ImageLoader::get_path() const
{
	return path_;
}

unsigned char* ImageLoader::get_pixels() const
{
	return pixels_;
}

int ImageLoader::get_initial_width() const
{
	return initial_width_;
}

int ImageLoader::get_initial_height() const
{
	return initial_height_;
}

int ImageLoader::get_channels() const
{
	return channels_;
}