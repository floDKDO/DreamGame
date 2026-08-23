#pragma once

#include "Render/mesh.h"
#include "utils.h"
#include "transform.h"

#include <optional>
#include <memory>

namespace gltf
{

class Node
{
	public:
		Node(std::string name, Transform transform, std::optional<Mesh> mesh);

		void draw(ShaderProgram& shader_program);
		void add_child(std::unique_ptr<Node> child_node);
		glm::mat4 compute_model() const;
		std::string get_name() const;

		Transform transform_;
		glm::mat4 parent_matrix_;
		std::vector<std::unique_ptr<Node>> children_nodes_;

	private:
		std::string name_;
		std::optional<Mesh> mesh_;
};

}