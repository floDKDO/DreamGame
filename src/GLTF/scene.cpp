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

//TODO : inutilisée et faux car ne met pas à jour les noeuds enfants
void Scene::set_model_position(glm::vec3 position)
{
	root_nodes_[0]->transform_.position_ = position;
}

//TODO : inutilisée et faux car ne met pas à jour les noeuds enfants
void Scene::set_model_rotation(glm::quat rotation)
{
	root_nodes_[0]->transform_.rotation_ = rotation;
}

//TODO : inutilisée et faux car ne met pas à jour les noeuds enfants
void Scene::set_model_scale(glm::vec3 scale)
{
	root_nodes_[0]->transform_.scale_ = scale;
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

void Scene::draw(ShaderProgram& shader_program)
{
	root_nodes_[0]->draw(shader_program);
}

}