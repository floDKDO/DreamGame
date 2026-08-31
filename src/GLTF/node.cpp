#include "node.h"
#include "gltf.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace gltf
{

Node::Node(std::string name, Transform transform, std::optional<Mesh> mesh, std::optional<Mesh> aabb)
	: name_(name), mesh_(mesh), aabb_(aabb), transform_(transform), parent_matrix_(1.0f)
{}

glm::mat4 Node::compute_model() const
{
	return gltf::get_transformation_matrix(parent_matrix_, transform_.position_, transform_.rotation_, transform_.scale_);
}

void Node::draw(ShaderProgram& shader_program)
{
	shader_program.set_uniform_matrix_4fv("model_matrix_", glm::value_ptr(compute_model()));

	if(mesh_.has_value())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mesh_->draw(shader_program);
	}

	if(aabb_.has_value())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //affichage wireframe pour AABB
		aabb_->draw(shader_program);
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