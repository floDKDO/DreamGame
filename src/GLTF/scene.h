#pragma once

#include "node.h"
//#include "gltf_file.h"

#include <string_view>
#include <vector>

namespace gltf
{

class Scene
{
	public:
		explicit Scene(std::vector<std::unique_ptr<Node>> root_nodes, bool is_model);

		glm::vec3 get_model_position() const; //get position of first root_node_
		std::string get_model_name() const; //get name of first root_node_
		Node* get_model() const;

		void draw(ShaderProgram& shader_program);
		//Model* get_model_by_name(std::string name);

	private:
		//glTFFile gltf_file_;
		//std::string path_;
		//std::vector<std::unique_ptr<Model>> models_;
		std::vector<std::unique_ptr<Node>> root_nodes_;
		bool is_model_; //if false => LEVEL
};

}