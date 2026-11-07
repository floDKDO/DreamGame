#pragma once

#include "glTF/node.h"
#include "glTF/gltf_file.h"

class Model
{
	public:
		struct Info
		{
			std::string name_;
			std::string path_;
			Transform transform_;
			std::string ambient_sound_path_;
		};

		explicit Model(Info model_info);

		void render();

		const glm::vec3& get_position() const; //retourne une référence car la position est utilisée en tant que membre (const ref) dans la classe Camera
		std::string get_name() const; //get name of root_node_
		const gltf::Node* get_root_node();

		void add_translation(glm::vec3 position);
		void add_translation_x(float x);
		void add_translation_y(float y);
		void add_translation_z(float z);

		void add_rotation(glm::quat rotation);
		void add_scale(glm::vec3 scale);

		void set_translation(glm::vec3 position);
		void set_rotation(glm::quat rotation);
		void set_scale(glm::vec3 scale);

	private:
		Info model_info_;
		gltf::glTFFile gltf_file_;
		std::unique_ptr<gltf::Node> root_node_;
		std::size_t ambient_sound_id_;
};