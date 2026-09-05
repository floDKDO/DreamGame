#pragma once

#include "node.h"

#include <vector>

namespace gltf
{

class Scene
{
	public:
		explicit Scene(std::vector<std::unique_ptr<Node>> root_nodes);

		void set_model_translation(glm::vec3 position);
		void set_model_rotation(glm::quat rotation);
		void set_model_scale(glm::vec3 scale);

		void add_model_translation(glm::vec3 position);
		void add_model_rotation(glm::quat rotation);
		void add_model_scale(glm::vec3 scale);

		glm::vec3& get_model_position() const; //get position of first root_node_
		std::string get_model_name() const; //get name of first root_node_
		Node* get_model() const;
		const std::unique_ptr<Node>& get_node() const;

		void draw(ShaderProgram& shader_program);

	private:
		void update_parent_matrix_of_children(gltf::Node* node);

		std::vector<std::unique_ptr<Node>> root_nodes_;
};

}