#pragma once

#include "Image/image_loader.h"

//TODO : renommer en Cubemap ?
class Skybox
{
	public:
		Skybox(std::string_view directory_path);

		void render() const;

	private:
		ImageLoader back_;
		ImageLoader bottom_;
		ImageLoader front_;
		ImageLoader left_;
		ImageLoader right_;
		ImageLoader top_;

		std::size_t mesh_id_;
};