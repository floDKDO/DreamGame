#pragma once

#include <tinygltf/tiny_gltf_v3.h>
#include <memory>
#include <string>
#include <string_view>

namespace gltf
{

class Scene;

class glTFFile
{
	public:
		explicit glTFFile(std::string_view path);
		~glTFFile();

		std::unique_ptr<Scene> get_scene() const;

	private:
		void open();
		void close();
		void print_info() const;

		std::string path_;
		tg3_model model_tg3_;
		tg3_error_stack error_stack_tg3_;
};

}