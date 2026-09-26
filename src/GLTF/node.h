#pragma once

#include "Collision/aabb.h"
#include "Common/transform.h"
#include "Resource/gl_resource_manager.h"

#include <optional>

namespace gltf
{

class Node
{
	public:
		struct Info
		{
			Transform transform_;
			glm::mat4 parent_matrix_;
			Mesh::Id mesh_id_;
			std::pair<Mesh::Id, std::optional<AABB>> aabb_;
			bool is_empty_node_ = false; //<=> empty node sur Blender (le noeud n'a pas de mesh ni d'AABB)
		};

		Node(std::string_view name, const Info& node_info);

		void render();
		void add_child(std::unique_ptr<Node> child_node);
		glm::mat4 compute_model() const;
		glm::mat4 get_parent_matrix() const;
		std::string get_name() const;
		std::optional<AABB> get_world_aabb() const;
		const std::vector<std::unique_ptr<Node>>& get_children_nodes() const;
		void set_empty_node();
		bool is_empty_node() const;

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

	private:
		glm::vec3 get_true_position(glm::vec3 position) const;
		glm::quat get_true_rotation(glm::quat rotation) const;
		glm::vec3 get_true_scale(glm::vec3 scale) const;
		std::vector<glm::vec3> get_aabb_from_position() const;

		void update_parent_matrix_of_root_children();
		void update_parent_matrix_of_children(const std::unique_ptr<Node>& node);

		std::string name_; //node id
		Info node_info_;
		std::vector<std::unique_ptr<Node>> children_nodes_;
};

}