#pragma once

#include "node.h"
//#include "gltf_file.h"

#include <memory>

namespace gltf
{

class Model
{
	public:
		//explicit Model(std::string_view path);
		explicit Model(std::unique_ptr<Node> root_node);

		void draw(ShaderProgram& shader_program);

		glm::vec3 get_position() const; //get position of root_node_
		std::string get_name() const; //get name of root_node_

		void set_transform(Transform transform);
		void set_rotation_info(Node::RotationInfo rotation_info);

		void translate(glm::vec3 position);
		void rotate(glm::vec4 rotation);
		void scale(glm::vec3 scale);

	private:
		void translate_children(Node* node, glm::vec3 position);
		void scale_children(Node* node, glm::vec3 scale);

		//glTFFile gltf_file_;
		std::unique_ptr<Node> root_node_;
};

}