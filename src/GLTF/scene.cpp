#include "scene.h"

#include <iostream>

namespace gltf
{

Scene::Scene(std::vector<std::unique_ptr<Node>> root_nodes)
	: root_nodes_(std::move(root_nodes))
{
	if(root_nodes_.size() > 1)
	{
		std::cout << "********** Warning: there are more than 1 root node. A model must have only one root node!\n";
	}
}

void Scene::set_model_translation(glm::vec3 position)
{
	root_nodes_[0]->transform_.position_ = position;
	update_parent_matrix_of_children(get_model());
}

void Scene::add_model_translation(glm::vec3 position)
{
	root_nodes_[0]->transform_.position_ += position;
	update_parent_matrix_of_children(get_model());
}

void Scene::set_model_rotation(glm::quat rotation)
{
	root_nodes_[0]->transform_.rotation_ = rotation;
	update_parent_matrix_of_children(get_model());
}

void Scene::add_model_rotation(glm::quat rotation)
{
	root_nodes_[0]->transform_.rotation_ += rotation;
	update_parent_matrix_of_children(get_model());
}

void Scene::set_model_scale(glm::vec3 scale)
{
	root_nodes_[0]->transform_.scale_ = scale;
	update_parent_matrix_of_children(get_model());
}

void Scene::add_model_scale(glm::vec3 scale)
{
	root_nodes_[0]->transform_.scale_ += scale;
	update_parent_matrix_of_children(get_model());
}

void Scene::update_parent_matrix_of_children(gltf::Node* node)
{
	for(std::unique_ptr<gltf::Node>& child_node : node->children_nodes_)
	{
		child_node->parent_matrix_ = node->compute_model();
		update_parent_matrix_of_children(child_node.get());
	}
}

glm::vec3& Scene::get_model_position() const //get position of first root_node_
{
	return root_nodes_[0]->transform_.position_;
}

std::string Scene::get_model_name() const //get name of first root_node_
{
	return root_nodes_[0]->get_name();
}

Node* Scene::get_model() const
{
	return root_nodes_[0].get();
}

const std::unique_ptr<Node>& Scene::get_node() const
{
	return root_nodes_[0];
}

void Scene::draw(ShaderProgram& shader_program)
{
	root_nodes_[0]->draw(shader_program);
}

}