#pragma once

#include <tinygltf/tiny_gltf_v3.h>
#include <string>
#include <string_view>

namespace gltf
{

class Node;

class glTFFile
{
	public:
		explicit glTFFile(std::string_view path);
		glTFFile(const glTFFile& gltf_file) = delete;
		glTFFile(glTFFile&& gltf_file);
		glTFFile& operator=(const glTFFile& gltf_file) = delete;
		glTFFile& operator=(glTFFile&& gltf_file);
		~glTFFile();

		Node get_root_node() const; //TODO : pas ouf, vaut mieux retourner qqc de const sans copie

	private:
		void open();
		void close();
		void print_info() const;

		std::string path_;
		tg3_model model_tg3_;
		tg3_error_stack error_stack_tg3_;
};

}