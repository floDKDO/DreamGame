#include "node.h"
#include "gltf.h"
#include "Logging/logging.h"
//#include "gl_resource_manager.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/io.hpp>

namespace gltf
{

Node::Node(std::string_view name, const NodeInfo& node_info)
	: name_(name), node_info_(node_info)
{}

glm::mat4 Node::compute_model() const
{
	return gltf::get_transformation_matrix(node_info_.parent_matrix_, node_info_.transform_.position_, node_info_.transform_.rotation_, node_info_.transform_.scale_);
}

glm::mat4 Node::get_parent_matrix() const
{
	return node_info_.parent_matrix_;
}

void Node::set_empty_node()
{
	node_info_.is_empty_node_ = true;
}

bool Node::is_empty_node() const
{
	return node_info_.is_empty_node_;
}

void Node::draw()
{
	resource::set_uniform_matrix_4fv("model_matrix_", glm::value_ptr(compute_model()));

	if(const Mesh* mesh = resource::get_mesh(node_info_.mesh_id_); mesh != nullptr)
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

glm::vec3 Node::get_true_position(glm::vec3 position) const
{
	return glm::vec3(node_info_.parent_matrix_ * glm::vec4(position, 1.0f));
}

void Node::set_translation(glm::vec3 position)
{
	node_info_.transform_.position_ = get_true_position(position);
	update_parent_matrix_of_children(*this);
}

void Node::add_translation(glm::vec3 position)
{
	node_info_.transform_.position_ += get_true_position(position);
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
	node_info_.transform_.rotation_ = rotation;
	//TODO : faire comme avec le membre position_ (avoir un membre qui prend en compte la parent_matrix_) ?
	update_parent_matrix_of_root_children();
}

void Node::add_rotation(glm::quat rotation)
{
	node_info_.transform_.rotation_ += rotation;
	//TODO : faire comme avec le membre position_ (avoir un membre qui prend en compte la parent_matrix_) ?
	update_parent_matrix_of_root_children();
}

void Node::set_scale(glm::vec3 scale)
{
	node_info_.transform_.scale_ = scale;
	//TODO : faire comme avec le membre position_ (avoir un membre qui prend en compte la parent_matrix_) ?
	update_parent_matrix_of_root_children();
}

void Node::add_scale(glm::vec3 scale)
{
	node_info_.transform_.scale_ += scale;
	//TODO : faire comme avec le membre position_ (avoir un membre qui prend en compte la parent_matrix_) ?
	update_parent_matrix_of_root_children();
}

void Node::update_parent_matrix_of_root_children()
{
	for(Node& child_node : children_nodes_)
	{
		child_node.node_info_.parent_matrix_ = compute_model();
		update_parent_matrix_of_children(child_node);
	}
}

void Node::update_parent_matrix_of_children(Node& node)
{
	for(Node& child_node : node.children_nodes_)
	{
		child_node.node_info_.parent_matrix_ = node.compute_model();
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
	std::vector<glm::vec3> aabb_points = node_info_.aabb_.value().get_corners();
	glm::vec3 min_values(std::numeric_limits<float>::max());
	glm::vec3 max_values(std::numeric_limits<float>::lowest());

	for(const glm::vec3& aabb_point : aabb_points)
	{
		glm::vec3 world_position_attribute = glm::vec4(aabb_point, 1.0f) * compute_model();
		min_values = glm::min(world_position_attribute, min_values);
		max_values = glm::max(world_position_attribute, max_values);
	}

	min_values += get_true_position(node_info_.transform_.position_);
	max_values += get_true_position(node_info_.transform_.position_);

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
	if(!node_info_.aabb_.has_value())
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
	return node_info_.transform_.position_;
}

}