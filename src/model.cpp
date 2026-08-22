#include "model.h"

#include <iostream>

Model::Model(std::string_view path)
	: gltf_file_(path), scene_(std::move(gltf_file_.get_scene()))
{}

Model::Model(std::string_view path, Transform transform)
	: gltf_file_(path), scene_(std::move(gltf_file_.get_scene()))
{
	scene_->set_model_position(transform.position_);
	scene_->set_model_rotation(transform.rotation_quaternion_);
	scene_->set_model_scale(transform.scale_);
}

/*Model::Model(std::unique_ptr<Node> root_node)
	: root_node_(std::move(root_node))
{}*/

void Model::draw(ShaderProgram& shader_program)
{
	/*for(std::pair<unsigned int, Node>& node_pair : nodes_)
	{
		Node& node = node_pair.second;
		node.draw(shader_program);
	}*/
	//root_node_->draw(shader_program);

	scene_->draw(shader_program);
}

glm::vec3 Model::get_position() const
{
	return scene_->get_model_position();
}

std::string Model::get_name() const
{
	return scene_->get_model_name();
}

void Model::set_transform(Transform transform)
{
	/*for(std::pair<unsigned int, Node>& node_pair : nodes_)
	{
		Node& node = node_pair.second;
		node.transform_ = transform;
	}*/
}

void Model::set_rotation_info(gltf::Node::RotationInfo rotation_info)
{
	/*for(std::pair<unsigned int, Node>& node_pair : nodes_)
	{
		Node& node = node_pair.second;
		node.rotation_info_ = rotation_info;
	}*/
}

void Model::translate(glm::vec3 position)
{
	translate_children(scene_->get_model(), position);
}

void Model::translate_children(gltf::Node* node, glm::vec3 position)
{
	node->transform_.position_ += position;
	for(std::unique_ptr<gltf::Node>& child_node : node->children_nodes_)
	{
		child_node->transform_.position_ += position;
		translate_children(child_node.get(), position);
	}
}

void Model::rotate(glm::vec4 rotation)
{
	/*for(std::pair<unsigned int, Node>& node_pair : nodes_)
	{
		Node& node = node_pair.second;
		node.transform_.rotation_ += rotation;
	}*/
}

void Model::scale(glm::vec3 scale)
{
	scale_children(scene_->get_model(), scale);
}

void Model::scale_children(gltf::Node* node, glm::vec3 scale)
{
	node->transform_.scale_ += scale;
	for(std::unique_ptr<gltf::Node>& child_node : node->children_nodes_)
	{
		child_node->transform_.position_ += scale;
		scale_children(child_node.get(), scale);
	}
}