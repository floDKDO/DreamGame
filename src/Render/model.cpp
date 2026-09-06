#include "model.h"

#include <iostream>

Model::Model(std::string_view path)
	: gltf_file_(path), root_node_(std::move(gltf_file_.get_root_node()))
{}

Model::Model(std::string_view path, Transform transform)
	: gltf_file_(path), root_node_(std::move(gltf_file_.get_root_node()))
{
	set_translation(transform.position_);
	set_rotation(transform.rotation_);
	set_scale(transform.scale_);
}

void Model::draw(ShaderProgram& shader_program)
{
	root_node_->draw(shader_program);
}

glm::vec3& Model::get_position() const
{
	return root_node_->transform_.position_;
}

std::string Model::get_name() const
{
	return root_node_->get_name();
}

gltf::Node* Model::get_root_node() const
{
	return root_node_.get();
}

void Model::add_translation(glm::vec3 position)
{
	root_node_->add_model_translation(position);
}

void Model::add_translation_x(float x)
{
	glm::vec3 position(0.0f);
	position.x += x;
	root_node_->add_model_translation(position);
}

void Model::add_translation_y(float y)
{
	glm::vec3 position(0.0f);
	position.y += y;
	root_node_->add_model_translation(position);
}

void Model::add_translation_z(float z)
{
	glm::vec3 position(0.0f);
	position.z += z;
	root_node_->add_model_translation(position);
}

void Model::set_translation(glm::vec3 position)
{
	root_node_->set_model_translation(position);
}

void Model::add_rotation(glm::quat rotation)
{
	root_node_->add_model_rotation(rotation);
}

void Model::set_rotation(glm::quat rotation)
{
	root_node_->set_model_rotation(rotation);
}

void Model::add_scale(glm::vec3 scale)
{
	root_node_->add_model_scale(scale);
}

void Model::set_scale(glm::vec3 scale)
{
	root_node_->set_model_scale(scale);
}