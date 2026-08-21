#pragma once

#include "glTF/scene.h"
#include "glTF/gltf_file.h"

#include <memory>

class Map
{
	public:
		explicit Map(std::string_view path);

		void draw(ShaderProgram& shader_program);

	private:
		gltf::glTFFile gltf_file_;
		std::unique_ptr<gltf::Scene> scene_;
};