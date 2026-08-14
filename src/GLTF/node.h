#pragma once

#include "Render/mesh.h"
#include "utils.h"
//#include "gltf_file.h"

#include <optional>
#include <memory>
//#include <tinygltf/tiny_gltf_v3.h>

namespace gltf
{

class Node
{
	public:
		struct Transform
		{
			glm::vec3 position_ = glm::vec3(0.0f);
			glm::vec4 rotation_ = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); //TODO : inutile pour l'instant (utiliser à terme glm::quat ??)
			glm::vec3 scale_ = glm::vec3(1.0f);
		};

		struct RotationInfo
		{
			float angle_;
			glm::vec3 axis_;
		};

		//explicit Node(std::string_view path);
		Node(std::string name, std::optional<Mesh> mesh, Transform transform);

		void draw(ShaderProgram& shader_program);
		void add_child(std::unique_ptr<Node> child_node);
		glm::mat4 compute_model(/*glm::vec3 translation_vector, float angle, glm::vec3 axis*/);
		std::string get_name() const;

		Transform transform_;
		RotationInfo rotation_info_;
		glm::mat4 parent_matrix_;
		std::vector<std::unique_ptr<Node>> children_nodes_;

	private:
		void load_mesh();

		//glTFFile gltf_file_;
		std::string name_;
		std::optional<Mesh> mesh_;
		//std::unique_ptr<Node> parent_node_;
		
		//glm::mat4 model_matrix_; //TODO : toujours utile ??
};

}