#pragma once

#include "aabb.h"
#include "Common/utils.h"
#include "Common/transform.h"

#include <optional>
#include <memory>

namespace gltf
{

class Node
{
	public:
		Node(std::string name, Transform transform, std::optional<Mesh> mesh, std::optional<AABB> aabb);

		void draw(ShaderProgram& shader_program);
		void add_child(std::unique_ptr<Node> child_node);
		glm::mat4 compute_model() const;
		std::string get_name() const;
		glm::vec3 get_min_values_aabb() const;
		glm::vec3 get_max_values_aabb() const;

		void set_model_translation(glm::vec3 position);
		void set_model_rotation(glm::quat rotation);
		void set_model_scale(glm::vec3 scale);

		void add_model_translation(glm::vec3 position);
		void add_model_rotation(glm::quat rotation);
		void add_model_scale(glm::vec3 scale);

		Transform transform_;
		glm::mat4 parent_matrix_;
		std::vector<std::unique_ptr<Node>> children_nodes_;

	private:
		void update_parent_matrix_of_children(gltf::Node* node);

		std::string name_;
		std::optional<Mesh> mesh_;
		std::optional<AABB> aabb_;
};

}