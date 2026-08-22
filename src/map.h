#pragma once

//#include "glTF/scene.h"
//#include "glTF/gltf_file.h"
#include "model.h"
#include "json_file.h"

#include <memory>

class Map
{
	public:
		explicit Map(std::string_view path);

		void draw(ShaderProgram& shader_program);

	private:
		//gltf::glTFFile gltf_file_;
		JSONFile map_file_;
		std::vector<std::unique_ptr<Model>> models_;

		//std::unique_ptr<gltf::Scene> scene_; //TODO : à retirer
};