#pragma once

#include <string_view>

//TODO : renommer add_cubemap ?
class Skybox
{
	public:
		Skybox(std::string_view directory_path);

		void render() const;

	private:
		std::size_t mesh_id_;
};