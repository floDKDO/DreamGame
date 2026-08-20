#include "node.h"
#include "gltf.h"

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

namespace gltf
{

Node::Node(std::string name, Transform transform, std::optional<Mesh> mesh)
	: name_(name), mesh_(mesh), transform_(transform), rotation_info_({0.0f, glm::vec3(0.0f, 1.0f, 0.0f)}), parent_matrix_(1.0f)
{}

glm::mat4 Node::compute_model()
{
	glm::mat4 model_matrix(1.0f); //reconstruction de la matrice model à chaque frame
	model_matrix = glm::translate(model_matrix, transform_.position_);

	//std::cout << "Dans model de " << name_ << ", Quaternion: " << transform_.rotation_quaternion_.x << ", " << transform_.rotation_quaternion_.y << ", " << transform_.rotation_quaternion_.z << ", " << transform_.rotation_quaternion_.w << std::endl;

	//TODO : rotation (vérifier)
	model_matrix *= glm::mat4_cast(transform_.rotation_quaternion_);
	//model_matrix = glm::rotate(model_matrix, rotation_info_.angle_, rotation_info_.axis_);
	model_matrix = glm::scale(model_matrix, transform_.scale_);
	model_matrix *= parent_matrix_;

	return model_matrix;
}

void Node::draw(ShaderProgram& shader_program)
{
	shader_program.set_uniform_matrix_4fv("model_matrix_", glm::value_ptr(compute_model()));

	if(mesh_.has_value())
	{
		mesh_->draw(shader_program);
	}

	for(std::unique_ptr<Node>& children_node : children_nodes_)
	{
		children_node->draw(shader_program);
	}
}

void Node::add_child(std::unique_ptr<Node> child_node)
{
	children_nodes_.push_back(std::move(child_node));
}

std::string Node::get_name() const
{
	return name_;
}

}