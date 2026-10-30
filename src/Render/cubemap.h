#pragma once

#include <string_view>

class Cubemap
{
	public:
		explicit Cubemap(std::string_view directory_path);

		void render() const;

	private:
		std::size_t mesh_id_;
};