#include "node.h"
#include "gltf.h"
#include "Logging/logging.h"
//#include "gl_resource_manager.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace gltf
{

Node::Node(std::string name, Transform transform, glm::mat4 parent_matrix, Mesh::MeshId mesh_id, std::optional<AABB> aabb)
	: is_empty_node_(false), name_(name), mesh_id_(mesh_id), aabb_(aabb), transform_(transform), parent_matrix_(parent_matrix), position_(glm::vec3(parent_matrix_ * glm::vec4(transform_.position_, 1.0f)))
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

	if(const Mesh* mesh = resource::get_mesh(mesh_id_); mesh != nullptr)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mesh->draw();
	}

	//TODO
	/*if(aabb_.has_value())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //affichage wireframe pour AABB
		aabb_->draw();
	}*/

	for(Node& children_node : children_nodes_)
	{
		children_node.draw();
	}
}

void Node::set_translation(glm::vec3 position)
{
	transform_.position_ = position;
	update_position();
	update_parent_matrix_of_children(*this);
}

void Node::add_translation(glm::vec3 position)
{
	transform_.position_ += position;
	update_position();
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
	//TODO : faire comme avec le membre position_ (avoir un membre qui prend en compte la parent_matrix_) ?
	update_parent_matrix_of_root_children();
}

void Node::add_rotation(glm::quat rotation)
{
	transform_.rotation_ += rotation;
	//TODO : faire comme avec le membre position_ (avoir un membre qui prend en compte la parent_matrix_) ?
	update_parent_matrix_of_root_children();
}

void Node::set_scale(glm::vec3 scale)
{
	transform_.scale_ = scale;
	//TODO : faire comme avec le membre position_ (avoir un membre qui prend en compte la parent_matrix_) ?
	update_parent_matrix_of_root_children();
}

void Node::add_scale(glm::vec3 scale)
{
	transform_.scale_ += scale;
	//TODO : faire comme avec le membre position_ (avoir un membre qui prend en compte la parent_matrix_) ?
	update_parent_matrix_of_root_children();
}

void Node::update_position()
{
	position_ = glm::vec3(parent_matrix_ * glm::vec4(transform_.position_, 1.0f));
}

void Node::update_parent_matrix_of_root_children()
{
	for(Node& child_node : children_nodes_)
	{
		child_node.parent_matrix_ = compute_model();
		child_node.update_position();
		update_parent_matrix_of_children(child_node);
	}
}

void Node::update_parent_matrix_of_children(Node& node)
{
	for(Node& child_node : node.children_nodes_)
	{
		child_node.parent_matrix_ = node.compute_model();
		child_node.update_position();
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

std::vector<glm::vec3> Node::get_aabb_from_position() const
{
	std::vector<glm::vec3> aabb_points = aabb_.value().get_corners();
	glm::vec3 min_values(std::numeric_limits<float>::max());
	glm::vec3 max_values(std::numeric_limits<float>::lowest());

	for(const glm::vec3& aabb_point : aabb_points)
	{
		glm::vec3 world_position_attribute = glm::vec4(aabb_point, 1.0f) * compute_model();
		min_values = glm::min(world_position_attribute, min_values);
		max_values = glm::max(world_position_attribute, max_values);
	}

	min_values += position_;
	max_values += position_;

	std::vector<glm::vec3> aabb_points_world
	{
		glm::vec3(max_values.x, min_values.y, min_values.z), //0 : bottom face, upper right
		glm::vec3(min_values.x, min_values.y, min_values.z), //1 : bottom face, upper left
		glm::vec3(max_values.x, min_values.y, max_values.z), //2 : bottom face, down right
		glm::vec3(min_values.x, min_values.y, max_values.z), //3 : bottom face, down left
		glm::vec3(max_values.x, max_values.y, min_values.z), //4 : top face, upper right
		glm::vec3(min_values.x, max_values.y, min_values.z), //5 : top face, upper left
		glm::vec3(max_values.x, max_values.y, max_values.z), //6 : top face, down right
		glm::vec3(min_values.x, max_values.y, max_values.z)  //7 : top face, down left
	};
	return aabb_points_world;
}

std::optional<AABB> Node::get_world_aabb() const
{
	if(!aabb_.has_value())
	{
		return std::nullopt;
	}
	else
	{
		return AABB(get_aabb_from_position());
	}
}

const std::vector<Node>&Node::get_children_nodes() const
{
	return children_nodes_;
}

const glm::vec3& Node::get_position() const
{
	return position_;
}

}