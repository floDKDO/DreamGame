#include "node.h"
#include "gltf.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace gltf
{

Node::Node(std::string name, Transform transform, std::optional<Mesh> mesh, std::optional<AABB> aabb)
	: name_(name), mesh_(mesh), aabb_(aabb), transform_(transform), parent_matrix_(1.0f)
{}

glm::mat4 Node::compute_model() const
{
	return gltf::get_transformation_matrix(parent_matrix_, transform_.position_, transform_.rotation_, transform_.scale_);
}

void Node::draw(ShaderProgram& shader_program)
{
	shader_program.set_uniform_matrix_4fv("model_matrix_", glm::value_ptr(compute_model()));

	if(mesh_.has_value())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mesh_->draw(shader_program);
	}

	if(aabb_.has_value())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //affichage wireframe pour AABB
		aabb_->draw(shader_program);
	}

	for(std::unique_ptr<Node>& children_node : children_nodes_)
	{
		children_node->draw(shader_program);
	}
}

void Node::set_model_translation(glm::vec3 position)
{
	transform_.position_ = position;
	update_parent_matrix_of_children(this);
}

void Node::add_model_translation(glm::vec3 position)
{
	transform_.position_ += position;
	update_parent_matrix_of_children(this);
}

void Node::set_model_rotation(glm::quat rotation)
{
	transform_.rotation_ = rotation;
	update_parent_matrix_of_children(this);
}

void Node::add_model_rotation(glm::quat rotation)
{
	transform_.rotation_ += rotation;
	update_parent_matrix_of_children(this);
}

void Node::set_model_scale(glm::vec3 scale)
{
	transform_.scale_ = scale;
	update_parent_matrix_of_children(this);
}

void Node::add_model_scale(glm::vec3 scale)
{
	transform_.scale_ += scale;
	update_parent_matrix_of_children(this);
}

void Node::update_parent_matrix_of_children(gltf::Node* node)
{
	for(std::unique_ptr<gltf::Node>& child_node : node->children_nodes_)
	{
		child_node->parent_matrix_ = node->compute_model();
		update_parent_matrix_of_children(child_node.get());
	}
}

void Node::add_child(std::unique_ptr<Node> child_node)
{
	children_nodes_.push_back(std::move(child_node));
}

std::string Node::get_name() const
{
	return name_;
}

glm::vec3 Node::get_min_values_aabb() const
{
	if(!aabb_.has_value())
	{
		//std::cout << "NO AABB for node " << name_ << "!\n";
		return glm::vec3(0.0f);
	}

	std::vector<glm::vec3> position_attributes = aabb_.value().get_corners();
	glm::vec3 min_value(std::numeric_limits<float>::max());

	for(const glm::vec3& position_attribute : position_attributes)
	{
		glm::vec3 world_position_attribute = glm::vec4(position_attribute, 1.0f) * compute_model();

		if(world_position_attribute.x < min_value.x)
		{
			min_value.x = world_position_attribute.x;
		}
		if(world_position_attribute.y < min_value.y)
		{
			min_value.y = world_position_attribute.y;
		}
		if(world_position_attribute.z < min_value.z)
		{
			min_value.z = world_position_attribute.z;
		}
	}
	return min_value;
}

glm::vec3 Node::get_max_values_aabb() const
{
	if(!aabb_.has_value())
	{
		//std::cout << "NO AABB for node " << name_ << "!\n";
		return glm::vec3(0.0f);
	}

	std::vector<glm::vec3> position_attributes = aabb_.value().get_corners();
	glm::vec3 max_value(std::numeric_limits<float>::min());

	for(const glm::vec3& position_attribute : position_attributes)
	{
		glm::vec3 world_position_attribute = glm::vec4(position_attribute, 1.0f) * compute_model();

		if(world_position_attribute.x > max_value.x)
		{
			max_value.x = world_position_attribute.x;
		}
		if(world_position_attribute.y > max_value.y)
		{
			max_value.y = world_position_attribute.y;
		}
		if(world_position_attribute.z > max_value.z)
		{
			max_value.z = world_position_attribute.z;
		}
	}
	return max_value;
}

}