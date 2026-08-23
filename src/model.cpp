#include "model.h"

#include <iostream>

Model::Model(std::string_view path)
	: gltf_file_(path), scene_(std::move(gltf_file_.get_scene()))
{}

Model::Model(std::string_view path, Transform transform)
	: gltf_file_(path), scene_(std::move(gltf_file_.get_scene()))
{
	scene_->set_model_position(transform.position_);
	scene_->set_model_rotation(transform.rotation_);
	scene_->set_model_scale(transform.scale_);
}

void Model::draw(ShaderProgram& shader_program)
{
	scene_->draw(shader_program);
}

glm::vec3& Model::get_position() const
{
	return scene_->get_model_position();
}

std::string Model::get_name() const
{
	return scene_->get_model_name();
}

void Model::add_translation(glm::vec3 position)
{
	translate_children(scene_->get_model(), position, true);
}

void Model::set_translation(glm::vec3 position)
{
	translate_children(scene_->get_model(), position, false);
}

void Model::translate_children(gltf::Node* node, glm::vec3 position, bool is_addition)
{
	if(is_addition)
	{
		node->transform_.position_ += position;
	}
	else
	{
		node->transform_.position_ = position;
	}

	for(std::unique_ptr<gltf::Node>& child_node : node->children_nodes_)
	{
		if(is_addition)
		{
			child_node->transform_.position_ += position;
		}
		else
		{
			child_node->transform_.position_ = position;
		}
		translate_children(child_node.get(), position, is_addition);
	}
}

//TODO : débugger les méthodes suivantes

void Model::add_rotation(glm::quat rotation)
{
	//rotate_children(scene_->get_model(), rotation, true);
}

void Model::set_rotation(glm::quat rotation)
{
	//rotate_children(scene_->get_model(), rotation, false);
}

void Model::rotate_children(gltf::Node* node, glm::quat rotation, bool is_addition)
{
	/*if(is_addition)
	{
		node->transform_.rotation_ += rotation;
	}
	else
	{
		node->transform_.rotation_ = rotation;
	}

	for(std::unique_ptr<gltf::Node>& child_node : node->children_nodes_)
	{
		if(is_addition)
		{
			child_node->transform_.rotation_ += rotation;
		}
		else
		{
			child_node->transform_.rotation_ = rotation;
		}
		rotate_children(child_node.get(), rotation, is_addition);
	}*/
}

void Model::add_scale(glm::vec3 scale)
{
	//scale_children(scene_->get_model(), scale, true);
}

void Model::set_scale(glm::vec3 scale)
{
	//scale_children(scene_->get_model(), scale, false);
}

void Model::scale_children(gltf::Node* node, glm::vec3 scale, bool is_addition)
{
	/*if(is_addition)
	{
		node->transform_.scale_ += scale;
	}
	else
	{
		node->transform_.scale_ = scale;
	}

	for(std::unique_ptr<gltf::Node>& child_node : node->children_nodes_)
	{
		if(is_addition)
		{
			child_node->transform_.scale_ += scale;
		}
		else
		{
			child_node->transform_.scale_ = scale;
		}
		scale_children(child_node.get(), scale, is_addition);
	}*/
}