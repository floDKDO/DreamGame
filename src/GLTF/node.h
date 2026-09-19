#pragma once

#include "Render/aabb.h"
#include "Common/transform.h"
#include "gl_resource_manager.h"

#include <optional>

namespace gltf
{

class Node
{
	public:
		Node(std::string name, Transform transform, glm::mat4 parent_matrix, resource::MeshKey mesh_key, std::optional<AABB> aabb);

		void set_empty_node();
		bool is_empty_node() const;

		void draw();
		void add_child(Node child_node);
		glm::mat4 compute_model() const;
		glm::mat4 get_parent_matrix() const;
		std::string get_name() const;
		glm::vec3 get_min_aabb_from_position() const;
		glm::vec3 get_max_aabb_from_position() const;
		glm::vec3 get_center() const;

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
		void update_position();
		void update_parent_matrix_of_children(Node& node);

		bool is_empty_node_;
		Transform transform_;
		glm::mat4 parent_matrix_;
		glm::vec3 position_;
		std::string name_;
		resource::MeshKey mesh_key_;
		std::optional<AABB> aabb_;
};

}