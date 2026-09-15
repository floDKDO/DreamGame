#include "node.h"
#include "gltf.h"
#include "Logging/logging.h"
//#include "gl_resource_manager.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace gltf
{

Node::Node(std::string name, Transform transform, glm::mat4 parent_matrix, resource::MeshKey mesh_key, std::optional<AABB> aabb)
	: is_empty_node_(false), name_(name), mesh_key_(mesh_key), aabb_(aabb), transform_(transform), parent_matrix_(parent_matrix)
{}

glm::mat4 Node::compute_model() const
{
	return gltf::get_transformation_matrix(parent_matrix_, transform_.position_, transform_.rotation_, transform_.scale_);
}

glm::mat4 Node::get_parent_matrix() const
{
	return parent_matrix_;
}

void Node::set_empty_node()
{
	is_empty_node_ = true;
}

bool Node::is_empty_node() const
{
	return is_empty_node_;
}

void Node::draw()
{
	resource::set_uniform_matrix_4fv("model_matrix_", glm::value_ptr(compute_model()));

	if(const Mesh* mesh = resource::get_mesh(mesh_key_); mesh != nullptr)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mesh->draw();
	}

	if(aabb_.has_value())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //affichage wireframe pour AABB
		aabb_->draw();
	}

	for(Node& children_node : children_nodes_)
	{
		children_node.draw();
	}
}

void Node::set_translation(glm::vec3 position)
{
	transform_.position_ = position;
	update_parent_matrix_of_children(*this);
}

void Node::add_translation(glm::vec3 position)
{
	transform_.position_ += position;
	update_parent_matrix_of_children(*this);
}

void Node::add_translation_x(float x)
{
	glm::vec3 position(0.0f);
	position.x += x;
	add_translation(position);
}

void Node::add_translation_y(float y)
{
	glm::vec3 position(0.0f);
	position.y += y;
	add_translation(position);
}

void Node::add_translation_z(float z)
{
	glm::vec3 position(0.0f);
	position.z += z;
	add_translation(position);
}

void Node::set_rotation(glm::quat rotation)
{
	transform_.rotation_ = rotation;
	update_parent_matrix_of_children(*this);
}

void Node::add_rotation(glm::quat rotation)
{
	transform_.rotation_ += rotation;
	update_parent_matrix_of_children(*this);
}

void Node::set_scale(glm::vec3 scale)
{
	transform_.scale_ = scale;
	update_parent_matrix_of_children(*this);
}

void Node::add_scale(glm::vec3 scale)
{
	transform_.scale_ += scale;
	update_parent_matrix_of_children(*this);
}

void Node::update_parent_matrix_of_children(Node& node)
{
	for(Node& child_node : node.children_nodes_)
	{
		child_node.parent_matrix_ = node.compute_model();
		update_parent_matrix_of_children(child_node);
	}
}

void Node::add_child(Node child_node)
{
	children_nodes_.push_back(child_node);
}

std::string Node::get_name() const
{
	return name_;
}

glm::vec3 Node::get_min_values_aabb() const
{
	if(!aabb_.has_value())
	{
		logging::log("The node \"" + name_ + " " + std::to_string(mesh_key_.mesh_index_) + "\" does not have a AABB", logging::Severity::NOTICE);
		return glm::vec3(0.0f);
	}

	std::vector<glm::vec3> position_attributes = aabb_.value().get_corners();
	glm::vec3 min_value(std::numeric_limits<float>::max());

	for(const glm::vec3& position_attribute : position_attributes)
	{
		glm::vec3 world_position_attribute = glm::vec4(position_attribute, 1.0f) * compute_model();
		min_value = glm::min(world_position_attribute, min_value);
	}
	return min_value;
}

glm::vec3 Node::get_max_values_aabb() const
{
	if(!aabb_.has_value())
	{
		logging::log("The node \"" + name_ + " " + std::to_string(mesh_key_.mesh_index_) + "\" does not have a AABB", logging::Severity::NOTICE);
		return glm::vec3(0.0f);
	}

	std::vector<glm::vec3> position_attributes = aabb_.value().get_corners();
	glm::vec3 max_value(std::numeric_limits<float>::min());

	for(const glm::vec3& position_attribute : position_attributes)
	{
		glm::vec3 world_position_attribute = glm::vec4(position_attribute, 1.0f) * compute_model();
		max_value = glm::max(world_position_attribute, max_value);
	}
	return max_value;
}

const glm::vec3& Node::get_position() const
{
	return transform_.position_;
}

}