#include "scene.h"

#include <iostream>

namespace gltf
{

Scene::Scene(std::vector<std::unique_ptr<Node>> root_nodes, bool is_model)
	: root_nodes_(std::move(root_nodes)), is_model_(is_model)
{}

void Scene::set_model_position(glm::vec3 position)
{
	root_nodes_[0]->transform_.position_ = position;
}

void Scene::set_model_rotation(glm::quat rotation)
{
	root_nodes_[0]->transform_.rotation_quaternion_ = rotation;
}

void Scene::set_model_scale(glm::vec3 scale)
{
	root_nodes_[0]->transform_.scale_ = scale;
}

glm::vec3& Scene::get_model_position() const //get position of first root_node_
{
	//TODO : voir si je garde les tests ou je pars du principe qu'il n'y aura pas d'erreur comme c'est mon code donc mes appels
	// de toute façon, comme je retourne une référence, le "else" est faux...
	if(is_model_)
	{
		return root_nodes_[0]->transform_.position_;
	}
	else
	{
		std::cout << "This is not a model!\n";
		return glm::vec3(0.0f);
	}
}

std::string Scene::get_model_name() const //get name of first root_node_
{
	if(is_model_)
	{
		return root_nodes_[0]->get_name();
	}
	else
	{
		std::cout << "This is not a model!\n";
		return "";
	}
}

Node* Scene::get_model() const
{
	if(is_model_)
	{
		return root_nodes_[0].get();
	}
	else
	{
		std::cout << "This is not a model!\n";
		return nullptr;
	}
}

void Scene::draw(ShaderProgram& shader_program)
{
	if(is_model_)
	{
		root_nodes_[0]->draw(shader_program);
	}
	else
	{
		for(std::unique_ptr<Node>& root_node : root_nodes_)
		{
			root_node->draw(shader_program);
		}
	}
}

/*Model* Scene::get_model_by_name(std::string name)
{
	for(std::unique_ptr<Model>& model : models_)
	{
		if(model->get_name() == name)
		{
			return model.get();
		}
	}
	return nullptr;
}*/

}