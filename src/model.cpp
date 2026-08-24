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
	scene_->get_model()->transform_.position_ += position;
	update_parent_matrix_of_children(scene_->get_model());
}

void Model::set_translation(glm::vec3 position)
{
	scene_->get_model()->transform_.position_ = position;
	update_parent_matrix_of_children(scene_->get_model());
}

void Model::add_rotation(glm::quat rotation)
{
	scene_->get_model()->transform_.rotation_ += rotation;
	update_parent_matrix_of_children(scene_->get_model());
}

void Model::set_rotation(glm::quat rotation)
{
	scene_->get_model()->transform_.rotation_ = rotation;
	update_parent_matrix_of_children(scene_->get_model());
}

void Model::add_scale(glm::vec3 scale)
{
	scene_->get_model()->transform_.scale_ += scale;
	update_parent_matrix_of_children(scene_->get_model());
}

void Model::set_scale(glm::vec3 scale)
{
	scene_->get_model()->transform_.scale_ = scale;
	update_parent_matrix_of_children(scene_->get_model());
}

void Model::update_parent_matrix_of_children(gltf::Node* node)
{
	for(std::unique_ptr<gltf::Node>& child_node : node->children_nodes_)
	{
		child_node->parent_matrix_ = node->compute_model();
		update_parent_matrix_of_children(child_node.get());
	}
}