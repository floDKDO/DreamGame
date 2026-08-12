#include "node.h"
#include "gltf.h"

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

namespace gltf
{

Node::Node(std::string_view path)
	: gltf_file_(path), mesh_index_(-1), //model_matrix_(glm::mat4(1.0f)), 
	position_(0.0f, 0.0f, 0.0f),
	rotation_(0.0f, 0.0f, 0.0f, 1.0f), rotation_info_({0.0f, glm::vec3(0.0f, 1.0f, 0.0f)}),
	scale_(1.0f, 1.0f, 1.0f)
{
	load_meshes();
}

glm::mat4 Node::compute_model(/*glm::vec3 translation_vector, float angle, glm::vec3 axis*/)
{
	glm::mat4 model_matrix(1.0f); //reset de la matrice model à chaque frame
	model_matrix = glm::translate(model_matrix, position_);

	//TODO : rotation (vérifier)
	//model_matrix *= glm::mat4_cast(glm::quat(rotation_));
	model_matrix = glm::rotate(model_matrix, rotation_info_.angle_, rotation_info_.axis_);

	model_matrix = glm::scale(model_matrix, scale_);

	return model_matrix;

	/*model_matrix_(1.0f); //reset de la matrice model à chaque frame
	model_matrix_ = glm::translate(model_matrix_, translation_vector);
	model_matrix_ = glm::rotate(model_matrix_, glm::radians(angle), axis);*/
}

//TODO : diviser en plusieurs méthodes et en mettre dans glTFFile
void Node::load_meshes()
{
	std::vector<Texture> textures = gltf_file_.get_textures();
	const tg3_model& model = gltf_file_.get_model();

	if(model.nodes_count != 1)
	{
		std::cerr << "Warning: multiple nodes for a node! Only taking the first node.\n";
	}

	tg3_node node = model.nodes[0];
	if(gltf::is_1d_matrix_identity(node.matrix)) //=> ignorer la matrice si elle est la matrice identité
	{
		position_ = glm::vec3(node.translation[0], node.translation[1], node.translation[2]);
		rotation_ = glm::vec4(node.rotation[0], node.rotation[1], node.rotation[2], node.rotation[3]);
		scale_ = glm::vec3(node.scale[0], node.scale[1], node.scale[2]);
	}
	else
	{
		//nodes_.push_back({Node(node.mesh, node.matrix), glm::vec3(node.translation[0], node.translation[1], node.translation[2])});

		//TODO : à vérifier
		//position_ = glm::vec3(node.matrix[3], node.matrix[7], node.matrix[11]);

		//glm::mat4 model_matrix(1.0f);
		//model_matrix = glm::decompose(model_matrix, scale_, glm::quat(rotation_), position_, glm::vec3(1.0f), glm::vec4(1.0f));
		//rotation_ = glm::conjugate(glm::quat(rotation_));
		//gltf::print_mat4(model_matrix);
		std::cerr << "************************CAS PAS ENCORE GERE************************\n";
	}
	meshes_ = gltf_file_.get_meshes(node);
}

void Node::draw(ShaderProgram& shader_program)
{
	shader_program.set_uniform_matrix_4fv("model_matrix_", glm::value_ptr(compute_model()));
	for(Mesh& mesh : meshes_)
	{
		mesh.draw(shader_program);
	}
}

}