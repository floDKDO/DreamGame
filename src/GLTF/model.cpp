#include "model.h"

namespace gltf
{

/*Model::Model(std::string_view path)
	: gltf_file_(path), nodes_(gltf_file_.get_nodes())
{}*/

Model::Model(std::unique_ptr<Node> root_node)
	: root_node_(std::move(root_node))
{}

void Model::draw(ShaderProgram& shader_program)
{
	/*for(std::pair<unsigned int, Node>& node_pair : nodes_)
	{
		Node& node = node_pair.second;
		node.draw(shader_program);
	}*/
	root_node_->draw(shader_program);
}

glm::vec3 Model::get_position() const
{
	return root_node_->transform_.position_;
}

std::string Model::get_name() const
{
	return root_node_->get_name();
}

void Model::set_transform(Transform transform)
{
	/*for(std::pair<unsigned int, Node>& node_pair : nodes_)
	{
		Node& node = node_pair.second;
		node.transform_ = transform;
	}*/
}

void Model::set_rotation_info(Node::RotationInfo rotation_info)
{
	/*for(std::pair<unsigned int, Node>& node_pair : nodes_)
	{
		Node& node = node_pair.second;
		node.rotation_info_ = rotation_info;
	}*/
}

void Model::translate(glm::vec3 position)
{
	translate_children(root_node_.get(), position);
}

void Model::translate_children(Node* node, glm::vec3 position)
{
	node->transform_.position_ += position;
	for(std::unique_ptr<Node>& child_node : node->children_nodes_)
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
	scale_children(root_node_.get(), scale);
}

void Model::scale_children(Node* node, glm::vec3 scale)
{
	node->transform_.scale_ += scale;
	for(std::unique_ptr<Node>& child_node : node->children_nodes_)
	{
		child_node->transform_.position_ += scale;
		scale_children(child_node.get(), scale);
	}
}

}