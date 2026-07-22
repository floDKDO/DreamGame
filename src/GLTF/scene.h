#pragma once

#include "node.h"

#include <string_view>
#include <vector>

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

	private:
		std::string path_;
		std::vector<Node> nodes_;
};