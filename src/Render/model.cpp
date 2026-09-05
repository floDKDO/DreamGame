#include "model.h"

#include <iostream>

Model::Model(std::string_view path)
	: gltf_file_(path), scene_(std::move(gltf_file_.get_scene()))
{}

Model::Model(std::string_view path, Transform transform)
	: gltf_file_(path), scene_(std::move(gltf_file_.get_scene()))
{
	set_translation(transform.position_);
	set_rotation(transform.rotation_);
	set_scale(transform.scale_);
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

gltf::Scene* Model::get_scene() const
{
	return scene_.get();
}

void Model::add_translation(glm::vec3 position)
{
	scene_->add_model_translation(position);
}

void Model::set_translation(glm::vec3 position)
{
	scene_->set_model_translation(position);
}

void Model::add_rotation(glm::quat rotation)
{
	scene_->add_model_rotation(rotation);
}

void Model::set_rotation(glm::quat rotation)
{
	scene_->set_model_rotation(rotation);
}

void Model::add_scale(glm::vec3 scale)
{
	scene_->add_model_scale(scale);
}

void Model::set_scale(glm::vec3 scale)
{
	scene_->set_model_scale(scale);
}