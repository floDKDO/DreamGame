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
		Node(std::string name, Transform transform, glm::mat4 parent_matrix, std::optional<Mesh> mesh, std::optional<AABB> aabb);

		void draw(ShaderProgram& shader_program);
		void add_child(Node child_node);
		glm::mat4 compute_model() const;
		glm::mat4 get_parent_matrix() const;
		std::string get_name() const;
		glm::vec3 get_min_values_aabb() const;
		glm::vec3 get_max_values_aabb() const;

		const glm::vec3& get_position() const; //retourne une référence car la position est utilisée en tant que membre (const ref) dans la classe Camera

		void set_translation(glm::vec3 position);
		void set_rotation(glm::quat rotation);
		void set_scale(glm::vec3 scale);

		void add_translation(glm::vec3 position);
		void add_translation_x(float x);
		void add_translation_y(float y);
		void add_translation_z(float z);

		void add_rotation(glm::quat rotation);
		void add_scale(glm::vec3 scale);

		std::vector<Node> children_nodes_;

	private:
		void update_parent_matrix_of_children(Node& node);

		Transform transform_;
		glm::mat4 parent_matrix_;
		std::string name_;
		std::optional<Mesh> mesh_;
		std::optional<AABB> aabb_;
};

}