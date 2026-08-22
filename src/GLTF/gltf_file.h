#pragma once

#include <tinygltf/tiny_gltf_v3.h>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace gltf
{

class Scene;

class glTFFile
{
	public:
		enum class glTFKind
		{
			UNKNOWN,
			MODEL,
			MAP
		};

		explicit glTFFile(std::string_view path);
		~glTFFile();

		std::unique_ptr<Scene> get_scene() const;
		//std::vector<std::unique_ptr<Model>> get_models() const; //TODO : à retirer
		//std::vector<std::unique_ptr<Node>> get_root_nodes() const;

	private:
		void open();
		void close();
		glTFKind get_glTFKind(std::string_view path) const;
		void print_info() const;

		std::string path_;
		glTFKind gltf_kind_;
		tg3_model model_tg3_;
		tg3_error_stack error_stack_tg3_;
};

}