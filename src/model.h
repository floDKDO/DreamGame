#pragma once

#include "glTF/scene.h"
#include "glTF/gltf_file.h"

#include <memory>

class Model
{
	public:
		explicit Model(std::string_view path);
		Model(std::string_view path, Transform transform);

		void draw(ShaderProgram& shader_program);

		glm::vec3& get_position() const; //get position of root_node_
		std::string get_name() const; //get name of root_node_

		void add_translation(glm::vec3 position);
		void add_rotation(glm::quat rotation);
		void add_scale(glm::vec3 scale);

		void set_translation(glm::vec3 position);
		void set_rotation(glm::quat rotation);
		void set_scale(glm::vec3 scale);

	private:
		void translate_children(gltf::Node* node, glm::vec3 position, bool is_addition);
		void rotate_children(gltf::Node* node, glm::quat rotation, bool is_addition);
		void scale_children(gltf::Node* node, glm::vec3 scale, bool is_addition);

		gltf::glTFFile gltf_file_;
		std::unique_ptr<gltf::Scene> scene_;
};