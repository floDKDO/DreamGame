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
	root_node_.draw(shader_program);
}

const glm::vec3& Model::get_position() const
{
	return root_node_.get_position();
}

std::string Model::get_name() const
{
	return root_node_.get_name();
}

gltf::Node& Model::get_root_node()
{
	return root_node_;
}

void Model::add_translation(glm::vec3 position)
{
	root_node_.add_translation(position);
}

void Model::add_translation_x(float x)
{
	root_node_.add_translation_x(x);
}

void Model::add_translation_y(float y)
{
	root_node_.add_translation_y(y);
}

void Model::add_translation_z(float z)
{
	root_node_.add_translation_z(z);
}

void Model::set_translation(glm::vec3 position)
{
	root_node_.set_translation(position);
}

void Model::add_rotation(glm::quat rotation)
{
	root_node_.add_rotation(rotation);
}

void Model::set_rotation(glm::quat rotation)
{
	root_node_.set_rotation(rotation);
}

void Model::add_scale(glm::vec3 scale)
{
	root_node_.add_scale(scale);
}

void Model::set_scale(glm::vec3 scale)
{
	root_node_.set_scale(scale);
}