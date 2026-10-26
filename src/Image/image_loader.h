#pragma once

#include "Render/texture.h"

#include <string_view>

class ImageLoader
{
	public:
		explicit ImageLoader(std::string_view path);
		//ImageLoader(unsigned char* data, int size);
		explicit ImageLoader(const ImageTexture& image_texture);
		ImageLoader(const ImageLoader&) = delete;
		ImageLoader(ImageLoader&& image_loader);
		ImageLoader& operator=(const ImageLoader&) = delete;
		ImageLoader& operator=(ImageLoader&& image_loader);
		~ImageLoader();

		std::string get_path() const;
		unsigned char* get_pixels() const;
		int get_initial_width() const;
		int get_initial_height() const;
		int get_channels() const;

	private:
		std::string path_;
		unsigned char* pixels_;
		int initial_width_, initial_height_; //width and height of the image before some eventual modifications
		int channels_;
};