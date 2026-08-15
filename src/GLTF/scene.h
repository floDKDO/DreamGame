#pragma once

#include "model.h"
#include "gltf_file.h"

#include <string_view>
#include <vector>

namespace gltf
{

class Scene
{
	public:
		explicit Scene(std::string_view path);
		Scene(const Scene& scene) = delete;
		Scene(Scene&& scene) = delete;
		Scene& operator=(const Scene& scene) = delete;
		Scene& operator=(Scene&& scene) = delete;
		~Scene();

		void draw(ShaderProgram& shader_program);
		Model* get_model_by_name(std::string name);

	private:
		glTFFile gltf_file_;
		std::string path_;
		std::vector<std::unique_ptr<Model>> models_;
};

}