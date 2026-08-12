#pragma once

#include "Render/mesh.h"
#include "utils.h"
#include "gltf_file.h"

//#include <tinygltf/tiny_gltf_v3.h>

namespace gltf
{

class Node
{
	public:
		explicit Node(std::string_view path);

		void draw(ShaderProgram& shader_program);

		//TODO : englober les 3 vecteurs dans une struct Transform
		glm::vec3 position_;

		glm::vec4 rotation_; //TODO : inutile pour l'instant (utiliser à terme glm::quat ??)

		struct RotationInfo
		{
			float angle_;
			glm::vec3 axis_;
		};
		RotationInfo rotation_info_;

		glm::vec3 scale_;

	private:
		glm::mat4 compute_model(/*glm::vec3 translation_vector, float angle, glm::vec3 axis*/);
		void load_meshes();

		glTFFile gltf_file_;
		std::vector<Mesh> meshes_;
		int32_t mesh_index_; //TODO : inutile ??
		//glm::mat4 model_matrix_; //TODO : toujours utile ??
};

}